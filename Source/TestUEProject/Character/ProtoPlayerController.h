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
	virtual void SpawnPlayerCameraManager() override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector CombatCameraOffset;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCombatModeCameraOffsetEffect> CombatCameraModifierType;

protected:
	void InitCameraModifiers();

	virtual void OnPossess(APawn* NewPawn) override;
	virtual void OnUnPossess() override;

	void OnEquippedWeaponChanged(AWeaponBase* Weapon);

	virtual void SetupInputComponent() override;

	void OnInventoryToggle();

	void OnItemThrow();

protected:
	UPROPERTY(Transient)
	UCombatModeCameraOffsetEffect* CombatModeCameraEffect = nullptr;
};
