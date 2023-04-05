// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrappleComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VROEMVROEM3000_API UGrappleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrappleComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	GrappleStates grappleState;

	void Retracted();
	void Firing();
	void NearingTarget();
	void OnTarget();

	AActor* owner;
	class UCameraComponent* camera;
	class AGrappleTarget* currentTarget;

	UPROPERTY(EditDefaultsOnly)
		float maxGrappleDist = 500;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

UENUM(BlueprintType)
enum class GrappleStates : uint8 {
	RETRACTED = 0,
	FIRING = 1,
	NEARING_TARGET = 2,
	ON_TARGET = 3
};
