// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "Amit.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "Diana.h"
#include "HealingAmitArea.h"
#include "SummonEnemy.h"


AAmit::AAmit() 
{
    WaitBeforeDestroy = 20.f;

    RadiusToNextRandomLocation = 10000.f;
    VerticalDistanceToKeep = 0.f;
    SpiritWalkSpeed = 2000.f;
    bSpellsCanMoveActor = false;
    Tags.Add(FName("Amit"));

    ActionSound = CreateDefaultSubobject<UAudioComponent>(FName("ActionSound"));
	ActionSound->SetupAttachment(GetMesh());
	ActionSound->bAutoActivate = false;

    JumpSound = CreateDefaultSubobject<UAudioComponent>(FName("JumpSound"));
	JumpSound->SetupAttachment(GetMesh());
	JumpSound->bAutoActivate = false;

    ExplosionSound = CreateDefaultSubobject<UAudioComponent>(FName("ExplosionSound"));
	ExplosionSound->SetupAttachment(GetMesh());
	ExplosionSound->bAutoActivate = false;

	DashEffect = CreateDefaultSubobject<UNiagaraComponent>(FName("DashEffect"));
	DashEffect->SetupAttachment(GetMesh());

    JumpExplosionEffect = CreateDefaultSubobject<UNiagaraComponent>(FName("JumpExplosionEffect"));
	JumpExplosionEffect->SetupAttachment(GetMesh());
	JumpExplosionEffect->bAutoActivate = false;

    ExplosionSphere = CreateDefaultSubobject<USphereComponent>(FName("ExplosionSphere"));
	ExplosionSphere->SetupAttachment(GetRootComponent());
    ExplosionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ExplosionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	ExplosionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);

    RangePhase = 1;
    bCanExplote = true;
}

void AAmit::BeginPlay() 
{
    Super::BeginPlay();
}

void AAmit::Landed(const FHitResult& Hit) 
{
    Super::Landed(Hit);

    if(bCanExplote)
    {
        Explote();
        bCanExplote = false;
    }
}

void AAmit::Explote() 
{
    if(JumpExplosionEffect) JumpExplosionEffect->Activate(true);
    if(ExplosionSound) ExplosionSound->Activate(true);

    PlayMontageByName(FName("Attack_4"), SpiritAnimationMontage, 1.f);

    TArray<AActor*> OverlappingActors;
    ExplosionSphere->GetOverlappingActors(OverlappingActors, ASpirit::StaticClass());
    
    for (auto& ActorHit : OverlappingActors)
    {
        ASpirit* EnemyIn = Cast<ASpirit>(ActorHit);
        if(EnemyIn)
        {
            if(EnemyIn != this && !EnemyIn->IsDead())
            {
                if(!EnemyIn->IsA(AEnemy::StaticClass()) || (EnemyIn->IsA(AEnemy::StaticClass()) && bShouldDamageAllies))
                {
                    EnemyIn->SpiritTakeDamage(DamageByTouch, DamageByTouchNature, this, EnemyIn->GetActorLocation(), false);
                }
            }
        }
    }

    if(bFightHasBegun)
    {
        if(NextJumpTime)
        {
            GetWorldTimerManager().SetTimer(TimerToNextAction, this, &AAmit::ExecuteNextAction, NextJumpTime, false);
        }
        else
        {
            GetWorldTimerManager().SetTimer(TimerToNextAction, this, &AAmit::ExecuteNextAction, 1.f, false);
        }
    }
    
}


void AAmit::InvokeArcanosProj(float TimeBeforeNextAction) 
{
    PlayMontageByName(FName("Attack_2"), SpiritAnimationMontage, 1.f);
    GetWorldTimerManager().SetTimer(TimerToNextAction, this, &AAmit::ExecuteNextAction, TimeBeforeNextAction, false);
}

