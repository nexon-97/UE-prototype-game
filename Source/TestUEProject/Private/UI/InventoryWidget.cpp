#include "UI/InventoryWidget.h"
#include "Character/Components/InventoryComponent.h"
#include "Character/Components/WeaponUser.h"

#include <Components/VerticalBox.h>
#include <Components/VerticalBoxSlot.h>

void UInventoryWidget::PopulateInventory()
{
	for (UUserWidget* item : ItemsWidgets)
	{
		item->RemoveFromParent();
	}
	ItemsWidgets.Empty();

	UInventoryComponent* InventoryComp = Cast<UInventoryComponent>(GetOwningPlayerPawn()->GetComponentByClass(UInventoryComponent::StaticClass()));
	check(nullptr != InventoryComp);
	UWeaponUser* WeaponUserComp = Cast<UWeaponUser>(GetOwningPlayerPawn()->GetComponentByClass(UWeaponUser::StaticClass()));
	check(nullptr != WeaponUserComp);

	for (const FInventoryItemEntry& itemEntry : InventoryComp->Items)
	{
		UUserWidget* itemWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), ItemWidgetClass);
		UVerticalBoxSlot* slot = Cast<UVerticalBoxSlot>(ItemsHostWidget->AddChild(itemWidget));

		ItemsWidgets.Add(itemWidget);
	}
	ItemsHostWidget->InvalidateLayoutAndVolatility();
}
