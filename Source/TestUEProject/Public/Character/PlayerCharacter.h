#pragma once

#include "CoreMinimal.h"
#include "GenericCharacter.h"
#include "WeaponBase.h"
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
	void StartWalk();
	void StopWalk();
	void StartSprint();
	void StopSprint();
	void StartFire();
	void StopFire();
	void ReloadWeapon();
	void EquipKnife();
	void EquipPistol();
	void EquipRifle();
	void OnPickItem();
	void OnThrowItem();

	void OnEquippedWeaponChanged(AWeaponBase* weapon);

	void Tick(float DeltaTime) override;

protected:
	void BeginPlay() override;

	/* Refreshes world actor, that the character is currently looking at, and able to interact with */
	void RefreshFocusedWorldItem();

private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (DisplayName = "Weapon User Component", AllowPrivateAccess = "true"))
	class UWeaponUser* m_weaponUser = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (DisplayName = "CameraBoom", AllowPrivateAccess = "true"))
	class USpringArmComponent* m_cameraSpringArm = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (DisplayName = "Camera", AllowPrivateAccess = "true"))
	class UCameraComponent* m_camera = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (DisplayName = "Inventory", AllowPrivateAccess = "true"))
	class UInventoryComponent* m_inventory = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (DisplayName = "Focused world actor", AllowPrivateAccess = "true"))
	AActor* FocusedWorldActor = nullptr;
};
