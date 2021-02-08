#pragma once

#include "CoreMinimal.h"
#include "GenericCharacter.h"
#include "Weapon/WeaponBase.h"
#include "PlayerCharacter.generated.h"

UENUM()
enum class EPlayerCameraMode
{
	FirstPerson,
	ThirdPerson,
};

UCLASS()
class TESTUEPROJECT_API APlayerCharacter
	: public AGenericCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
	void ReloadWeapon();
	void OnPickItem();
	void OnThrowItem();

	void SetCameraMode(EPlayerCameraMode NewCameraMode);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	/* Refreshes world actor, that the character is currently looking at, and able to interact with */
	void RefreshFocusedWorldItem();

	void RefreshCameraParams();

private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (DisplayName = "Weapon User Component", AllowPrivateAccess = "true"))
	class UWeaponUser* WeaponUser = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (DisplayName = "Inventory", AllowPrivateAccess = "true"))
	class UInventoryComponent* m_inventory = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (DisplayName = "Focused world actor", AllowPrivateAccess = "true"))
	AActor* FocusedWorldActor = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCamera;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* ThirdPersonCamera;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraSpringArm;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	EPlayerCameraMode CameraMode;

	UPROPERTY(Transient)
	class UCameraComponent* PlayerCamera = nullptr;

	bool bIsShooting = false;
};
