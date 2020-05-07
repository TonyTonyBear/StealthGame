// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"

#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SenseComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing Component"));

	// Bind to OnSeePawn Event.
	SenseComponent->OnSeePawn.AddDynamic(this, &AFPSAIGuard::ReactToSeeingPawn);
	SenseComponent->OnHearNoise.AddDynamic(this, &AFPSAIGuard::ReactToHearingNoise);

	GuardState = EAIState::IDLE;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();

	OriginalRotation = GetActorRotation();

	if (bIsPatroling)
	{
		MoveToNextPatrolPoint();
	}
	
}

void AFPSAIGuard::ReactToSeeingPawn(APawn * SeenPawn)
{
	if (SeenPawn == nullptr)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.f, 12, FColor::Red, false, 10.f);

	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());

	if (GM)
	{
		GM->CompleteMission(SeenPawn, false);
	}

	SetGuardState(EAIState::ALERTED);

	AController* Controller = GetController();

	if (Controller)
	{
		Controller->StopMovement();
	}
}

void AFPSAIGuard::ReactToHearingNoise(APawn * NoiseInstigator, const FVector & Location, float Volume)
{
	if (GuardState == EAIState::ALERTED)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), Location, 32.f, 12, FColor::Green, false, 10.f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.f;
	NewLookAt.Roll = 0.f;

	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.f);

	SetGuardState(EAIState::SUSPICIOUS);

	AController* Controller = GetController();

	if (Controller)
	{
		Controller->StopMovement();
	}
}

void AFPSAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::ALERTED)
	{
		return;
	}

	SetActorRotation(OriginalRotation);

	SetGuardState(EAIState::IDLE);

	if (bIsPatroling)
	{
		MoveToNextPatrolPoint();
	}
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if (NewState == GuardState)
	{
		return;
	}

	GuardState = NewState;

	OnStateChanged(GuardState);
}

void AFPSAIGuard::MoveToNextPatrolPoint()
{
	if (CurrentPatrolPoint == nullptr || CurrentPatrolPoint == SecondPatrolPoint)
	{
		CurrentPatrolPoint = FirstPatrolPoint;
	}
	else
	{
		CurrentPatrolPoint = SecondPatrolPoint;
	}

	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPatrolPoint);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// Check patrol point distances.
	if (CurrentPatrolPoint)
	{
		FVector Delta = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();
		float Distance = Delta.Size();

		if (Distance < 100.f)
		{
			MoveToNextPatrolPoint();
		}
	}
}


