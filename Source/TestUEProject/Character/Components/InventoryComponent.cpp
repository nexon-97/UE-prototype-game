#include "InventoryComponent.h"
#include "Items/Components/InventoryItemComponent.h"
#include "System/ProtoGameModeBase.h"

#include "Engine.h"

UInventoryComponent::UInventoryComponent(const FObjectInitializer& Initializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	AProtoGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AProtoGameModeBase>();
	UInventoryItemsDB* ItemsDB = GameMode->InventoryItemsDB;
	FInventoryItemDef* ItemDef = nullptr;

	int32 EntryId = 0;
	for (FInventoryItemEntry& Entry : Items)
	{
		Entry.EntryId = EntryId;
		++EntryId;

		if (bAutoEquipWeapons && ItemsDB->GetItemDef(Entry.ItemId, ItemDef))
		{
			if (ItemDef->ItemClass == EItemClass::Weapon)
			{
				// It's a weapon, need to get it's slot info and equip if there is free room for that
			}
		}
	}

	LastItemId = EntryId;
}

void UInventoryComponent::AddItem(const FInventoryItemEntry& ItemData)
{
	if (ItemData.ItemId == NAME_None)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Trying to add item with invalid ID to inventory!"));
		return;
	}
	else if (ItemData.Quantity < 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Trying to add item with invalid quantity to inventory!"));
		return;
	}

	AProtoGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AProtoGameModeBase>();
	FInventoryItemDef* ItemDef;
	if (GameMode->InventoryItemsDB->GetItemDef(ItemData.ItemId, ItemDef))
	{
		if (ItemDef->bIsBreakable)
		{
			DoAddItem(ItemData);
		}
		else
		{
			// Not breakable, so can be stacked
			auto Predicate = [&ItemData](const FInventoryItemEntry& Item)
			{
				return Item.ItemId == ItemData.ItemId;
			};
			FInventoryItemEntry* ArrayItemPtr = Items.FindByPredicate(Predicate);

			if (nullptr != ArrayItemPtr)
			{
				ArrayItemPtr->Quantity += ItemData.Quantity;
			}
			else
			{
				DoAddItem(ItemData);
			}
		}
	}
}

void UInventoryComponent::AddItem(UInventoryItemComponent* ItemComp)
{
	FInventoryItemEntry ItemEntry;
	ItemEntry.ItemId = ItemComp->ItemId;
	ItemEntry.Quantity = ItemComp->Quantity;
	ItemEntry.State = 100.f;

	AddItem(ItemEntry);
}

void UInventoryComponent::RemoveItem(const int32 EntryId)
{
	int FoundId = INDEX_NONE;
	for (int i = 0; i < Items.Num(); i++)
	{
		if (Items[i].EntryId == EntryId)
		{
			FoundId = i;
			break;
		}
	}

	if (FoundId != INDEX_NONE)
	{
		Items.RemoveAtSwap(FoundId);
	}
}

int32 UInventoryComponent::FindItemIndexById(const FName& ItemId)
{
	int32 i = INDEX_NONE;
	for (const FInventoryItemEntry& Item : Items)
	{
		i++;

		if (Item.ItemId == ItemId)
		{
			return i;
		}
	}

	return INDEX_NONE;
}

FInventoryItemEntry UInventoryComponent::GetItemEntry(int32 Index) const
{
	return Items[Index];
}

void UInventoryComponent::AddItemQuantity(int32 Index, int32 Quantity)
{
	if (Quantity < 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Trying to add invalid quantity to inventory item!"));
		return;
	}

	Items[Index].Quantity += Quantity;
}

void UInventoryComponent::RemoveItemQuantity(int32 Index, int32 Quantity)
{
	if (Quantity < 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Trying to remove invalid quantity from inventory item!"));
		return;
	}

	FInventoryItemEntry& ItemEntry = Items[Index];
	ItemEntry.Quantity -= FMath::Min(Quantity, ItemEntry.Quantity);

	if (ItemEntry.Quantity <= 0)
	{
		// Remove items with zero quantity
		Items.RemoveAt(Index, 1, true);
	}
}

void UInventoryComponent::DoAddItem(const FInventoryItemEntry& ItemData)
{
	LastItemId++;

	Items.Add(ItemData);
	FInventoryItemEntry& InsertedItemData = Items.Last();
	InsertedItemData.EntryId = LastItemId;
}
