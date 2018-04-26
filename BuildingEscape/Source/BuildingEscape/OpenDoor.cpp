// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (!PressurePlate) { UE_LOG(LogTemp, Warning, TEXT("No assignement for the PressurePlate pointer")); }
}


void UOpenDoor::CloseDoor()
{
	if (!Owner) { return; }	
	//Set door rotation
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Vollume
	// If the ActorThatOpens is in the volume
	if (GetTotalMassOfActorsOnPlate()>= TriggerMass)
	{
		OnOpen.Broadcast();
	}
	else 
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;
	TArray<AActor*> OverlappingActors;
	// Find all the overlapping actors

	if (!PressurePlate) { return -1.0f; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	
	for (const auto& OverlappingActor:OverlappingActors)
	{
		// Add their masses
		TotalMass += OverlappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *OverlappingActor->GetName());
	}
	return TotalMass;
}