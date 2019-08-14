#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DB/InventoryItemsDB.h"
#include "ProtoGameModeBase.generated.h"

UCLASS()
class TESTUEPROJECT_API AProtoGameModeBase
	: public AGameModeBase
{
	GENERATED_BODY()

public:
	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<AInventoryItemsDB> InventoryItemsDBClass;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly)
	AInventoryItemsDB* InventoryItemsDB;
};
