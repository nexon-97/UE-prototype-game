#include "GenericCharacter.h"
#include "Animation/PlayerAnimInstance.h"
#include "Weapon/Components/WeaponUser.h"

#include "Engine.h"

namespace
{

const float WalkSpeed = 80.f;
const float JogSpeed = 400.f;
const float SprintSpeed = 600.f;

}

AGenericCharacter::AGenericCharacter(const FObjectInitializer& Initializer)
{
	PrimaryActorTick.bCanEverTick = true;

	KillableComponent = CreateDefaultSubobject<UKillableComponent>(TEXT("Killable"));
	KillableComponent->bEditableWhenInherited = true;
}

void AGenericCharacter::BeginPlay()
{
	KillableComponent->ActorExhaustedEvent.AddDynamic(this, &AGenericCharacter::OnExhausted);
	KillableComponent->ActorKilledEvent.AddDynamic(this, &AGenericCharacter::OnKilled);
	
	Super::BeginPlay();
}

void AGenericCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGenericCharacter::OnExhausted()
{
	if (CharacterMovementMode == ECharacterMovementMode::Jog || CharacterMovementMode == ECharacterMovementMode::Sprint)
	{
		SetMovementMode(ECharacterMovementMode::Walk);
	}
}

void AGenericCharacter::OnKilled()
{
	// Start death animation
	UPlayerAnimInstance* AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != AnimInstance)
	{
		AnimInstance->bIsDead = true;
	}

	// Throw all weapons
	if (UWeaponUser* WeaponUser = FindComponentByClass<UWeaponUser>())
	{
		WeaponUser->SetWeaponAtSlot(EWeaponSlotType::Knife, nullptr, EWeaponUnequipMethod::Throw);
		WeaponUser->SetWeaponAtSlot(EWeaponSlotType::Rifle, nullptr, EWeaponUnequipMethod::Throw);
		WeaponUser->SetWeaponAtSlot(EWeaponSlotType::Pistol, nullptr, EWeaponUnequipMethod::Throw);
		WeaponUser->SetWeaponAtSlot(EWeaponSlotType::Grenade, nullptr, EWeaponUnequipMethod::Throw);
	}

	// Queue ragdoll enable
	FTimerHandle RagdollEnableTimerHandle;
	GetWorldTimerManager().SetTimer(RagdollEnableTimerHandle, this, &AGenericCharacter::EnableRagdoll, 0.25f);
}

void AGenericCharacter::EnableRagdoll()
{
	// Disable capsule collision
	GetCapsuleComponent()->SetCollisionProfileName(FName("NoCollision"));

	// Enable ragdoll
	GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
}

void AGenericCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

ECharacterMovementMode AGenericCharacter::GetMovementMode() const
{
	return CharacterMovementMode;
}

void AGenericCharacter::SetMovementMode(ECharacterMovementMode NewMovementMode)
{
	if (CharacterMovementMode != NewMovementMode)
	{
		CharacterMovementMode = NewMovementMode;
		OnCharacterMovementModeChanged.Broadcast(NewMovementMode);

		switch (CharacterMovementMode)
		{
			case ECharacterMovementMode::Jog:
				GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
				break;
			case ECharacterMovementMode::Walk:
				GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
				break;
			case ECharacterMovementMode::Sprint:
				GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
				break;
		}
	}
}
