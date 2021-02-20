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

	/* Entry id in the inventory items list (unique for single UInventoryComponent instance), assigned when item entry is created */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 EntryId;
};

class UInventoryItemComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TESTUEPROJECT_API UInventoryComponent
	: public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent(const FObjectInitializer& Initializer);

	virtual void BeginPlay() override;

	void AddItem(const FInventoryItemEntry& ItemData);

	void AddItem(UInventoryItemComponent* ItemComp);

	void RemoveItem(const int32 EntryId);

	/* Finds item index by item inventory id */
	UFUNCTION(BlueprintCallable)
	int32 FindItemIndexById(const FName& ItemId);

	/* Returns item entry copy by index */
	UFUNCTION(BlueprintCallable)
	FInventoryItemEntry GetItemEntry(int32 Index) const;

	/* Adds quantity to item at index */
	UFUNCTION(BlueprintCallable)
	void AddItemQuantity(int32 Index, int32 Quantity);

	/* Removes given quantity from item at index */
	UFUNCTION(BlueprintCallable)
	void RemoveItemQuantity(int32 Index, int32 Quantity);

protected:
	void DoAddItem(const FInventoryItemEntry& ItemData);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FInventoryItemEntry> Items;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bAutoEquipWeapons = false;
	
	int32 LastItemId = -1;
};
