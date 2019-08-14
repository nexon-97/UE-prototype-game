#include "Character/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Character/Components/WeaponUser.h"
#include "Character/Components/InventoryComponent.h"
#include "Character/PlayerAnimInstance.h"
#include "Components/InputComponent.h"
#include "InventoryItemDef.h"

#include "Engine.h"

APlayerCharacter::APlayerCharacter()
{
	m_weaponUser = CreateDefaultSubobject<UWeaponUser>(TEXT("WeaponUser"));
	m_weaponUser->bEditableWhenInherited = true;

	FAttachmentTransformRules attachmentRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false);

	m_cameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	m_cameraSpringArm->bEditableWhenInherited = true;
	m_cameraSpringArm->AttachToComponent(RootComponent, attachmentRules);

	m_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	m_camera->bEditableWhenInherited = true;
	m_camera->AttachToComponent(m_cameraSpringArm, attachmentRules, FName(TEXT("SpringEndpoint")));

	GetMesh()->SetAnimInstanceClass(UPlayerAnimInstance::StaticClass());

	m_weaponUser->ActorMesh = Cast<UMeshComponent>(GetMesh());
	m_weaponUser->EquippedWeaponChangedEvent.AddUObject(this, &APlayerCharacter::OnEquippedWeaponChanged);

	m_inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
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

	// Setup weapon user input
	InputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::StartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::StopFire);
	InputComponent->BindAction("Reload", IE_Pressed, this, &APlayerCharacter::ReloadWeapon);
	InputComponent->BindAction("EquipKnife", IE_Pressed, this, &APlayerCharacter::EquipKnife);
	InputComponent->BindAction("EquipPistol", IE_Pressed, this, &APlayerCharacter::EquipPistol);
	InputComponent->BindAction("EquipRifle", IE_Pressed, this, &APlayerCharacter::EquipRifle);
	//InputComponent->BindAction("EquipGrenade", IE_Pressed, this, &APlayerCharacter::EquipGrenade);
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

void APlayerCharacter::StartFire()
{
	m_weaponUser->StartFire();
}

void APlayerCharacter::StopFire()
{
	m_weaponUser->StopFire();
}

void APlayerCharacter::ReloadWeapon()
{
	if (nullptr != m_weaponUser->EquippedWeapon)
	{
		// Find ammo in inventory
		FInventoryItemEntry ammoItemEntry;
		int32 index;
		if (m_inventory->FindItemById(m_weaponUser->EquippedWeapon->AmmoTypeId, ammoItemEntry, index))
		{
			int loadedCount = m_weaponUser->EquippedWeapon->LoadClip(ammoItemEntry.Quantity);
			m_inventory->GetItem(index).Quantity -= loadedCount;
		}
	}
}

void APlayerCharacter::EquipKnife()
{
	m_weaponUser->EquipWeapon(EWeaponSlotType::Knife);
}

void APlayerCharacter::EquipPistol()
{
	m_weaponUser->EquipWeapon(EWeaponSlotType::Pistol);
}

void APlayerCharacter::EquipRifle()
{
	m_weaponUser->EquipWeapon(EWeaponSlotType::Rifle);
}

void APlayerCharacter::OnEquippedWeaponChanged(AWeaponBase* weapon)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Equipped Weapon Changed"));

	UPlayerAnimInstance* animInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != animInstance)
	{
		animInstance->bIsEquippingWeapon = (nullptr != weapon);
	}
}
