#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "Weapon/Components/WeaponUser.h"
#include "EquipWeaponTask.generated.h"

UCLASS()
class UEquipWeaponTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool StartEquipWeaponSlot(UBehaviorTreeComponent& OwnerComp, UWeaponUser* WeaponUser, EWeaponSlotType WeaponSlot);

	void OnEquipTimerFinish(UBehaviorTreeComponent* OwnerComp) const;

	FTimerHandle EquipTimer;
};
