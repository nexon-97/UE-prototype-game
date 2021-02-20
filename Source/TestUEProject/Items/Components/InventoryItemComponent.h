#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/InventoryItemDef.h"
#include "InventoryItemComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTUEPROJECT_API UInventoryItemComponent
	: public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryItemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FInventoryItemDef* GetItemDef() const;

	UFUNCTION(BlueprintCallable)
	FString GetInventoryName() const;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (DisplayName = "Item info ID"))
	FName ItemId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Units"))
	int Quantity;
};
