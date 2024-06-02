// Fill out your copyright notice in the Description page of Project Settings.

#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"
#include "Components/CapsuleComponent.h"
#include "Math/UnrealMathUtility.h"

ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	//Confiure GeometryCollection
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);

	//Configure Collision
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_IK, ECollisionResponse::ECR_Ignore);

	//Configure Capsule and its collision
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (bBroken) return;

	UWorld* World = GetWorld();
	if (World && TreasureClasses.Num() > 0)
	{
		bBroken = true;
		FVector Location = GetActorLocation();
		Location.Z += spawnZ;

		const int32 Selection = FMath::RandRange(0, TreasureClasses.Num() - 1);

		World->SpawnActor<ATreasure>(TreasureClasses[Selection], Location, GetActorRotation());
	}
}
