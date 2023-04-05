// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleComponent.h"
#include "GrappleTarget.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UGrappleComponent::UGrappleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrappleComponent::BeginPlay()
{
	Super::BeginPlay();

	owner = GetOwner();
	
}

void UGrappleComponent::Retracted()
{
	// Objecttypes
	TArray< TEnumAsByte< EObjectTypeQuery > > ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	// Out array
	TArray<AActor*> targets;

	// Look for targets
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		owner->GetActorLocation(),
		maxGrappleDist,
		ObjectTypes,
		AGrappleTarget::StaticClass(),
		TArray<AActor*>(),
		targets);

	// Find best target
	FHitResult bestTarget;

	for (int i = 0; i < targets.Num(); i++) // AActor* const target in targets)
	{
		UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			owner->GetActorLocation(),
			targets[i]->GetActorLocation(),
			ETraceTypeQuery(),
			false,
			TArray<AActor*>(),
			EDrawDebugTrace::None,
			bestTarget,
			true
			);

	}


}

void UGrappleComponent::Firing()
{
}

void UGrappleComponent::NearingTarget()
{
}

void UGrappleComponent::OnTarget()
{
}


// Called every frame
void UGrappleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	switch (grappleState) {
	case GrappleStates::RETRACTED:
		Retracted();
	case GrappleStates::FIRING:
		Firing();
	case GrappleStates::NEARING_TARGET:
		NearingTarget();
	case GrappleStates::ON_TARGET:
		OnTarget();
	}
}

