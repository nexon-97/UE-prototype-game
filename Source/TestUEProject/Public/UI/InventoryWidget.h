#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UVerticalBox;

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
	TSubclassOf<UUserWidget> ItemWidgetClass;

private:
	TArray<UUserWidget*> ItemsWidgets;
};
