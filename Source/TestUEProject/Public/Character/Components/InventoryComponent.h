#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItemEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName ItemId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Quantity;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float State = 100.f;
};

class UInventoryItemComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TESTUEPROJECT_API UInventoryComponent
	: public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	void AddItem(const FInventoryItemEntry& itemData);

	void AddItem(UInventoryItemComponent* itemComp);

	UFUNCTION(BlueprintCallable)
	bool FindItemById(const FName& itemId, FInventoryItemEntry& outItem, int32& index);

	UFUNCTION(BlueprintCallable)
	FInventoryItemEntry& GetItem(int32 index);

protected:
	virtual void BeginPlay() override;		

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FInventoryItemEntry> Items;
};
