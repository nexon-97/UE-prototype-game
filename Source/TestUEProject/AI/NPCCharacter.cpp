#include "AI/NPCCharacter.h"

ANPCCharacter::ANPCCharacter(const FObjectInitializer& ObjectInitializer)
{
	UNPCInfo* NPCInfo = CreateDefaultSubobject<UNPCInfo>(TEXT("NPCInfo"));
	NPCInfo->bEditableWhenInherited = true;
}

void ANPCCharacter::BeginPlay()
{
	NPCInfoComponent = FindComponentByClass<UNPCInfo>();
	
	Super::BeginPlay();
}

UNPCInfo* ANPCCharacter::GetNPCInfo() const
{
	return NPCInfoComponent;
}
