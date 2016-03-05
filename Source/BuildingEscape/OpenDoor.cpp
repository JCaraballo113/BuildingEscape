// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"


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

	//Get the actor that opens the door
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn(); //The controller is the 'Mind' the Pawn is the 'Body' which is what I want
	
}

void UOpenDoor::TriggerDoor(float direction) {
	Owner->SetActorRotation(FRotator(0.0f, direction, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (DoorTrigger->IsOverlappingActor(ActorThatOpens)) {
		TriggerDoor(OpenAngle);
		DoorLastOpen = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - DoorLastOpen > DoorDelay) {
		TriggerDoor(CloseAngle);
	}
}

