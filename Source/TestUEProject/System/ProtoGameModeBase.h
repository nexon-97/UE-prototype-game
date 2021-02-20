#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/InventoryItemsDB.h"
#include "ProtoGameModeBase.generated.h"

UCLASS()
class TESTUEPROJECT_API AProtoGameModeBase
	: public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UDataTable* InventoryItemsTable;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
	UInventoryItemsDB* InventoryItemsDB;
};
