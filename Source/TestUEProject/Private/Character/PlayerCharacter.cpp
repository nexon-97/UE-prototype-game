#include "Character/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Character/Components/WeaponUser.h"
#include "Character/Components/InventoryComponent.h"
#include "Character/PlayerAnimInstance.h"
#include "Components/InputComponent.h"
#include "InventoryItemDef.h"
#include "InventoryItemComponent.h"

#include "Engine.h"

namespace
{

const float AimFOVDegress = 45.f;
const float RegularFOVDegress = 90.f;

}

APlayerCharacter::APlayerCharacter()
{
	m_weaponUser = CreateDefaultSubobject<UWeaponUser>(TEXT("WeaponUser"));
	m_weaponUser->bEditableWhenInherited = true;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false);

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraSpringArm->bEditableWhenInherited = true;
	CameraSpringArm->AttachToComponent(RootComponent, AttachmentRules);

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FirstPersonCamera->bEditableWhenInherited = true;
	FirstPersonCamera->AttachToComponent(RootComponent, AttachmentRules);
	FirstPersonCamera->SetActive(false);

	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	ThirdPersonCamera->bEditableWhenInherited = true;
	ThirdPersonCamera->AttachToComponent(CameraSpringArm, AttachmentRules);
	ThirdPersonCamera->SetActive(false);

	GetMesh()->SetAnimInstanceClass(UPlayerAnimInstance::StaticClass());

	m_weaponUser->ActorMesh = Cast<UMeshComponent>(GetMesh());
	m_weaponUser->EquippedWeaponChangedEvent.AddUObject(this, &APlayerCharacter::OnEquippedWeaponChanged);

	m_inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
}

void APlayerCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	RefreshCameraParams();
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set default fov of camera
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		ensure(PlayerController->PlayerCameraManager);
		PlayerController->PlayerCameraManager->SetFOV(RegularFOVDegress);
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetMovementMode() == ECharacterMovementMode::Sprint)
	{
		const float SprintStaminaConsumption = 20.f;
		KillableComponent->RemoveStamina(SprintStaminaConsumption * DeltaTime);
	}

	RefreshFocusedWorldItem();
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

	InputComponent->BindAction("Walk", IE_Pressed, this, &APlayerCharacter::StartWalk);
	InputComponent->BindAction("Walk", IE_Released, this, &APlayerCharacter::StopWalk);
	InputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::StartSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::StopSprint);

	// Setup weapon user input
	InputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::StartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::StopFire);
	InputComponent->BindAction("Reload", IE_Pressed, this, &APlayerCharacter::ReloadWeapon);
	InputComponent->BindAction("EquipKnife", IE_Pressed, this, &APlayerCharacter::EquipKnife);
	InputComponent->BindAction("EquipPistol", IE_Pressed, this, &APlayerCharacter::EquipPistol);
	InputComponent->BindAction("EquipRifle", IE_Pressed, this, &APlayerCharacter::EquipRifle);
	InputComponent->BindAction("Pick", IE_Pressed, this, &APlayerCharacter::OnPickItem);
	InputComponent->BindAction("Throw", IE_Pressed, this, &APlayerCharacter::OnThrowItem);
	InputComponent->BindAction("Aim", IE_Pressed, this, &APlayerCharacter::OnStartAim);
	InputComponent->BindAction("Aim", IE_Released, this, &APlayerCharacter::OnStopAim);
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

void APlayerCharacter::StartWalk()
{
	SetMovementMode(ECharacterMovementMode::Walk);
}

void APlayerCharacter::StopWalk()
{
	if (!KillableComponent->bIsExhausted)
	{
		SetMovementMode(ECharacterMovementMode::Jog);
	}
}

void APlayerCharacter::StartSprint()
{
	if (!KillableComponent->bIsExhausted)
	{
		SetMovementMode(ECharacterMovementMode::Sprint);
	}
}

void APlayerCharacter::StopSprint()
{
	SetMovementMode(ECharacterMovementMode::Jog);
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
		int32 index = m_inventory->FindItemIndexById(m_weaponUser->EquippedWeapon->AmmoTypeId);
		if (-1 != index)
		{
			FInventoryItemEntry ammoItemEntry = m_inventory->GetItemEntry(index);
			int loadedCount = m_weaponUser->EquippedWeapon->LoadClip(ammoItemEntry.Quantity);
			m_inventory->RemoveItemQuantity(index, loadedCount);
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

void APlayerCharacter::OnPickItem()
{
	if (nullptr != FocusedWorldActor)
	{
		UInventoryItemComponent* inventoryItemComp = Cast<UInventoryItemComponent>(FocusedWorldActor->GetComponentByClass(UInventoryItemComponent::StaticClass()));
		if (nullptr != inventoryItemComp)
		{
			m_inventory->AddItem(inventoryItemComp);

			// Destroy world actor
			FocusedWorldActor->Destroy();
			FocusedWorldActor = nullptr;
		}
	}
}

void APlayerCharacter::OnThrowItem()
{
	if (nullptr != m_weaponUser->EquippedWeapon)
	{
		m_weaponUser->UnequipWeapon(EWeaponUnequipMethod::Throw);
	}
}

void APlayerCharacter::OnStartAim()
{
	// Set aiming fov of camera
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		ensure(PlayerController->PlayerCameraManager);
		PlayerController->PlayerCameraManager->SetFOV(AimFOVDegress);
	}
}

void APlayerCharacter::OnStopAim()
{
	// Set default fov of camera
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		ensure(PlayerController->PlayerCameraManager);
		PlayerController->PlayerCameraManager->SetFOV(RegularFOVDegress);
	}
}

void APlayerCharacter::SetCameraMode(EPlayerCameraMode NewCameraMode)
{
	CameraMode = NewCameraMode;
	RefreshCameraParams();
}

void APlayerCharacter::RefreshCameraParams()
{
	const TMap<EPlayerCameraMode, FName> CameraModeNames =
	{
		{ EPlayerCameraMode::FirstPerson, TEXT("FPSCamera") },
		{ EPlayerCameraMode::ThirdPerson, TEXT("FollowCamera") },
	};

	const FName* TargetCameraName = CameraModeNames.Find(CameraMode);
	TArray<UActorComponent*> Cameras = GetComponentsByClass(UCameraComponent::StaticClass());
	for (UActorComponent* Camera : Cameras)
	{
		Camera->SetActive(Camera->GetFName() == *TargetCameraName);
	}
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

void APlayerCharacter::RefreshFocusedWorldItem()
{
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(this);

	FHitResult hitResult;

	FVector deprojectedLocation;
	FVector deprojectedDirection;
	APlayerController* controller = Cast<APlayerController>(GetController());
	FVector2D viewportSize;
	GetWorld()->GetGameViewport()->GetViewportSize(viewportSize);
	controller->DeprojectScreenPositionToWorld(viewportSize[0] * 0.5f, viewportSize[1] * 0.5f, deprojectedLocation, deprojectedDirection);

	FVector traceStart = deprojectedLocation;
	const float traceDistance = 1000.f;
	const ECollisionChannel traceCollisionChannel = ECollisionChannel::ECC_WorldDynamic;
	FVector traceEnd = deprojectedLocation + deprojectedDirection * traceDistance;

	bool traceSucceeded = GetWorld()->LineTraceSingleByChannel(hitResult, traceStart, traceEnd, traceCollisionChannel, queryParams);
	if (traceSucceeded)
	{
		FocusedWorldActor = hitResult.GetActor();
	}
	else
	{
		FocusedWorldActor = nullptr;
	}
}
