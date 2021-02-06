#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraModifier.h"
#include "CombatModeCameraOffsetEffect.generated.h"

UCLASS()
class TESTUEPROJECT_API UCombatModeCameraOffsetEffect
	: public UCameraModifier
{
	GENERATED_BODY()
	
public:
	virtual void ModifyCamera(float DeltaTime, FVector ViewLocation, FRotator ViewRotation, float FOV, FVector& NewViewLocation, FRotator& NewViewRotation, float& NewFOV) override;

	UPROPERTY(EditAnywhere)
	FVector OffsetValue;
};
