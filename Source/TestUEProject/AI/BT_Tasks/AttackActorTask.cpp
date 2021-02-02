#include "AI/BT_Tasks/AttackActorTask.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Weapon/Components/WeaponUser.h"

EBTNodeResult::Type UAttackActorTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UWeaponUser* WeaponUser = OwnerComp.GetAIOwner()->GetPawn()->FindComponentByClass<UWeaponUser>();
	if (WeaponUser && WeaponUser->IsWeaponEquipped())
	{
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BB_TargetActor.SelectedKeyName));
		if (TargetActor && WeaponUser->EquippedWeapon->TryShootAtLocation(TargetActor->GetActorLocation()))
		{
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
