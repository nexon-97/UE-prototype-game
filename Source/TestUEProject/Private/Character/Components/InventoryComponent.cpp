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
