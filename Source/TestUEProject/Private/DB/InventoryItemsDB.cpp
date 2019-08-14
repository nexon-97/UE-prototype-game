#include "DB/InventoryItemsDB.h"

#include <Engine.h>

bool AInventoryItemsDB::GetItemDef(const FName& itemId, FInventoryItemDef& itemDef) const
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("GetItemDef call"));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, itemId.ToString());

	if (nullptr != InventoryDataAsset)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("InventoryDataAsset OK"));

		FString context = "CONTEXT";
		FInventoryItemDef* itemDefPtr = InventoryDataAsset->FindRow<FInventoryItemDef>(itemId, context, false);

		if (nullptr != itemDefPtr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Item found"));

			itemDef = *itemDefPtr;
			return true;
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Item not found"));
	return false;
}

AActor* AInventoryItemsDB::SpawnItem(const FName& itemId) const
{
	return nullptr;
}
