#include "AI/GenericNPCController.h"

#include "AI/NPCCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NPC/Components/NPCInfo.h"
#include "System/ProtoGameInstance.h"
#include "Weapon/Components/WeaponUser.h"

namespace
{
	
	const FName HasWeapon_Key = TEXT("HasWeapon");
	const FName NextPatrolPointIndex_Key = TEXT("NextPatrolPointIndex");
	const FName PatrolMode_Key = TEXT("PatrolMode");
	const FName PatrolSpline_Key = TEXT("PatrolSpline");
	
}

AGenericNPCController::AGenericNPCController(const FObjectInitializer& ObjectInitializer)
{
	UBlackboardComponent* AIBlackboard = CreateDefaultSubobject<UBlackboardComponent>("Blackboard");
	AIBlackboard->bEditableWhenInherited = true;
}

void AGenericNPCController::OnPossess(APawn* InPawn)
{
	ANPCCharacter* NPC = Cast<ANPCCharacter>(InPawn);
	ThisNPCInfo = InPawn->FindComponentByClass<UNPCInfo>();
	ThisWeaponUser = InPawn->FindComponentByClass<UWeaponUser>();
	
	if (BlackboardAsset)
	{
		InitializeBlackboard(*Blackboard, *BlackboardAsset);
	}

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}

	// Configure default blackboard state
	Blackboard->SetValueAsBool(HasWeapon_Key, ThisWeaponUser ? ThisWeaponUser->HasAnyWeapon() : false);
	BBConfigurePatrolMode(NPC);

	Super::OnPossess(InPawn);
}

void AGenericNPCController::BBConfigurePatrolMode(ANPCCharacter* NPC) const
{
	Blackboard->SetValueAsInt(NextPatrolPointIndex_Key, -1);

	if (NPC)
	{
		const bool bPatrolModeEnabled = (NPC->bShouldPatrol && NPC->PatrolPath && NPC->PatrolPath->Spline);
		Blackboard->SetValueAsBool(PatrolMode_Key, bPatrolModeEnabled);
		Blackboard->SetValueAsObject(PatrolSpline_Key, NPC->PatrolPath ? NPC->PatrolPath->Spline : nullptr);

		if (bPatrolModeEnabled)
		{
			// Patrol movement should be slow
			NPC->SetMovementMode(ECharacterMovementMode::Walk);
		}
	}
	else
	{
		Blackboard->SetValueAsBool(PatrolMode_Key, false);
		Blackboard->SetValueAsObject(PatrolSpline_Key, nullptr);
	}
}

void AGenericNPCController::OnUnPossess()
{
	ThisNPCInfo = nullptr;
	ThisWeaponUser = nullptr;

	Super::OnUnPossess();
}

void AGenericNPCController::BeginPlay()
{
	Super::BeginPlay();
}

void AGenericNPCController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
