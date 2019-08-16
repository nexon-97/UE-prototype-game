#include "InventoryComponent.h"
#include "InventoryItemComponent.h"

#include <Engine.h>

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
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

bool UInventoryComponent::FindItemById(const FName& itemId, FInventoryItemEntry& outItem, int32& index)
{
	int32 i = -1;

	for (FInventoryItemEntry& item : Items)
	{
		++i;

		if (item.ItemId == itemId)
		{
			outItem = item;
			index = i;
			return true;
		}
	}

	return false;
}

FInventoryItemEntry& UInventoryComponent::GetItem(int32 index)
{
	return Items[index];
}
