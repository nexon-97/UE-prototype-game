#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Widgets/InventoryWidget.h"
#include "GameHUD.generated.h"

class UCanvasPanel;

UCLASS()
class TESTUEPROJECT_API AGameHUD
	: public AHUD
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void GainPlayerInputFocus();

	UFUNCTION(BlueprintCallable)
	void FreePlayerInputFocus();

	void OnOpenInventory();

protected:
	void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UInventoryWidget* InventoryWidget = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UUserWidget* InGameLayoutWidget = nullptr;

	/* Host widget for all overlay screens (for ex inventory) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCanvasPanel* OverlaysHostWidget = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsInventoryOpen = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bHasPlayerInputFocus = false;
};
