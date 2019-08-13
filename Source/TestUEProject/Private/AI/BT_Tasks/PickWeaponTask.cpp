#include "AI/BT_Tasks/PickWeaponTask.h"
#include "WeaponBase.h"
#include "Character/Components/WeaponUser.h"

#include <BehaviorTree/BlackboardComponent.h>

#include "Engine.h"

EBTNodeResult::Type UPickWeaponTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	AWeaponBase* targetWeapon = Cast<AWeaponBase>(blackboard->GetValueAsObject(FName("TargetWeaponForPick")));

	if (nullptr != targetWeapon)
	{
		AController* owner = Cast<AController>(OwnerComp.GetOwner()); // Owner is controller
		APawn* controlledPawn = owner->GetPawn();
		UWeaponUser* weaponUser = Cast<UWeaponUser>(controlledPawn->GetComponentByClass(UWeaponUser::StaticClass()));

		if (nullptr != weaponUser)
		{
			weaponUser->SetWeaponAtSlot(targetWeapon->weaponSlotType, targetWeapon);
			blackboard->SetValueAsBool(FName("HasWeapon"), true);
			blackboard->SetValueAsObject(FName("TargetWeaponForPick"), nullptr);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
