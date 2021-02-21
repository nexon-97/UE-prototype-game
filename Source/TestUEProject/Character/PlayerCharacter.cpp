#include "Character/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Weapon/Components/WeaponUser.h"
#include "Character/Components/InventoryComponent.h"
#include "Animation/PlayerAnimInstance.h"
#include "Components/InputComponent.h"
#include "Data/InventoryItemDef.h"
#include "Items/Components/InventoryItemComponent.h"

#include "Engine.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& Initializer)
{
	WeaponUser = CreateDefaultSubobject<UWeaponUser>(TEXT("WeaponUser"));
	WeaponUser->bEditableWhenInherited = true;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false);

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraSpringArm->bEditableWhenInherited = true;
	CameraSpringArm->AttachToComponent(RootComponent, AttachmentRules);

	// FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	// FirstPersonCamera->bEditableWhenInherited = true;
	// FirstPersonCamera->AttachToComponent(RootComponent, AttachmentRules);
	// FirstPersonCamera->SetActive(false);

	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	ThirdPersonCamera->bEditableWhenInherited = true;
	ThirdPersonCamera->AttachToComponent(CameraSpringArm, AttachmentRules);
	ThirdPersonCamera->SetActive(false);

	GetMesh()->SetAnimInstanceClass(UPlayerAnimInstance::StaticClass());

	WeaponUser->ActorMesh = Cast<UMeshComponent>(GetMesh());

	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
}

void APlayerCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	RefreshCameraParams();
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
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

	if (bIsShooting && WeaponUser->EquippedWeapon)
	{
		// Run line trace to determine target to hit
		const float TraceDistance = 30000.f; // 300 Meters ahead
		FVector DeprojectedLocation;
		FVector DeprojectedDirection;
		FVector2D ViewportSize;
		GetWorld()->GetGameViewport()->GetViewportSize(ViewportSize);
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		PlayerController->DeprojectScreenPositionToWorld(ViewportSize[0] * 0.5f, ViewportSize[1] * 0.5f, DeprojectedLocation, DeprojectedDirection);

		FVector TraceStart = DeprojectedLocation;
		const FVector TraceEnd = DeprojectedLocation + DeprojectedDirection * TraceDistance;
		
		WeaponUser->EquippedWeapon->TryShootAtLocation(TraceEnd);
	}

	// Refresh movement detection
	const FVector MovementDelta = GetActorLocation() - LastCharacterLocation;
	const bool bHasMovedLastFrame = !MovementDelta.IsNearlyZero();
	bIsMoving = bHasMovedLastFrame;
	LastCharacterLocation = GetActorLocation();

	// Make moving character follow control rotation yaw
	bUseControllerRotationYaw = bIsMoving;
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* InInputComponent)
{
	Super::SetupPlayerInputComponent(InInputComponent);

	InInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	InInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	InInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
	InInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);

	InInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::StartJump);
	InInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJump);

	// Setup weapon user input
	InInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::StartFire);
	InInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::StopFire);
	InInputComponent->BindAction("Reload", IE_Pressed, this, &APlayerCharacter::ReloadWeapon);
	InInputComponent->BindAction("Pick", IE_Pressed, this, &APlayerCharacter::OnPickItem);
}

void APlayerCharacter::MoveForward(float AxisValue)
{
	FRotator Rotation = GetControlRotation();
	Rotation.Roll = 0.f;
	Rotation.Pitch = 0.f;

	//bIsMoving = !FMath::IsNearlyZero(AxisValue);
	// RefreshControllerControlYawSync();

	AddMovementInput(Rotation.Quaternion().GetForwardVector(), AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	FRotator Rotation = GetControlRotation();
	Rotation.Roll = 0.f;
	Rotation.Pitch = 0.f;

	AddMovementInput(Rotation.Quaternion().GetRightVector(), AxisValue);
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
	bIsShooting = WeaponUser->IsWeaponEquipped();
}

void APlayerCharacter::StopFire()
{
	bIsShooting = false;
}

void APlayerCharacter::ReloadWeapon()
{
	if (nullptr != WeaponUser->EquippedWeapon)
	{
		// Find ammo in inventory
		int32 index = Inventory->FindItemIndexById(WeaponUser->EquippedWeapon->AmmoTypeId);
		if (-1 != index)
		{
			FInventoryItemEntry ammoItemEntry = Inventory->GetItemEntry(index);
			int loadedCount = WeaponUser->EquippedWeapon->LoadClip(ammoItemEntry.Quantity);
			Inventory->RemoveItemQuantity(index, loadedCount);
		}
	}
}

UInventoryComponent* APlayerCharacter::GetInventory() const
{
	return Inventory;
}

bool APlayerCharacter::IsMoving() const
{
	return bIsMoving;
}

void APlayerCharacter::OnPickItem()
{
	if (nullptr != FocusedWorldActor)
	{
		UInventoryItemComponent* inventoryItemComp = Cast<UInventoryItemComponent>(FocusedWorldActor->GetComponentByClass(UInventoryItemComponent::StaticClass()));
		if (nullptr != inventoryItemComp)
		{
			Inventory->AddItem(inventoryItemComp);

			// Destroy world actor
			FocusedWorldActor->Destroy();
			FocusedWorldActor = nullptr;
		}
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

void APlayerCharacter::RefreshControllerControlYawSync()
{
	if (bUseControllerRotationYaw != bIsMoving)
	{
		bUseControllerRotationYaw = bIsMoving;
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
