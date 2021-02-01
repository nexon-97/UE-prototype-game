#include "Character/Components/WeaponUser.h"
#include "Character/Components/InventoryComponent.h"
#include "Engine.h"

UWeaponUser::UWeaponUser(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponUser::BeginPlay()
{
	Super::BeginPlay();

	// If there are weapon slots assigned, actually attach them to the actor sockets
	for (auto& WeaponSlotData : WeaponSlots)
	{
		if (nullptr == WeaponSlotData.Value)
			continue;
		
		if (WeaponSlotData.Value == EquippedWeapon)
		{
			AttachWeaponActorToOwnerHands(WeaponSlotData.Value);
		}
		else
		{
			AttachWeaponActorToOwnerSlot(WeaponSlotData.Value);
		}
	}
}

bool UWeaponUser::EquipWeapon(const EWeaponSlotType Slot)
{
	if (nullptr != EquippedWeapon && EquippedWeapon->weaponSlotType == Slot)
	{
		// TODO: We aren't asked to unequip weapon, that action should be explicitly asked by the higher level code
		UnequipWeapon(EWeaponUnequipMethod::LeaveAtSlot);
		return false;
	}
	else
	{
		AWeaponBase* Weapon = nullptr;
		AWeaponBase** WeaponPtr = WeaponSlots.Find(Slot);
		if (nullptr != WeaponPtr)
		{
			Weapon = *WeaponPtr;
		}

		if (Weapon != EquippedWeapon)
		{
			UnequipWeapon(EWeaponUnequipMethod::LeaveAtSlot);

			if (nullptr != Weapon)
			{
				AttachWeaponActorToOwnerHands(Weapon);
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("Weapon equipped"));

				EquippedWeapon = Weapon;
				EquippedWeaponChangedEvent.Broadcast(EquippedWeapon);

				return true;
			}
		}
	}

	return false;
}

void UWeaponUser::UnequipWeapon(EWeaponUnequipMethod UnequipMethod)
{
	if (nullptr != EquippedWeapon)
	{
		switch (UnequipMethod)
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

bool UWeaponUser::IsWeaponEquipped() const
{
	return nullptr != EquippedWeapon;
}

void UWeaponUser::SetWeaponAtSlot(const EWeaponSlotType Slot, AWeaponBase* Weapon, EWeaponUnequipMethod UnequipMethod)
{
	AWeaponBase** CurrentWeaponAtSlot = WeaponSlots.Find(Slot);

	if (nullptr != CurrentWeaponAtSlot)
	{
		switch (UnequipMethod)
		{
		case EWeaponUnequipMethod::HideToInventory:
			HideWeaponToInventory(*CurrentWeaponAtSlot);
			break;
		case EWeaponUnequipMethod::Throw:
			ThrowWeapon(*CurrentWeaponAtSlot);
			break;
		case EWeaponUnequipMethod::LeaveAtSlot:
			// TODO: If we want to set new weapon to the slot, old one should be hidden to inventory instead of just destroyed?
			Weapon->Destroy();
			break;
		}

		WeaponSlots.Remove(Slot);
	}

	if (nullptr != Weapon)
	{
		WeaponSlots.Add(Slot, Weapon);

		AttachWeaponActorToOwnerSlot(Weapon);
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

void UWeaponUser::AttachWeaponActorToOwnerSlot(AWeaponBase* Weapon)
{
	// Disable weapon physics
	Weapon->weaponCollision->SetSimulatePhysics(false);
	Weapon->WeaponMesh->SetSimulatePhysics(false);

	// We can't steal weapons from character slot
	Weapon->CanBePicked = false;

	// Attach weapon actor to a socket on owner
	const FName* SocketName = WeaponSlotSockets.Find(Weapon->weaponSlotType);
	if (nullptr != SocketName)
	{
		const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
		Weapon->AttachToComponent(ActorMesh, AttachmentRules, *SocketName);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Socket binding for weapon slot not found"));
	}
}

void UWeaponUser::AttachWeaponActorToOwnerHands(AWeaponBase* Weapon) const
{
	// Disable weapon physics
	Weapon->weaponCollision->SetSimulatePhysics(false);
	Weapon->WeaponMesh->SetSimulatePhysics(false);

	Weapon->CanBePicked = false;

	// Attach weapon actor to a socket on owner
	const FName SocketName = TEXT("EquippedPistolHost");
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	Weapon->AttachToComponent(ActorMesh, AttachmentRules, SocketName);
}

void UWeaponUser::ThrowWeapon(AWeaponBase* Weapon) const
{
	// Detach weapon actor from any parent
	const FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true);
	Weapon->DetachFromActor(DetachmentRules);

	// Enable weapon physics
	Weapon->weaponCollision->SetSimulatePhysics(true);
	Weapon->WeaponMesh->SetSimulatePhysics(true);

	// Set pickable flag
	Weapon->CanBePicked = true;
}

void UWeaponUser::HideWeaponToInventory(AWeaponBase* Weapon) const
{
	// Add inventory item
	UInventoryComponent* inventoryComp = Cast<UInventoryComponent>(GetOwner()->GetComponentByClass(UInventoryComponent::StaticClass()));
	if (nullptr != inventoryComp)
	{
		inventoryComp->AddItem(Weapon->InventoryItemData);
	}

	// Destroy weapon actor
	Weapon->Destroy();
}
