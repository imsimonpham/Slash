// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Item.generated.h"

UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	float TransformedSin(float Value);
	
	UFUNCTION()
	float TransformedCos(float Value);
	
private:
		UPROPERTY(VisibleAnywhere)
		float RunningTime;

		UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* ItemMesh;

		UPROPERTY(EditAnywhere, Category="Sine Params")
		float Amplitude = 0.25f;

		UPROPERTY(EditAnywhere, Category = "Sine Params")
		float TimeConstant = 1.f;
};
