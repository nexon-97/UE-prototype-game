#include "NPC/Components/NPCInfo.h"
#include "ProtoGameInstance.h"

namespace
{

int32 GNextNPCIngameId = 1;

}

UNPCInfo::UNPCInfo(const FObjectInitializer& ObjectInitializer)
{
	// Assign next ingame id to this NPC state
	IngameId = GNextNPCIngameId++;
}

void UNPCInfo::BeginPlay()
{
	Super::BeginPlay();

	UProtoGameInstance::Get()->EnemyDetectionService->AddNPCInfo(this);
}

void UNPCInfo::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UProtoGameInstance::Get()->EnemyDetectionService->RemoveNPCInfo(this);
}

int32 UNPCInfo::GetId() const
{
	return IngameId;
}

const FString& UNPCInfo::GetQuestId() const
{
	return QuestId;
}

void UNPCInfo::SetQuestId(const FString& NewQuestId)
{
	ensure(QuestId.IsEmpty());
	QuestId = NewQuestId;
}

void UNPCInfo::OverrideNextIngameId(const int32 NextIngameId)
{
	GNextNPCIngameId = NextIngameId;
}

void UNPCInfo::OverrideNPCRelation(UNPCInfo* OtherNPC, ENPCRelation RelationOverride)
{
	ensure(OtherNPC);
	NPCRelationOverrides.Add(OtherNPC->GetId(), RelationOverride);
}

ENPCRelation UNPCInfo::GetNPCRelation(UNPCInfo* OtherNPC) const
{
	ensure(OtherNPC);

	const ENPCRelation* RelationPtr = NPCRelationOverrides.Find(OtherNPC->GetId());
	if (RelationPtr)
	{
		return *RelationPtr;
	}

	return ENPCRelation::Neutral;
}


void UNPCInfo::SetGroup(const FName& NewGroup)
{
	Group = NewGroup;
}

const FName& UNPCInfo::GetGroup() const
{
	return Group;
}
