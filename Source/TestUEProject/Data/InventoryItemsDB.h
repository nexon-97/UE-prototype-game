#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/InventoryItemDef.h"
#include "InventoryItemsDB.generated.h"

UCLASS()
class TESTUEPROJECT_API UInventoryItemsDB
	: public UObject
{
	GENERATED_BODY()
	
public:
	void Init(UDataTable* InInventoryItemsTable);
	
	bool GetItemDef(const FName& ItemId, FInventoryItemDef*& OutItemDef) const;

	UFUNCTION(BlueprintCallable)
	bool BlueprintGetItemDef(const FName& ItemId, FInventoryItemDef& OutItemDef) const;

	AActor* SpawnItem(const FName& ItemId, const FVector& Location, const FRotator& Rotation, const FActorSpawnParameters& SpawnParameters) const;

public:
	UPROPERTY(Transient)
	UDataTable* InventoryItemsTable;

	TMap<FName, FInventoryItemDef*> ItemsDefLookupTable;
};
