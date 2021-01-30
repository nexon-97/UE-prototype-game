#include "InventoryItemComponent.h"
#include "UObject/ConstructorHelpers.h"
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
	AProtoGameModeBase* gameMode = GetWorld()->GetAuthGameMode<AProtoGameModeBase>();
	return gameMode->InventoryItemsDB->GetItemDef(m_itemInfoId, itemDef);
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
