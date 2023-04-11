// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleComponent.h"
#include "GrappleTarget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"
#include "Components/WidgetComponent.h"

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
	grappleState = GrappleStates::RETRACTED;
	owner = GetOwner();
	camera = owner->FindComponentByClass<UCameraComponent>();
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
	AActor* bestTarget = NULL;
	float bestAngle = INFINITY;

	for (int i = 0; i < targets.Num(); i++)
	{
		// Cast line to targets
		FHitResult result;
		UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			owner->GetActorLocation(),
			targets[i]->GetActorLocation(),
			ETraceTypeQuery(),
			false,
			TArray<AActor*>(),
			EDrawDebugTrace::None,
			result,
			true
			);
		// Check if better target
		/*if (targets[i] == result.GetActor()) */if (result.ImpactPoint == targets[i]->GetActorLocation()) {
			FVector normalizedDist = targets[i]->GetActorLocation() - owner->GetActorLocation();
			normalizedDist.Normalize();
			float currentAngle = UKismetMathLibrary::DegAcos(FVector::DotProduct(normalizedDist, camera->GetForwardVector()));
			// UE_LOG(LogTemp, Warning, TEXT("Some variable values: x: %f"), currentAngle);
			if (currentAngle < bestAngle || !IsValid(bestTarget)) {
				bestAngle = currentAngle;
				bestTarget = targets[i];
			}
		}
	}

	// Set visibility
	AGrappleTarget* selectedTarget = Cast<AGrappleTarget>(bestTarget);
	if (selectedTarget != currentTarget || !IsValid(currentTarget)) {
		if (IsValid(currentTarget)) { currentTarget->SetActive(false); }
		currentTarget = selectedTarget;
		if (IsValid(currentTarget))
			currentTarget->SetActive(true);
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

