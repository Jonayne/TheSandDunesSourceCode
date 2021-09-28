// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "ProjectilDasher.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"


AProjectilDasher::AProjectilDasher() 
{
    WaitBeforeDestroy = 5.f;
    TimeBeforeAction = 1.f;

    Tags.Add(FName("ProjectilDasher"));
    DashDistance = 3000.f;
}

void AProjectilDasher::DasherAction() 
{
    if(ActionSound) ActionSound->Activate(true);
    InvokeRandomSpell(FName());
}

FVector AProjectilDasher::CalculateDashVelocity() 
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

    DashDirection->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(StartLocation, DestLocation));

    // obvio cambiar esto, usar el vector referente a rotationtoface or something like that.
    FVector VelocityVector = DashDirection->GetForwardVector() * DashDistance;

    return VelocityVector;
}
