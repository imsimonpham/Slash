// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Slash/DebugMacros.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Characters/Charactertypes.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	//Initialize variables
	HitReactMontage = nullptr;
	HitSound = nullptr;
	HitParticles = nullptr;

	//Configure mesh collision
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	//Create attribute component subobject 
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	//Create subobject for widget component
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	//Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}

	/*EnemyController = Cast<AAIController>(GetController());
	if (EnemyController && PatrolTarget)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(PatrolTarget);
		MoveRequest.SetAcceptanceRadius(15.f);
		FNavPathSharedPtr NavPath;
		EnemyController->MoveTo(MoveRequest, &NavPath);
		TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();
		for (auto& Point : PathPoints)
		{
			const FVector& Location = Point.Location;
			DrawDebugSphere(GetWorld(), Location, 12.f, 12, FColor::Green, false, 10.f);
		}
	}*/
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Calculate between enemy and combat target
	if (CombatTarget)
	{
		if (!InTargetRange(CombatTarget, CombarRadius))
		{
			CombatTarget = nullptr;
			if (HealthBarWidget)
			{
				HealthBarWidget->SetVisibility(false);
			}
		}
	}

	/*if (PatrolTarget && EnemyController)
	{
		if (InTargetRange(PatrolTarget, PatrolRadius))
		{
			TArray<AActor*> ValidTargets;
			for (auto Target : PatrolTargets)
			{
				if (Target != PatrolTarget)
				{
					ValidTargets.AddUnique(Target);
				}
			}

			const int32 PatrolTargetNum = ValidTargets.Num();
			const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);

			if (PatrolTargetNum > 0)
			{
				AActor* Target = ValidTargets[TargetSelection];
				PatrolTarget = Target;

				FAIMoveRequest MoveRequest;
				MoveRequest.SetGoalActor(PatrolTarget);
				MoveRequest.SetAcceptanceRadius(PatrolRadius);
				EnemyController->MoveTo(MoveRequest);
			}
		}
	}*/
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

void AEnemy::Die()
{
	//play a random death montage
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
		const int32 Selection = FMath::RandRange(0, 5);
		FName SelectionName = FName();
		switch (Selection)
		{
		case 0:
			SelectionName = FName("Death1");
			DeathPose = EDeathPose::EDP_Death1;
			break;
		case 1:
			SelectionName = FName("Death2");
			DeathPose = EDeathPose::EDP_Death2;
			break;
		case 2:
			SelectionName = FName("Death3");
			DeathPose = EDeathPose::EDP_Death3;
			break;
		case 3:
			SelectionName = FName("Death4");
			DeathPose = EDeathPose::EDP_Death4;
			break;
		case 4:
			SelectionName = FName("Death5");
			DeathPose = EDeathPose::EDP_Death5;
			break;
		case 5:
			SelectionName = FName("Death6");
			DeathPose = EDeathPose::EDP_Death6;
			break;
		}
		AnimInstance->Montage_JumpToSection(SelectionName, DeathMontage);
	}

	//hide healthbar, disable collision, set lifespan
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(2.f);
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

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
	if (Attributes && Attributes->IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else
	{
		Die();
	}

	/*DRAW_SPHERE(ImpactPoint,FColor::Purple);*/
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);
	}
}

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
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
	/*UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 10.f, FColor::Blue, 5.f);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("The hit angle is: %f and Section is %s"), Alpha, *Section.ToString()));
	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 10.f, FColor::Red, 5.f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 10.f, FColor::Green, 5.f);*/
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}

	CombatTarget = EventInstigator->GetPawn();
	return DamageAmount;
}

