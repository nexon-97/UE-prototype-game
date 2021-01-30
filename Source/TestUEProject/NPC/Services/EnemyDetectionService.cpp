#include "NPC/Services/EnemyDetectionService.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "Character/Components/KillableComponent.h"

namespace
{
float DetectionDelay = 2.f;
float ConeOfSightAngle = FMath::DegreesToRadians(80.f);
}

UEnemyDetectionService::UEnemyDetectionService()
{
	UKillableComponent::GenericActorKilledEvent.AddUObject(this, &UEnemyDetectionService::OnActorKilled);
}

void UEnemyDetectionService::OnActorKilled(AActor* KilledActor)
{
	// Remove NPC info from detection service, if was added
	UNPCInfo* KilledNPCInfo = KilledActor->FindComponentByClass<UNPCInfo>();
	if (KilledNPCInfo)
	{
		RemoveNPCInfo(KilledNPCInfo);
	}
}

void UEnemyDetectionService::AddNPCInfo(UNPCInfo* NPCInfo)
{
	LiveNPCs.Emplace(NPCInfo, FEnemyDetectionInfo());
}

void UEnemyDetectionService::RemoveNPCInfo(UNPCInfo* NPCInfo)
{
	LiveNPCs.Remove(NPCInfo);
}

TStatId UEnemyDetectionService::GetStatId() const
{
	return TStatId();
}

bool UEnemyDetectionService::IsTickable() const
{
	return nullptr != GetWorld();
}

void UEnemyDetectionService::Tick(float DeltaTime)
{
	for (TPair<TWeakObjectPtr<UNPCInfo>, FEnemyDetectionInfo>& NPCInfo : LiveNPCs)
	{
		if (UNPCInfo* NPCInfoRaw = NPCInfo.Key.Get())
		{
			APawn* OwnerPawn = Cast<APawn>(NPCInfoRaw->GetOwner());
			if (OwnerPawn && !OwnerPawn->IsPlayerControlled())
			{
				// Only tick NPC info for pawns
				TickNPC(DeltaTime, NPCInfoRaw, NPCInfo.Value);
			}
		}
	}
}

void UEnemyDetectionService::TickNPC(float DeltaTime, UNPCInfo* NPCInfo, FEnemyDetectionInfo& DetectionInfo)
{
	switch (DetectionInfo.DetectionState)
	{
		case EEnemyDetectionState::Idle:
			{
				const TArray<UNPCInfo*> Enemies = BuildEnemiesList(NPCInfo);
				if (Enemies.Num() > 0)
				{
					UNPCInfo* EnemyAtSight = FindFirstEnemyAtSight(NPCInfo, Enemies);
					if (EnemyAtSight)
					{
						// Zero detection progress
						DetectionInfo.DetectionProgress = 0.f;
						// Assign visible enemy as target
						DetectionInfo.TargetNPC = EnemyAtSight;
						ChangeDetectionState(NPCInfo, EEnemyDetectionState::EnemyIsBeingDetected);
					}
				}
			}
		break;
		case EEnemyDetectionState::EnemyIsBeingDetected:
			{
				// Check if target NPC is still visible
				if (NPCIsSeenBy(NPCInfo, DetectionInfo.TargetNPC.Get()))
				{
					// Increase detection progress
					DetectionInfo.DetectionProgress = FMath::Clamp(DetectionInfo.DetectionProgress + DeltaTime / DetectionDelay, 0.f,1.f);
					if (FMath::IsNearlyEqual(DetectionInfo.DetectionProgress, 1.f))
					{
						ChangeDetectionState(NPCInfo, EEnemyDetectionState::Combat);
					}
				}
				else
				{
					// Decrease detection progress
					DetectionInfo.DetectionProgress = FMath::Clamp(DetectionInfo.DetectionProgress - DeltaTime / DetectionDelay, 0.f,1.f);
					if (FMath::IsNearlyEqual(DetectionInfo.DetectionProgress, 0.f))
					{
						// Reset target
						DetectionInfo.TargetNPC = nullptr;
						ChangeDetectionState(NPCInfo, EEnemyDetectionState::Idle);
					}
				}
			}
		break;
		case EEnemyDetectionState::Combat:
			{
				if (NPCIsSeenBy(NPCInfo, DetectionInfo.TargetNPC.Get()))
				{
					// Process combat logic
				}
				else
				{
					// Enemy out of sight, start looking for it
					DetectionInfo.LastTargetPosition = DetectionInfo.TargetNPC->GetOwner()->GetActorLocation();
					ChangeDetectionState(NPCInfo, EEnemyDetectionState::SearchEnemy);
				}
			}
		break;
		case EEnemyDetectionState::SearchEnemy:
			{
				// Search enemy logic, should move to DetectionInfo.LastTargetPosition
			}
		break;
		case EEnemyDetectionState::Warned:
			{
				// Do nothing
			}
		break;
	}
}

