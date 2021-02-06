#include "AI/BT_Tasks/EquipWeaponTask.h"

#include <msctf.h>

#include "AIController.h"

EBTNodeResult::Type UEquipWeaponTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	if (APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn())
	{
		UWeaponUser* WeaponUser = Pawn->FindComponentByClass<UWeaponUser>();
		if (WeaponUser)
		{
			if (bEquip)
			{
				// Weapon is already equipped, success
				if (WeaponUser->IsWeaponEquipped())
				{
					return EBTNodeResult::Succeeded;
				}
				else
				{
					// Find some ranged weapon
					if (WeaponUser->GetWeaponAtSlot(EWeaponSlotType::Rifle))
					{
						if (StartEquipWeaponSlot(OwnerComp, WeaponUser, EWeaponSlotType::Rifle))
						{
							return EBTNodeResult::InProgress;
						}
					}

					if (WeaponUser->GetWeaponAtSlot(EWeaponSlotType::Pistol))
					{
						if (StartEquipWeaponSlot(OwnerComp, WeaponUser, EWeaponSlotType::Pistol))
						{
							return EBTNodeResult::InProgress;
						}	
					}
				}
			}
			else
			{
				// Weapon is not equipped, success
				if (!WeaponUser->IsWeaponEquipped())
				{
					return EBTNodeResult::Succeeded;
				}
				else
				{
					StartUnequipWeapon(OwnerComp, WeaponUser);
					return EBTNodeResult::InProgress;
				}
			}
		}
	}

	// If no weapon already equipped, and no equip started - we failed
	return EBTNodeResult::Failed;
}

bool UEquipWeaponTask::StartEquipWeaponSlot(UBehaviorTreeComponent& OwnerComp, UWeaponUser* WeaponUser, EWeaponSlotType WeaponSlot)
{
	if (WeaponUser->EquipWeapon(WeaponSlot))
	{
		// Start equip weapon
		GetWorld()->GetTimerManager().SetTimer(EquipTimer, FTimerDelegate::CreateUObject(this, &UEquipWeaponTask::OnEquipTimerFinish, &OwnerComp), 2.f, false);

		return true;
	}

	return false;
}

void UEquipWeaponTask::StartUnequipWeapon(UBehaviorTreeComponent& OwnerComp, UWeaponUser* WeaponUser)
{
	WeaponUser->UnequipWeapon(EWeaponUnequipMethod::LeaveAtSlot);

	// Start unequip timer
	GetWorld()->GetTimerManager().SetTimer(EquipTimer, FTimerDelegate::CreateUObject(this, &UEquipWeaponTask::OnEquipTimerFinish, &OwnerComp), 2.f, false);
}

void UEquipWeaponTask::OnEquipTimerFinish(UBehaviorTreeComponent* OwnerComp) const
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
