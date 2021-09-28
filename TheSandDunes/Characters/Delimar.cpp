// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "Delimar.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SummonEnemy.h"


ADelimar::ADelimar() 
{
    WaitBeforeDestroy = 10.f;
    DashDuration = 3.f;
    DashDistance = 7000.f;
    RadiusToNextRandomLocation = 8000.f;
    SpiritWalkSpeed = 2000.f;
    bSpellsCanMoveActor = false;
    Tags.Add(FName("Delimar"));

    DashSound = CreateDefaultSubobject<UAudioComponent>(FName("DashSound"));
	DashSound->SetupAttachment(GetMesh());
	DashSound->bAutoActivate = false;
    
    ActionSound = CreateDefaultSubobject<UAudioComponent>(FName("ActionSound"));
	ActionSound->SetupAttachment(GetMesh());
	ActionSound->bAutoActivate = false;

	DashEffect = CreateDefaultSubobject<UNiagaraComponent>(FName("DashEffect"));
	DashEffect->SetupAttachment(GetMesh());

    DashAreaEffect = CreateDefaultSubobject<UNiagaraComponent>(FName("DashAreaEffect"));
	DashAreaEffect->SetupAttachment(GetMesh());
	DashAreaEffect->bAutoActivate = false;

    DashDirection = CreateDefaultSubobject<UArrowComponent>(FName("DashDirection"));
	DashDirection->SetupAttachment(GetMesh());

    RangePhase = 1;
    SetOfAttacks = 1;
}

void ADelimar::BeginPlay() 
{
    Super::BeginPlay();
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
    if(SetOfAttacks == 1)
    {
        GetMesh()->SetMaterial(0, ShortRangeMat);
    }
    else if(SetOfAttacks == 2)
    {
        GetMesh()->SetMaterial(0, MediumRangeMat);
    }
    else if(SetOfAttacks == 3)
    {
        GetMesh()->SetMaterial(0, LongRangeMat);
    }
}

void ADelimar::MakeDash(bool bTowardDiana, float TimeBeforeNextAction)
{
    if(DashSound) DashSound->Activate(true);

    GetCharacterMovement()->GroundFriction = 0.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 0.f;

    if(DashAreaEffect) DashAreaEffect->Activate(true);

    bIsManuallyMovingToNextLocation = false;

    LaunchCharacter(CalculateDashVelocity(bTowardDiana), false, false);
    GetWorldTimerManager().SetTimer(DashDurationTimer, this, &ADelimar::StopDash, DashDuration, false);
    GetWorldTimerManager().SetTimer(TimerToNextAction, this, &ADelimar::ChooseNextAction, TimeBeforeNextAction+DashDuration, false);
}

void ADelimar::StopDash() 
{
    if(DashAreaEffect) DashAreaEffect->Deactivate();

    GetCharacterMovement()->GroundFriction = 8.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2048.f;
}

void ADelimar::ThrowBigProjectiles(float TimeBeforeNextAction) 
{
    PlayMontageByName(FName("Attack"), SpiritAnimationMontage, 1.f);
    InvokeSpell(FString("BigProjectile"), FName("WEAPON_R"));
    GetWorldTimerManager().SetTimer(TimerToNextAction, this, &ADelimar::ChooseNextAction, TimeBeforeNextAction, false);
}

void ADelimar::Explode(float TimeBeforeNextAction) 
{
    PlayMontageByName(FName("Explode"), SpiritAnimationMontage, 1.f);
    InvokeSpell(FString("Explosion"), FName());
    GetWorldTimerManager().SetTimer(TimerToNextAction, this, &ADelimar::ChooseNextAction, TimeBeforeNextAction, false);
}

void ADelimar::MoveDelimar() 
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

void ADelimar::SummonSwords(float TimeBeforeNextAction) 
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;

    PlayMontageByName(FName("Summon"), SpiritAnimationMontage, 1.f);
    ASummonEnemy* Spawner = GetWorld()->SpawnActor<ASummonEnemy>(SummonSwordsClass, SpawnParams);
    Spawner->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
    GetWorldTimerManager().SetTimer(TimerToNextAction, this, &ADelimar::ChooseNextAction, TimeBeforeNextAction, false);
}

void ADelimar::ThrowHighSpeedProjectiles(float TimeBeforeNextAction) 
{
    PlayMontageByName(FName("Attack"), SpiritAnimationMontage, 1.f);
    InvokeSpell(FString("HighSpeedProjectil"), FName("WEAPON_R"));
    GetWorldTimerManager().SetTimer(TimerToNextAction, this, &ADelimar::ChooseNextAction, TimeBeforeNextAction, false);
}

