// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrapplingHook.generated.h"

UCLASS()
class VROEMVROEM3000_API AGrapplingHook : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrapplingHook();

	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* hook;
	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* cableEnd;
	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(EditAnywhere)
		class UCableComponent* cable;
	UPROPERTY(EditAnywhere)
		class UPhysicsConstraintComponent* physicsConstraint;
	UPROPERTY(EditAnywhere)
		float GrappleShootSpeed = 5000;

	void Init(AActor* _target, AActor* _owner);
	void StopVelocity();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AActor* target;
	AActor* owner;
	FVector startLocation;
	FVector endLocation;
	FVector direction;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
