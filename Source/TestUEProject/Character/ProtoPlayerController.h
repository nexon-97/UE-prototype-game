#pragma once

#include "CoreMinimal.h"

#include "PlayerCharacter.h"
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

	UFUNCTION(BlueprintCallable)
	bool IsAiming() const;

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraModifier> IdleCameraModifierType;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCombatModeCameraOffsetEffect> CombatCameraModifierType;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraModifier> AimCameraModifierType;

protected:
	void InitCameraModifiers();

	virtual void OnPossess(APawn* NewPawn) override;
	virtual void OnUnPossess() override;

	void OnEquippedWeaponChanged(AWeaponBase* Weapon);

	virtual void SetupInputComponent() override;

	void OnInventoryToggle();
	void OnItemThrow();
	void OnStartAim();
	void OnStopAim();
	void StartWalk();
	void StopWalk();
	void StartSprint();
	void StopSprint();
	void EquipKnife();
	void EquipPistol();
	void EquipRifle();

protected:
	UPROPERTY(Transient)
	UCombatModeCameraOffsetEffect* CombatModeCameraEffect = nullptr;

	UPROPERTY(Transient)
	UCameraModifier* IdleCameraModifier = nullptr;

	UPROPERTY(Transient)
	UCameraModifier* AimCameraModifier = nullptr;

	UPROPERTY(Transient)
	APlayerCharacter* ControlledCharacter = nullptr;

	UPROPERTY(Transient)
	UWeaponUser* WeaponUser = nullptr;

	bool bIsAiming = false;
};
