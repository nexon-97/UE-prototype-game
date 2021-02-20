#include "ProtoPlayerController.h"
#include "Weapon/Components/WeaponUser.h"
#include "UI/GameHUD.h"

#include "Engine.h"

void AProtoPlayerController::SpawnPlayerCameraManager()
{
	Super::SpawnPlayerCameraManager();

	PlayerCameraManager->DefaultFOV = 90.f;
	InitCameraModifiers();
}

void AProtoPlayerController::InitCameraModifiers()
{
	if (IdleCameraModifierType.Get())
	{
		IdleCameraModifier = PlayerCameraManager->AddNewCameraModifier(IdleCameraModifierType);
		IdleCameraModifier->EnableModifier();
	}
	
	if (CombatCameraModifierType.Get())
	{
		CombatModeCameraEffect = Cast<UCombatModeCameraOffsetEffect>(PlayerCameraManager->AddNewCameraModifier(CombatCameraModifierType));
		CombatModeCameraEffect->DisableModifier();
	}

	if (AimCameraModifierType.Get())
	{
		AimCameraModifier = PlayerCameraManager->AddNewCameraModifier(AimCameraModifierType);
		AimCameraModifier->DisableModifier();
	}
}

void AProtoPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);

	ControlledCharacter = Cast<APlayerCharacter>(NewPawn);
	check(ControlledCharacter);

	WeaponUser = NewPawn->FindComponentByClass<UWeaponUser>();
	if (WeaponUser)
	{
		OnEquippedWeaponChanged(WeaponUser->EquippedWeapon);
		WeaponUser->EquippedWeaponChangedEvent.AddUObject(this, &AProtoPlayerController::OnEquippedWeaponChanged);
	}
}

void AProtoPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	ControlledCharacter = nullptr;
	WeaponUser = nullptr;
}

void AProtoPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Inventory", IE_Pressed, this, &AProtoPlayerController::OnInventoryToggle);
	InputComponent->BindAction("Throw", IE_Pressed, this, &AProtoPlayerController::OnItemThrow);
	InputComponent->BindAction("Aim", IE_Pressed, this, &AProtoPlayerController::OnStartAim);
	InputComponent->BindAction("Aim", IE_Released, this, &AProtoPlayerController::OnStopAim);
	InputComponent->BindAction("Walk", IE_Pressed, this, &AProtoPlayerController::StartWalk);
	InputComponent->BindAction("Walk", IE_Released, this, &AProtoPlayerController::StopWalk);
	InputComponent->BindAction("Sprint", IE_Pressed, this, &AProtoPlayerController::StartSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &AProtoPlayerController::StopSprint);
	InputComponent->BindAction("EquipKnife", IE_Pressed, this, &AProtoPlayerController::EquipKnife);
	InputComponent->BindAction("EquipPistol", IE_Pressed, this, &AProtoPlayerController::EquipPistol);
	InputComponent->BindAction("EquipRifle", IE_Pressed, this, &AProtoPlayerController::EquipRifle);
}

void AProtoPlayerController::OnInventoryToggle()
{
	if (AGameHUD* Hud = Cast<AGameHUD>(GetHUD()))
	{
		Hud->OnOpenInventory();
	}
}

void AProtoPlayerController::OnItemThrow()
{

}

void AProtoPlayerController::StartWalk()
{
	if (ControlledCharacter)
	{
		ControlledCharacter->SetMovementMode(ECharacterMovementMode::Walk);
	}
}

void AProtoPlayerController::StopWalk()
{
	if (ControlledCharacter)
	{
		UKillableComponent* Killable = ControlledCharacter->FindComponentByClass<UKillableComponent>();
		if (!Killable->bIsExhausted)
		{
			ControlledCharacter->SetMovementMode(ECharacterMovementMode::Jog);
		}
	}
}

void AProtoPlayerController::StartSprint()
{
	if (ControlledCharacter)
	{
		UKillableComponent* Killable = ControlledCharacter->FindComponentByClass<UKillableComponent>();
		if (!Killable->bIsExhausted)
		{
			ControlledCharacter->SetMovementMode(ECharacterMovementMode::Sprint);
		}
	}
}

void AProtoPlayerController::StopSprint()
{
	if (ControlledCharacter)
	{
		ControlledCharacter->SetMovementMode(ECharacterMovementMode::Jog);
	}
}

void AProtoPlayerController::EquipKnife()
{
	WeaponUser->EquipWeapon(EWeaponSlotType::Knife);
}

void AProtoPlayerController::EquipPistol()
{
	WeaponUser->EquipWeapon(EWeaponSlotType::Pistol);
}

void AProtoPlayerController::EquipRifle()
{
	WeaponUser->EquipWeapon(EWeaponSlotType::Rifle);
}

void AProtoPlayerController::OnEquippedWeaponChanged(AWeaponBase* Weapon)
{
	if (nullptr != Weapon)
	{
		if (CombatModeCameraEffect)
		{
			CombatModeCameraEffect->EnableModifier();
		}

		if (IdleCameraModifier)
		{
			IdleCameraModifier->DisableModifier();
		}

		// Free pitch
		PlayerCameraManager->ViewPitchMin = -85.f;
		PlayerCameraManager->ViewPitchMax = 85.f;
	}
	else
	{
		if (CombatModeCameraEffect)
		{
			CombatModeCameraEffect->DisableModifier();
		}

		if (IdleCameraModifier)
		{
			IdleCameraModifier->EnableModifier();
		}

		// Free pitch
		PlayerCameraManager->ViewPitchMin = -85.f;
		PlayerCameraManager->ViewPitchMax = 85.f;
	}
}

void AProtoPlayerController::OnStartAim()
{
	if (AimCameraModifier && WeaponUser->IsWeaponEquipped())
	{
		AimCameraModifier->EnableModifier();
	}
}

void AProtoPlayerController::OnStopAim()
{
	if (AimCameraModifier && !AimCameraModifier->IsDisabled())
	{
		AimCameraModifier->DisableModifier();
	}
}
