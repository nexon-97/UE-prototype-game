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
	bool ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV) override;
};
