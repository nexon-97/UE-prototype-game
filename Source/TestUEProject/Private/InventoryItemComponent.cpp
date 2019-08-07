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

const FString& UInventoryItemComponent::GetInventoryName() const
{
	if (nullptr != g_inventoryDB)
	{
		FString context = "CONTEXT";
		FInventoryItemDef* itemDef = g_inventoryDB->FindRow<FInventoryItemDef>(FName(*m_itemInfoId), context, false);

		if (nullptr != itemDef)
		{
			return itemDef->InventoryName;
		}
	}

	return k_defaultItemName;
}
