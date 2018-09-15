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
    
	///Look for the physicshandle.
	FindPhysicsHandleComponent();
    
	///Look for InputComponent.
	FindInputComponent();
	
}

void UGrabber::FindPhysicsHandleComponent() {
	///Look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		///Physics handle not found
		UE_LOG(LogTemp, Error, TEXT("Physics Handle not found on %s"), *(GetOwner()->GetName()))
	}
}

void UGrabber::FindInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Input Component not found on %s"), *(GetOwner()->GetName()))
	}
}

void UGrabber::Grab()
{
///Do something
	UE_LOG(LogTemp, Warning, TEXT(" was grabbed"))

	/// Try and reach any actors with physics body collision channel set
     auto HitResult = GetFirstPhysicsBodyInReach();
	 auto ComponentToGrab = HitResult.GetComponent();
	 auto ActorHit = HitResult.GetActor();

    ///If we hit something then attach a physics handle
	 if (ActorHit)
	 {
		 //attach physics handle
		 PhysicsHandle->GrabComponentAtLocationWithRotation(
			 ComponentToGrab,
			 NAME_None, //no bones needed
			 ComponentToGrab->GetOwner()->GetActorLocation(),
			 ComponentToGrab->GetOwner()->GetActorRotation()
		 );
	 }
}

void UGrabber::Release()
{
	///Do something
	UE_LOG(LogTemp, Warning, TEXT(" was Released"))
		PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	///TODO if the physics handle is attatched move the object we're holding
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}


const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	///Setup query paramters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	FHitResult Hit;

	///Ray-casting out to reach distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	return Hit;
}


FVector UGrabber::GetReachLineStart() {

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT	PlayerViewPointLocation,
		OUT	PlayerViewPointRotation);

	return PlayerViewPointLocation;

}

FVector UGrabber::GetReachLineEnd() {

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT	PlayerViewPointLocation,
		OUT	PlayerViewPointRotation);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

}