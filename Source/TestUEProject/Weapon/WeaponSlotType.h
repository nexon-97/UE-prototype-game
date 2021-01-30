#pragma once

UENUM(BlueprintType)
enum class EWeaponSlotType : uint8
{
	Knife UMETA(DisplayName = "Knife"),
	Pistol UMETA(DisplayName = "Pistol"),
	Rifle UMETA(DisplayName = "Rifle"),
	Grenade UMETA(DisplayName = "Grenade"),
};
