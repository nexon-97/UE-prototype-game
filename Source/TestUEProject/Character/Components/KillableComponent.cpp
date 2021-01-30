#include "KillableComponent.h"
#include "Engine.h"

FGenericActorKilledEvent UKillableComponent::GenericActorKilledEvent;

UKillableComponent::UKillableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UKillableComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RestoreHealthAndStamina(DeltaTime);
}

void UKillableComponent::RestoreHealthAndStamina(float DeltaTime)
{
	AddHealth(HealthRestoreRate * DeltaTime);
	AddStamina(StaminaRestoreRate * DeltaTime);
}

void UKillableComponent::AddHealth(const float hp)
{
	if (bIsKilled)
		return;

	Health = FMath::Clamp(Health + FMath::Max(0.f, hp), 0.f, MaxHP);
}

void UKillableComponent::RemoveHealth(const float hp)
{
	if (bIsKilled || bIsInvulnerable)
		return;

	if (hp >= Health)
	{
		Health = 0.f;
		bIsKilled = true;

		// Disconnect from controller and release controller
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		if (nullptr != OwnerPawn)
		{
			AController* Controller = OwnerPawn->GetController();
			if ensure(Controller)
			{
				Controller->UnPossess();
				Controller->Destroy();
			}
		}

		// Send kill events
		ActorKilledEvent.Broadcast();
		GenericActorKilledEvent.Broadcast(GetOwner());

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Actor killed!"));
	}
	else
	{
		Health -= hp;
	}
}

void UKillableComponent::AddStamina(const float StaminaPoints)
{
	if (bIsKilled)
		return;

	Stamina = FMath::Clamp(Stamina + FMath::Max(0.f, StaminaPoints), 0.f, MaxStamina);
}

void UKillableComponent::RemoveStamina(const float StaminaPoints)
{
	if (bIsKilled || bIsInvulnerable)
		return;

	if (StaminaPoints >= Stamina)
	{
		Stamina = 0.f;
		bIsExhausted = true;

		// Send exhaust event
		ActorExhaustedEvent.Broadcast();
	}
	else
	{
		Stamina -= StaminaPoints;
	}
}
