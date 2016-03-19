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

}

void UOpenDoor::TriggerDoor(float direction) {
	Owner->SetActorRotation(FRotator(0.0f, direction, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	//Poll the Volume

	if (GetTotalMass() > 30.0f) {
		TriggerDoor(OpenAngle);
		DoorLastOpen = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - DoorLastOpen > DoorDelay) {
		TriggerDoor(CloseAngle);
	}
}

float UOpenDoor::GetTotalMass()
{
	float totalMass = 0.f;

	// Find all overlapping actors
	TArray<AActor*> OverlappingActors;

	DoorTrigger->GetOverlappingActors(OUT OverlappingActors);

	//Iterate through them adding their masses
	for(const auto& OverlappingActor : OverlappingActors)
	{
		totalMass += OverlappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return totalMass;
}