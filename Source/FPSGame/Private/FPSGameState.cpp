// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"
#include "FPSPlayerController.h"

void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bMissionSuccessful)
{
	for (FConstPlayerControllerIterator Itr = GetWorld()->GetPlayerControllerIterator(); Itr; Itr++)
	{
		AFPSPlayerController* PC = Cast<AFPSPlayerController>(Itr->Get());

		if (PC && PC->IsLocalController())
		{
			PC->OnMissionCompleted(InstigatorPawn, bMissionSuccessful);


			// Disable input.
			APawn* Pawn = PC->GetPawn();

			if (Pawn)
			{
				Pawn->DisableInput(PC);
			}
		}
	}
}