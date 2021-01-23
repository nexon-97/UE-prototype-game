#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KillableComponent.h"
#include "Character/CharacterMovementMode.h"
#include "GenericCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterMovementModeChanged, ECharacterMovementMode);

UCLASS()
class TESTUEPROJECT_API AGenericCharacter
	: public ACharacter
{
	GENERATED_BODY()

public:
	AGenericCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	ECharacterMovementMode GetMovementMode() const;
	UFUNCTION(BlueprintCallable)
	void SetMovementMode(ECharacterMovementMode NewMovementMode);

	FCharacterMovementModeChanged OnCharacterMovementModeChanged;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (DisplayName = "Killable Component", AllowPrivateAccess = "true"))
	UKillableComponent* KillableComponent = nullptr;

	ECharacterMovementMode CharacterMovementMode = ECharacterMovementMode::Jog;

	UFUNCTION()
	void OnExhausted();
	UFUNCTION()
	void OnKilled();

	void EnableRagdoll();
};
