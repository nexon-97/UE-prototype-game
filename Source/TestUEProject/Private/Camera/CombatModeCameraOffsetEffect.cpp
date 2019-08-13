#include "Camera/CombatModeCameraOffsetEffect.h"
#include "Camera/CameraTypes.h"

#include "Engine.h"

bool UCombatModeCameraOffsetEffect::ModifyCamera(float DeltaTime, struct FMinimalViewInfo& InOutPOV)
{
	const FVector right = InOutPOV.Rotation.Quaternion().GetRightVector();
	const FVector up = InOutPOV.Rotation.Quaternion().GetUpVector();
	const FVector offset(60.f, 0.f, 50.f);

	InOutPOV.Location += (right * offset.X + up * offset.Z);

	return Super::ModifyCamera(DeltaTime, InOutPOV);
}
