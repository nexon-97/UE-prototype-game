#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "AI/EnemyDetectionState.h"

#include "ChangeDetectionStateTask.generated.h"

UCLASS()
class TESTUEPROJECT_API UChangeDetectionStateTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector BB_DetectionState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EEnemyDetectionState TargetState;
};
