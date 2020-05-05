// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtractionZone.h"

#include "Components/BoxComponent.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	OverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap Component"));
	OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComponent->SetBoxExtent(FVector(200.f));
	OverlapComponent->SetHiddenInGame(false);
	RootComponent = OverlapComponent;

	OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);
}

void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapped with extraction zone"));
}

