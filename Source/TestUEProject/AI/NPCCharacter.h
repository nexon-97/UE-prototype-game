#pragma once

#include "CoreMinimal.h"
#include "Character/GenericCharacter.h"
#include "AI/SplinePathActor.h"
#include "NPC/Components/NPCInfo.h"
#include "NPCCharacter.generated.h"

UCLASS(Blueprintable)
class TESTUEPROJECT_API ANPCCharacter
	: public AGenericCharacter
{
	GENERATED_BODY()

public:
	ANPCCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	UNPCInfo* GetNPCInfo() const;

public:
	UPROPERTY(EditAnywhere, Category = "NPC|Patrol")
	bool bShouldPatrol;

	UPROPERTY(EditAnywhere, Category = "NPC|Patrol", meta = (EditCondition = "bShouldPatrol"))
	ASplinePathActor* PatrolPath;

private:
	UPROPERTY(Transient)
	UNPCInfo* NPCInfoComponent;
};
