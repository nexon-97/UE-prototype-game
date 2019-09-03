#include "UI/InventoryWidget.h"
#include "Character/Components/InventoryComponent.h"
#include "Character/Components/WeaponUser.h"
#include "Character/PlayerCharacter.h"
#include "ProtoGameModeBase.h"
#include "ProtoPlayerController.h"

#include <Components/VerticalBox.h>
#include <Components/VerticalBoxSlot.h>
#include <Engine.h>

void UInventoryWidgetClickContext::OnClick()
{
	const FInventoryItemEntry& itemEntry = InventoryItemWidget->GetInventoryItemEntry();
	const FInventoryItemDef& itemDef = InventoryItemWidget->GetInventoryItemDef();

	bool isWeapon = itemDef.WorldActorType->IsChildOf(AWeaponBase::StaticClass());
	if (isWeapon)
	{
		// Double click on a weapon tries to put it into slot

		// Create weapon actor instance
		if (itemDef.WorldActorType)
		{
			AWeaponBase* weaponInstance = OwningWorld->SpawnActor<AWeaponBase>(itemDef.WorldActorType.Get());
			EWeaponSlotType weaponSlot = weaponInstance->weaponSlotType;

			// Find weapon user component in pawn, and put the created weapon to slot with policy to hide existing slot weapon to inventory
			AProtoPlayerController* controller = OwningWorld->GetFirstPlayerController<AProtoPlayerController>();
			APawn* pawn = controller->GetPawn();

			UWeaponUser* weaponUserComp = Cast<UWeaponUser>(pawn->GetComponentByClass(UWeaponUser::StaticClass()));
			weaponUserComp->SetWeaponAtSlot(weaponSlot, weaponInstance, EWeaponUnequipMethod::HideToInventory);

			// Remove item from inventory
			UInventoryComponent* inventoryComp = Cast<UInventoryComponent>(pawn->GetComponentByClass(UInventoryComponent::StaticClass()));
			inventoryComp->RemoveItem(itemEntry.EntryId);

			// Refresh items
			InventoryWidget->PopulateInventory();
		}
	}
}

UWorld* UInventoryWidgetClickContext::GetWorld() const
{
	return OwningWorld;
}

void UInventoryWidget::PopulateInventory()
{
	// Remove current inventory item widgets and click handler contexts
	for (UUserWidget* item : ItemsWidgets)
	{
		item->RemoveFromParent();
	}
	ItemsWidgets.Empty();
	ClickContexts.Empty();

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

		UInventoryWidgetClickContext* context = NewObject<UInventoryWidgetClickContext>();
		context->InventoryItemWidget = itemWidget;
		context->InventoryWidget = this;
		context->OwningWorld = GetOwningPlayer()->GetWorld();
		itemWidget->InventoryItemDblClickDelegate.AddDynamic(context, &UInventoryWidgetClickContext::OnClick);
		ClickContexts.Add(context);

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

			//auto clickHandler = [widgetData.Value]()
			//{
			//
			//};
			//UInventoryWidgetClickContext* context = NewObject<UInventoryWidgetClickContext>();
			//context->InventoryItemWidget = widgetData.Value;
			//widgetData.Value->InventoryItemDblClickDelegate.AddDynamic(context, &UInventoryWidgetClickContext::OnClick);
			//widgetData.Value->InventoryItemDblClickDelegate.CreateStatic(clickHandler);
			//ClickContexts.Add(context);
		}

		widgetData.Value->SetVisibility(hasItemAtSlot ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}
