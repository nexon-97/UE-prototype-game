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

	// Spawn services
	EnemyDetectionService = NewObject<UEnemyDetectionService>(this, FName("EnemyDetectionService"));
}

void UProtoGameInstance::Shutdown()
{
	Super::Shutdown();

	// Release services
	EnemyDetectionService = nullptr;

	GGameInstanceRef = nullptr;
}

UEnemyDetectionService* UProtoGameInstance::GetEnemyDetectionService() const
{
	return EnemyDetectionService;
}
