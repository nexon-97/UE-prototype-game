#pragma once

UENUM(BlueprintType)
enum class EEnemyDetectionState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	EnemyIsBeingDetected UMETA(DisplayName = "EnemyIsBeingDetected"),
	Combat UMETA(DisplayName = "Combat"),
	SearchEnemy UMETA(DisplayName = "SearchEnemy"),
	Warned UMETA(DisplayName = "Warned"),
};
