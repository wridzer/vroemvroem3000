// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "VehiclePawn.generated.h"

/**
 * 
 */
UCLASS()
class VROEMVROEM3000_API AVehiclePawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()
	
public:
	AVehiclePawn();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Shoot grappiling hook
	void Shoot();

	// Throttle/steering
	void ApplyThrottle(float Val);
	void ApplySteering(float Val);

	// Look around
	void LookUp(float Val);
	void Turn(float Val);

	// Handbrake
	void OnHandbrakePressed();
	void OnHandbrakeReleased();

	// In air physics
	void UpdateInAirControl(float DeltaTime);

	UPROPERTY(EditAnywhere)
		float AirPitchForce;
	UPROPERTY(EditAnywhere)
		float AirRollForce;
	UPROPERTY(EditAnywhere)
		float FlippedRollForce;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> grapplingHook;

protected:

	// Camera and swingarm
	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAcces = "true"))
		class USpringArmComponent* springArm;
	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAcces = "true"))
		class UCameraComponent* Camera;
};
