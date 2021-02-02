#include "InventoryItemComponent.h"
#include "System/ProtoGameModeBase.h"

namespace
{
FString k_defaultItemName = "$ITEM_NAME$";
}

UInventoryItemComponent::UInventoryItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoryItemComponent::GetItemDef(FInventoryItemDef& itemDef) const
{
	AProtoGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AProtoGameModeBase>();
	return GameMode->InventoryItemsDB->GetItemDef(m_itemInfoId, itemDef);
}

FString UInventoryItemComponent::GetInventoryName() const
{
	FInventoryItemDef itemDef;
	if (GetItemDef(itemDef))
	{
		return itemDef.InventoryName;
	}

	return k_defaultItemName;
}
