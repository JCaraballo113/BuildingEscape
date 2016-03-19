// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();


	void TriggerDoor(float direction);

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	UPROPERTY(EditAnywhere)
	float OpenAngle = -90.0f;

	UPROPERTY(EditAnywhere)
	float DoorDelay = 1.0f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* DoorTrigger = nullptr;
		
	UPROPERTY(VisibleAnywhere)
	AActor* ActorThatOpens = nullptr;

	UPROPERTY(VisibleAnywhere)
	AActor* Owner = nullptr;

	float CloseAngle = 0.0f;
	float DoorLastOpen = 0.0f;

	float GetTotalMass();

};
