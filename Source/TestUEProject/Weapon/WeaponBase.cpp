#include "Weapon/WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Items/Components/InventoryItemComponent.h"
#include "Character/Components/KillableComponent.h"
#include "Engine.h"
#include "NPC/Components/NPCInfo.h"

const ECollisionChannel k_projectileCollisionChannel = ECollisionChannel::ECC_Pawn;

AWeaponBase::AWeaponBase(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	WeaponMesh->bEditableWhenInherited = true;
	WeaponMesh->SetCollisionProfileName(TEXT("Pickable"));

	weaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	weaponCollision->SetupAttachment(RootComponent);
	weaponCollision->bEditableWhenInherited = true;

	InventoryItemData = CreateDefaultSubobject<UInventoryItemComponent>(TEXT("InventoryItemData"));
	InventoryItemData->bEditableWhenInherited = true;
	InventoryItemData->m_count = 1;
	InventoryItemData->m_itemInfoId = InventoryId;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	WeaponMesh->SetSimulatePhysics(true);

	FireSocket = WeaponMesh->GetSocketByName(TEXT("MuzzleFlash"));
}

bool AWeaponBase::CanShoot() const
{
	const bool bTimerElapsed = !GetWorld()->GetTimerManager().IsTimerActive(ShootTimer);
	return AmmoCount > 0 && bTimerElapsed;
}

int AWeaponBase::GetAmmoCountInClip() const
{
	return AmmoCount;
}

bool AWeaponBase::TryShootAtLocation(const FVector& TargetLocation)
{
	if (CanShoot())
	{
		--AmmoCount;

		// Run warmup timer
		GetWorld()->GetTimerManager().SetTimer(ShootTimer, shootFrequency, false);

		if (nullptr != FireSocket)
		{
			const FTransform SocketTransform = FireSocket->GetSocketTransform(WeaponMesh);
			
			// Run line trace to determine target to hit
			FHitResult HitResult;
			const FVector TraceStart = SocketTransform.GetLocation();
			const float TraceDistance = 30000.f; // 300 Meters ahead
			const FVector TraceDirection = (TargetLocation - TraceStart).GetSafeNormal();
			const FVector TraceEnd = TraceStart + TraceDirection * TraceDistance;

			// Ignore the actor who shoots
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);
			QueryParams.AddIgnoredActor(GetAttachParentActor());

			const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, k_projectileCollisionChannel, QueryParams);
			if (bHit)
			{
				DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor(255, 0, 0), false, 2.f, 0, 2);
				DrawDebugSolidBox(GetWorld(), HitResult.Location, FVector(5.f, 5.f, 5.f), FColor(255, 0, 0), false, 2.f);

				AActor* HitActor = HitResult.GetActor();
				
				// Apply damage to the hit target
				UKillableComponent* Killable = HitActor->FindComponentByClass<UKillableComponent>();
				if (Killable)
				{
					Killable->RemoveHealth(26.f);
				}

				// Spoil relationships between instigator and damaged actor
				UNPCInfo* NPCInfo = HitActor->FindComponentByClass<UNPCInfo>();
				if (NPCInfo)
				{
					AActor* AttachParent = GetAttachParentActor();
					if (AttachParent)
					{
						UNPCInfo* InstigatorInfo = AttachParent->FindComponentByClass<UNPCInfo>();
						if (InstigatorInfo)
						{
							NPCInfo->OverrideNPCRelation(InstigatorInfo, ENPCRelation::Enemy);
						}
					}
				}
			}
			else
			{
				DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor(255, 0, 0), false, 2.f, 0, 2);
			}
		}
	}

	return false;
}

// void AWeaponBase::ShootInternal()
// {
// 	--AmmoCount;
//
// 	// Run warmup timer
// 	GetWorld()->GetTimerManager().SetTimer(ShootTimer, shootFrequency, false);
//
// 	if (nullptr != FireSocket)
// 	{
// 		FTransform socketTransform = FireSocket->GetSocketTransform(WeaponMesh);
// 		
// 		// Run line trace to determine target to hit
// 		FHitResult hitResult;
// 		FVector fakeTraceStart = socketTransform.GetLocation();
// 		const float traceDistance = 30000.f; // 300 Meters ahead
// 		//FVector traceEnd = traceStart + socketTransform.GetRotation().GetForwardVector() * traceDistance;
// 		FVector deprojectedLocation;
// 		FVector deprojectedDirection;
// 		APlayerController* controller = GetWorld()->GetFirstPlayerController();
// 		FVector2D viewportSize;
// 		GetWorld()->GetGameViewport()->GetViewportSize(viewportSize);
// 		controller->DeprojectScreenPositionToWorld(viewportSize[0] * 0.5f, viewportSize[1] * 0.5f, deprojectedLocation, deprojectedDirection);
//
// 		FVector traceStart = deprojectedLocation;
// 		FVector traceEnd = deprojectedLocation + deprojectedDirection * traceDistance;
//
// 		FCollisionQueryParams queryParams;
// 		queryParams.AddIgnoredActor(GetOwner());
// 		queryParams.AddIgnoredActor(this);
//
// 		bool traceSucceeded = GetWorld()->LineTraceSingleByChannel(hitResult, traceStart, traceEnd, k_projectileCollisionChannel, queryParams);
// 		if (traceSucceeded)
// 		{
// 			DrawDebugLine(GetWorld(), fakeTraceStart, hitResult.Location, FColor(255, 0, 0), false, 2.f, 0, 2);
// 			DrawDebugSolidBox(GetWorld(), hitResult.Location, FVector(5.f, 5.f, 5.f), FColor(255, 0, 0), false, 2.f);
//
// 			UKillableComponent* Killable = hitResult.GetActor()->FindComponentByClass<UKillableComponent>();
// 			if (Killable)
// 			{
// 				Killable->RemoveHealth(26.f);
// 			}
//
// 			// Spoil relationships between instigator and damaged actor
// 			UNPCInfo* NPCInfo = hitResult.GetActor()->FindComponentByClass<UNPCInfo>();
// 			if (NPCInfo)
// 			{
// 				AActor* AttachParent = GetAttachParentActor();
// 				if (AttachParent)
// 				{
// 					UNPCInfo* InstigatorInfo = AttachParent->FindComponentByClass<UNPCInfo>();
// 					if (InstigatorInfo)
// 					{
// 						NPCInfo->OverrideNPCRelation(InstigatorInfo, ENPCRelation::Enemy);
// 					}
// 				}
// 			}
// 		}
// 		else
// 		{
// 			DrawDebugLine(GetWorld(), fakeTraceStart, traceEnd, FColor(255, 0, 0), false, 2.f, 0, 2);
// 		}
// 	}
// }

int AWeaponBase::LoadClip(int32 LoadCount)
{
	const int32 RemainingToFullLoad = clipSize - AmmoCount;
	const int32 AmmoToLoad = FMath::Min(RemainingToFullLoad, LoadCount);
	AmmoCount += AmmoToLoad;

	return AmmoToLoad;
}
