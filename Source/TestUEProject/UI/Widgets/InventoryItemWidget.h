#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/Components/InventoryComponent.h"
#include "Data/InventoryItemDef.h"
#include "Components/Button.h"
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

	void NativeOnInitialized() override;

	const FInventoryItemEntry& GetInventoryItemEntry() const;

	const FInventoryItemDef& GetInventoryItemDef() const;
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText QuantityValueText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText ItemName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float StateProgress;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ESlateVisibility StateProgressVisibility;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnButtonClickedEvent InventoryItemClickDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnButtonClickedEvent InventoryItemDblClickDelegate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UButton* InteractionWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsSelected = false;

private:
	UFUNCTION()
	void OnClickInternal();

	float LastClickTime = -1.f;

	FInventoryItemEntry ItemEntry;

	FInventoryItemDef* ItemDef;
};
