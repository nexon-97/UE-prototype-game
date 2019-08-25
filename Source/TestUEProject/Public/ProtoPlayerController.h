#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CombatModeCameraOffsetEffect.h"
#include "ProtoPlayerController.generated.h"

class AWeaponBase;

UCLASS()
class TESTUEPROJECT_API AProtoPlayerController
	: public APlayerController
{
	GENERATED_BODY()
	
public:
	void SpawnPlayerCameraManager() override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector CombatCameraOffset;

protected:
	void InitCameraModifiers();

	void OnPossess(APawn* aPawn) override;
	void OnUnPossess() override;

	void OnEquippedWeaponChanged(AWeaponBase* weapon);

	void SetupInputComponent() override;

	void OnInventoryToggle();

	void OnItemThrow();

protected:
	UCombatModeCameraOffsetEffect* CombatModeCameraEffect = nullptr;
};
