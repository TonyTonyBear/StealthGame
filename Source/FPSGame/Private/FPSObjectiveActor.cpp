// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSObjectiveActor.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"

// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	RootComponent = MeshComponent;
	SphereComponent->SetupAttachment(MeshComponent);

	SetReplicates(true);
}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSObjectiveActor::PlayEffects()
{
	if (PickupEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, PickupEffect, GetActorLocation());
	}
}

void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	PlayEffects();

	if (Role == ROLE_Authority)
	{
		AFPSCharacter* PlayerCharacter = Cast<AFPSCharacter>(OtherActor);

		if (PlayerCharacter)
		{
			PlayerCharacter->bIsCarryingObjective = true;

			Destroy();
		}
	}

}

