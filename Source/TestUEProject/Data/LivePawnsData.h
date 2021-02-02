#pragma once
#include "NPC/Components/NPCInfo.h"
#include "LivePawnsData.generated.h"

UCLASS()
class ULivePawnsData : public UObject
{
	GENERATED_BODY()

public:
	void RegisterNPCInfo(UNPCInfo* NPCInfo);
	void UnregisterNPCInfo(UNPCInfo* NPCInfo);

	const TArray<TWeakObjectPtr<UNPCInfo>>& GetLiveNPCsList() const;

	TArray<UNPCInfo*> GetEnemiesFor(UNPCInfo* NPC) const;

private:
	TArray<TWeakObjectPtr<UNPCInfo>> LiveNPCs;
};
