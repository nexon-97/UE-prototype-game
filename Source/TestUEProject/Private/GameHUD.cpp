#include "GameHUD.h"

#include <GameFramework/PlayerController.h>
#include <Components/PanelWidget.h>
#include <Components/CanvasPanelSlot.h>
#include <Components/CanvasPanel.h>

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

	check(nullptr != OverlaysHostWidget);

	InventoryWidget = CreateWidget<UInventoryWidget>(GetOwningPlayerController(), InventoryWidgetClass);
	InventoryWidget->AddToViewport();
	InventoryWidget->SetVisibility(bIsInventoryOpen ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(OverlaysHostWidget->AddChild(InventoryWidget));
	FAnchorData anchorData;
	anchorData.Anchors = FAnchors(0.f, 0.f, 1.f, 1.f);
	anchorData.Offsets = FMargin();
	anchorData.Alignment = FVector2D(0.f, 0.f);
	slot->SetLayout(anchorData);
	OverlaysHostWidget->InvalidateLayoutAndVolatility();
}

void AGameHUD::OnOpenInventory()
{
	bIsInventoryOpen = !bIsInventoryOpen;
	GetOwningPlayerController()->bShowMouseCursor = bIsInventoryOpen;
	InventoryWidget->SetVisibility(bIsInventoryOpen ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	if (bIsInventoryOpen)
	{
		GainPlayerInputFocus();

		InventoryWidget->PopulateInventory();
	}
	else
	{
		FreePlayerInputFocus();
	}
}

void AGameHUD::GainPlayerInputFocus()
{
	if (!bHasPlayerInputFocus)
	{
		// Disconnect pawn input
		APawn* controlledPawn = GetOwningPlayerController()->GetPawn();
		controlledPawn->DisableInput(GetOwningPlayerController());

		// Add UI input processing
		FInputModeGameAndUI gameAndUiInputMode;
		gameAndUiInputMode.SetHideCursorDuringCapture(false);
		gameAndUiInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		GetOwningPlayerController()->SetInputMode(gameAndUiInputMode);

		bHasPlayerInputFocus = true;
	}
}

void AGameHUD::FreePlayerInputFocus()
{
	if (bHasPlayerInputFocus)
	{
		// Activate pawn input again
		APawn* controlledPawn = GetOwningPlayerController()->GetPawn();
		controlledPawn->EnableInput(GetOwningPlayerController());

		// Disable UI input totally
		FInputModeGameOnly gameOnlyInputMode;
		gameOnlyInputMode.SetConsumeCaptureMouseDown(false);
		GetOwningPlayerController()->SetInputMode(gameOnlyInputMode);

		bHasPlayerInputFocus = false;
	}
}
