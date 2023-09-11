// Fill out your copyright notice in the Description page of Project Settings.


#include "GrapplingHook.h"
#include "CableComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "GrappleTarget.h"

// Sets default values
AGrapplingHook::AGrapplingHook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Create Cable
	cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	RootComponent = cable;

	// Add cable end
	cableEnd = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("cableEnd"));
	cableEnd->SetupAttachment(cable);
	cableEnd->Mobility = EComponentMobility::Movable;
	cableEnd->SetSimulatePhysics(true);

	// Create projectile
	hook = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hook"));
	hook->SetupAttachment(cable);
	hook->Mobility = EComponentMobility::Movable;

	// Add projectile movement component
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(hook);
	ProjectileMovementComponent->InitialSpeed = GrappleShootSpeed;
	ProjectileMovementComponent->MaxSpeed = GrappleShootSpeed;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	// Add physics constraint
	FVector EndLocation = cable->EndLocation;
	physicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	physicsConstraint->SetWorldLocation(EndLocation);
	physicsConstraint->SetActive(false);
	//physicsConstraint->SetupAttachment(cable);

}

void AGrapplingHook::Init(AActor* _target, AActor* _owner)
{
	target = _target;
	owner = _owner;
	endLocation = _target->GetActorLocation();

	/*USkeletalMeshComponent* SkeletalMeshComponent = owner->FindComponentByClass<USkeletalMeshComponent>();
	if (SkeletalMeshComponent)
	{
		physicsConstraint->AttachToComponent(SkeletalMeshComponent, FAttachmentTransformRules::KeepWorldTransform, TEXT("GrapplePoint"));

	}*/
}

void AGrapplingHook::StopVelocity()
{
	ProjectileMovementComponent->Velocity = FVector();
	USkeletalMeshComponent* MeshComponent = owner->FindComponentByClass<USkeletalMeshComponent>();
	if (MeshComponent) {
		// Set cable length
		//cable->CableLength = FVector::Distance(endLocation, hook->GetComponentLocation());
		//physicsConstraint->ConstraintInstance.ProfileInstance.LinearLimit.Limit = cable->CableLength;

		physicsConstraint->SetActive(true);
	}
}

// Called when the game starts or when spawned
void AGrapplingHook::BeginPlay()
{
	Super::BeginPlay();
	
	if (!owner) {
	}
	else {
		startLocation = GetActorLocation();
		direction = endLocation - startLocation;
		direction.Normalize();
		FVector ownerLocation = owner->GetActorLocation();
		cableEnd->SetWorldLocation(ownerLocation);
		cableEnd->AttachToComponent(owner->FindComponentByClass<USkeletalMeshComponent>(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GrapplePoint"));
		ProjectileMovementComponent->Velocity = direction * GrappleShootSpeed;
	}

	physicsConstraint->SetConstrainedComponents(cable, NAME_None, cableEnd, NAME_None);
}

// Called every frame
void AGrapplingHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

