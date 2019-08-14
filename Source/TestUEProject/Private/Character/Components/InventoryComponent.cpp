#include "InventoryComponent.h"

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

}

void UInventoryComponent::AddItem(UInventoryItemComponent* itemComp)
{

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
