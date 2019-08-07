#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "InventoryItemDef.generated.h"

USTRUCT(BlueprintType)
struct TESTUEPROJECT_API FInventoryItemDef
	: public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	FInventoryItemDef()
	{}

	/* Unique string id assigned to an item */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Id;

	/* Displayed inventory name */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString InventoryName;

	/* Max size of a stack for an item */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture> InventoryIcon;
};
