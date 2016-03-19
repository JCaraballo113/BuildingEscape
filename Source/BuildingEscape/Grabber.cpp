// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Check if we are hitting anything AND looking at it
	if(!Grabbing)
	{
		GetFirstPhysicsBodyInReach();
	}

	if (!PhysicsHandle) { return; }

	// If the physics handle is attached
	if(PhysicsHandle->GrabbedComponent)
	{
		// move the object that we're holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

// Look for attached Physics Handle Component
void UGrabber::FindPhysicsHandleComponent()
{
	// Look for attatched Physics Handle Component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
	{
		LogMissing("Physics Handle Component");
	}
}

// Look for attached Input Component (only appears at run time)
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		LogMissing("Input Component");
	}
}

void UGrabber:: Grab()
{
	Grabbing = true;
	// LINE TRACE and see if we reach any actors with the physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); // Gets the mesh in our case
	auto ActorHit = HitResult.GetActor();

	if(ActorHit)
	{
		LightUp(ActorHit, false);
		// attach physics handle
		PhysicsHandle->GrabComponent(
			ComponentToGrab, // thing to grab with our 'hands'
			NAME_None, // no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
			);
	}
	
}

void UGrabber:: Release()
{
	PhysicsHandle->ReleaseComponent();
	Grabbing = false;
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Setup query parameters to ignore coliding with ourselves
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Line trace (AKA ray-cast) out to reach distance
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
		);

	if(HitResult.GetActor())
	{
		ActorHit = HitResult.GetActor();
		LightUp(ActorHit, true);
	}
	else if(ActorHit)
	{
		LightUp(ActorHit, false);
		ActorHit = nullptr;
	}

	return HitResult;
}

FVector UGrabber::GetReachLineStart()
{
	/// Get player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
		);

	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	/// Get player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
		);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * GrabDistance;
}

void UGrabber::LightUp(AActor* ActorHit, bool Highlight)
{
	UStaticMeshComponent* Mesh = ActorHit->FindComponentByClass<UStaticMeshComponent>();

	Mesh->SetRenderCustomDepth(Highlight);
}

void UGrabber::LogMissing(FString ComponentReq)
{
	UE_LOG(LogTemp, Warning, TEXT("No %s found. Please attatch one to %s."), *ComponentReq, *GetOwner()->GetName())
}
