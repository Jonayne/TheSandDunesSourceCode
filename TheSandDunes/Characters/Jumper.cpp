// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "Jumper.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraComponent.h"


AJumper::AJumper() 
{

    WaitBeforeDestroy = 5.f;
    TimeBetweenTryingJumps = 4.f;
    TimeBeforeAction = 1.f;
    JumpDuration = 3.f;
    
    Tags.Add(FName("Jumper"));

    JumpSound = CreateDefaultSubobject<UAudioComponent>(FName("JumpSound"));
	JumpSound->SetupAttachment(GetMesh());
	JumpSound->bAutoActivate = false;
    
    ActionSound = CreateDefaultSubobject<UAudioComponent>(FName("ActionSound"));
	ActionSound->SetupAttachment(GetMesh());
	ActionSound->bAutoActivate = false;
	
	ActionEffect = CreateDefaultSubobject<UNiagaraComponent>(FName("ActionEffect"));
	ActionEffect->SetupAttachment(GetMesh());
	ActionEffect->bAutoActivate = false;
}

void AJumper::BeginPlay() 
{
    Super::BeginPlay();

    HaveLanded = true;
}

void AJumper::Landed(const FHitResult& Hit) 
{
    Super::Landed(Hit);

    HaveLanded = true;
}

void AJumper::MakeJump()
{
    if(!bStunned && HaveLanded) 
    {
        HaveLanded = false;
        if(JumpSound) JumpSound->Activate(true);
        Jump();

        GetWorldTimerManager().SetTimer(TimerToAction, this, &AJumper::JumperAction, TimeBeforeAction, false);
    }
}

void AJumper::JumperAction() 
{
    if(ActionSound) ActionSound->Activate(true);
}

void AJumper::KillThisSpirit() 
{
    GetWorldTimerManager().ClearTimer(TimerToAction);
    GetWorldTimerManager().ClearTimer(TimerToJump);

    Super::KillThisSpirit();
}

void AJumper::OnHaveSeenAnEnemy() 
{
    Super::OnHaveSeenAnEnemy();
    if(!bExecutedOnce && bCanStartFight)
    {
        bExecutedOnce = true;
        MakeJump();
        GetWorldTimerManager().SetTimer(TimerToJump, this, &AJumper::MakeJump, TimeBetweenTryingJumps, true);
    }
}

FVector AJumper::CalculateJumpVelocity() 
{
    FVector StartLocation = GetActorLocation();
    FVector DestLocation = FVector();
    
    SetRandomPointOnArea(GetTargetToHitLocation());
    DestLocation = NextLocation;

    FVector VelocityVector = FVector();

    VelocityVector.X = (DestLocation.X - StartLocation.X)/JumpDuration;
    VelocityVector.Y = (DestLocation.Y - StartLocation.Y)/JumpDuration;
    VelocityVector.Z = (DestLocation.Z - (StartLocation.Z + (FMath::Square(JumpDuration) * -0.5f * 982.f)))/JumpDuration;

    return VelocityVector;
}
