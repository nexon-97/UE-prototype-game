#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "AttackActorTask.generated.h"

UCLASS()
class UAttackActorTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector BB_TargetActor;
};
