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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly)
		class UCableComponent* cable;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
