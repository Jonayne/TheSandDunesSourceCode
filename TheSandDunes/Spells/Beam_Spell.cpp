// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "Beam_Spell.h"
#include "Spirit.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "Shield.h"
#include "Diana.h"
#include "Perception/AISense_Hearing.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"


ABeam_Spell::ABeam_Spell() 
{
	SpellDuration = 5.f;
    BeamRange = 1000.f;

    NiagaraHitParticle->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
    HitAudio->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
    bShouldHitWorld = false;
}

void ABeam_Spell::BeginPlay() 
{
	Super::BeginPlay();

    Sphere->OnComponentBeginOverlap.Clear();
	Sphere->OnComponentEndOverlap.Clear();

    if (OwnerPawn)
	{
		Diana = Cast<ADiana>(OwnerPawn);
	}

    if(NiagaraParticle) ChangeBeamEnd();

    GetWorldTimerManager().SetTimer(SpawnedTimerHandle, this, &ABeam_Spell::TurnOffEffects, SpellDuration, false);
}

void ABeam_Spell::Tick(float DeltaTime) 
{
    if(NiagaraParticle) 
    {
        ChangeBeamEnd();
    }
    ShotBeamTick();
}

void ABeam_Spell::ChangeBeamEnd() 
{
    if(Diana)
    {
        Diana->UpdateDianaTargetToHit(BeamRange);
        TargetLocationToHit = Diana->GetTargetToHitLocation();
        TargetLocationToHit.Z = TargetLocationToHit.Z + 500.f;
    }
    else
    {
        if(Target)
        {
            FVector HitLocation = Target->GetActorLocation();
            FRotator Direction = UKismetMathLibrary::FindLookAtRotation(OwnerPawn->GetActorLocation(), HitLocation);
            TargetLocationToHit = OwnerPawn->GetActorLocation() + (UKismetMathLibrary::GetForwardVector(Direction) * BeamRange);
        }
    }

    NiagaraParticle->SetNiagaraVariableVec3(TEXT("End"), TargetLocationToHit);
    NiagaraHitParticle->SetNiagaraVariableVec3(TEXT("End"), TargetLocationToHit);
}

void ABeam_Spell::ShotBeamTick() 
{
	FHitResult Hit;
	
	FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(OwnerPawn);
    
	GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), TargetLocationToHit, ECollisionChannel::ECC_WorldDynamic, QueryParams);

    AActor* HitActor = Hit.GetActor();

	if (Hit.bBlockingHit && HitActor && HitActor->IsA(ASpirit::StaticClass()))
	{
        ASpirit* HitSpirit = Cast<ASpirit>(HitActor);

        NiagaraParticle->SetNiagaraVariableVec3(TEXT("End"), Hit.ImpactPoint);
        NiagaraHitParticle->SetNiagaraVariableVec3(TEXT("End"), Hit.ImpactPoint);
        PlayBeamParticles();

        if(!HitSpirit->IsDead())
        {
            HitSpirit->SpiritTakeDamage(Damage, FinalTypeOfDamage, OwnerPawn, Hit.ImpactPoint, true);
        }
	}
    else if (Hit.bBlockingHit && HitActor && !HitActor->IsA(AShield::StaticClass()) && HitActor != OwnerPawn)
	{
		NiagaraParticle->SetNiagaraVariableVec3(TEXT("End"), Hit.ImpactPoint);
        NiagaraHitParticle->SetNiagaraVariableVec3(TEXT("End"), Hit.ImpactPoint);
        PlayBeamParticles();
	}
    else
    {
        bPlayingHitParticles = false;
        NiagaraHitParticle->Deactivate();
        if(HitAudio) HitAudio->Deactivate();
    }
    
}

void ABeam_Spell::PlayBeamParticles() 
{
    if(!bPlayingHitParticles)
    {
        bPlayingHitParticles = true;
        NiagaraHitParticle->Activate(true);
        if(HitAudio) HitAudio->Activate(true);
        if(OwnerPawn)
        {
            UAISense_Hearing::ReportNoiseEvent(this, OwnerPawn->GetActorLocation(), 1.f, OwnerPawn, 5000.f, FName("HearEvent"));
        }
    }
}

void ABeam_Spell::TurnOffEffects() 
{
    if(Particle) Particle->Deactivate();
	if(NiagaraParticle) NiagaraParticle->Deactivate();
    if(NiagaraHitParticle) NiagaraHitParticle->Deactivate();
	
	SetActorTickEnabled(false);

    GetWorldTimerManager().SetTimer(TimerHandle, this, &ASpell::DestroySpell, WaitBeforeDestroy, false);
}
