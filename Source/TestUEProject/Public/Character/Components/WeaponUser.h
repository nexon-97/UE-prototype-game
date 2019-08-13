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
	void EquipWeapon(const EWeaponSlotType slot);

	UFUNCTION(BlueprintCallable)
	void UnequipWeapon();

	UFUNCTION(BlueprintCallable)
	void ReloadWeapon();

	UFUNCTION(BlueprintCallable)
	void SetWeaponAtSlot(const EWeaponSlotType slot, AWeaponBase* weapon);

	UFUNCTION(BlueprintCallable)
	AWeaponBase* GetWeaponAtSlot(const EWeaponSlotType slot) const;

	UFUNCTION(BlueprintCallable)
	bool HasAnyWeapon() const;

	void StartFire();
	void StopFire();

public:
	UPROPERTY(BlueprintReadOnly)
	bool IsShooting = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AWeaponBase* EquippedWeapon = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<EWeaponSlotType, AWeaponBase*> WeaponSlots;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TMap<EWeaponSlotType, FName> WeaponSlotSockets;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UMeshComponent* ActorMesh = nullptr;

	DECLARE_MULTICAST_DELEGATE_OneParam(FEquippedWeaponChangedEvent, AWeaponBase*);

	//UPROPERTY(BlueprintAssignable)
	FEquippedWeaponChangedEvent EquippedWeaponChangedEvent;

private:
	void AttachWeaponActorToOwnerSlot(AWeaponBase* weapon);
	void AttachWeaponActorToOwnerHands(AWeaponBase* weapon);
};
