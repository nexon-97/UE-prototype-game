#include "InventoryComponent.h"
#include "InventoryItemComponent.h"

#include <Engine.h>

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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
		
	Items.Add(itemData);
}

void UInventoryComponent::AddItem(UInventoryItemComponent* itemComp)
{
	FInventoryItemEntry itemEntry;
	itemEntry.ItemId = itemComp->m_itemInfoId;
	itemEntry.Quantity = itemComp->m_count;
	itemEntry.State = 100.f;

	AddItem(itemEntry);
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
