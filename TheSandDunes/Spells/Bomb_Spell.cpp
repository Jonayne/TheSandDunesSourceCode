// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "Bomb_Spell.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "Diana.h"
#include "Shield.h"
#include "Perception/AISense_Hearing.h"

ABomb_Spell::ABomb_Spell() 
{
	TimeToWaitForExplosion = 10.f;
    ExplosionTime = 3.f;

    ExplosionSphere = CreateDefaultSubobject<USphereComponent>(FName("ExplosionSpellSphere"));
	ExplosionSphere->SetupAttachment(GetRootComponent());
	ExplosionSphere->SetCollisionProfileName(FName("Spells"));

    ExplosionParticle = CreateDefaultSubobject<UParticleSystemComponent>(FName("ExplosionSpellParticle"));
	ExplosionParticle->SetupAttachment(GetRootComponent());
    ExplosionParticle->bAutoActivate = false;

	ExplosionAudio = CreateDefaultSubobject<UAudioComponent>(FName("ExplosionSpellAudio"));
	ExplosionAudio->SetupAttachment(GetRootComponent());
    ExplosionAudio->bAutoActivate = false;

	ExplosionNiagaraParticle = CreateDefaultSubobject<UNiagaraComponent>(FName("ExplosionSpellNiagaraParticle"));
	ExplosionNiagaraParticle->SetupAttachment(GetRootComponent());
    ExplosionNiagaraParticle->bAutoActivate = false;

	bAlreadyExploded = false;
}

void ABomb_Spell::BeginPlay() 
{
	SpellLifeSpan = TimeToWaitForExplosion + 3.f;
	
	Super::BeginPlay();

    GetWorldTimerManager().SetTimer(SpawnExplosionTimerHandle, this, &ABomb_Spell::PlayHitEffects, TimeToWaitForExplosion, false);
}

void ABomb_Spell::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
    // SI CHOCAMOS CON UN ESPIRITU.
	if (OtherActor && OtherActor->IsA(ASpirit::StaticClass()))
	{
		if(OtherActor != OwnerPawn)
		{	
			if(OwnerEnemy && OtherActor->IsA(AEnemy::StaticClass()) && !OwnerEnemy->bShouldDamageAllies)
			{
				return;
			}

			
			PlayHitEffects();
		}	
	}
}

void ABomb_Spell::PlayHitEffects() 
{   
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Sphere->Deactivate();
	SetActorTickEnabled(false);
    GetWorldTimerManager().ClearTimer(SpawnExplosionTimerHandle);

    if(Particle) Particle->Deactivate();
	if(NiagaraParticle) NiagaraParticle->Deactivate();
	
	if(!bAlreadyExploded)
	{
		bAlreadyExploded = true;

		if(ExplosionParticle) ExplosionParticle->Activate(true);
		if(ExplosionNiagaraParticle) ExplosionNiagaraParticle->Activate(true);
		if(ExplosionAudio) ExplosionAudio->Activate(true);

		TArray<AActor*> SpiritsIn;
		ExplosionSphere->GetOverlappingActors(SpiritsIn, ASpirit::StaticClass());

		for (auto& ActorHit : SpiritsIn)
		{
			ASpirit* SpiritHit = Cast<ASpirit>(ActorHit);
			if(SpiritHit)
			{
				if(!SpiritHit->IsDead())
				{
					SpiritHit->SpiritTakeDamage(Damage, FinalTypeOfDamage, OwnerPawn, SpiritHit->GetActorLocation(), false);
				}
			}
		}

		if(OwnerPawn)
		{
			UAISense_Hearing::ReportNoiseEvent(this, GetActorLocation(), 1.f, OwnerPawn, 5000.f, FName("HearEvent"));
		}

		GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &ABomb_Spell::TurnOffSpell, ExplosionTime, false);
	}
}

void ABomb_Spell::TurnOffSpell() 
{
	if(ExplosionParticle) ExplosionParticle->Deactivate();
	if(ExplosionNiagaraParticle) ExplosionNiagaraParticle->Deactivate();
	if(ExplosionAudio) ExplosionAudio->Deactivate();

    GetWorldTimerManager().SetTimer(TimerHandle, this, &ASpell::DestroySpell, WaitBeforeDestroy, false);
}