FVector ADelimar::CalculateDashVelocity(bool bTowardDiana) 
{
    FVector StartLocation = GetActorLocation();
    FVector VelocityVector = FVector();

    if(bTowardDiana)
    {
        FVector DestLocation = GetTargetToHitLocation();
        DashDirection->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(StartLocation, DestLocation)); 
        VelocityVector = DashDirection->GetForwardVector() * (FVector::Dist(StartLocation, DestLocation));
    }
    else
    {
        if(GetTargetToHit())
        {
            SetEnemyRandomLocation(GetTargetToHit());
        }
        else
        {
            SetEnemyRandomLocation(this);
        }
        
        DashDirection->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(StartLocation, NextLocation));
        VelocityVector = DashDirection->GetForwardVector() * DashDistance;
    }

    return VelocityVector;
}

void ADelimar::ChooseNextAction() 
{
    if(RangePhase == 1)
    {
        SetOfAttacks = SetOfAttacks == 3 ? 1 : SetOfAttacks+1;            
        if(SetOfAttacks == 1)
        {
            GetMesh()->SetMaterial(0, ShortRangeMat);
        }
        else if(SetOfAttacks == 2)
        {
            GetMesh()->SetMaterial(0, MediumRangeMat);
        }
        else if(SetOfAttacks == 3)
        {
            GetMesh()->SetMaterial(0, LongRangeMat);
        }
    }
    
    ExecuteNextAction();
}

void ADelimar::ExecuteNextAction() 
{
    if(SetOfAttacks == 3)
    {
        if(RangePhase == 1)
        {
            ThrowBigProjectiles(5.f);
            RangePhase++;
        }
        else if(RangePhase == 2)
        {
            ThrowHighSpeedProjectiles(5.f);
            RangePhase++;
        }
        else if(RangePhase == 3)
        {
            DashDuration = 2.f;
            MakeDash(false, .1f);
            RangePhase++;
        }
        else if(RangePhase == 4)
        {
            ThrowHighSpeedProjectiles(5.f);
            RangePhase++;
        }
        else if(RangePhase == 5)
        {
            ThrowBigProjectiles(5.f);
            RangePhase++;
        }
        else if(RangePhase == 6)
        {
            DashDuration = 2.f;
            MakeDash(true, 0.1f);
            RangePhase++;
        }
        else
        {
            Explode(1.f);
            RangePhase = 1;
        }
    }
    else if(SetOfAttacks == 2)
    {
        if(RangePhase == 1)
        {
            ThrowHighSpeedProjectiles(3.f);
            RangePhase++;
        }
        else if(RangePhase == 2)
        {
            DashDuration = 2.f;
            MakeDash(true, 4.f);
            RangePhase++;
        }
        else if(RangePhase == 3)
        {
            DashDuration = 2.f;
            MakeDash(true, 2.f);
            RangePhase++;
        }
        else if(RangePhase == 4)
        {
            Explode(0.05f);
            RangePhase++;
        }
        else if(RangePhase == 5)
        {
            ThrowHighSpeedProjectiles(3.f);
            RangePhase++;
        }
        else if(RangePhase == 6)
        {
            DashDuration = 2.f;
            MakeDash(true, 2.f);
            RangePhase++;
        }
        else if(RangePhase == 7)
        {
            Explode(1.f);
            RangePhase++;
        }
        else
        {
            ThrowBigProjectiles(3.f);
            RangePhase = 1;
        }
    }
    else if(SetOfAttacks == 1)
    {
        if(RangePhase == 1)
        {
            SummonSwords(1.f);
            RangePhase++;
        }
        else if(RangePhase == 2)
        {
            SummonSwords(0.1f);
            RangePhase++;
        }
        else if(RangePhase == 3)
        {
            DashDuration = 3.f;
            MakeDash(false, 2.f);
            RangePhase++;
        }
        else if(RangePhase == 4)
        {
            DashDuration = 2.f;
            MakeDash(true, 0.1f);
            RangePhase++;
        }
        else if(RangePhase == 5)
        {
            DashDuration = 3.f;
            MakeDash(false, .1f);
            RangePhase++;
        }
        else if(RangePhase == 6)
        {
            SummonSwords(1.f);
            RangePhase++;
        }
        else
        {
            SummonSwords(5.f);
            RangePhase = 1;
        }
    }

    ActionSound->Activate(true);
}

void ADelimar::KillThisSpirit() 
{
    GetWorldTimerManager().ClearTimer(TimerToNextAction);
    GetWorldTimerManager().ClearTimer(TryToMoveTimer);

    Super::KillThisSpirit();
}

void ADelimar::OnHaveSeenAnEnemy() 
{
    Super::OnHaveSeenAnEnemy();
    if(!bExecutedOnce && bCanStartFight)
    {
        bExecutedOnce = true;
        ExecuteNextAction();
        GetWorldTimerManager().SetTimer(TryToMoveTimer, this, &ADelimar::MoveDelimar, 1.f, true);
    }
}