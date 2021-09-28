// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "SpiritHelper.h"
#include "Diana.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"
#include "Components/AudioComponent.h"


ASpiritHelper::ASpiritHelper() 
{
    WaitBeforeDestroy = 2.f;

    TimeBetweenActions = 2.f;
    TimeBeforeStartingMin = 3.f;
    TimeBeforeStartingMax = 4.f;

    TimeBeforeDying = 15.f;

    VerticalMovement = 200.f;

    bWillAttack = true;

    SphereEffect = CreateDefaultSubobject<UNiagaraComponent>(FName("SphereEffect"));
	SphereEffect->SetupAttachment(GetMesh());

    SphereEndEffect = CreateDefaultSubobject<UNiagaraComponent>(FName("SphereEndEffect"));
	SphereEndEffect->SetupAttachment(GetMesh());
    SphereEndEffect->bAutoActivate = false;

    AttackEffect = CreateDefaultSubobject<UNiagaraComponent>(FName("AttackEffect"));
	AttackEffect->SetupAttachment(GetMesh());
    AttackEffect->bAutoActivate = false;

    AttackSound = CreateDefaultSubobject<UAudioComponent>(FName("AttackSound"));
	AttackSound->SetupAttachment(GetMesh());
	AttackSound->bAutoActivate = false;
    
    EnemiesDetectionBox = CreateDefaultSubobject<UBoxComponent>(FName("EnemiesDetectionBox"));
    EnemiesDetectionBox->SetupAttachment(GetRootComponent());
	EnemiesDetectionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	EnemiesDetectionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

    Tags.Add(FName("SpiritHelper"));
}

void ASpiritHelper::BeginPlay() 
{
    Super::BeginPlay();

    DianaOwner = GetOwner<ADiana>();
    if(!DianaOwner) Destroy();

    NextLocation = GetActorLocation();

    StartChangeTrajectory();

    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void ASpiritHelper::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);

    if(bIsManuallyMovingToNextLocation)
	{
		MoveHelper(DeltaTime);
	}
}

void ASpiritHelper::KillThisSpirit() 
{
    GetWorldTimerManager().ClearTimer(TimerBeetwenActions);

    if(SphereEndEffect) SphereEndEffect->Activate(true);
    GetMesh()->SetVisibility(false, false);

    Super::KillThisSpirit();
}

void ASpiritHelper::StartChangeTrajectory() 
{
    if(!bIsManuallyMovingToNextLocation)
	{
		if(bWasGoingUp)
        {
            bWasGoingUp = false;
            NextLocation.Z -= VerticalMovement;
        }
        else
        {
            bWasGoingUp = true;
        	NextLocation.Z += VerticalMovement;
        }

		bIsManuallyMovingToNextLocation = true;
	}
}

void ASpiritHelper::StartActions() 
{
    GetWorldTimerManager().SetTimer(TimerBeforeDying, this, &ASpiritHelper::KillThisSpirit, TimeBeforeDying, false);
    GetWorldTimerManager().SetTimer(TimerBeforeAction, this, &ASpiritHelper::ThrowSpell, FMath::FRandRange(TimeBeforeStartingMin, TimeBeforeStartingMax), false);
}

void ASpiritHelper::ThrowSpell() 
{
    if(bWillAttack)
    {
        TArray<AActor*> OverlappingActors;
        EnemiesDetectionBox->GetOverlappingActors(OverlappingActors, AEnemy::StaticClass());

        for (auto& ActorHit : OverlappingActors)
        {
            AEnemy* EnemyIn = Cast<AEnemy>(ActorHit);
            if(EnemyIn)
            {
                if(!EnemyIn->IsDead() && EnemyIn->ActorHasTag(FName("Enemy")))
                {
                    FVector EnemyLoc = EnemyIn->GetActorLocation();
                    
                    EnemyIn->SpiritTakeDamage(BaseDamage, TEXT("Sagrado"), DianaOwner, EnemyLoc, false);
                    SetActorRotation((EnemyLoc - GetActorLocation()).Rotation());

                    if(AttackEffect) AttackEffect->Activate(true);
                    if(AttackSound) AttackSound->Activate(true);

                    break;
                }
            }
        }
    }
    else
    {
        InvokeRandomSpell(FName());
    }

    GetWorldTimerManager().SetTimer(TimerBeetwenActions, this, &ASpiritHelper::ThrowSpell, TimeBetweenActions, false);
}

void ASpiritHelper::MoveHelper(float DeltaTime) 
{
    if(DianaOwner)
    {
        FVector ActualLocation = GetActorLocation();
        if(!FVector::PointsAreNear(ActualLocation, NextLocation, 3.f))
        {
            FVector NextLerpLocation = FMath::VInterpTo(ActualLocation, NextLocation, DeltaTime, 1.8f);
            SetActorLocation(NextLerpLocation);
        }
        else
        {
            bIsManuallyMovingToNextLocation = false;
            StartChangeTrajectory();
        }
    } 
}
