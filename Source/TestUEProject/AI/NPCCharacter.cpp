#include "AI/NPCCharacter.h"
#include "NPC/Components/NPCInfo.h"

ANPCCharacter::ANPCCharacter(const FObjectInitializer& ObjectInitializer)
{
	UNPCInfo* NPCInfo = CreateDefaultSubobject<UNPCInfo>(TEXT("NPCInfo"));
	NPCInfo->bEditableWhenInherited = true;
}

void ANPCCharacter::BeginPlay()
{
	Super::BeginPlay();
}
