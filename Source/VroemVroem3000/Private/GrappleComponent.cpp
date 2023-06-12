// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleComponent.h"
#include "GrappleTarget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"
#include <GrapplingHook.h>
#include <Kismet/GameplayStatics.h>

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
	mesh = owner->FindComponentByClass<UStaticMeshComponent>();

	// I wanted to create a empty default gameobject, but didnt know how in UE5 yet
	USceneComponent* GrappleAttachment = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("GrappleAttachment")));
	if (mesh->GetSocketByName("GrapplePoint")) {
		grappleAttachmentPoint = mesh->GetSocketLocation("GrapplePoint");
	}
	else {
		grappleAttachmentPoint = FVector(350, 0, 50);
	}
}

void UGrappleComponent::AttemptGrapple()
{
	if (grappleState == GrappleStates::RETRACTED && IsValid(currentTarget)) {
		FVector startPos = owner->GetActorLocation() + owner->GetActorRotation().RotateVector(grappleAttachmentPoint);
		FRotator myRot = camera->GetComponentRotation();
		FTransform SpawnTransform(myRot, startPos);
		currentHook = Cast<AGrapplingHook>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, grapplingHook, SpawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
		{
			currentHook->Init(currentTarget, owner);
			UGameplayStatics::FinishSpawningActor(currentHook, SpawnTransform);
		}
		grappleState = GrappleStates::FIRING;
	}
	if (grappleState == GrappleStates::ON_TARGET) {
		currentHook->Destroy();
		currentHook = nullptr;
		currentTarget = nullptr;
		grappleState = GrappleStates::RETRACTED;
	}
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

	// Actors to ignore
	TArray<AActor*> ignoreActors;
	ignoreActors.Init(owner, 1);

	for (int i = 0; i < targets.Num(); i++)
	{
		// Cast line to targets
		FHitResult result;
		UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			owner->GetActorLocation() + owner->GetActorRotation().RotateVector(grappleAttachmentPoint),
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
			FVector normalizedDist = targets[i]->GetActorLocation() - owner->GetActorLocation();
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
		if (IsValid(currentTarget))
			currentTarget->SetActive(false);
		currentTarget = selectedTarget;
		if (IsValid(currentTarget))
			currentTarget->SetActive(true);
	}

}

void UGrappleComponent::Firing()
{
	float hookDist = FVector::DistSquared(currentHook->GetActorLocation(), currentTarget->GetActorLocation());
	float ownerDist = FVector::DistSquared(owner->GetActorLocation(), currentTarget->GetActorLocation());

	if (hookDist >= ownerDist) {
		currentHook->SetActorLocation(currentTarget->GetActorLocation());
		currentHook->StopVelocity();
		grappleState = GrappleStates::ON_TARGET;
	}
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
		break;
	case GrappleStates::FIRING:
		Firing();
		break;
	case GrappleStates::ON_TARGET:
		OnTarget();
		break;
	}
}

