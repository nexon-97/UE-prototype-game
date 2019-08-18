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

	/* Finds item index by item inventory id */
	UFUNCTION(BlueprintCallable)
	int32 FindItemIndexById(const FName& itemId);

	/* Returns item entry copy by index */
	UFUNCTION(BlueprintCallable)
	FInventoryItemEntry GetItemEntry(int32 index) const;

	/* Adds quantity to item at index */
	UFUNCTION(BlueprintCallable)
	void AddItemQuantity(int32 index, int32 quantity);

	/* Removes given quantity from item at index */
	UFUNCTION(BlueprintCallable)
	void RemoveItemQuantity(int32 index, int32 quantity);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FInventoryItemEntry> Items;
};
