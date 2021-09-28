// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "HealingArea.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Enemy.h"
#include "Diana.h"


// Sets default values
AHealingArea::AHealingArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereArea = CreateDefaultSubobject<USphereComponent>(FName("SphereArea"));
	RootComponent = SphereArea;
	SphereArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SphereArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);

	HealingAreaAudioComponent = CreateDefaultSubobject<UAudioComponent>(FName("HealingAreaAudio"));
	HealingAreaAudioComponent->SetupAttachment(GetRootComponent());

	HealingAreaParticle = CreateDefaultSubobject<UParticleSystemComponent>(FName("HealingAreaParticle"));
	HealingAreaParticle->SetupAttachment(GetRootComponent());
	
	HealingAreaNiagaraParticle = CreateDefaultSubobject<UNiagaraComponent>(FName("HealingAreaNiagaraParticle"));
	HealingAreaNiagaraParticle->SetupAttachment(GetRootComponent());

	HealingAreaDuration = 10.f;
	WaitBeforeDestroy = 1.f;
	HealQuantity = 50.f;

	HealingTypeDamage = TEXT("Arcanos");
}

// Called when the game starts or when spawned
void AHealingArea::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerPawn = GetOwner();
	if(OwnerPawn)
	{
		SpiritOwner = Cast<ASpirit>(OwnerPawn);
	}

	GetWorldTimerManager().SetTimer(HealingAreaTimerHandler, this, &AHealingArea::StopHealingArea, HealingAreaDuration, false);
	GetWorldTimerManager().SetTimer(HealTickTimerHandler, this, &AHealingArea::ApplyHealingInArea, 1.f, true);
}

// Called every frame
void AHealingArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealingArea::ApplyHealingInArea() 
{
	TArray<AActor*> SpiritsIn;
	SphereArea->GetOverlappingActors(SpiritsIn, ASpirit::StaticClass());
	for (auto& ActorHit : SpiritsIn)
	{
		ASpirit* SpiritHit = Cast<ASpirit>(ActorHit);
		if(SpiritHit)
		{
			if(SpiritOwner && !SpiritHit->IsDead())
			{
				if((SpiritOwner->IsA(AEnemy::StaticClass()) && SpiritHit->IsA(ADiana::StaticClass())) || 
					(SpiritOwner->IsA(ADiana::StaticClass()) && SpiritHit->IsA(AEnemy::StaticClass())))
				{
					SpiritHit->SpiritTakeDamage(HealQuantity, HealingTypeDamage, SpiritOwner, SpiritHit->GetActorLocation(), false);
				}
				else
				{
					SpiritHit->ModifyHealth(HealQuantity);
				}
			}
		}
	}
}

void AHealingArea::StopHealingArea() 
{
	GetWorldTimerManager().ClearTimer(HealTickTimerHandler);

	if(HealingAreaParticle) HealingAreaParticle->Deactivate();
	if(HealingAreaNiagaraParticle) HealingAreaNiagaraParticle->Deactivate();

	GetWorldTimerManager().SetTimer(HealingAreaDestroyTimerHandler, this, &AHealingArea::DestroyHealingArea, WaitBeforeDestroy, false);
}

void AHealingArea::DestroyHealingArea() 
{
	Destroy();
}
