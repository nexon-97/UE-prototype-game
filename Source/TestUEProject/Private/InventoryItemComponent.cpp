#include "InventoryItemComponent.h"

UInventoryItemComponent::UInventoryItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryItemComponent::BeginPlay()
{
	Super::BeginPlay();	
}
