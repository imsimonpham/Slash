// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Slash/DebugMacros.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	//Initialize variables
	HitReactMontage = nullptr;

	//Configure mesh collision
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	DRAW_SPHERE(ImpactPoint,FColor::Purple);

	const FVector Forward = GetActorForwardVector();
	// lower Impact Point to the enemy's actor location z so the two vectors are parallel
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward||ToHit| * cos(alpha)
	// |Forward| = 1, |ToHit| = 1 => Forward * ToHit = cost(alpha) = FVector::DotProduct(Forward, ToHit)
	const double CosAlpha = FVector::DotProduct(Forward, ToHit);
	// return Alpha angle in radians
	double Alpha = FMath::Acos(CosAlpha);
	// convert radians to degrees 
	Alpha = FMath::RadiansToDegrees(Alpha);

	// if CrossProduct points down, Alpha is negative (opposite to regular math rules)
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Alpha *= -1.f;	
	}

	FName Section("FromBack");

	if (Alpha >= -45.f && Alpha < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Alpha >= -135.f && Alpha < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Alpha >= 45.f && Alpha < 135.f)
	{
		Section = FName("FromRight");
	}

	PlayHitReactMontage(FName(Section));

	//Debug
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 10.f, FColor::Blue, 5.f);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("The hit angle is: %f and Section is %s"), Alpha, *Section.ToString()));
	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 10.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 10.f, FColor::Green, 5.f);
}

