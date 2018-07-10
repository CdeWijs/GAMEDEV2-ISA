#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetupInputComponent();
}

// Find attached Physics Handle component
void UGrabber::FindPhysicsHandleComponent()
{
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (physicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handle has been found on %s!"), *GetOwner()->GetName());
	}
}

// Setup attached Input Component
void UGrabber::SetupInputComponent()
{
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent)
	{
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Release", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No input component has been foundon %s!"), *GetOwner()->GetName());
	}
}

// Raycast and grab what's in reach
void UGrabber::Grab()
{
	FHitResult hitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* componentToGrab = hitResult.GetComponent();
	AActor* actorHit = hitResult.GetActor();

	/// If we hit something then attach a physics handle
	if (actorHit)
	{
		physicsHandle->GrabComponent(
			componentToGrab, 
			NAME_None, 
			componentToGrab->GetOwner()->GetActorLocation(), 
			true
		);
	}
}

void UGrabber::Release()
{
	physicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (physicsHandle)
	{
		// If the physics handle is attached
		if (physicsHandle->GrabbedComponent)
		{
			// move the object that we're holding
			physicsHandle->SetTargetLocation(GetReachLineEnd());
		}
	}
}

// Line trace and see if we reach any actors with physics body collision channel set
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Line-trace (aka Raycast) out to reach distance
	FHitResult hitResult;
	FCollisionQueryParams traceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT hitResult, 
		GetReachLineStart(), 
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), 
		traceParameters
	);
	return hitResult;
}

// Returns current start of reach line
FVector UGrabber::GetReachLineStart()
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLocation, OUT playerViewPointRotation);
	return playerViewPointLocation;
}

// Returns current end of reach line
FVector UGrabber::GetReachLineEnd()
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLocation, OUT playerViewPointRotation);

	// Calculate the end of the line trace
	FVector lineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * reach;
	return lineTraceEnd;
}

