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
	UWeaponUser(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	bool EquipWeapon(const EWeaponSlotType Slot);

	UFUNCTION(BlueprintCallable)
	void UnequipWeapon(EWeaponUnequipMethod UnequipMethod = EWeaponUnequipMethod::HideToInventory);

	UFUNCTION(BlueprintCallable)
	bool IsWeaponEquipped() const;

	UFUNCTION(BlueprintCallable)
	void SetWeaponAtSlot(const EWeaponSlotType Slot, AWeaponBase* Weapon, EWeaponUnequipMethod UnequipMethod = EWeaponUnequipMethod::HideToInventory);

	UFUNCTION(BlueprintCallable)
	AWeaponBase* GetWeaponAtSlot(const EWeaponSlotType slot) const;

	UFUNCTION(BlueprintCallable)
	bool HasAnyWeapon() const;

public:
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
	void AttachWeaponActorToOwnerSlot(AWeaponBase* Weapon);
	void AttachWeaponActorToOwnerHands(AWeaponBase* Weapon) const;
	void ThrowWeapon(AWeaponBase* Weapon) const;
	void HideWeaponToInventory(AWeaponBase* weapon) const;
};
