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

	// Spawn services for game mode
	EnemyDetectionService = NewObject<UEnemyDetectionService>(this, FName("EnemyDetectionService"));
}

void UProtoGameInstance::Shutdown()
{
	Super::Shutdown();

	EnemyDetectionService = nullptr;

	GGameInstanceRef = nullptr;
}
