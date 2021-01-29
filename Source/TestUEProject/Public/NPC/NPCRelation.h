#pragma once

UENUM(BlueprintType)
enum class ENPCRelation : uint8
{
	Enemy UMETA(DisplayName = "Enemy"),
	Objection UMETA(DisplayName = "Objection"),
	Neutral UMETA(DisplayName = "Neutral"),
	Mate UMETA(DisplayName = "Mate"),
	Friend UMETA(DisplayName = "Friend"),
};
