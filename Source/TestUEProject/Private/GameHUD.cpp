#include "GameHUD.h"

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

		bHasPlayerInputFocus = false;
	}
}
