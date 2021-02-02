#include "AI/BT_Services/FindEnemyService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "System/ProtoGameInstance.h"

namespace
{
	float DetectionDelay = 2.f;
	float ConeOfSightAngle = FMath::DegreesToRadians(80.f);
	const FName EyeSocketName = TEXT("Eyes");
	const float MaxCheckDistance = 5000.f; // 50 meters
}

void UFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	
	const EEnemyDetectionState DetectionState = static_cast<EEnemyDetectionState>(Blackboard->GetValueAsEnum(BB_EnemyDetectionState.SelectedKeyName));
	AActor* Self = OwnerComp.GetAIOwner()->GetPawn();
	UNPCInfo* SelfNPCInfo = Self->FindComponentByClass<UNPCInfo>();
	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(BB_TargetEnemy.SelectedKeyName));
	ULivePawnsData* LivePawnsData = UProtoGameInstance::Get()->GetLivePawnsData();
	
	switch (DetectionState)
	{
		case EEnemyDetectionState::Idle:
			{
				const TArray<UNPCInfo*> Enemies = LivePawnsData->GetEnemiesFor(SelfNPCInfo);
				if (Enemies.Num() > 0)
				{
					UNPCInfo* EnemyAtSight = FindFirstEnemyAtSight(Self, Enemies);
					if (EnemyAtSight)
					{
						// Zero detection progress
						Blackboard->SetValueAsFloat(BB_EnemyDetectionProgress.SelectedKeyName, 0.f);
						// Assign visible enemy as target
						Blackboard->SetValueAsObject(BB_TargetEnemy.SelectedKeyName, EnemyAtSight->GetOwner());

						ChangeDetectionState(Blackboard, EEnemyDetectionState::EnemyIsBeingDetected);
					}
				}
			}
		break;
		case EEnemyDetectionState::EnemyIsBeingDetected:
			{
				float DetectionProgress = Blackboard->GetValueAsFloat(BB_EnemyDetectionProgress.SelectedKeyName);
				
				// Check if target NPC is still visible
				if (ActorIsSeen(Self, TargetActor))
				{
					// Increase detection progress
					DetectionProgress = FMath::Clamp(DetectionProgress + DeltaSeconds / DetectionDelay, 0.f,1.f);

					if (FMath::IsNearlyEqual(DetectionProgress, 1.f))
					{
						ChangeDetectionState(Blackboard, EEnemyDetectionState::Combat);
					}
				}
				else
				{
					// Decrease detection progress
					DetectionProgress = FMath::Clamp(DetectionProgress - DeltaSeconds / DetectionDelay, 0.f,1.f);
					if (FMath::IsNearlyEqual(DetectionProgress, 0.f))
					{
						// Reset target
						Blackboard->SetValueAsObject(BB_TargetEnemy.SelectedKeyName, nullptr);
						
						ChangeDetectionState(Blackboard, EEnemyDetectionState::Idle);
					}
				}

				Blackboard->SetValueAsFloat(BB_EnemyDetectionProgress.SelectedKeyName, DetectionProgress);
			}
		break;
		case EEnemyDetectionState::Combat:
			{
				if (ActorIsSeen(Self, TargetActor))
				{
					// Process combat logic
				}
				else
				{
					// Enemy out of sight, start looking for it
					Blackboard->SetValueAsVector(BB_LastEnemyLocation.SelectedKeyName, TargetActor->GetActorLocation());

					ChangeDetectionState(Blackboard, EEnemyDetectionState::SearchEnemy);
				}
			}
		break;
		case EEnemyDetectionState::SearchEnemy:
			{
				// If enemy is seen again, enter combat mode immediately
				if (ActorIsSeen(Self, TargetActor))
				{
					ChangeDetectionState(Blackboard, EEnemyDetectionState::Combat);
				}
			}
		break;
		case EEnemyDetectionState::Warned:
			{
				// Do nothing, become idle after longer cooldown
			}
		break;
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}

void UFindEnemyService::ChangeDetectionState(UBlackboardComponent* Blackboard, EEnemyDetectionState DetectionState) const
{
	Blackboard->SetValueAsEnum(BB_EnemyDetectionState.SelectedKeyName, static_cast<uint8>(DetectionState));
}

bool UFindEnemyService::ActorIsSeen(AActor* Self, AActor* Target) const
{
	// Set source location based on NPC eyes position
	FVector SourceLocation;
	if (USkeletalMeshComponent* ActorMesh = Self->FindComponentByClass<USkeletalMeshComponent>())
	{
		SourceLocation = ActorMesh->GetSocketLocation(EyeSocketName);
	}
	else
	{
		SourceLocation = Self->GetActorLocation();
	}
	
	const FVector TargetLocation = Target->GetActorLocation();
	const FVector TargetDir = (TargetLocation - SourceLocation).GetSafeNormal();
	const FVector SourceForward = Self->GetActorForwardVector();

	// Check cone of sight
	const float DotProduct = FVector::DotProduct(SourceForward, TargetDir);
	if (DotProduct > FMath::Cos(ConeOfSightAngle))
	{
		// Check distance
		if (FVector::DistSquared(SourceLocation, TargetLocation) < MaxCheckDistance * MaxCheckDistance)
		{
			// Finally cast a ray to know if there is no visibility blocker between source and target
			FHitResult OutHit;
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(Self);
			QueryParams.AddIgnoredActor(Target);
			const bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, SourceLocation, TargetLocation, ECollisionChannel::ECC_Visibility, QueryParams);

			// Debug draw visibility traces
			if (bHit)
			{
				DrawDebugLine(GetWorld(), SourceLocation, OutHit.Location, FColor(255, 0, 0), false, 0.f, 0, 1);
				DrawDebugSolidBox(GetWorld(), OutHit.Location, FVector(4.f, 4.f, 4.f), FColor(255, 0, 0), false, 0.f);
			}
			else
			{
				DrawDebugLine(GetWorld(), SourceLocation, TargetLocation, FColor(0, 255, 0), false, 0.f, 0, 1);
			}

			return !bHit;
		}
	}

	return false;
}

UNPCInfo* UFindEnemyService::FindFirstEnemyAtSight(AActor* Self, const TArray<UNPCInfo*>& Enemies) const
{
	for (UNPCInfo* EnemyInfo : Enemies)
	{
		if (ActorIsSeen(Self, EnemyInfo->GetOwner()))
		{
			return EnemyInfo;
		}
	}
	
	return nullptr;
}
