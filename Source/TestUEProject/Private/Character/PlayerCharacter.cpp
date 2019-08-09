#include "Character/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Character/Components/WeaponUser.h"
#include "Components/InputComponent.h"

APlayerCharacter::APlayerCharacter()
{
	m_weaponUser = CreateDefaultSubobject<UWeaponUser>(TEXT("WeaponUser"));
	m_weaponUser->bEditableWhenInherited = true;

	m_cameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	m_cameraSpringArm->bEditableWhenInherited = true;
	m_cameraSpringArm->AttachTo(RootComponent);

	m_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	m_camera->bEditableWhenInherited = true;
	m_camera->AttachTo(m_cameraSpringArm, FName(TEXT("SpringEndpoint")));
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	InputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
	InputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);

	InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::StartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJump);

	APlayerController* controller = GetController<APlayerController>();
	if (nullptr != controller)
	{
		controller->PlayerCameraManager->ViewPitchMin = -50.f;
		controller->PlayerCameraManager->ViewPitchMax = 0.f;
	}

	// Setup weapon user input
}

void APlayerCharacter::MoveForward(float AxisValue)
{
	FRotator rotation = GetControlRotation();
	rotation.Roll = 0.f;
	rotation.Pitch = 0.f;

	AddMovementInput(rotation.Quaternion().GetForwardVector(), AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	FRotator rotation = GetControlRotation();
	rotation.Roll = 0.f;
	rotation.Pitch = 0.f;

	AddMovementInput(rotation.Quaternion().GetRightVector(), AxisValue);
}

void APlayerCharacter::Turn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void APlayerCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void APlayerCharacter::StartJump()
{
	Jump();
}

void APlayerCharacter::StopJump()
{
	StopJumping();
}