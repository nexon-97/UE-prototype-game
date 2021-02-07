#include "ProtoPlayerController.h"
#include "Weapon/Components/WeaponUser.h"
#include "UI/GameHUD.h"

#include "Engine.h"

void AProtoPlayerController::SpawnPlayerCameraManager()
{
	Super::SpawnPlayerCameraManager();

	InitCameraModifiers();
}

void AProtoPlayerController::InitCameraModifiers()
{
	if (CombatCameraModifierType.Get())
	{
		CombatModeCameraEffect = Cast<UCombatModeCameraOffsetEffect>(PlayerCameraManager->AddNewCameraModifier(CombatCameraModifierType));
		CombatModeCameraEffect->EnableModifier();
	}
}

void AProtoPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);

	UWeaponUser* WeaponUser = Cast<UWeaponUser>(NewPawn->GetComponentByClass(UWeaponUser::StaticClass()));
	if (nullptr != WeaponUser)
	{
		OnEquippedWeaponChanged(WeaponUser->EquippedWeapon);

		WeaponUser->EquippedWeaponChangedEvent.AddUObject(this, &AProtoPlayerController::OnEquippedWeaponChanged);
	}
}

void AProtoPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AProtoPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Inventory", IE_Pressed, this, &AProtoPlayerController::OnInventoryToggle);
	InputComponent->BindAction("Throw", IE_Pressed, this, &AProtoPlayerController::OnItemThrow);
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

void AProtoPlayerController::OnEquippedWeaponChanged(AWeaponBase* Weapon)
{
	if (nullptr != Weapon)
	{
		if (CombatModeCameraEffect)
		{
			CombatModeCameraEffect->EnableModifier();
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

		// Limit camera pitch
		//PlayerCameraManager->ViewPitchMin = -50.f;
		//PlayerCameraManager->ViewPitchMax = 0.f;

		// Free pitch
		PlayerCameraManager->ViewPitchMin = -85.f;
		PlayerCameraManager->ViewPitchMax = 85.f;
	}
}
