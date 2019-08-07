#include "InventoryItemComponent.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
UDataTable* g_inventoryDB = nullptr;
}

UInventoryItemComponent::UInventoryItemComponent()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableFinder(TEXT("/Game/Data/InventoryItemsDB"));
	g_inventoryDB = DataTableFinder.Object;

	PrimaryComponentTick.bCanEverTick = false;
}

FString UInventoryItemComponent::GetInventoryName() const
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

	return "$ITEM_NAME$";
}
