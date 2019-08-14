#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EWeaponSlotType.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;

UCLASS()
class TESTUEPROJECT_API AWeaponBase 
	: public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	bool CanShoot() const;

	UFUNCTION(BlueprintCallable)
	int GetAmmoCountInClip() const;

	UFUNCTION(BlueprintCallable)
	virtual bool Shoot();

	// Loads cartridges to the clip
	// @param ammoCount - cartridges count to load
	// @return Actually loaded cartridges count 
	UFUNCTION(BlueprintCallable)
	int LoadClip(int ammoCount);

protected:
	virtual void ShootInternal();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* weaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* weaponCollision;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Weapon Params", meta=(DisplayName="Weapon slot type"))
	EWeaponSlotType weaponSlotType = EWeaponSlotType::Knife;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Params", meta = (DisplayName = "Shoot frequency"))
	float shootFrequency = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Params", meta = (DisplayName = "Clip size"))
	int clipSize = 30;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Can be picked"))
	// Indicates if the weapon can be targeted by AI for picking
	bool CanBePicked = true;

	/* Inventory ID of the weapon */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (DisplayName = "Inventory ID"))
	FName InventoryId;

	/* Inventory ID of ammo, that can be used for this weapon */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (DisplayName = "Ammo type inventory ID"))
	FName AmmoTypeId;

private:
	int ammoCount = 0;
	FTimerHandle ShootTimer;
	USkeletalMeshSocket const* FireSocket = nullptr;
};
