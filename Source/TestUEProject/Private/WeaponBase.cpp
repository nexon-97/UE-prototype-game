// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine.h"


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
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));
}

