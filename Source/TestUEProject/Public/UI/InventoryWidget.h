#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/InventoryItemWidget.h"
#include "EWeaponSlotType.h"
#include "InventoryWidget.generated.h"

class UVerticalBox;

UCLASS()
class TESTUEPROJECT_API UInventoryWidgetClickContext
	: public UObject
{
	GENERATED_BODY()

public:
	UInventoryWidgetClickContext()
	{}

	UFUNCTION()
	void OnClick();

	UInventoryItemWidget* InventoryItemWidget = nullptr;
};

UCLASS()
class TESTUEPROJECT_API UInventoryWidget
	: public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void PopulateInventory();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UVerticalBox* ItemsHostWidget = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UInventoryItemWidget> ItemWidgetClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EWeaponSlotType, UInventoryItemWidget*> WeaponSlotWidgets;

private:
	TArray<UInventoryItemWidget*> ItemsWidgets;

	TArray<UInventoryWidgetClickContext*> ClickContexts;
};
