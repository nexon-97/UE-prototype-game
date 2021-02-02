#include "Data/LivePawnsData.h"

void ULivePawnsData::RegisterNPCInfo(UNPCInfo* NPCInfo)
{
	LiveNPCs.AddUnique(NPCInfo);
}

void ULivePawnsData::UnregisterNPCInfo(UNPCInfo* NPCInfo)
{
	LiveNPCs.Remove(NPCInfo);
}

const TArray<TWeakObjectPtr<UNPCInfo>>& ULivePawnsData::GetLiveNPCsList() const
{
	return LiveNPCs;
}

TArray<UNPCInfo*> ULivePawnsData::GetEnemiesFor(UNPCInfo* NPCInfo) const
{
	TArray<UNPCInfo*> Enemies;
	Enemies.Reserve(LiveNPCs.Num());
	
	for (const auto& OtherNPCInfo : LiveNPCs)
	{
		if (UNPCInfo* OtherNPCInfoRaw = OtherNPCInfo.Get())
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
