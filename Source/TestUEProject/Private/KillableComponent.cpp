#include "KillableComponent.h"

#include "Engine.h"

UKillableComponent::UKillableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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

		// Send kill event
		ActorKilledEvent.Broadcast(GetOwner());

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Actor killed!"));
	}
	else
	{
		Health -= hp;
	}
}
