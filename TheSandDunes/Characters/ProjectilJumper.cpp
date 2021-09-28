// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "ProjectilJumper.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraComponent.h"


AProjectilJumper::AProjectilJumper() 
{
    WaitBeforeDestroy = 5.f;
    JumpDuration = 4.f;

    Tags.Add(FName("ProjectilJumper"));
}

void AProjectilJumper::JumperAction() 
{
    Super::JumperAction();

    LaunchCharacter(CalculateJumpVelocity(), true, true);
    InvokeRandomSpell(FName("MouthSocket"));
}

FVector AProjectilJumper::CalculateJumpVelocity() 
{
    FVector StartLocation = GetActorLocation();
    if(GetTargetToHit())
    {
        SetEnemyRandomLocation(GetTargetToHit());
    }
    else
    {
        SetEnemyRandomLocation(this);
    }
    FVector DestLocation = NextLocation;

    FVector VelocityVector = FVector();

    VelocityVector.X = (DestLocation.X - StartLocation.X)/JumpDuration;
    VelocityVector.Y = (DestLocation.Y - StartLocation.Y)/JumpDuration;
    VelocityVector.Z = (DestLocation.Z - (StartLocation.Z + (FMath::Square(JumpDuration) * -0.5f * 982.f)))/JumpDuration;

    return VelocityVector;
}
