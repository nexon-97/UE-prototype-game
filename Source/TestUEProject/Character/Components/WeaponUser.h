#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/WeaponBase.h"
#include "WeaponUser.generated.h"

UENUM(BlueprintType)
enum class EWeaponUnequipMethod : uint8
{
	HideToInventory UMETA(DisplayName = "Hide to inventory"),
	LeaveAtSlot UMETA(DisplayName = "Leave at slot"),
	Throw UMETA(DisplayName = "Throw"),
};

/*
* @brief Weapon user handles several weapon slots, and single equipped weapon, which is actually used
*/
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
	void UnequipWeapon(EWeaponUnequipMethod unequipMethod = EWeaponUnequipMethod::HideToInventory);

	UFUNCTION(BlueprintCallable)
	void SetWeaponAtSlot(const EWeaponSlotType slot, AWeaponBase* weapon, EWeaponUnequipMethod unequipMethod = EWeaponUnequipMethod::HideToInventory);

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
	void ThrowWeapon(AWeaponBase* weapon);
	void HideWeaponToInventory(AWeaponBase* weapon);
};
