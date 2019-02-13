#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EWeaponSlotType.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;
class UBoxComponent;

UCLASS()
class TESTUEPROJECT_API AWeaponBase : public AActor
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

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* weaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* weaponCollision;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="WeaponParams", meta=(DisplayName="Weapon slot type"))
	EWeaponSlotType weaponSlotType = EWeaponSlotType::Knife;
};
