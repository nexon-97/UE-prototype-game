#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryItemDef.h"
#include "InventoryItemComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTUEPROJECT_API UInventoryItemComponent
	: public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryItemComponent();

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (DisplayName = "Item info ID"))
	FString m_itemInfoId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Units"))
	int m_count;

	UFUNCTION(BlueprintCallable)
	const FString& GetInventoryName() const;
};
