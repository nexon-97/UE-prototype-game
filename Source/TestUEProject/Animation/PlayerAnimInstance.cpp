#include "PlayerAnimInstance.h"
#include "Weapon/Components/WeaponUser.h"

void UPlayerAnimInstance::NativeBeginPlay()
{
	if (UWeaponUser* WeaponUser = GetOwningActor()->FindComponentByClass<UWeaponUser>())
	{
		WeaponUser->EquippedWeaponChangedEvent.AddUObject(this, &UPlayerAnimInstance::OnEquippedWeaponChanged);
	}
}

void UPlayerAnimInstance::OnEquippedWeaponChanged(AWeaponBase* EquippedWeapon)
{
	bIsEquippingWeapon = (nullptr != EquippedWeapon);
}
