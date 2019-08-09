#include "GenericCharacter.h"

AGenericCharacter::AGenericCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGenericCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGenericCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGenericCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AGenericCharacter::GetHealth() const
{
	return m_health;
}
