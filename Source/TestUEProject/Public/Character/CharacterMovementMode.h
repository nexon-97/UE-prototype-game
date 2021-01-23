#pragma once

UENUM(BlueprintType)
enum class ECharacterMovementMode : uint8
{
	Walk UMETA(DisplayName = "Walk"),
	Jog UMETA(DisplayName = "Jog"),
	Sprint UMETA(DisplayName = "Sprint"),
	Crouch UMETA(DisplayName = "Crouch"),
	Creep UMETA(DisplayName = "Creep"),
	Ladder UMETA(DisplayName = "Ladder"),
	Swim UMETA(DisplayName = "Swim"),
};
