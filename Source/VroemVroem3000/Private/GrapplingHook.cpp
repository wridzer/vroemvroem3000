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

	// Create projectile
	hook = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hook"));
	hook->SetupAttachment(RootComponent);
	RootComponent = hook;
	
	// Add projectile movement component
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(hook);
	ProjectileMovementComponent->InitialSpeed = GrappleShootSpeed;
	ProjectileMovementComponent->MaxSpeed = GrappleShootSpeed;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	// Add physics constraint
	physicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	physicsConstraint->SetupAttachment(hook);

	physicsConstraint->ConstraintInstance.SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0);
	physicsConstraint->ConstraintInstance.SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0);
	physicsConstraint->ConstraintInstance.SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0);

	physicsConstraint->ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 45.0f);
	physicsConstraint->ConstraintInstance.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	physicsConstraint->ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);

	physicsConstraint->ConstraintInstance.ProfileInstance.ConeLimit.Stiffness = 50.0f;
	physicsConstraint->ConstraintInstance.ProfileInstance.ConeLimit.Damping = 10.0f;
	physicsConstraint->ConstraintInstance.ProfileInstance.TwistLimit.Stiffness = 50.0f;
	physicsConstraint->ConstraintInstance.ProfileInstance.TwistLimit.Damping = 10.0f;


	// Create Cable
	cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	cable->SetupAttachment(hook);
	cable->AttachToComponent(hook, FAttachmentTransformRules::KeepRelativeTransform);
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
	// Attach physics
	USkeletalMeshComponent* MeshComponent = owner->FindComponentByClass<USkeletalMeshComponent>();
	if (MeshComponent) {
		physicsConstraint->AttachToComponent(MeshComponent, FAttachmentTransformRules::KeepWorldTransform);
		FVector SocketLocation = MeshComponent->GetSocketLocation(TEXT("GrapplePoint"));
		FVector HalfwayLocation = FMath::Lerp(SocketLocation, target->GetActorLocation(), 0.5f);
		physicsConstraint->SetRelativeLocation(HalfwayLocation);
		physicsConstraint->SetConstrainedComponents(hook, NAME_None, MeshComponent, NAME_None);
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
		cable->SetAttachEndTo(owner, TEXT("Mesh"), TEXT("GrapplePoint"));
		UE_LOG(LogTemp, Warning, TEXT("HERE"));

		ProjectileMovementComponent->Velocity = direction * GrappleShootSpeed;
	}
}

// Called every frame
void AGrapplingHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// cable->CableLength = FVector::Distance(endLocation, hook->GetComponentLocation());
}

