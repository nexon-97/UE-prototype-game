#include "ProtoPlayerController.h"

#include "Engine.h"

void AProtoPlayerController::SpawnPlayerCameraManager()
{
	Super::SpawnPlayerCameraManager();

	InitCameraModifiers();

	// Limit camera pitch
	PlayerCameraManager->ViewPitchMin = -50.f;
	PlayerCameraManager->ViewPitchMax = 0.f;
}

void AProtoPlayerController::InitCameraModifiers()
{
	CombatModeCameraEffect = Cast<UCombatModeCameraOffsetEffect>(PlayerCameraManager->AddNewCameraModifier(UCombatModeCameraOffsetEffect::StaticClass()));
	CombatModeCameraEffect->EnableModifier();
}
