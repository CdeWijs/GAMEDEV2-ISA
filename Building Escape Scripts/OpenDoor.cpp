// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
	if (GetTotalMassOfActorsInTrigger() > triggerMass) // TODO make into a parameter
	{
		onOpen.Broadcast();
	}
	else
	{
		onClose.Broadcast();
	}
}

// Returns total mass in kg
float UOpenDoor::GetTotalMassOfActorsInTrigger()
{
	float totalMass = 0.0f;

	if (trigger)
	{
		// Find all the overlapping actors
		TArray<AActor*> overlappingActors;
		trigger->GetOverlappingActors(OUT overlappingActors);

		// Iterate through them adding their masses
		for (const auto* actor : overlappingActors)
		{
			totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Please assign trigger volume to door!"));
	}
	
	return totalMass;
}

