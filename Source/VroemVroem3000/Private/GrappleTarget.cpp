// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleTarget.h"
#include "Components/BillboardComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AGrappleTarget::AGrappleTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Billboard
	billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	billboard->SetupAttachment(RootComponent);

	// Create Widget
	widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	widget->SetupAttachment(billboard);
	widget->SetGenerateOverlapEvents(false);
	widget->CanCharacterStepUp(false);
	widget->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Create Sphere collider
	sphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	sphereCollider->SetupAttachment(billboard);
	sphereCollider->SetVisibility(false);
	sphereCollider->CanCharacterStepUp(false);
	sphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	sphereCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	sphereCollider->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

}

void AGrappleTarget::SetActive(bool activate)
{
	widget->SetVisibility(activate);
}

// Called when the game starts or when spawned
void AGrappleTarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
//void AGrappleTarget::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

