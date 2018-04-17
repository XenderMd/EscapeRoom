// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
	// ...
	
}

//look for attached Input Component
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component found!"));
		///Bind the input axis
		InputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component not found!"));
	}
}

//look for attached Physics Component
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
	{
		//Physics handle is found
	}
	else {

		UE_LOG(LogTemp, Warning, TEXT("Physics Handle is missing"));
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent)
	{
		FVector PlayerViewPointLocation, LineTraceEnd;
		FRotator PlayerViewPointRotation;

		// Get the player viewpoint this tick
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
		LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
	//if the physics handle is attached
	     // move the object that we're holding
	
}

FHitResult UGrabber::GetFirstBodyinReach()
{
	AActor *ActorHit;
	FHitResult Hit;
	FVector PlayerViewPointLocation, LineTraceEnd;
	FRotator PlayerViewPointRotation;

	// Get the player viewpoint this tick
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;
	
	/// Ray-cast out to reach distance
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters);

	ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		//See what we hit
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()));
	}

	return Hit;
}

void UGrabber::Grab()
{
	//LINE TRACE here and see if we reach any actors with physcis body collision channel set
	auto HitResult = GetFirstBodyinReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	// If we hit something, theen attach a physcs handle
	if (ActorHit)
	{
		//TODO attach physics handle
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Object released"));
	PhysicsHandle->ReleaseComponent();
	//TODO release physics handle
}

