// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

#define OUT

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn)	
{
	// Disable all input to pawns.
	// Give points to instigator of this event.
	if (InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr); // We use nullptr to get the default character controller for the game mode.

		if (SpectatingViewpointClass)
		{
			APlayerController* Controller = Cast<APlayerController>(InstigatorPawn);

			TArray<AActor*> Spectators;
			UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, OUT Spectators);

			if (Controller && Spectators.Num() > 0)
			{
				Controller->SetViewTargetWithBlend(Spectators[0], 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("SpectatingViewpointClass is nullptr. Please assign a value to this field."));
		}
	}

	OnMissionCompleted(InstigatorPawn);
}