void AAmit::InvokeSombrasProj(float TimeBeforeNextAction) 
{
    PlayMontageByName(FName("Attack_1"), SpiritAnimationMontage, 1.f);
    GetWorldTimerManager().SetTimer(TimerToNextAction, this, &AAmit::ExecuteNextAction, TimeBeforeNextAction, false);
}

void AAmit::InvokeAguaProj(float TimeBeforeNextAction) 
{
    PlayMontageByName(FName("Attack_7"), SpiritAnimationMontage, 1.f);
    GetWorldTimerManager().SetTimer(TimerToNextAction, this, &AAmit::ExecuteNextAction, TimeBeforeNextAction, false);
}

void AAmit::InvokeRayo(float TimeBeforeNextAction) 
{
    PlayMontageByName(FName("Attack_3"), SpiritAnimationMontage, 1.f);
    GetWorldTimerManager().SetTimer(TimerToNextAction, this, &AAmit::ExecuteNextAction, TimeBeforeNextAction, false);
}

void AAmit::MakeJump(float TimeBeforeNextAction)
{
    bCanExplote = true;
    if(JumpSound) JumpSound->Activate(true);
    Jump();

    GetWorldTimerManager().SetTimer(JumpTimer, this, &AAmit::TeleportToDiana, JumpDuration, false);
    NextJumpTime = TimeBeforeNextAction;
}

void AAmit::SummonAllies(float TimeBeforeNextAction) 
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    PlayMontageByName(FName("Attack_4"), SpiritAnimationMontage, 1.f);

    ASummonEnemy* Spawner = GetWorld()->SpawnActor<ASummonEnemy>(ClassToSummon, SpawnParams);
    Spawner->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
    
    GetWorldTimerManager().SetTimer(TimerToNextAction, this, &AAmit::ExecuteNextAction, TimeBeforeNextAction, false);
}

void AAmit::SummonOffensiveAllies(float TimeBeforeNextAction) 
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    PlayMontageByName(FName("Attack_4"), SpiritAnimationMontage, 1.f);

    ASummonEnemy* Spawner = GetWorld()->SpawnActor<ASummonEnemy>(Class2ToSummon, SpawnParams);
    Spawner->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
    
    GetWorldTimerManager().SetTimer(TimerToNextAction, this, &AAmit::ExecuteNextAction, TimeBeforeNextAction, false);
}

void AAmit::TeleportToDiana() 
{
    StopJumping();
    FVector DianaLoc = GetTargetToHitLocation();
    SetActorLocation(FVector(DianaLoc.X, DianaLoc.Y, GetActorLocation().Z));
}

void AAmit::InvokeHealArea(float TimeBeforeNextAction) 
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    
    AHealingAmitArea* HealArea = GetWorld()->SpawnActor<AHealingAmitArea>(HealAreaClass, GetActorLocation(), GetActorRotation(), SpawnParams);

    GetWorldTimerManager().SetTimer(TimerToNextAction, this, &AAmit::ExecuteNextAction, TimeBeforeNextAction, false);
}

