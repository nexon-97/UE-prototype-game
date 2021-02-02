#include "AI/BT_Tasks/ReloadWeaponTask.h"
#include "Weapon/Components/WeaponUser.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UReloadWeaponTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UWeaponUser* WeaponUser = OwnerComp.GetAIOwner()->GetPawn()->FindComponentByClass<UWeaponUser>();
	if (WeaponUser && WeaponUser->EquippedWeapon)
	{
		// Load full clip if not loaded (AI players cheat, don't need to take ammo from inventory)
		const int32 AmmoCountToLoad = WeaponUser->EquippedWeapon->clipSize - WeaponUser->EquippedWeapon->GetAmmoCountInClip();
		if (AmmoCountToLoad > 0)
		{
			const TWeakObjectPtr<AWeaponBase> WeaponWeak = WeaponUser->EquippedWeapon;
			const FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UReloadWeaponTask::OnReloadTimerFinish, WeaponWeak, AmmoCountToLoad, &OwnerComp);
			GetWorld()->GetTimerManager().SetTimer(ReloadTimer, TimerDelegate, 2.f, false);

			return EBTNodeResult::InProgress;
		}
		else
		{
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}

void UReloadWeaponTask::OnReloadTimerFinish(TWeakObjectPtr<AWeaponBase> Weapon, int32 AmmoCountToLoad, UBehaviorTreeComponent* OwnerComp) const
{
	if (AWeaponBase* WeaponPtr = Weapon.Get())
	{
		// Load ammo into weapon
		WeaponPtr->LoadClip(AmmoCountToLoad);
		// Update blackboard value so AI would get updated ammo count
		OwnerComp->GetBlackboardComponent()->SetValueAsInt(BB_NumAmmoInClip.SelectedKeyName, WeaponPtr->GetAmmoCountInClip());
		
		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		FinishLatentTask(*OwnerComp, EBTNodeResult::Aborted);
	}
}
