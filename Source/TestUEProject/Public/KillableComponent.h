#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KillableComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TESTUEPROJECT_API UKillableComponent
	: public UActorComponent
{
	GENERATED_BODY()

public:	
	UKillableComponent();

	UFUNCTION(BlueprintCallable)
	void AddHealth(const float hp);

	UFUNCTION(BlueprintCallable)
	void RemoveHealth(const float hp);

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Health = 100.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxHP = 100.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsKilled = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsInvulnerable = false;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorKilledEvent, AActor*, KilledActor);

	UPROPERTY(BlueprintAssignable)
	FActorKilledEvent ActorKilledEvent;
};
