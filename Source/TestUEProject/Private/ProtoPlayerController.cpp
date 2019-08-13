#include "ProtoPlayerController.h"
#include "Character/Components/WeaponUser.h"

#include "Engine.h"

void AProtoPlayerController::SpawnPlayerCameraManager()
{
	Super::SpawnPlayerCameraManager();

	InitCameraModifiers();
}

void AProtoPlayerController::InitCameraModifiers()
{
	CombatModeCameraEffect = Cast<UCombatModeCameraOffsetEffect>(PlayerCameraManager->AddNewCameraModifier(UCombatModeCameraOffsetEffect::StaticClass()));
	CombatModeCameraEffect->EnableModifier();
}

void AProtoPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	UWeaponUser* weaponUser = Cast<UWeaponUser>(aPawn->GetComponentByClass(UWeaponUser::StaticClass()));
	if (nullptr != weaponUser)
	{
		OnEquippedWeaponChanged(weaponUser->EquippedWeapon);

		weaponUser->EquippedWeaponChangedEvent.AddUObject(this, &AProtoPlayerController::OnEquippedWeaponChanged);
	}
}

void AProtoPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AProtoPlayerController::OnEquippedWeaponChanged(AWeaponBase* weapon)
{
	if (nullptr != weapon)
	{
		CombatModeCameraEffect->EnableModifier();

		// Free pitch
		PlayerCameraManager->ViewPitchMin = -85.f;
		PlayerCameraManager->ViewPitchMax = 85.f;
	}
	else
	{
		CombatModeCameraEffect->DisableModifier();

		// Limit camera pitch
		PlayerCameraManager->ViewPitchMin = -50.f;
		PlayerCameraManager->ViewPitchMax = 0.f;
	}
}
