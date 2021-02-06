#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class AWeaponBase;

UCLASS()
class TESTUEPROJECT_API UPlayerAnimInstance
	: public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsEquippingWeapon = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsDead = false;

private:
	void OnEquippedWeaponChanged(AWeaponBase* EquippedWeapon);
};
