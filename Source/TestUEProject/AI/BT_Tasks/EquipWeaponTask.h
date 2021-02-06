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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bEquip = true;

private:
	bool StartEquipWeaponSlot(UBehaviorTreeComponent& OwnerComp, UWeaponUser* WeaponUser, EWeaponSlotType WeaponSlot);
	void StartUnequipWeapon(UBehaviorTreeComponent& OwnerComp, UWeaponUser* WeaponUser);

	void OnEquipTimerFinish(UBehaviorTreeComponent* OwnerComp) const;

	FTimerHandle EquipTimer;
};
