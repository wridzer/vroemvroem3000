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
	RootComponent->Mobility = EComponentMobility::Movable;
	FVector EndLocation = cable->EndLocation;

	// Add cable end
	cableEnd = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("cableEnd"));
	cableEnd->SetupAttachment(cable);
	cableEnd->Mobility = EComponentMobility::Movable;
	cableEnd->SetWorldLocation(EndLocation);

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
	physicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	physicsConstraint->SetActive(false);
	physicsConstraint->SetupAttachment(hook);

}

void AGrapplingHook::Init(AActor* _target, AActor* _owner)
{
	target = _target;
	owner = _owner;
	endLocation = _target->GetActorLocation();
}

void AGrapplingHook::StopVelocity()
{
	ProjectileMovementComponent->Velocity = FVector();
	USkeletalMeshComponent* MeshComponent = owner->FindComponentByClass<USkeletalMeshComponent>();
	if (MeshComponent) {
		// Set cable length
		//cable->CableLength = FVector::Distance(endLocation, hook->GetComponentLocation());
		//physicsConstraint->ConstraintInstance.ProfileInstance.LinearLimit.Limit = cable->CableLength;

		physicsConstraint->SetConstrainedComponents(hook, NAME_None, MeshComponent, NAME_None);
		cableEnd->ComponentVelocity = FVector();
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
		//cableEnd->AttachToComponent(owner->FindComponentByClass<USkeletalMeshComponent>(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GrapplePoint"));
		ProjectileMovementComponent->Velocity = direction * GrappleShootSpeed;
		UE_LOG(LogTemp, Warning, TEXT("here3"));
	}

	
}

// Called every frame
void AGrapplingHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

