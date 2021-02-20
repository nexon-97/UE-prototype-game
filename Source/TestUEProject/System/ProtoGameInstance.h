#pragma once
#include "Engine/GameInstance.h"
#include "Data/GlobalGameConfig.h"
#include "Data/LivePawnsData.h"
#include "ProtoGameInstance.generated.h"

UCLASS()
class TESTUEPROJECT_API UProtoGameInstance
	: public UGameInstance
{
	GENERATED_BODY()

public:
	static UProtoGameInstance* Get();

	virtual void Init() override;
	virtual void Shutdown() override;

	ULivePawnsData* GetLivePawnsData() const;
	
public:
	UPROPERTY(EditDefaultsOnly)
	UGlobalGameConfig* GlobalConfig;

private:	
	UPROPERTY(Transient)
	ULivePawnsData* LivePawnsData;
};
