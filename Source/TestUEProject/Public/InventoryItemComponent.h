#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryItemComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTUEPROJECT_API UInventoryItemComponent
	: public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryItemComponent();

protected:
	void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (DisplayName = "Inventory name"))
	FString m_name;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (DisplayName = "Item info ID"))
	FString m_itemInfoId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Units"))
	int m_count;
};
