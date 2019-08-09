#include "Character/Components/WeaponUser.h"
#include "GameFramework/Pawn.h"
#include "Components/InputComponent.h"

#include "Engine.h"

UWeaponUser::UWeaponUser()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponUser::BeginPlay()
{
	Super::BeginPlay();
}

void UWeaponUser::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsShooting && nullptr != EquippedWeapon)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("Shoot"));

		EquippedWeapon->Shoot();
	}
}

void UWeaponUser::SetShooting(const bool shooting)
{
	if (shooting != IsShooting)
	{
		IsShooting = shooting;
	}
}

void UWeaponUser::EquipWeapon(const EWeaponSlotType slot)
{
	AWeaponBase* weapon = nullptr;
	AWeaponBase** weaponPtr = WeaponSlots.Find(slot);
	if (nullptr != weaponPtr)
	{
		weapon = *weaponPtr;
	}

	if (weapon != EquippedWeapon)
	{
		if (nullptr != EquippedWeapon)
		{
			// Un-equip weapon
		}

		EquippedWeapon = weapon;

		if (nullptr != EquippedWeapon)
		{
			
		}
	}
}

void UWeaponUser::ReloadWeapon()
{
	if (nullptr != EquippedWeapon)
	{
		EquippedWeapon->LoadClip(1);
	}
}

void UWeaponUser::SetWeaponAtSlot(const EWeaponSlotType slot, AWeaponBase* weapon)
{
	AWeaponBase** currentWeaponAtSlot = WeaponSlots.Find(slot);
	bool removeSlot = (nullptr == weapon);
	bool addSlot = (nullptr != currentWeaponAtSlot) && !removeSlot;

	if (nullptr != currentWeaponAtSlot)
	{
		// Remove current weapon from slot...


		WeaponSlots.Remove(slot);
	}

	if (nullptr != weapon)
	{
		WeaponSlots.Add(slot, weapon);

		AttachWeaponActorToOwner(weapon);
	}

	// Emit event, that weapon at slot has changed
}

AWeaponBase* UWeaponUser::GetWeaponAtSlot(const EWeaponSlotType slot) const
{
	AWeaponBase* const* currentWeaponAtSlot = WeaponSlots.Find(slot);
	if (nullptr != currentWeaponAtSlot)
	{
		return *currentWeaponAtSlot;
	}

	return nullptr;
}

void UWeaponUser::StartFire()
{
	SetShooting(true);
}

void UWeaponUser::StopFire()
{
	SetShooting(false);
}

void UWeaponUser::AttachWeaponActorToOwner(AWeaponBase* weapon)
{
	// Disable weapon physics
	weapon->weaponCollision->SetSimulatePhysics(false);
	weapon->weaponMesh->SetSimulatePhysics(false);

	// Attach weapon actor to a socket on owner
	FName* socketName = WeaponSlotSockets.Find(weapon->weaponSlotType);
	if (nullptr != socketName)
	{
		FAttachmentTransformRules attachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
		weapon->AttachToComponent(ActorMesh, attachmentRules, *socketName);

		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("Weapon attached"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Socket binding for weapon slot not found"));
	}
}
