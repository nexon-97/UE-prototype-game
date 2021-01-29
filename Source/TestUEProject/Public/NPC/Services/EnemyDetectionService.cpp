#include "NPC/Services/EnemyDetectionService.h"

UEnemyDetectionService::UEnemyDetectionService()
{

}

void UEnemyDetectionService::AddNPCInfo(UNPCInfo* NPCInfo)
{
	LiveNPCs.Emplace(NPCInfo, FEnemyDetectionInfo());
}

void UEnemyDetectionService::RemoveNPCInfo(UNPCInfo* NPCInfo)
{
	LiveNPCs.Remove(NPCInfo);
}

void UEnemyDetectionService::Tick(float DeltaTime)
{

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

void UEnemyDetectionService::ChangeDetectionState(UNPCInfo* NPCInfo, EEnemyDetectionState NewState)
{
	FEnemyDetectionInfo* DetectionInfo = LiveNPCs.Find(NPCInfo);
	if (DetectionInfo)
	{
		if (NewState != DetectionInfo->DetectionState)
		{
			DetectionInfo->DetectionState = NewState;
			OnEnemyDetectionStateChange.Broadcast(NPCInfo, NewState);
		}
	}
}
