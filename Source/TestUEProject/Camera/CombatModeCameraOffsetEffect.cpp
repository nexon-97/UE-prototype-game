#include "Camera/CombatModeCameraOffsetEffect.h"
#include "Engine.h"

void UCombatModeCameraOffsetEffect::ModifyCamera(float DeltaTime, FVector ViewLocation, FRotator ViewRotation, float FOV, FVector& NewViewLocation, FRotator& NewViewRotation, float& NewFOV)
{
	const FQuat RotationQuat = ViewRotation.Quaternion();
	const FVector Right = RotationQuat.GetRightVector();
	const FVector Up = RotationQuat.GetUpVector();

	NewViewLocation = ViewLocation + Alpha * (Right * OffsetValue.X + Up * OffsetValue.Z);
}
