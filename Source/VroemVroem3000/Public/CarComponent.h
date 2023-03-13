// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Wheel.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CarComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VROEMVROEM3000_API UCarComponent : public UActorComponent
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		TArray<AWheel*> wheels;
		UPROPERTY(EditAnywhere)
		float suspension;
		UPROPERTY(EditAnywhere)
		float weight;
		UPROPERTY(EditAnywhere)
		float torque;
		UPROPERTY(EditAnywhere)
		float handling;
		UPROPERTY(EditAnywhere)
		float acceleration;
		UPROPERTY(EditAnywhere)
		int wheelDrive; // 1 = FWD, 2 = RWD, 3 = AWD

public:	
	// Sets default values for this component's properties
	UCarComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(BlueprintCallable, Category = "Game|Components|Car")
	void Throttle(float _input, APawn* _car);
	UFUNCTION(BlueprintCallable, Category = "Game|Components|Car")
	void Brake();
	UFUNCTION(BlueprintCallable, Category = "Game|Components|Car")
	void Steer();
};
