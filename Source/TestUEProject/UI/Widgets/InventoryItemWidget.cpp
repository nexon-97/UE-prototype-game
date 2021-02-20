#include "UI/Widgets/InventoryItemWidget.h"
#include "System/ProtoGameModeBase.h"

#include "Kismet/GameplayStatics.h"

namespace
{
	
	const FText k_undefinedItemName = FText::FromString(TEXT("<undefined>"));
	const FText k_undefinedQuantityText = FText::FromString(TEXT("0"));
	const float k_timeDeltaForDoubleClick = 0.25f;
	
}

UInventoryItemWidget::UInventoryItemWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemName = k_undefinedItemName;
	StateProgressVisibility = ESlateVisibility::HitTestInvisible;
	QuantityValueText = k_undefinedQuantityText;
}

void UInventoryItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr != InteractionWidget)
	{
		InteractionWidget->OnClicked.AddDynamic(this, &UInventoryItemWidget::OnClickInternal);
	}
}

void UInventoryItemWidget::OnClickInternal()
{
	InventoryItemClickDelegate.Broadcast();

	const float timeStamp = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	if (LastClickTime > 0.f)
	{
		const float timeDelta = timeStamp - LastClickTime;
		if (timeDelta < k_timeDeltaForDoubleClick)
		{
			InventoryItemDblClickDelegate.Broadcast();
		}
	}

	LastClickTime = timeStamp;
}

void UInventoryItemWidget::SetItemEntry(const FInventoryItemEntry& Entry)
{
	ItemEntry = Entry;

	StateProgress = Entry.State * 0.01f;
	QuantityValueText = FText::FromString(FString::FromInt(Entry.Quantity));
	AProtoGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AProtoGameModeBase>();

	if (GameMode->InventoryItemsDB->GetItemDef(Entry.ItemId, ItemDef))
	{
		ItemName = FText::FromString(ItemDef->InventoryName);
		StateProgressVisibility = ItemDef->bIsBreakable ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed;
	}
	else
	{
		ItemName = k_undefinedItemName;
		StateProgressVisibility = ESlateVisibility::Collapsed;
		ItemDef = nullptr;
	}
}

const FInventoryItemEntry& UInventoryItemWidget::GetInventoryItemEntry() const
{
	return ItemEntry;
}

const FInventoryItemDef& UInventoryItemWidget::GetInventoryItemDef() const
{
	return *ItemDef;
}
