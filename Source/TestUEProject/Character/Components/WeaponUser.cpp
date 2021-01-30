#include "Character/Components/WeaponUser.h"
#include "Character/PlayerAnimInstance.h"
#include "GameFramework/Pawn.h"
#include "Components/InputComponent.h"
#include "Character/Components/InventoryComponent.h"

#include "Engine.h"

UWeaponUser::UWeaponUser()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponUser::BeginPlay()
{
	Super::BeginPlay();

	// If there are weapon slots assigned, actually attach them to the actor sockets
	for (auto& weaponSlotData : WeaponSlots)
	{
		if (nullptr == weaponSlotData.Value)
			continue;
		
		if (weaponSlotData.Value == EquippedWeapon)
		{
			AttachWeaponActorToOwnerHands(weaponSlotData.Value);
		}
		else
		{
			AttachWeaponActorToOwnerSlot(weaponSlotData.Value);
		}
	}
}

void UWeaponUser::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsShooting && nullptr != EquippedWeapon)
	{
		EquippedWeapon->Shoot();
	}
}

void UWeaponUser::SetShooting(const bool shooting)
{
	if (shooting != IsShooting)
	{
		IsShooting = shooting;
	}
}

void UWeaponUser::EquipWeapon(const EWeaponSlotType slot)
{
	if (nullptr != EquippedWeapon && EquippedWeapon->weaponSlotType == slot)
	{
		UnequipWeapon(EWeaponUnequipMethod::LeaveAtSlot);
	}
	else
	{
		AWeaponBase* weapon = nullptr;
		AWeaponBase** weaponPtr = WeaponSlots.Find(slot);
		if (nullptr != weaponPtr)
		{
			weapon = *weaponPtr;
		}

		if (weapon != EquippedWeapon)
		{
			UnequipWeapon(EWeaponUnequipMethod::LeaveAtSlot);

			if (nullptr != weapon)
			{
				AttachWeaponActorToOwnerHands(weapon);
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("Weapon equipped"));

				EquippedWeapon = weapon;
				EquippedWeaponChangedEvent.Broadcast(EquippedWeapon);
			}
		}
	}
}

void UWeaponUser::UnequipWeapon(EWeaponUnequipMethod unequipMethod)
{
	if (nullptr != EquippedWeapon)
	{
		switch (unequipMethod)
		{
		case EWeaponUnequipMethod::HideToInventory:
			HideWeaponToInventory(EquippedWeapon);
			break;
		case EWeaponUnequipMethod::LeaveAtSlot:
			AttachWeaponActorToOwnerSlot(EquippedWeapon);
			break;
		case EWeaponUnequipMethod::Throw:
			SetWeaponAtSlot(EquippedWeapon->weaponSlotType, nullptr);
			ThrowWeapon(EquippedWeapon);
			break;
		}
		
		EquippedWeapon = nullptr;
		EquippedWeaponChangedEvent.Broadcast(nullptr);
	}
}

void UWeaponUser::SetWeaponAtSlot(const EWeaponSlotType slot, AWeaponBase* weapon, EWeaponUnequipMethod unequipMethod)
{
	AWeaponBase** currentWeaponAtSlot = WeaponSlots.Find(slot);
	bool removeSlot = (nullptr == weapon);
	bool addSlot = (nullptr != currentWeaponAtSlot) && !removeSlot;

	if (nullptr != currentWeaponAtSlot)
	{
		switch (unequipMethod)
		{
		case EWeaponUnequipMethod::HideToInventory:
			HideWeaponToInventory(*currentWeaponAtSlot);
			break;
		case EWeaponUnequipMethod::Throw:
			ThrowWeapon(*currentWeaponAtSlot);
			break;
		case EWeaponUnequipMethod::LeaveAtSlot:
			weapon->Destroy();
			break;
		}

		WeaponSlots.Remove(slot);
	}

	if (nullptr != weapon)
	{
		WeaponSlots.Add(slot, weapon);

		AttachWeaponActorToOwnerSlot(weapon);
	}

	// Emit event, that weapon at slot has changed
}

AWeaponBase* UWeaponUser::GetWeaponAtSlot(const EWeaponSlotType slot) const
{
	AWeaponBase* const* currentWeaponAtSlot = WeaponSlots.Find(slot);
	if (nullptr != currentWeaponAtSlot)
	{
		return *currentWeaponAtSlot;
	}

	return nullptr;
}

bool UWeaponUser::HasAnyWeapon() const
{
	return WeaponSlots.Num() > 0;
}

void UWeaponUser::StartFire()
{
	SetShooting(true);
}

void UWeaponUser::StopFire()
{
	SetShooting(false);
}

void UWeaponUser::AttachWeaponActorToOwnerSlot(AWeaponBase* weapon)
{
	// Disable weapon physics
	weapon->weaponCollision->SetSimulatePhysics(false);
	weapon->weaponMesh->SetSimulatePhysics(false);

	weapon->CanBePicked = false;

	// Attach weapon actor to a socket on owner
	FName* socketName = WeaponSlotSockets.Find(weapon->weaponSlotType);
	if (nullptr != socketName)
	{
		FAttachmentTransformRules attachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
		weapon->AttachToComponent(ActorMesh, attachmentRules, *socketName);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Socket binding for weapon slot not found"));
	}
}

void UWeaponUser::AttachWeaponActorToOwnerHands(AWeaponBase* weapon)
{
	// Disable weapon physics
	weapon->weaponCollision->SetSimulatePhysics(false);
	weapon->weaponMesh->SetSimulatePhysics(false);

	weapon->CanBePicked = false;

	// Attach weapon actor to a socket on owner
	FName socketName = TEXT("EquippedPistolHost");
	FAttachmentTransformRules attachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	weapon->AttachToComponent(ActorMesh, attachmentRules, socketName);
}

void UWeaponUser::ThrowWeapon(AWeaponBase* weapon)
{
	// Detach weapon actor from any parent
	FDetachmentTransformRules detachmentRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true);
	weapon->DetachFromActor(detachmentRules);

	// Enable weapon physics
	weapon->weaponCollision->SetSimulatePhysics(true);
	weapon->weaponMesh->SetSimulatePhysics(true);

	// Set pickable flag
	weapon->CanBePicked = true;
}

void UWeaponUser::HideWeaponToInventory(AWeaponBase* weapon)
{
	// Add inventory item
	UInventoryComponent* inventoryComp = Cast<UInventoryComponent>(GetOwner()->GetComponentByClass(UInventoryComponent::StaticClass()));
	if (nullptr != inventoryComp)
	{
		inventoryComp->AddItem(weapon->InventoryItemData);
	}

	// Destroy weapon actor
	weapon->Destroy();
}
