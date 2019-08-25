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
};
