#include "ProtoGameModeBase.h"

#include <Engine.h>

void AProtoGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if (InventoryItemsDBClass)
	{
		InventoryItemsDB = GetWorld()->SpawnActor<AInventoryItemsDB>(InventoryItemsDBClass.Get());
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("InventoryItemsDBClass instantiated"));
	}
}
