#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NPC/NPCRelation.h"
#include "NPCInfo.generated.h"

UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent))
class TESTUEPROJECT_API UNPCInfo
	: public UActorComponent
{
	GENERATED_BODY()

public:
	UNPCInfo(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	static void OverrideNextIngameId(const int32 NextIngameId);

	UFUNCTION(BlueprintCallable)
	int32 GetId() const;

	UFUNCTION(BlueprintCallable)
	const FString& GetQuestId() const;

	UFUNCTION(BlueprintCallable)
	void SetQuestId(const FString& NewQuestId);

	UFUNCTION(BlueprintCallable)
	void OverrideNPCRelation(UNPCInfo* OtherNPC, ENPCRelation RelationOverride);

	UFUNCTION(BlueprintCallable)
	ENPCRelation GetNPCRelation(UNPCInfo* OtherNPC) const;

	UFUNCTION(BlueprintCallable)
	void SetGroup(const FName& Group);

	UFUNCTION(BlueprintCallable)
	const FName& GetGroup() const;

private:
	// Unique integer id of the NPC in the game world, can't be overriden
	UPROPERTY()
	int32 IngameId;

	// Is used for non generic characters, for example quest system can use them to do specific stuff, and configure properly
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FString QuestId;

	// Group name NPC belongs to
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FName Group;

	// Subgroup name NPC belongs to
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FName SubGroup;

	// Personal relations with other NPCs
	UPROPERTY()
	TMap<int32, ENPCRelation> NPCRelationOverrides;
};
