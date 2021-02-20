#pragma once

UENUM(BlueprintType)
enum class EItemClass : uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Ammo UMETA(DisplayName = "Ammo"),
	Food UMETA(DisplayName = "Food"),
	Props UMETA(DisplayName = "Props"),
	Cloth UMETA(DisplayName = "Cloth"),
	Misc UMETA(DisplayName = "Misc"),
};