// Forma medio fea de hacer su IA. No usé Behavior Trees porque quise experimentar. Otros enemigos sí los usan.
void AAmit::ExecuteNextAction() 
{
    if(RangePhase == 1)
    {
        SummonAllies(8.f);
    }
    else if(RangePhase == 2)
    {
        InvokeArcanosProj(2.5f);
    }
    else if(RangePhase == 3)
    {
        MakeJump(1.5f);
    }
    else if(RangePhase == 4)
    {
        InvokeRayo(3.f);
    }
    else if(RangePhase == 5)
    {
        InvokeHealArea(4.f);
    }
    else if(RangePhase == 6)
    {
        InvokeAguaProj(3.f);
    }
    else if(RangePhase == 7)
    {
        InvokeAguaProj(3.f);
    }
    else if(RangePhase == 8)
    {
        InvokeAguaProj(3.f);
    }
    else if(RangePhase == 9)
    {
        InvokeSombrasProj(3.f);
    }
    else if(RangePhase == 10)
    {
        InvokeSombrasProj(3.f);
    }
    else if(RangePhase == 11)
    {
        InvokeSombrasProj(3.f);
    }
    else if(RangePhase == 12)
    {
        InvokeArcanosProj(2.5f);
    }
    else if(RangePhase == 13)
    {
        InvokeArcanosProj(2.5f);
    }
    else if(RangePhase == 14)
    {
        InvokeArcanosProj(2.5f);
    }
    else if(RangePhase == 15)
    {
        InvokeArcanosProj(2.5f);
    }
    else if(RangePhase == 16)
    {
        InvokeArcanosProj(2.5f);
    }
    else if(RangePhase == 17)
    {   
        InvokeArcanosProj(2.5f); 
    }
    else if(RangePhase == 18)
    {
        InvokeArcanosProj(2.5f);
    }
    else if(RangePhase == 19)
    {
        InvokeArcanosProj(2.5f);
    }
    else if(RangePhase == 20)
    {
        InvokeRayo(3.f);
    }
    else if(RangePhase == 21)
    {
        InvokeSombrasProj(3.f);
    }
    else if(RangePhase == 22)
    {
        InvokeSombrasProj(3.f);
    }
    else if(RangePhase == 23)
    {
        InvokeHealArea(4.f);
    }
    else if(RangePhase == 24)
    {
        InvokeArcanosProj(2.5f);
    }
    else if(RangePhase == 25)
    {
        MakeJump(4.f);
    }
    else if(RangePhase == 26)
    {
        InvokeRayo(3.f);
    }
    else if(RangePhase == 27)
    {
        MakeJump(4.f);
    }
    else if(RangePhase == 28)
    {
        SummonOffensiveAllies(4.5f);
    }
    else if(RangePhase == 29)
    {
       InvokeAguaProj(3.5f);
    }
    else if(RangePhase == 30)
    {
        InvokeAguaProj(3.5f);
    }
    else if(RangePhase == 31)
    {
        InvokeSombrasProj(3.5f);
    }
    else if(RangePhase == 32)
    {
        InvokeArcanosProj(2.5f);
    }
    else if(RangePhase == 33)
    {
        InvokeAguaProj(3.5f);
    }
    else if(RangePhase == 34)
    {
        InvokeAguaProj(3.5f);
    }
    else if(RangePhase == 35)
    {
        InvokeAguaProj(3.5f);
    }
    else if(RangePhase == 36)
    {
        InvokeAguaProj(3.5f);
    }
    else if(RangePhase == 37)
    {
        MakeJump(4.f);
    }
    else if(RangePhase == 37)
    {
        MakeJump(4.f);
    }
    else if(RangePhase == 38)
    {
        InvokeArcanosProj(2.5f);
    }
    else if(RangePhase == 39)
    {
        InvokeArcanosProj(2.5f);
    }
    else if(RangePhase == 40)
    {
        InvokeAguaProj(3.5f);
    }
    else if(RangePhase == 41)
    {
        InvokeSombrasProj(3.f);
    }
    else if(RangePhase == 42)
    {
        InvokeSombrasProj(3.f);
    }
    else if(RangePhase == 43)
    {
        InvokeAguaProj(3.5f);
    }
    else
    {
        InvokeAguaProj(8.f);
        RangePhase = 0;
    }

    RangePhase++;
    ActionSound->Activate();
}

void AAmit::KillThisSpirit() 
{
    GetWorldTimerManager().ClearTimer(TimerToNextAction);
    GetWorldTimerManager().ClearTimer(JumpTimer);

    Super::KillThisSpirit();
}

void AAmit::OnHaveSeenAnEnemy() 
{
    Super::OnHaveSeenAnEnemy();
    if(!bExecutedOnce && bCanStartFight)
    {
        bExecutedOnce = true;
        bFightHasBegun = true;

        ExecuteNextAction();
    }
}
