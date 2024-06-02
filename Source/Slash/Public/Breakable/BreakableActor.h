// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

#define ECC_IK ECC_GameTraceChannel1

class UGeometryCollectionComponent;
class UCapsuleComponent;
class ATreasure;

UCLASS()
class SLASH_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	float spawnZ;

private:
	UPROPERTY(EditAnywhere, Category = "Breakable Properties")
	TArray< TSubclassOf<ATreasure>> TreasureClasses;

	bool bBroken = false;
};
