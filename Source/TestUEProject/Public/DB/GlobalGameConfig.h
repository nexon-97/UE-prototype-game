﻿#pragma once
#include "Engine/DataAsset.h"
#include "DB/GroupDefinitionAsset.h"
#include "GlobalGameConfig.generated.h"

UCLASS(Blueprintable)
class UGlobalGameConfig
    : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, UGroupDefinitionAsset*> GroupDefinitions;
};
