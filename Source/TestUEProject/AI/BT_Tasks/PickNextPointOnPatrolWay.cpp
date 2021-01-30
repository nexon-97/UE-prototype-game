#include "AI/BT_Tasks/PickNextPointOnPatrolWay.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <Components/SplineComponent.h>

#include "AIController.h"
#include "Engine.h"

namespace
{

const FName PatrolSplineKey = FName("PatrolSpline");
const FName TargetLocationKey = FName("TargetLocation");
const FName NextPatrolPointIndexKey = FName("NextPatrolPointIndex");
const float PatrolMovementGranularity = 500.f; // Define new patrol point every two meters

}

EBTNodeResult::Type UPickNextPointOnPatrolWay::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	USplineComponent* PatrolSpline = Cast<USplineComponent>(Blackboard->GetValueAsObject(PatrolSplineKey));

	if (nullptr != PatrolSpline)
	{
		int32 TargetPatrolPointIndex = Blackboard->GetValueAsInt(NextPatrolPointIndexKey);
		const int32 NumPoints = PatrolSpline->GetNumberOfSplinePoints();

		if (TargetPatrolPointIndex < 0)
		{
			// Pick nearest point
			const float ClosestSplineKey = PatrolSpline->FindInputKeyClosestToWorldLocation(Pawn->GetActorLocation());
			TargetPatrolPointIndex = FMath::CeilToInt(ClosestSplineKey) % NumPoints;
		}
		else
		{
			TargetPatrolPointIndex = (TargetPatrolPointIndex < NumPoints - 1) ? (TargetPatrolPointIndex + 1) : 0;
		}

		const FVector TargetLocation = PatrolSpline->GetWorldLocationAtSplinePoint(TargetPatrolPointIndex);

		// Update blackboard values
		Blackboard->SetValueAsVector(TargetLocationKey, TargetLocation);
		Blackboard->SetValueAsInt(NextPatrolPointIndexKey, TargetPatrolPointIndex);

		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
