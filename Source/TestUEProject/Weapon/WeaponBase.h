#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/WeaponSlotType.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;
class UInventoryItemComponent;

UCLASS()
class TESTUEPROJECT_API AWeaponBase 
	: public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	bool CanShoot() const;

	UFUNCTION(BlueprintCallable)
	int GetAmmoCountInClip() const;

	// Try shoot at given location
	// @return True if shot has been made
	UFUNCTION(BlueprintCallable)
	virtual bool TryShootAtLocation(const FVector& TargetLocation);

	// Loads cartridges to the clip
	// @param ammoCount - cartridges count to load
	// @return Actually loaded cartridges count 
	UFUNCTION(BlueprintCallable)
	int LoadClip(int32 LoadCount);

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UBoxComponent* weaponCollision;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UInventoryItemComponent* InventoryItemData;

	/* Inventory ID of the weapon */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (DisplayName = "Inventory ID"))
	FName InventoryId;

	/* Inventory ID of ammo, that can be used for this weapon */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (DisplayName = "Ammo type inventory ID"))
	FName AmmoTypeId;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Weapon Params", meta=(DisplayName="Weapon slot type"))
	EWeaponSlotType weaponSlotType = EWeaponSlotType::Knife;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Params", meta = (DisplayName = "Shoot frequency"))
	float shootFrequency = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon Params", meta = (DisplayName = "Clip size"))
	int clipSize = 30;

	/* Weapon state in percents. 100 is totally fine state, 0 is broken state. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Health State"))
	float WeaponState = 100.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Can be picked"))
	// Indicates if the weapon can be targeted by AI for picking
	bool CanBePicked = true;

private:
	int AmmoCount = 0;
	
	FTimerHandle ShootTimer;
	
	UPROPERTY(Transient)
	USkeletalMeshSocket const* FireSocket = nullptr;
};
