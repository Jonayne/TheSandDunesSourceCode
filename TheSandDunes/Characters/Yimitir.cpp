// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "Yimitir.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SummonEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Diana.h"
#include "HealingArea.h"


AYimitir::AYimitir() 
{
    WaitBeforeDestroy = 10.f;
    RadiusToNextRandomLocation = 7000.f;
    VerticalDistanceToKeep = 500.f;
    SpiritWalkSpeed = 1700.f;
    TimeBeetwenExplosions = 1.f;
    NumberOfExplosions = 6;
    bSpellsCanMoveActor = false;
    Tags.Add(FName("Yimitir"));
    
    ActionSound = CreateDefaultSubobject<UAudioComponent>(FName("ActionSound"));
	ActionSound->SetupAttachment(GetMesh());
	ActionSound->bAutoActivate = false;

	DashEffect = CreateDefaultSubobject<UNiagaraComponent>(FName("DashEffect"));
	DashEffect->SetupAttachment(GetMesh());

    RangePhase = 1;
    SetOfAttacks = 1;
}

void AYimitir::BeginPlay() 
{
    Super::BeginPlay();
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

void AYimitir::SummonExplosionsTime(float TimeBeforeNextAction) 
{

    PlayMontageByName(FName("Summon"), SpiritAnimationMontage, 1.f);
    SummonExplosions();
    GetWorldTimerManager().SetTimer(TimerToNextAction, this, &AYimitir::ChooseNextAction, TimeBeforeNextAction, false);
}

void AYimitir::SummonExplosions() 
{
    if(NumerOfExplosionsAux <= NumberOfExplosions)
    {
        NumerOfExplosionsAux++;

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        
        ADiana* DianaRef = Cast<ADiana>(UGameplayStatics::GetPlayerPawn(this, 0));
        FVector DianaLoc = DianaRef->GetActorLocation();

        FVector NewPoint = UKismetMathLibrary::RandomUnitVector() * (FMath::RandRange(100.f, 500.f));
        NewPoint.Z = 0.f;

        FVector SummonExplosionLoc = DianaLoc + NewPoint;
        AActor* Explosion = GetWorld()->SpawnActor<AActor>(ExplosionClass, SummonExplosionLoc, FRotator(0.f), SpawnParams);

        GetWorldTimerManager().SetTimer(ExplosionsHandler, this, &AYimitir::SummonExplosions, TimeBeetwenExplosions, false);
    }
    else
    {
        GetWorldTimerManager().ClearTimer(ExplosionsHandler);
    }   
}

void AYimitir::MoveYimitir() 
{
    if(GetTargetToHit())
    {
        MoveEnemyToRandomLocation(GetTargetToHit());
    }
    else
    {
        MoveEnemyToRandomLocation(this);
    }
}

void AYimitir::MoveAgain() 
{
    GetWorldTimerManager().SetTimer(TryToMoveTimer, this, &AYimitir::MoveYimitir, 1.f, true);
}

void AYimitir::SummonAllies(float TimeBeforeNextAction) 
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    PlayMontageByName(FName("Summon"), SpiritAnimationMontage, 1.f);

    ASummonEnemy* Spawner = GetWorld()->SpawnActor<ASummonEnemy>(ClassToSummon, SpawnParams);
    Spawner->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
    
    GetWorldTimerManager().SetTimer(TimerToNextAction, this, &AYimitir::ChooseNextAction, TimeBeforeNextAction, false);
}

void AYimitir::InvokeAutiAIMProjectile(float TimeBeforeNextAction) 
{
    PlayMontageByName(FName("Attack"), SpiritAnimationMontage, 1.f);
    InvokeSpell(FString("AutoAIM"), FName("WEAPON_R"));
    GetWorldTimerManager().SetTimer(TimerToNextAction, this, &AYimitir::ChooseNextAction, TimeBeforeNextAction, false);
}

void AYimitir::InvokeFastProjectile(float TimeBeforeNextAction) 
{
    PlayMontageByName(FName("Attack"), SpiritAnimationMontage, 1.f);
    InvokeSpell(FString("FastProjectile"), FName("WEAPON_R"));
    GetWorldTimerManager().SetTimer(TimerToNextAction, this, &AYimitir::ChooseNextAction, TimeBeforeNextAction, false);
}

