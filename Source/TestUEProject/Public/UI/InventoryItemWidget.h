#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/InventoryComponent.h"
#include "InventoryItemWidget.generated.h"

UCLASS()
class TESTUEPROJECT_API UInventoryItemWidget
	: public UUserWidget
{
	GENERATED_BODY()

public:
	UInventoryItemWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void SetItemEntry(const FInventoryItemEntry& entry);
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText QuantityValueText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float StateProgress;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ESlateVisibility StateProgressVisibility;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryItemClickDelegate);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryItemDblClickDelegate);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FInventoryItemClickDelegate InventoryItemClickDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FInventoryItemDblClickDelegate InventoryItemDblClickDelegate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsSelected = false;
};
