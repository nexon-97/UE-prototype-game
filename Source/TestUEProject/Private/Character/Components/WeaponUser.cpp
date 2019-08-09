#include "Character/Components/WeaponUser.h"

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
}

void UWeaponUser::SetShooting(const bool shooting)
{
	if (shooting != IsShooting)
	{
		IsShooting = shooting;
	}
}

void UWeaponUser::EquipWeapon(AWeaponBase* weapon)
{

}

void UWeaponUser::ReloadWeapon()
{
	if (nullptr != EquippedWeapon)
	{

	}
}

void UWeaponUser::SetWeaponAtSlot(const EWeaponSlotType slot, AWeaponBase* weapon)
{
	AWeaponBase** currentWeaponAtSlot = WeaponSlots.Find(slot);
	if (nullptr != currentWeaponAtSlot)
	{
		*currentWeaponAtSlot = weapon;
	}
	else
	{
		WeaponSlots.Add(slot, weapon);
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
