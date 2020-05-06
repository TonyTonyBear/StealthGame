// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Launcher.generated.h"

class UBoxComponent;

UCLASS()
class FPSGAME_API ALauncher : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALauncher();

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult
	);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	FVector LaunchForce;
};
