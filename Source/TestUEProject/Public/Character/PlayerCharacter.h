#pragma once

#include "CoreMinimal.h"
#include "GenericCharacter.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class TESTUEPROJECT_API APlayerCharacter
	: public AGenericCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*
	* @brief Input handlers
	*/
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Turn(float AxisValue);
	void LookUp(float AxisValue);
	void StartJump();
	void StopJump();
	void StartFire();
	void StopFire();
	void EquipKnife();
	void EquipPistol();
	void EquipRifle();

private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (DisplayName = "Weapon User Component", AllowPrivateAccess = "true"))
	class UWeaponUser* m_weaponUser = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (DisplayName = "CameraBoom", AllowPrivateAccess = "true"))
	class USpringArmComponent* m_cameraSpringArm = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (DisplayName = "Camera", AllowPrivateAccess = "true"))
	class UCameraComponent* m_camera = nullptr;
};
