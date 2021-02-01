#include "AI/GenericNPCController.h"

#include "AI/NPCCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NPC/Components/NPCInfo.h"
#include "System/ProtoGameInstance.h"
#include "Character/Components/WeaponUser.h"

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
	ThisNPCInfo = InPawn->FindComponentByClass<UNPCInfo>();
	ThisWeaponUser = InPawn->FindComponentByClass<UWeaponUser>();
	ThisBlackboard = GetBlackboardComponent();
	
	if (BlackboardAsset)
	{
		InitializeBlackboard(*ThisBlackboard, *BlackboardAsset);
	}

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}

	// Configure default blackboard state
	ThisBlackboard->SetValueAsBool(HasWeapon_Key, ThisWeaponUser ? ThisWeaponUser->HasAnyWeapon() : false);
	ThisBlackboard->SetValueAsInt(NextPatrolPointIndex_Key, -1);

	if (ANPCCharacter* NPC = Cast<ANPCCharacter>(InPawn))
	{
		// Configure patrol mode
		if (NPC->bShouldPatrol && NPC->PatrolPath)
		{
			ThisBlackboard->SetValueAsObject(PatrolSpline_Key, NPC->PatrolPath->Spline);
			ThisBlackboard->SetValueAsBool(PatrolMode_Key, true);

			// Patrol movement should be slow
			NPC->SetMovementMode(ECharacterMovementMode::Walk);
		}
		else
		{
			ThisBlackboard->SetValueAsObject(PatrolSpline_Key, nullptr);
			ThisBlackboard->SetValueAsBool(PatrolMode_Key, false);
		}
	}
	else
	{
		ThisBlackboard->SetValueAsBool(PatrolMode_Key, false);
		ThisBlackboard->SetValueAsObject(PatrolSpline_Key, nullptr);
	}

	Super::OnPossess(InPawn);
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

	if (ThisNPCInfo)
	{
		const FEnemyDetectionInfo& DetectionInfo = UProtoGameInstance::Get()->GetEnemyDetectionService()->GetEnemyDetectionInfo(ThisNPCInfo);
		if (DetectionInfo.DetectionState == EEnemyDetectionState::Combat && nullptr != DetectionInfo.TargetNPC)
		{
			// Ensure weapon is equipped
			EquipBestWeapon();
			
			// Try shoot target actor
			AttackActor(DetectionInfo.TargetNPC->GetOwner());
		}
	}
}

void AGenericNPCController::AttackActor(AActor* TargetActor)
{
	if (ThisWeaponUser && ThisWeaponUser->IsWeaponEquipped())
	{
		ThisWeaponUser->EquippedWeapon->TryShootAtLocation(TargetActor->GetActorLocation());
	}
}

bool AGenericNPCController::EquipBestWeapon() const
{
	if (ThisWeaponUser)
	{
		if (ThisWeaponUser->IsWeaponEquipped())
		{
			return true;
		}
		else
		{
			// TODO: Choose best weapon slot
			const bool bEquipped = ThisWeaponUser->EquipWeapon(EWeaponSlotType::Rifle);

			if (bEquipped && ThisWeaponUser->EquippedWeapon)
			{
				// Load full clip if not loaded (AI players cheat, don't need to take ammo from inventory)
				const int32 CurrentClipLoad = ThisWeaponUser->EquippedWeapon->GetAmmoCountInClip();
				if (CurrentClipLoad < 1)
				{
					const int32 AmmoCountToLoad = ThisWeaponUser->EquippedWeapon->clipSize - CurrentClipLoad;
					ThisWeaponUser->EquippedWeapon->LoadClip(AmmoCountToLoad);
				}
			}

			return bEquipped;
		}
	}

	return false;
}

void AGenericNPCController::HideWeapon() const
{
	if (ThisWeaponUser && ThisWeaponUser->IsWeaponEquipped())
	{
		// Unequip weapon back to weapon slot
		ThisWeaponUser->UnequipWeapon(EWeaponUnequipMethod::LeaveAtSlot);
	}
}
