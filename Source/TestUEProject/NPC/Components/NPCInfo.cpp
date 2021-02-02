#include "NPC/Components/NPCInfo.h"
#include "System/ProtoGameInstance.h"

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

	UProtoGameInstance::Get()->GetLivePawnsData()->RegisterNPCInfo(this);
}

void UNPCInfo::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UProtoGameInstance::Get()->GetLivePawnsData()->UnregisterNPCInfo(this);
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

	// Check individual relation
	const ENPCRelation* RelationPtr = NPCRelationOverrides.Find(OtherNPC->GetId());
	if (RelationPtr)
	{
		return *RelationPtr;
	}

	// Check group relation
	UGroupDefinitionAsset* const* GroupDefinition = UProtoGameInstance::Get()->GlobalConfig->GroupDefinitions.Find(GetGroup());
	if (GroupDefinition)
	{
		const ENPCRelation* GroupRelationPtr = (*GroupDefinition)->GroupRelations.Find(OtherNPC->GetGroup());
		if (GroupRelationPtr)
		{
			return *GroupRelationPtr;
		}
	}

	// Neutral by default
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
