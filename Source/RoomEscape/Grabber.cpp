// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"


#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG(LogTemp, Warning, TEXT("GRABBER REPORTING FOR DUTY"));

	///Look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		///Physics handle found
	}
	else {
	UE_LOG(LogTemp, Error, TEXT("Physics Handle not found on %s"), *(GetOwner()->GetName()))
	}
    
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Input Component not found on %s"), *(GetOwner()->GetName()))
	}
}


void UGrabber::Grab()
{
///Do something
	UE_LOG(LogTemp, Warning, TEXT(" was grabbed"))
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	// Get player viewPoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
	 OUT	PlayerViewPointLocation, 
	 OUT	PlayerViewPointRotation);

	
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	////Draw a red trace in the world to visualize
	//DrawDebugLine(
	//	GetWorld(),
	//	PlayerViewPointLocation,
	//	LineTraceEnd,
	//	FColor(255,0,0),
	//	false,
	//	0.f,
	//	0.f,
	//	10.f
	//);

	///Setup query paramters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	FHitResult Hit;

	///Ray-casting out to reach distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	//then check what we hit.

	AActor* HitObject = Hit.GetActor();

	if (HitObject) {
		UE_LOG(LogTemp, Warning, TEXT("Line Trace Hit: %s"), *(HitObject->GetName()));
	}

}

