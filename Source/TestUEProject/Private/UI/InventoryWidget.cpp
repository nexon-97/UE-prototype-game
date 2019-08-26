#include "UI/InventoryWidget.h"
#include "Character/Components/InventoryComponent.h"
#include "Character/Components/WeaponUser.h"
#include "ProtoGameModeBase.h"

#include <Components/VerticalBox.h>
#include <Components/VerticalBoxSlot.h>

void UInventoryWidget::PopulateInventory()
{
	// Remove current inventory item widgets and click handler contexts
	for (UUserWidget* item : ItemsWidgets)
	{
		item->RemoveFromParent();
	}
	ItemsWidgets.Empty();

	// Search for inventory and weapon user components, needed to fill the widgets data
	UInventoryComponent* InventoryComp = Cast<UInventoryComponent>(GetOwningPlayerPawn()->GetComponentByClass(UInventoryComponent::StaticClass()));
	check(nullptr != InventoryComp);
	UWeaponUser* WeaponUserComp = Cast<UWeaponUser>(GetOwningPlayerPawn()->GetComponentByClass(UWeaponUser::StaticClass()));
	check(nullptr != WeaponUserComp);

	AProtoGameModeBase* gameMode = GetWorld()->GetAuthGameMode<AProtoGameModeBase>();

	// Populate inventory items widgets
	for (const FInventoryItemEntry& itemEntry : InventoryComp->Items)
	{
		UInventoryItemWidget* itemWidget = CreateWidget<UInventoryItemWidget>(GetOwningPlayer(), ItemWidgetClass);
		itemWidget->SetItemEntry(itemEntry);
		UVerticalBoxSlot* slot = Cast<UVerticalBoxSlot>(ItemsHostWidget->AddChild(itemWidget));

		ItemsWidgets.Add(itemWidget);
	}
	ItemsHostWidget->InvalidateLayoutAndVolatility();

	// Populate weapon slots widgets
	for (auto widgetData : WeaponSlotWidgets)
	{
		AWeaponBase* weapon = WeaponUserComp->GetWeaponAtSlot(widgetData.Key);
		bool hasItemAtSlot = (nullptr != weapon);

		if (hasItemAtSlot)
		{
			FInventoryItemEntry fakeWeaponItemEntry;
			fakeWeaponItemEntry.ItemId = weapon->InventoryId;
			fakeWeaponItemEntry.Quantity = 1;
			fakeWeaponItemEntry.State = weapon->WeaponState;

			widgetData.Value->SetItemEntry(fakeWeaponItemEntry);
		}

		widgetData.Value->SetVisibility(hasItemAtSlot ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}
