#pragma once
#include "Engine/GameInstance.h"
#include "NPC/Services/EnemyDetectionService.h"
#include "DB/GlobalGameConfig.h"
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

	UEnemyDetectionService* GetEnemyDetectionService() const;
	
public:
	UPROPERTY(EditDefaultsOnly)
	UGlobalGameConfig* GlobalConfig;

private:
	UPROPERTY(Transient)
	UEnemyDetectionService* EnemyDetectionService;
};
