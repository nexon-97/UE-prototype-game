#include "InventoryItemComponent.h"
#include "System/ProtoGameModeBase.h"

namespace
{

	const FString DefaultItemName = "$ITEM_NAME$";

}

UInventoryItemComponent::UInventoryItemComponent(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

FInventoryItemDef* UInventoryItemComponent::GetItemDef() const
{
	AProtoGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AProtoGameModeBase>();
	FInventoryItemDef* ItemDef = nullptr;
	if (GameMode->InventoryItemsDB->GetItemDef(ItemId, ItemDef))
	{
		return ItemDef;
	}

	return nullptr;
}

FString UInventoryItemComponent::GetInventoryName() const
{
	FInventoryItemDef* ItemDef = GetItemDef();
	if (ItemDef)
	{
		return ItemDef->InventoryName;
	}

	return DefaultItemName;
}
