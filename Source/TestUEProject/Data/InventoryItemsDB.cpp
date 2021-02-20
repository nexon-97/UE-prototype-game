#include "Data/InventoryItemsDB.h"
#include "Engine.h"

void UInventoryItemsDB::Init(UDataTable* InInventoryItemsTable)
{
	InventoryItemsTable = InInventoryItemsTable;

	// Fill lookup by id map
	const FString Context = TEXT("CONTEXT");
	TArray<FInventoryItemDef*> Rows;
	InventoryItemsTable->GetAllRows<FInventoryItemDef>(Context, Rows);
	
	for (FInventoryItemDef* Row : Rows)
	{
		ItemsDefLookupTable.Add(Row->Id, Row);
	}
}

bool UInventoryItemsDB::GetItemDef(const FName& ItemId, FInventoryItemDef*& OutItemDef) const
{
	FInventoryItemDef* const* ItemDefPtr = ItemsDefLookupTable.Find(ItemId);
	if (ItemDefPtr)
	{
		OutItemDef = *ItemDefPtr;
		return true;
	}
	
	return false;
}

bool UInventoryItemsDB::BlueprintGetItemDef(const FName& ItemId, FInventoryItemDef& OutItemDef) const
{
	FInventoryItemDef* ItemDef;
	if (GetItemDef(ItemId, ItemDef))
	{
		OutItemDef = *ItemDef;
		return true;
	}

	return false;
}

AActor* UInventoryItemsDB::SpawnItem(const FName& ItemId, const FVector& Location, const FRotator& Rotation, const FActorSpawnParameters& SpawnParameters) const
{
	FInventoryItemDef* ItemDef;
	if (GetItemDef(ItemId, ItemDef))
	{
		if (ItemDef->WorldActorType)
		{
			AActor* ItemActor = GetWorld()->SpawnActor<AActor>(ItemDef->WorldActorType.Get(), Location, Rotation, SpawnParameters);
			return ItemActor;
		}
	}

	return nullptr;
}
