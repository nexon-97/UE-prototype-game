#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CombatModeCameraOffsetEffect.h"
#include "ProtoPlayerController.generated.h"

UCLASS()
class TESTUEPROJECT_API AProtoPlayerController
	: public APlayerController
{
	GENERATED_BODY()
	
public:
	void SpawnPlayerCameraManager() override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector CombatCameraOffset;

protected:
	void InitCameraModifiers();

protected:
	UCombatModeCameraOffsetEffect* CombatModeCameraEffect = nullptr;
};
