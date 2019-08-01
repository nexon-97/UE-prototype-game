#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EWeaponSlotType.h"
#include "InventoryItem.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;

UCLASS()
class TESTUEPROJECT_API AWeaponBase 
	: public AActor
	, public IInventoryItem
{
	GENERATED_BODY()
	
public:	
	AWeaponBase(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon Params", meta = (DisplayName = "Shoot frequency"))
	float shootFrequency = 1.f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon Params", meta = (DisplayName = "Clip size"))
	int clipSize = 30;

private:
	int ammoCount = 0;
	FTimerHandle ShootTimer;
	USkeletalMeshSocket const* FireSocket = nullptr;
};
