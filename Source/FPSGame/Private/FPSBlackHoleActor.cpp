// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBlackHoleActor.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"

#define OUT

// Sets default values
AFPSBlackHoleActor::AFPSBlackHoleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComponent;

	InnerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Inner Sphere"));
	InnerSphere->SetSphereRadius(100);
	InnerSphere->SetupAttachment(MeshComponent);
	InnerSphere->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHoleActor::OverlapInnerSphere);
	
	OuterSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Outer Sphere"));
	OuterSphere->SetSphereRadius(3000);
	OuterSphere->SetupAttachment(MeshComponent);
}

// Called every frame
void AFPSBlackHoleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComponents;

	GetOverlappingComponents(OUT OverlappingComponents);

	for (int i = 0; i < OverlappingComponents.Num(); i++)
	{
		UPrimitiveComponent* OverlappingComponent = OverlappingComponents[i];

		if (OverlappingComponent && OverlappingComponent->IsSimulatingPhysics())
		{
			const float Radius = OuterSphere->GetScaledSphereRadius();
			const float Force = -2000.f;

			OverlappingComponent->AddRadialForce(GetActorLocation(), Radius, Force, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
}

void AFPSBlackHoleActor::OverlapInnerSphere(
	UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult
)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

