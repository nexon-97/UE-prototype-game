#pragma once
#include "Engine/GameInstance.h"
#include "NPC/Services/EnemyDetectionService.h"
#include "ProtoGameInstance.generated.h"

UCLASS()
class TESTUEPROJECT_API UProtoGameInstance
	: public UGameInstance
{
	GENERATED_BODY()

public:
	static UProtoGameInstance* Get();

	void Init() override;
	void Shutdown() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UEnemyDetectionService* EnemyDetectionService;
};
