// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrappleTarget.generated.h"

UCLASS()
class VROEMVROEM3000_API AGrappleTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrappleTarget();

	UPROPERTY(EditAnywhere)
		class UBillboardComponent* billboard;
	UPROPERTY(EditAnywhere)
		class UWidgetComponent* widget;
	UPROPERTY(EditAnywhere)
		class USphereComponent* sphereCollider;

	void SetActive(bool _activate);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

};