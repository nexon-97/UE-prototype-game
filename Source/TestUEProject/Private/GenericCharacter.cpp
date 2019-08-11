#include "GenericCharacter.h"
#include "Character/PlayerAnimInstance.h"

#include "Engine.h"

AGenericCharacter::AGenericCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	KillableComponent = CreateDefaultSubobject<UKillableComponent>(TEXT("Killable"));
	KillableComponent->bEditableWhenInherited = true;
	KillableComponent->ActorKilledEvent.BindUObject(this, &AGenericCharacter::OnKill);
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

void AGenericCharacter::OnKill(AActor*)
{
	UPlayerAnimInstance* animInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != animInstance)
	{
		animInstance->bIsDead = true;
	}
}
