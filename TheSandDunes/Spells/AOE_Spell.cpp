// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "AOE_Spell.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"

AAOE_Spell::AAOE_Spell() 
{
	SpellDuration = 5.f;
	bShouldHitWorld = false;
}

void AAOE_Spell::BeginPlay() 
{
	Super::BeginPlay();

	PlayHitEffects();
}

void AAOE_Spell::PlayHitEffects() 
{
    if(Particle) Particle->Activate(true);
	if(NiagaraParticle) NiagaraParticle->Activate(true);
	if(Audio) Audio->Activate(true);

	TArray<AActor*> SpiritsIn;
	Sphere->GetOverlappingActors(SpiritsIn, ASpirit::StaticClass());

	for (auto& ActorHit : SpiritsIn)
	{
		ASpirit* SpiritHit = Cast<ASpirit>(ActorHit);
		if(SpiritHit && SpiritHit != OwnerPawn)
		{
			if(!SpiritHit->IsDead())
			{
				SpiritHit->SpiritTakeDamage(Damage, FinalTypeOfDamage, OwnerPawn, SpiritHit->GetActorLocation(), false);
			}
		}
	}

   	GetWorldTimerManager().SetTimer(AOETimerHandle, this, &AAOE_Spell::TurnOffSpell, SpellDuration, false);
}

void AAOE_Spell::TurnOffSpell() 
{
	if(Particle) Particle->Deactivate();
	if(NiagaraParticle) NiagaraParticle->Deactivate();
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASpell::DestroySpell, WaitBeforeDestroy, false);
}
