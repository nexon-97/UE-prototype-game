#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

UCLASS()
class TESTUEPROJECT_API AGameHUD
	: public AHUD
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void OnOpenInventory();

	UFUNCTION(BlueprintCallable)
	void GainPlayerInputFocus();

	UFUNCTION(BlueprintCallable)
	void FreePlayerInputFocus();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsInventoryOpen = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bHasPlayerInputFocus = false;
};
