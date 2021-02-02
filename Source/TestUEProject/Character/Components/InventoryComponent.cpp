#include "InventoryComponent.h"
#include "Items/Components/InventoryItemComponent.h"
#include "System/ProtoGameModeBase.h"

#include "Engine.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	int32 entryId = 0;
	for (FInventoryItemEntry& entry : Items)
	{
		entry.EntryId = entryId;
		++entryId;
	}

	LastItemId = entryId;
}

void UInventoryComponent::AddItem(const FInventoryItemEntry& itemData)
{
	if (itemData.ItemId == NAME_None)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Trying to add item with invalid ID to inventory!"));
		return;
	}
	else if (itemData.Quantity < 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Trying to add item with invalid quantity to inventory!"));
		return;
	}

	AProtoGameModeBase* gameMode = GetWorld()->GetAuthGameMode<AProtoGameModeBase>();
	FInventoryItemDef itemDef;
	if (gameMode->InventoryItemsDB->GetItemDef(itemData.ItemId, itemDef))
	{
		if (itemDef.bIsBreakable)
		{
			DoAddItem(itemData);
		}
		else
		{
			// Not breakable, so can be stacked
			auto predicate = [&itemData](const FInventoryItemEntry& item)
			{
				return item.ItemId == itemData.ItemId;
			};
			FInventoryItemEntry* arrayItemPtr = Items.FindByPredicate(predicate);

			if (nullptr != arrayItemPtr)
			{
				arrayItemPtr->Quantity += itemData.Quantity;
			}
			else
			{
				DoAddItem(itemData);
			}
		}
	}
}

void UInventoryComponent::AddItem(UInventoryItemComponent* itemComp)
{
	FInventoryItemEntry itemEntry;
	itemEntry.ItemId = itemComp->m_itemInfoId;
	itemEntry.Quantity = itemComp->m_count;
	itemEntry.State = 100.f;

	AddItem(itemEntry);
}

void UInventoryComponent::RemoveItem(const int32 entryId)
{
	int foundId = -1;
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].EntryId == entryId)
		{
			foundId = i;
			break;
		}
	}

	if (foundId >= 0)
	{
		Items.RemoveAtSwap(foundId);
	}
}

int32 UInventoryComponent::FindItemIndexById(const FName& itemId)
{
	int32 i = -1;
	for (const FInventoryItemEntry& item : Items)
	{
		++i;

		if (item.ItemId == itemId)
		{
			return i;
		}
	}

	return -1;
}

FInventoryItemEntry UInventoryComponent::GetItemEntry(int32 index) const
{
	return Items[index];
}

void UInventoryComponent::AddItemQuantity(int32 index, int32 quantity)
{
	if (quantity < 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Trying to add invalid quantity to inventory item!"));
		return;
	}

	Items[index].Quantity += quantity;
}

void UInventoryComponent::RemoveItemQuantity(int32 index, int32 quantity)
{
	if (quantity < 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Trying to remove invalid quantity from inventory item!"));
		return;
	}

	FInventoryItemEntry& itemEntry = Items[index];
	itemEntry.Quantity -= FMath::Min(quantity, itemEntry.Quantity);

	if (itemEntry.Quantity <= 0)
	{
		// Remove items with zero quantity
		Items.RemoveAt(index, 1, true);
	}
}

void UInventoryComponent::DoAddItem(const FInventoryItemEntry& itemData)
{
	++LastItemId;

	Items.Add(itemData);
	FInventoryItemEntry& insertedItemData = Items.Last();
	insertedItemData.EntryId = LastItemId;
}
