// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "Tornado.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Spirit.h"
#include "TimerManager.h"
#include "DianaSpell_NaturalezaStrong.h"

// Sets default values
ATornado::ATornado()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TornadoAudioComponent = CreateDefaultSubobject<UAudioComponent>(FName("TornadoAudio"));
	RootComponent = TornadoAudioComponent;

	TornadoParticle = CreateDefaultSubobject<UParticleSystemComponent>(FName("TornadoParticle"));
	TornadoParticle->SetupAttachment(GetRootComponent());
	
	TornadoNiagaraParticle = CreateDefaultSubobject<UNiagaraComponent>(FName("TornadoNiagaraParticle"));
	TornadoNiagaraParticle->SetupAttachment(GetRootComponent());

	TornadoDuration = 10.f;
	WaitBeforeDestroy = 1.f;
	TornadoGravityModification = -2.f;
	InvulnerabilityExtra = .2f;
}

// Called when the game starts or when spawned
void ATornado::BeginPlay()
{
	Super::BeginPlay();

	ADianaSpell_NaturalezaStrong* SpellOwner = Cast<ADianaSpell_NaturalezaStrong>(GetOwner());

	Spirit = SpellOwner->GetOwnerSpirit();

	if(Spirit)
	{
		Spirit->ChangeSpiritGravity(TornadoGravityModification);
		Spirit->DodgeProbabilityIfState += InvulnerabilityExtra;
	}

	GetWorldTimerManager().SetTimer(TornadoTimerHandler, this, &ATornado::StopTornado, TornadoDuration, false);
}

// Called every frame
void ATornado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATornado::StopTornado() 
{
	if(Spirit)
	{
		Spirit->ChangeSpiritGravity((-1.f) * TornadoGravityModification);
		Spirit->DodgeProbabilityIfState -= InvulnerabilityExtra;
	}

	if(TornadoParticle) TornadoParticle->Deactivate();
	if(TornadoNiagaraParticle) TornadoNiagaraParticle->Deactivate();

	GetWorldTimerManager().SetTimer(TornadoDestroyTimerHandler, this, &ATornado::DestroyTornado, WaitBeforeDestroy, false);
}

void ATornado::DestroyTornado() 
{
	Destroy();
}
