// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleComponent.h"
#include "GrappleTarget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"
#include <GrapplingHook.h>

// Sets default values for this component's properties
UGrappleComponent::UGrappleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	GrapplingHookAttachmentPoint = CreateDefaultSubobject<USceneComponent>(TEXT("GrapplingHook Attachment Point"));
	GrapplingHookAttachmentPoint->AddRelativeLocation(FVector(230, 0, 0));
	// ...
}

void UGrappleComponent::AttemptGrapple()
{
	if (grappleState == GrappleStates::RETRACTED && IsValid(currentTarget)) {
		FVector spawnPos = GrapplingHookAttachmentPoint->GetComponentLocation();
		FActorSpawnParameters SpawnInfo;
		FRotator myRot = GrapplingHookAttachmentPoint->GetComponentRotation();
		GetWorld()->SpawnActor<AGrapplingHook>(grapplingHook, spawnPos, myRot, SpawnInfo);
	}
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
		GrapplingHookAttachmentPoint->GetComponentLocation(),
		maxGrappleDist,
		ObjectTypes,
		AGrappleTarget::StaticClass(),
		TArray<AActor*>(),
		targets);

	// Find best target
	AActor* bestTarget = NULL;
	float bestAngle = INFINITY;

	// Actors to ignore
	TArray<AActor*> ignoreActors;
	ignoreActors.Init(owner, 1);

	for (int i = 0; i < targets.Num(); i++)
	{
		// Cast line to targets
		FHitResult result;
		UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			GrapplingHookAttachmentPoint->GetComponentLocation(),
			targets[i]->GetActorLocation(),
			ETraceTypeQuery(),
			false,
			ignoreActors,
			EDrawDebugTrace::None,
			result,
			true
			);
		// Check if better target
		if (targets[i] == result.GetActor()) {
			FVector normalizedDist = targets[i]->GetActorLocation() - GrapplingHookAttachmentPoint->GetComponentLocation();
			normalizedDist.Normalize();
			float currentAngle = UKismetMathLibrary::DegAcos(FVector::DotProduct(normalizedDist, camera->GetForwardVector()));
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

