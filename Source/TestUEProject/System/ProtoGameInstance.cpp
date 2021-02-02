#include "ProtoGameInstance.h"

namespace
{

UProtoGameInstance* GGameInstanceRef = nullptr;

}

UProtoGameInstance* UProtoGameInstance::Get()
{
	return GGameInstanceRef;
}

void UProtoGameInstance::Init()
{
	Super::Init();

	GGameInstanceRef = this;

	check(GlobalConfig);

	// Spawn game subsystems
	LivePawnsData = NewObject<ULivePawnsData>(this, FName("LivePawnsData"));
}

void UProtoGameInstance::Shutdown()
{
	Super::Shutdown();

	// Release game subsystems
	LivePawnsData = nullptr;

	GGameInstanceRef = nullptr;
}

ULivePawnsData* UProtoGameInstance::GetLivePawnsData() const
{
	return LivePawnsData;
}
