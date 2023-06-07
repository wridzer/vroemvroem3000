// Fill out your copyright notice in the Description page of Project Settings.


#include "GrapplingHook.h"
#include "CableComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
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
}

// Called when the game starts or when spawned
void AGrapplingHook::BeginPlay()
{
	Super::BeginPlay();
	
	if (!owner) {
		UE_LOG(LogTemp, Warning, TEXT("no owner!"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("This a testing statement. %s"), *owner->GetName());
		startLocation = GetActorLocation();
		direction = endLocation - startLocation;
		direction.Normalize();
		cable->SetAttachEndTo(owner, TEXT("Mesh"), TEXT("GrapplePoint"));
		ProjectileMovementComponent->Velocity = direction * GrappleShootSpeed;
	}
}

// Called every frame
void AGrapplingHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// cable->CableLength = FVector::Distance(endLocation, hook->GetComponentLocation());
}