TArray<UNPCInfo*> UEnemyDetectionService::BuildEnemiesList(UNPCInfo* NPCInfo) const
{
	TArray<UNPCInfo*> Enemies;
	Enemies.Reserve(LiveNPCs.Num());

	for (const TPair<TWeakObjectPtr<UNPCInfo>, FEnemyDetectionInfo>& OtherNPCInfo : LiveNPCs)
	{
		if (UNPCInfo* OtherNPCInfoRaw = OtherNPCInfo.Key.Get())
		{
			// Skip self
			if (NPCInfo == OtherNPCInfoRaw)
				continue;
			
			// Check if NPC is enemy
			const ENPCRelation Relation = NPCInfo->GetNPCRelation(OtherNPCInfoRaw);
			if (Relation == ENPCRelation::Enemy)
			{
				Enemies.Add(OtherNPCInfoRaw);
			}
		}
	}

	return Enemies;
}

UNPCInfo* UEnemyDetectionService::FindFirstEnemyAtSight(UNPCInfo* NPCInfo, const TArray<UNPCInfo*>& Enemies) const
{
	for (UNPCInfo* Enemy : Enemies)
	{
		if (NPCIsSeenBy(NPCInfo, Enemy))
		{
			return Enemy;
		}
	}
	
	return nullptr;
}

bool UEnemyDetectionService::NPCIsSeenBy(UNPCInfo* NPCInfo, UNPCInfo* OtherNPC) const
{
	AActor* SourceActor = NPCInfo->GetOwner();
	AActor* TargetActor = OtherNPC->GetOwner();
	
	const FVector SourceLocation = SourceActor->GetActorLocation();
	const FVector TargetLocation = TargetActor->GetActorLocation();
	const FVector TargetDir = (TargetLocation - SourceLocation).GetSafeNormal();
	const FVector SourceForward = SourceActor->GetActorForwardVector();

	// Check cone of sight
	const float DotProduct = FVector::DotProduct(SourceForward, TargetDir);
	if (DotProduct > FMath::Cos(ConeOfSightAngle))
	{
		// Check distance
		const float MaxDistance = 5000.f; // 50 meters
		if (FVector::DistSquared(SourceLocation, TargetLocation) < MaxDistance * MaxDistance)
		{
			// Finally cast a ray to know if actor is not obscured
			FHitResult OutHit;
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(SourceActor);
			const bool bHit = NPCInfo->GetWorld()->LineTraceSingleByChannel(OutHit, SourceLocation, TargetLocation, ECollisionChannel::ECC_Visibility, QueryParams);
			if (bHit)
			{
				DrawDebugLine(GetWorld(), SourceLocation, OutHit.Location, FColor(0, 200, 0), false, 0.f, 0, 1);
				DrawDebugSolidBox(GetWorld(), OutHit.Location, FVector(4.f, 4.f, 4.f), FColor(0, 255, 0), false, 0.f);
				return OutHit.Actor == TargetActor;
			}
			else
			{
				DrawDebugLine(GetWorld(), SourceLocation, TargetLocation, FColor(255, 255, 0), false, 0.f, 0, 1);
			}
		}
	}

	return false;
}

EEnemyDetectionState UEnemyDetectionService::GetDetectionState(UNPCInfo* NPCInfo) const
{
	const FEnemyDetectionInfo* DetectionInfo = LiveNPCs.Find(NPCInfo);
	if (DetectionInfo)
	{
		return DetectionInfo->DetectionState;
	}

	return EEnemyDetectionState::Idle;
}

const FEnemyDetectionInfo& UEnemyDetectionService::GetEnemyDetectionInfo(UNPCInfo* NPCInfo) const
{
	const FEnemyDetectionInfo* Info = LiveNPCs.Find(NPCInfo);
	if (Info)
	{
		return *Info;
	}

	static const FEnemyDetectionInfo EmptyDetectionInfo;
	return EmptyDetectionInfo;
}

void UEnemyDetectionService::ChangeDetectionState(UNPCInfo* NPCInfo, EEnemyDetectionState NewState)
{
	FEnemyDetectionInfo* DetectionInfo = LiveNPCs.Find(NPCInfo);
	if (DetectionInfo)
	{
		if (NewState != DetectionInfo->DetectionState)
		{
			DetectionInfo->DetectionState = NewState;
			UE_LOG(LogTemp, Log, TEXT("NPC detection state has changed: [%d]"), static_cast<int32>(NewState));
			OnEnemyDetectionStateChange.Broadcast(NPCInfo, NewState);
		}
	}
}
