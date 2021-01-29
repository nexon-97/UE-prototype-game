#pragma once
#include "CoreMinimal.h"
#include "NPC/Services/EnemyDetectionState.h"
#include "NPC/Components/NPCInfo.h"
#include "EnemyDetectionService.generated.h"

/*

Enemy detection service is used to automatically look for enemies within combat range

When AI doesn't see player, it is taking dot product of vector to player and vertor of sight to know if he can see the player, and the distance
is closer then some predefined distance, so the enemy is in cone of sight
If it's true, visibility raycast is issued to see if AI can actually see the player (maybe some sweep test of character sphere)
If AI sees the player, then it's detection system is processed.

State transitions:
* Idle -> EnemyIsBeingDetected (when enemy is first detected, start cooldown timer)
* EnemyIsBeingDetected -> Idle (enemy out of sight, cooldown timer expired)
* EnemyIsBeingDetected -> Combat (enemy is visible, combat)
* Combat -> Idle (enemy eliminated)
* Combat -> SearchEnemy (enemy out of sight, but not eliminated)
* SearchEnemy -> Warned (failed to find enemy)
* SearchEnemy -> Idle (enemy death reported)
* Warned -> Idle (warning cooldown timer expired)
* Warned -> Combat (enemy in sight)

*/

DECLARE_MULTICAST_DELEGATE_TwoParams(FEnemyDetectionStateChanged, UNPCInfo*, EEnemyDetectionState);

USTRUCT()
struct FEnemyDetectionInfo
{
	GENERATED_BODY()

	EEnemyDetectionState DetectionState = EEnemyDetectionState::Idle;
};

UCLASS()
class UEnemyDetectionService
	: public UObject
{
	GENERATED_BODY()

public:
	UEnemyDetectionService();

	void AddNPCInfo(UNPCInfo* NPCInfo);
	void RemoveNPCInfo(UNPCInfo* NPCInfo);

	void Tick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	EEnemyDetectionState GetDetectionState(UNPCInfo* NPCInfo) const;

public:
	FEnemyDetectionStateChanged OnEnemyDetectionStateChange;

private:
	void ChangeDetectionState(UNPCInfo* NPCInfo, EEnemyDetectionState NewState);

private:
	TMap<TWeakObjectPtr<UNPCInfo>, FEnemyDetectionInfo> LiveNPCs;
};
