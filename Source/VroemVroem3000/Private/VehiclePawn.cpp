// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "ChaosVehicleMovementComponent.h"


AVehiclePawn::AVehiclePawn()
{
	//UChaosVehicleMovementComponent* Vehicle = CastChecked<UChaosVehicleMovementComponent>(GetVehicleMovement());

	//Vehicle->RPM;

	// Create Springarm
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(RootComponent);
	springArm->TargetArmLength = 250.0f;
	springArm->bUsePawnControlRotation = true;

	// Create Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
	Camera->SetupAttachment(springArm, USpringArmComponent::SocketName);
	Camera->FieldOfView = 90.0f;
}

void AVehiclePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateInAirControl(DeltaTime);
}

void AVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AVehiclePawn::ApplyThrottle);
	PlayerInputComponent->BindAxis("MoveRight", this, &AVehiclePawn::ApplySteering);
	PlayerInputComponent->BindAxis("LookUp", this, &AVehiclePawn::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AVehiclePawn::Turn);

	PlayerInputComponent->BindAction("HandBrake", IE_Pressed, this, &AVehiclePawn::OnHandbrakePressed);
	PlayerInputComponent->BindAction("HandBrake", IE_Released, this, &AVehiclePawn::OnHandbrakeReleased);
}

void AVehiclePawn::ApplyThrottle(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);
}

void AVehiclePawn::ApplySteering(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void AVehiclePawn::LookUp(float Val)
{
	if (Val != 0.f) {
		AddControllerPitchInput(-Val);
	}
}

void AVehiclePawn::Turn(float Val)
{
	if (Val != 0.f) {
		AddControllerYawInput(Val);
	}
}

void AVehiclePawn::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void AVehiclePawn::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void AVehiclePawn::UpdateInAirControl(float DeltaTime)
{
	if (UChaosVehicleMovementComponent* Vehicle = CastChecked<UChaosVehicleMovementComponent>(GetVehicleMovement())) {
		FCollisionQueryParams QuaryParams;
		QuaryParams.AddIgnoredActor(this);

		const FVector TraceStart = GetActorLocation() + FVector(0, 0, 50);
		const FVector TraceEnd = GetActorLocation() - FVector(0, 0, 200);

		FHitResult Hit;

		const bool bInAir = !GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QuaryParams);
		const bool bNotGrounded = FVector::DotProduct(GetActorUpVector(), FVector::UpVector) < 0.1;

		if (bInAir || bNotGrounded) {
			const float ForwardInput = InputComponent->GetAxisValue("MoveForward");
			const float RightInput = InputComponent->GetAxisValue("MoveRight");

			const float AirMovementForcePitch = AirPitchForce;
			const float AirMovementForceRoll = !bInAir && bNotGrounded ? FlippedRollForce : AirRollForce;

			if (UPrimitiveComponent* VehicleMesh = Vehicle->UpdatedPrimitive) {
				const FVector MovementVector = FVector(RightInput * -AirMovementForceRoll, ForwardInput * AirMovementForcePitch, 0) * DeltaTime * 200;
				const FVector NewAngularMovement = GetActorRotation().RotateVector(MovementVector);

				VehicleMesh->SetPhysicsAngularVelocityInDegrees(NewAngularMovement, true);
			}
		}
	}
}
