#include "GameHUD.h"

#include <GameFramework/PlayerController.h>

void AGameHUD::OnOpenInventory_Implementation()
{

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
