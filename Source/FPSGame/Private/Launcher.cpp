// Fill out your copyright notice in the Description page of Project Settings.


#include "Launcher.h"

#include "Components/BoxComponent.h"
#include "FPSCharacter.h"

#define OUT

// Sets default values
ALauncher::ALauncher()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->bHiddenInGame = false;
	BoxComponent->SetBoxExtent(FVector(100.f));
	RootComponent = BoxComponent;

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALauncher::OnOverlap);
}

void ALauncher::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFPSCharacter* Player = Cast<AFPSCharacter>(OtherActor);

	if (Player)
	{
		Player->LaunchCharacter(LaunchForce, false, false);
	}
	else
	{
		// Check to see if we can throw it anyway.
		TArray<UPrimitiveComponent*> OverlappingComponents;
		GetOverlappingComponents(OUT OverlappingComponents);

		for (int i = 0; i < OverlappingComponents.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("Adding force."));
			OverlappingComponents[i]->AddImpulse(LaunchForce, NAME_None, true);
		}
	}
}

