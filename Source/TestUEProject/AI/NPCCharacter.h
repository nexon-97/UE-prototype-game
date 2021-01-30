#pragma once

#include "CoreMinimal.h"
#include "Character/GenericCharacter.h"
#include "AI/SplinePathActor.h"
#include "NPCCharacter.generated.h"

UCLASS(Blueprintable)
class TESTUEPROJECT_API ANPCCharacter
	: public AGenericCharacter
{
	GENERATED_BODY()

public:
	ANPCCharacter(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(EditAnywhere, Category = "NPC|Patrol")
	bool bShouldPatrol;

	UPROPERTY(EditAnywhere, Category = "NPC|Patrol", meta = (EditCondition = "bShouldPatrol"))
	ASplinePathActor* PatrolPath;

protected:
	void BeginPlay() override;
};
