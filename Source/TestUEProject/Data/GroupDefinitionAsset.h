#pragma once
#include "NPC/NPCRelation.h"
#include "Engine/DataAsset.h"
#include "GroupDefinitionAsset.generated.h"

UCLASS(Blueprintable)
class UGroupDefinitionAsset
	: public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, ENPCRelation> GroupRelations;
};
