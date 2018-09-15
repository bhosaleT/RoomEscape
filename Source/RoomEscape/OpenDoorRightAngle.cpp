// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoorRightAngle.h"


#define OUT
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
	Owner = GetOwner();

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing pressurePlate components"), *GetOwner()->GetName() )
	}

}

void UOpenDoorRightAngle::OpenDoor()
{
	// set the door rotation
	Owner->SetActorRotation(FRotator(0.0f, openAngle, 0.0f));
}

void UOpenDoorRightAngle::CloseDoor()
{
	// set the door rotation
	Owner->SetActorRotation(FRotator(0.0f, closeAngle, 0.0f));
}

// Called every frame
void UOpenDoorRightAngle::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);



	// ...
	//Poll the trigger volume
	if (GetTotalMassOfActorsOnPlate() > weightThreshHold) //TODO MAKE INTO A PARAMTERE
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	//Check if its time to close the door.
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) {
		CloseDoor();
	}
}

float UOpenDoorRightAngle::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;

	TArray<AActor*> OverLappingActors;
	if (!PressurePlate) { return; }
	//find all the overlapping actors and then iterate through them adding their weight.
	PressurePlate->GetOverlappingActors(OUT OverLappingActors);
	for (const auto& Actor : OverLappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName() )
	}


	return TotalMass;
}