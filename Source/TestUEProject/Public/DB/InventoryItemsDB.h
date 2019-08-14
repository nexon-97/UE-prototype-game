#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItemDef.h"
#include "InventoryItemsDB.generated.h"

UCLASS()
class TESTUEPROJECT_API AInventoryItemsDB
	: public AActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	bool GetItemDef(const FName& itemId, FInventoryItemDef& itemDef) const;

	UFUNCTION(BlueprintCallable)
	AActor* SpawnItem(const FName& itemId) const;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UDataTable* InventoryDataAsset = nullptr;
};
