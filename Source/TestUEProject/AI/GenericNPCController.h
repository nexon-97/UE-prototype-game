#pragma once
#include "AIController.h"
#include "GenericNPCController.generated.h"

UCLASS(Blueprintable)
class TESTUEPROJECT_API AGenericNPCController
	: public AAIController
{
	GENERATED_BODY()

public:
	AGenericNPCController(const FObjectInitializer& ObjectInitializer);
	
	void BeginPlay() override;
	
	void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool EquipBestWeapon() const;
	UFUNCTION(BlueprintCallable)
	void HideWeapon() const;

	void AttackActor(AActor* TargetActor);

	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;

public:
	UPROPERTY(EditDefaultsOnly)
	bool bCanUseWeapon;

	UPROPERTY(EditDefaultsOnly)
	bool bCanTrade;

	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly)
	UBlackboardData* BlackboardAsset;

private:
	UPROPERTY(Transient)
	class UNPCInfo* ThisNPCInfo;
	UPROPERTY(Transient)
	class UWeaponUser* ThisWeaponUser;
	UPROPERTY(Transient)
	class UBlackboardComponent* ThisBlackboard;
};
