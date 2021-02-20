#include "ProtoGameModeBase.h"

void AProtoGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if ensure(InventoryItemsTable)
	{
		InventoryItemsDB = NewObject<UInventoryItemsDB>(this);
		InventoryItemsDB->Init(InventoryItemsTable);
	}
}
