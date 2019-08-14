#include "InventoryItemComponent.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
UDataTable* g_inventoryDB = nullptr;
const FString k_defaultItemName = "$ITEM_NAME$";
}

UInventoryItemComponent::UInventoryItemComponent()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableFinder(TEXT("/Game/Data/InventoryItemsDB"));
	g_inventoryDB = DataTableFinder.Object;

	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoryItemComponent::GetItemDef(FInventoryItemDef& itemDef) const
{
	if (nullptr != g_inventoryDB)
	{
		FString context = "CONTEXT";
		FInventoryItemDef* itemDefPtr = g_inventoryDB->FindRow<FInventoryItemDef>(FName(*m_itemInfoId), context, false);

		itemDef = *itemDefPtr;
		return true;
	}

	return false;
}

const FString& UInventoryItemComponent::GetInventoryName() const
{
	FInventoryItemDef itemDef;

	if (GetItemDef(itemDef))
	{
		return itemDef.InventoryName;
	}

	return k_defaultItemName;
}