void AYimitir::InvokeHealArea(float TimeBeforeNextAction) 
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    
    AHealingArea* HealArea = GetWorld()->SpawnActor<AHealingArea>(HealAreaClass, GetActorLocation(), GetActorRotation(), SpawnParams);

    bIsManuallyMovingToNextLocation = false;
    GetWorldTimerManager().ClearTimer(TryToMoveTimer);
    GetWorldTimerManager().SetTimer(WaitToMoveAgainHandler, this, &AYimitir::MoveAgain, HealArea->HealingAreaDuration, false);
    GetWorldTimerManager().SetTimer(TimerToNextAction, this, &AYimitir::ChooseNextAction, TimeBeforeNextAction, false);
}

void AYimitir::ChooseNextAction() 
{
    if(RangePhase == 1)
    {
        SetOfAttacks = SetOfAttacks == 3 ? 1 : SetOfAttacks+1;            
    }
    
    ExecuteNextAction();
}

void AYimitir::ExecuteNextAction() 
{
    if(SetOfAttacks == 1)
    {
        if(RangePhase == 1)
        {
            InvokeFastProjectile(1.f);
            RangePhase++;
        }
        else if(RangePhase == 2)
        {
            NumerOfExplosionsAux = 0;
            SummonExplosionsTime(0.1f);
            RangePhase++;
        }
        else if(RangePhase == 3)
        {
            InvokeAutiAIMProjectile(1.f);
            RangePhase++;
        }
        else if(RangePhase == 4)
        {
            InvokeFastProjectile(6.f);
            RangePhase++;
        }
        else
        {
            InvokeAutiAIMProjectile(10.f);
            RangePhase = 1;
        }
    }
    else if(SetOfAttacks == 2)
    {
        if(RangePhase == 1)
        {
            SummonAllies(5.f);
            RangePhase++;
        }
        else if(RangePhase == 2)
        {
            NumerOfExplosionsAux = 0;
            SummonExplosionsTime(4.f);
            RangePhase++;
        }
        else if(RangePhase == 3)
        {
            InvokeFastProjectile(10.f);
            RangePhase++;
        }
        else if(RangePhase == 4)
        {
            InvokeAutiAIMProjectile(10.f);
            RangePhase++;
        }
        else if(RangePhase == 5)
        {
            InvokeHealArea(3.f);
            RangePhase++;
        }
        else if(RangePhase == 6)
        {
            InvokeFastProjectile(3.f);
            RangePhase++;
        }
        else
        {
            InvokeAutiAIMProjectile(10.f);
            RangePhase = 1;
        }
    }
    else if(SetOfAttacks == 3)
    {
        if(RangePhase == 1)
        {
            NumerOfExplosionsAux = 0;
            SummonExplosionsTime(4.f);
            RangePhase++;
        }
        else if(RangePhase == 2)
        {
            InvokeAutiAIMProjectile(2.f);
            RangePhase++;
        }
        else if(RangePhase == 3)
        {
            InvokeFastProjectile(10.f);
            RangePhase++;
        }
        else if(RangePhase == 4)
        {
            InvokeHealArea(3.f);
            RangePhase++;
        }
        else if(RangePhase == 5)
        {
            InvokeAutiAIMProjectile(3.f);
            RangePhase++;
        }
        else
        {
            InvokeFastProjectile(10.f);
            RangePhase = 1;
        }
    }

    ActionSound->Activate(true);
}

void AYimitir::KillThisSpirit() 
{
    GetWorldTimerManager().ClearTimer(TimerToNextAction);
    GetWorldTimerManager().ClearTimer(TryToMoveTimer);
    GetWorldTimerManager().ClearTimer(ExplosionsHandler);
    GetWorldTimerManager().ClearTimer(WaitToMoveAgainHandler);

    Super::KillThisSpirit();
}

void AYimitir::OnHaveSeenAnEnemy() 
{
    Super::OnHaveSeenAnEnemy();
    if(!bExecutedOnce && bCanStartFight)
    {
        bExecutedOnce = true;
        ExecuteNextAction();
        GetWorldTimerManager().SetTimer(TryToMoveTimer, this, &AYimitir::MoveYimitir, 1.f, true);
    }
}
