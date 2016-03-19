// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	//Get the Owner
	Owner = GetOwner();

	if(!DoorTrigger)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Door Trigger Volume"))
	}

}

void UOpenDoor::OpenDoor() {
	//Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
	UE_LOG(LogTemp, Warning, TEXT("Calling Open DOor"))
	OnOpenRequest.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	//Poll the Volume

	if (GetTotalMass() > 30.0f) {
		OpenDoor();
		DoorLastOpen = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - DoorLastOpen > DoorDelay) {
		UE_LOG(LogTemp, Warning, TEXT("This shit is getting called"))

		CloseDoor();
	}
}

float UOpenDoor::GetTotalMass()
{
	float totalMass = 0.f;

	// Find all overlapping actors
	TArray<AActor*> OverlappingActors;

	if (!DoorTrigger) { return totalMass; }
	DoorTrigger->GetOverlappingActors(OUT OverlappingActors);

	//Iterate through them adding their masses
	for(const auto& OverlappingActor : OverlappingActors)
	{
		totalMass += OverlappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return totalMass;
}