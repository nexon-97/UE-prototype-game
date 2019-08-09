#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericCharacter.generated.h"

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
	float GetHealth() const;

private:
	float m_health = 100.f;
};
