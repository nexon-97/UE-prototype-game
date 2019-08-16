// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "KillableComponent.h"
#include "Engine.h"

const ECollisionChannel k_projectileCollisionChannel = ECollisionChannel::ECC_WorldDynamic;

AWeaponBase::AWeaponBase(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	weaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = weaponMesh;
	weaponMesh->bEditableWhenInherited = true;
	weaponMesh->SetCollisionProfileName(TEXT("Pickable"));

	weaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	weaponCollision->SetupAttachment(RootComponent);
	weaponCollision->bEditableWhenInherited = true;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	weaponMesh->SetSimulatePhysics(true);

	FireSocket = weaponMesh->GetSocketByName(TEXT("MuzzleFlash"));
}

bool AWeaponBase::CanShoot() const
{
	bool timerElapsed = !GetWorld()->GetTimerManager().IsTimerActive(ShootTimer);
	return ammoCount > 0 && timerElapsed;
}

int AWeaponBase::GetAmmoCountInClip() const
{
	return ammoCount;
}

bool AWeaponBase::Shoot()
{
	if (CanShoot())
	{
		ShootInternal();
		return true;
	}

	return false;
}

void AWeaponBase::ShootInternal()
{
	--ammoCount;

	// Run warmup timer
	GetWorld()->GetTimerManager().SetTimer(ShootTimer, shootFrequency, false);

	if (nullptr != FireSocket)
	{
		FTransform socketTransform = FireSocket->GetSocketTransform(weaponMesh);
		
		// Run line trace to determine target to hit
		FHitResult hitResult;
		FVector fakeTraceStart = socketTransform.GetLocation();
		const float traceDistance = 30000.f; // 300 Meters ahead
		//FVector traceEnd = traceStart + socketTransform.GetRotation().GetForwardVector() * traceDistance;
		FVector deprojectedLocation;
		FVector deprojectedDirection;
		APlayerController* controller = GetWorld()->GetFirstPlayerController();
		FVector2D viewportSize;
		GetWorld()->GetGameViewport()->GetViewportSize(viewportSize);
		controller->DeprojectScreenPositionToWorld(viewportSize[0] * 0.5f, viewportSize[1] * 0.5f, deprojectedLocation, deprojectedDirection);

		FVector traceStart = deprojectedLocation;
		FVector traceEnd = deprojectedLocation + deprojectedDirection * traceDistance;

		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(GetOwner());
		queryParams.AddIgnoredActor(this);

		bool traceSucceeded = GetWorld()->LineTraceSingleByChannel(hitResult, traceStart, traceEnd, k_projectileCollisionChannel, queryParams);
		if (traceSucceeded)
		{
			DrawDebugLine(GetWorld(), fakeTraceStart, hitResult.Location, FColor(255, 0, 0), false, 2.f, 0, 2);
			DrawDebugSolidBox(GetWorld(), hitResult.Location, FVector(5.f, 5.f, 5.f), FColor(255, 0, 0), false, 2.f);

			TArray<UKillableComponent*> killables;
			hitResult.GetActor()->GetComponents(killables, false);

			if (killables.Num() > 0)
			{
				UKillableComponent* killable = *killables.begin();
				killable->RemoveHealth(26.f);
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), fakeTraceStart, traceEnd, FColor(255, 0, 0), false, 2.f, 0, 2);
		}
	}
}

int AWeaponBase::LoadClip(int loadCount)
{
	const int remainingToFullLoad = clipSize - ammoCount;
	int ammoLoaded = FMath::Min(remainingToFullLoad, loadCount);
	ammoCount += ammoLoaded;

	return ammoLoaded;
}
