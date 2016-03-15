// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;


private:
	// How far ahead of the player can we reach
	UPROPERTY(EditAnywhere)
	float GrabDistance = 100.0f;

	UPROPERTY(VisibleAnywhere)
	AActor* ActorHit = nullptr;

	bool Grabbing = false;
	
	UPROPERTY(VisibleAnywhere)
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UInputComponent* InputComponent = nullptr;

	// Find attatched Physics Handle Component
	void FindPhysicsHandleComponent();

	// Setup assumed attached Input Component
	void SetupInputComponent();

	// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();
	
	// Method used to execute code when grab key pressed
	void Grab();

	// Method used to execute code when grab key released
	void Release();

	// Get a line trace from the player's perspective
	FVector GetReachLineStart();

	// Get a line trace from the player's perspective
	FVector GetReachLineEnd();

	// Method for simple testing, logs a warning about missing component
	void LogMissing(FString ComponentReq);

	void LightUp(AActor* ActorHit, bool Highlight);
};
