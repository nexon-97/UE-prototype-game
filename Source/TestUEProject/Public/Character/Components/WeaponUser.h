#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponBase.h"
#include "WeaponUser.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TESTUEPROJECT_API UWeaponUser
	: public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponUser();

protected:
	void BeginPlay() override;

public:	
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetShooting(const bool shooting);

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(AWeaponBase* weapon);

	UFUNCTION(BlueprintCallable)
	void ReloadWeapon();

	UFUNCTION(BlueprintCallable)
	void SetWeaponAtSlot(const EWeaponSlotType slot, AWeaponBase* weapon);

	UFUNCTION(BlueprintCallable)
	AWeaponBase* GetWeaponAtSlot(const EWeaponSlotType slot) const;

public:
	UPROPERTY(BlueprintReadOnly)
	bool IsShooting = false;

	UPROPERTY(BlueprintReadOnly)
	AWeaponBase* EquippedWeapon = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TMap<EWeaponSlotType, AWeaponBase*> WeaponSlots;
};
