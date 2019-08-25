#include "UI/InventoryItemWidget.h"
#include "ProtoGameModeBase.h"

namespace
{
const FText k_undefinedItemName = FText::FromString(TEXT("<undefined>"));
const FText k_undefinedQuantityText = FText::FromString(TEXT("0"));
}

UInventoryItemWidget::UInventoryItemWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemName = k_undefinedItemName;
	StateProgressVisibility = ESlateVisibility::HitTestInvisible;
	QuantityValueText = k_undefinedQuantityText;
}

void UInventoryItemWidget::SetItemEntry(const FInventoryItemEntry& entry)
{
	StateProgress = entry.State * 0.01f;
	QuantityValueText = FText::FromString(FString::FromInt(entry.Quantity));
	AProtoGameModeBase* gameMode = GetWorld()->GetAuthGameMode<AProtoGameModeBase>();

	FInventoryItemDef itemDef;
	if (gameMode->InventoryItemsDB->GetItemDef(entry.ItemId, itemDef))
	{
		ItemName = FText::FromString(itemDef.InventoryName);
		StateProgressVisibility = itemDef.bIsBreakable ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed;
	}
	else
	{
		ItemName = k_undefinedItemName;
		StateProgressVisibility = ESlateVisibility::Collapsed;
	}
}
