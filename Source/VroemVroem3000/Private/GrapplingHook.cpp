// Fill out your copyright notice in the Description page of Project Settings.


#include "GrapplingHook.h"
#include "CableComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AGrapplingHook::AGrapplingHook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create projectile
	hook = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hook"));
	hook->SetupAttachment(RootComponent);
	
	// Add projectile movement component
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;

	// Create Cable
	cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	cable->SetupAttachment(hook);

}

// Called when the game starts or when spawned
void AGrapplingHook::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrapplingHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

