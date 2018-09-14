// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoorRightAngle.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"


// Sets default values for this component's properties
UOpenDoorRightAngle::UOpenDoorRightAngle()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoorRightAngle::BeginPlay()
{
	Super::BeginPlay();

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoorRightAngle::OpenDoor()
{
	// ...
	//finding the owner
	AActor* Owner = GetOwner();

	//creating a rotator
	FRotator newRotation = FRotator(0.0f, 90.0f, 0.0f);

	// set the door rotation
	Owner->SetActorRotation(newRotation);
}


// Called every frame
void UOpenDoorRightAngle::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//Poll the trigger volume
	if (PressurePlate->IsOverlappingActor(ActorThatOpens)) {
		OpenDoor();
	}
}

