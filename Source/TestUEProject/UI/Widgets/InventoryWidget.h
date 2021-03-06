#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widgets/InventoryItemWidget.h"
#include "Weapon/WeaponSlotType.h"
#include "InventoryWidget.generated.h"

class UVerticalBox;
class UInventoryWidget;

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

	UWorld* GetWorld() const override;

	UInventoryWidget* InventoryWidget = nullptr;

	UInventoryItemWidget* InventoryItemWidget = nullptr;

	UWorld* OwningWorld = nullptr;
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
