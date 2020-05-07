// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "FPSGameState.h"

#define OUT

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
	
	GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bMissionSuccessful)	
{
	// Disable all input to pawns.
	// Give points to instigator of this event.
	if (InstigatorPawn)
	{
		if (SpectatingViewpointClass)
		{
			TArray<AActor*> Spectators;
			UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, OUT Spectators);

			if (Spectators.Num() > 0)
			{
				for (FConstPlayerControllerIterator Itr = GetWorld()->GetPlayerControllerIterator(); Itr; Itr++)
				{
					APlayerController* PC = Itr->Get();
				
					if (PC)
					{
						PC->SetViewTargetWithBlend(Spectators[0], 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("SpectatingViewpointClass is nullptr. Please assign a value to this field."));
		}
	}

	AFPSGameState* GameState = GetGameState<AFPSGameState>();

	if (GameState)
	{
		GameState->MulticastOnMissionComplete(InstigatorPawn, bMissionSuccessful);
	}

	OnMissionCompleted(InstigatorPawn, bMissionSuccessful);
}
