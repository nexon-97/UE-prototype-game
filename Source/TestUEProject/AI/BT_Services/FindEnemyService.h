#pragma once
#include "Engine.h"
#include "BehaviorTree/BTService.h"
#include "AI/EnemyDetectionState.h"
#include "NPC/Components/NPCInfo.h"
#include "FindEnemyService.generated.h"

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

UCLASS()
class UFindEnemyService : public UBTService
{
	GENERATED_BODY()

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector BB_EnemyDetectionState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector BB_TargetEnemy;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector BB_LastEnemyLocation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector BB_EnemyDetectionProgress;

private:
	void ChangeDetectionState(UBlackboardComponent* Blackboard, EEnemyDetectionState DetectionState) const;
	bool ActorIsSeen(AActor* Self, AActor* Target) const;
	bool ActorIsDead(AActor* Target) const;
	UNPCInfo* FindFirstEnemyAtSight(AActor* Self, const TArray<UNPCInfo*>& Enemies) const;
};
