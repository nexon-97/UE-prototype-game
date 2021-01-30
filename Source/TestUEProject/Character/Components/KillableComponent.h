#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KillableComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FGenericActorKilledEvent, AActor*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActorKilledEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActorDamagedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActorExhaustedEvent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TESTUEPROJECT_API UKillableComponent
	: public UActorComponent
{
	GENERATED_BODY()

public:	
	UKillableComponent();

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void AddHealth(const float HealthPoints);

	UFUNCTION(BlueprintCallable)
	void AddStamina(const float StaminaPoints);

	UFUNCTION(BlueprintCallable)
	void RemoveHealth(const float HealthPoints);

	UFUNCTION(BlueprintCallable)
	void RemoveStamina(const float StaminaPoints);

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Health = 100.f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Stamina = 100.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxHP = 100.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxStamina = 100.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HealthRestoreRate = 0.5f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float StaminaRestoreRate = 2.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsKilled = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsExhausted = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsInvulnerable = false;

	UPROPERTY(BlueprintAssignable)
	FActorKilledEvent ActorKilledEvent;

	static FGenericActorKilledEvent GenericActorKilledEvent;

	UPROPERTY(BlueprintAssignable)
	FActorExhaustedEvent ActorExhaustedEvent;

	UPROPERTY(BlueprintAssignable)
	FActorDamagedEvent OnActorDamaged;

protected:
	void RestoreHealthAndStamina(float DeltaTime);
};
