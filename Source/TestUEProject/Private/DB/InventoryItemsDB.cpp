#include "DB/InventoryItemsDB.h"

#include <Engine.h>

bool AInventoryItemsDB::GetItemDef(const FName& itemId, FInventoryItemDef& itemDef) const
{
	if (nullptr != InventoryDataAsset)
	{
		FString context = "CONTEXT";
		FInventoryItemDef* itemDefPtr = InventoryDataAsset->FindRow<FInventoryItemDef>(itemId, context, false);

		if (nullptr != itemDefPtr)
		{
			itemDef = *itemDefPtr;
			return true;
		}
	}

	return false;
}

AActor* AInventoryItemsDB::SpawnItem(const FName& itemId) const
{
	return nullptr;
}
