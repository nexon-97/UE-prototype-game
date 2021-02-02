#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Weapon/WeaponBase.h"
#include "ReloadWeaponTask.generated.h"

UCLASS()
class UReloadWeaponTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector BB_NumAmmoInClip;

private:
	void OnReloadTimerFinish(TWeakObjectPtr<AWeaponBase> Weapon, int32 AmmoCountToLoad, UBehaviorTreeComponent* OwnerComp) const;
	
	FTimerHandle ReloadTimer;
};
