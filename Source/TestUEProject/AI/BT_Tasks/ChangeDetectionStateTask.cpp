#include "AI/BT_Tasks/ChangeDetectionStateTask.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UChangeDetectionStateTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(BB_DetectionState.SelectedKeyName, static_cast<uint8>(TargetState));
	return EBTNodeResult::Succeeded;
}
