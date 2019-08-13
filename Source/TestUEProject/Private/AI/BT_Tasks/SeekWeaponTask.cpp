#include "AI/BT_Tasks/SeekWeaponTask.h"
#include "WeaponBase.h"

#include <BehaviorTree/BlackboardComponent.h>
#include <AIController.h>
#include <NavigationSystem.h>

#include "Engine.h"

EBTNodeResult::Type USeekWeaponTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Find weapon in surrounding area
	TArray<FHitResult> outHits;
	FVector pawnLocation = OwnerComp.GetOwner()->GetActorLocation();
	FVector traceEnd = pawnLocation + FVector(0.f, 0.f, 15.f);
	FQuat rotation;
	FCollisionObjectQueryParams objectParams(FCollisionObjectQueryParams::AllObjects);
	FCollisionQueryParams queryParams(NAME_None, false, nullptr);
	const float sightRadius = 5000.f;
	bool hitFound = GetWorld()->SweepMultiByObjectType(outHits, pawnLocation, traceEnd, rotation, objectParams, FCollisionShape::MakeSphere(sightRadius), queryParams);

	if (hitFound)
	{
		TArray<AWeaponBase*> foundWeapons;

		// Prepare for navmesh query
		AAIController* aiController = Cast<AAIController>(OwnerComp.GetOwner());
		const UNavigationSystemV1* NavigationSys = UNavigationSystemV1::GetCurrent(GetWorld());
		const ANavigationData* NavData = NavigationSys->GetNavDataForProps(aiController->GetNavAgentPropertiesRef());

		TSubclassOf<UNavigationQueryFilter> FilterClass = UNavigationQueryFilter::StaticClass();
		FSharedConstNavQueryFilter QueryFilter = UNavigationQueryFilter::GetQueryFilter(*NavData, FilterClass);

		for (const FHitResult& hitResult : outHits)
		{
			AWeaponBase* weaponActor = Cast<AWeaponBase>(hitResult.Actor);
			if (nullptr != weaponActor && weaponActor->CanBePicked)
			{
				//FNavLocation outLocation;
				//if (NavData->ProjectPoint(weaponActor->GetActorLocation(), outLocation, FVector(15.f), QueryFilter))
				//{
				//	foundWeapons.Add(weaponActor);
				//}

				// Check the actor is reachable by the AI
				FPathFindingQuery WeaponReachQuery = FPathFindingQuery(this, *NavData, pawnLocation, weaponActor->GetActorLocation(), QueryFilter);
				if (NavigationSys->TestPathSync(WeaponReachQuery, EPathFindingMode::Regular))
				{
					foundWeapons.Add(weaponActor);
				}
			}
		}

		if (foundWeapons.Num() > 0)
		{
			// Sort weapons by distance to pawn
			auto predicate = [pawnLocation](AWeaponBase& lhs, AWeaponBase& rhs)
			{
				float distanceToLhsSq = FVector::DistSquared(pawnLocation, lhs.GetActorLocation());
				float distanceToRhsSq = FVector::DistSquared(pawnLocation, rhs.GetActorLocation());

				return distanceToLhsSq < distanceToRhsSq;
			};
			foundWeapons.Sort(predicate);

			// Pick the nearest weapon
			AWeaponBase* pickedWeapon = *foundWeapons.begin();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Weapon found: %s; Num variants: %d"), *pickedWeapon->GetName(), foundWeapons.Num()));

			UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
			blackboard->SetValueAsVector(FName("TargetLocation"), pickedWeapon->GetActorLocation());
			blackboard->SetValueAsObject(FName("TargetWeaponForPick"), pickedWeapon);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
