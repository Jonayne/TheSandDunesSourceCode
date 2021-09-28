// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "HealingAmitArea.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Enemy.h"
#include "Diana.h"


// Sets default values
AHealingAmitArea::AHealingAmitArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereArea = CreateDefaultSubobject<USphereComponent>(FName("SphereArea"));
	RootComponent = SphereArea;
	SphereArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SphereArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);

	HealingAmitAreaAudioComponent = CreateDefaultSubobject<UAudioComponent>(FName("HealingAmitAreaAudio"));
	HealingAmitAreaAudioComponent->SetupAttachment(GetRootComponent());

	HealingAmitAreaParticle = CreateDefaultSubobject<UParticleSystemComponent>(FName("HealingAmitAreaParticle"));
	HealingAmitAreaParticle->SetupAttachment(GetRootComponent());
	
	HealingAmitAreaNiagaraParticle = CreateDefaultSubobject<UNiagaraComponent>(FName("HealingAmitAreaNiagaraParticle"));
	HealingAmitAreaNiagaraParticle->SetupAttachment(GetRootComponent());

	HealingAmitAreaDuration = 10.f;
	WaitBeforeDestroy = 1.f;
	HealQuantity = 50.f;

	HealingTypeDamage = TEXT("Sagrado");
}

// Called when the game starts or when spawned
void AHealingAmitArea::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerPawn = GetOwner();
	if(OwnerPawn)
	{
		SpiritOwner = Cast<ASpirit>(OwnerPawn);
	}

	GetWorldTimerManager().SetTimer(HealingAmitAreaTimerHandler, this, &AHealingAmitArea::StopHealingAmitArea, HealingAmitAreaDuration, false);
	GetWorldTimerManager().SetTimer(HealTickTimerHandler, this, &AHealingAmitArea::ApplyHealingInArea, 1.f, true);
}

// Called every frame
void AHealingAmitArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealingAmitArea::ApplyHealingInArea() 
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
					ADiana* DianaRef = Cast<ADiana>(SpiritHit);
					if(DianaRef)
					{
						DianaRef->ModifyStamina(-HealQuantity);
					}
				}
				else
				{
					SpiritHit->ModifyHealth(HealQuantity);
				}
			}
		}
	}
}

void AHealingAmitArea::StopHealingAmitArea() 
{
	GetWorldTimerManager().ClearTimer(HealTickTimerHandler);

	if(HealingAmitAreaParticle) HealingAmitAreaParticle->Deactivate();
	if(HealingAmitAreaNiagaraParticle) HealingAmitAreaNiagaraParticle->Deactivate();

	GetWorldTimerManager().SetTimer(HealingAmitAreaDestroyTimerHandler, this, &AHealingAmitArea::DestroyHealingAmitArea, WaitBeforeDestroy, false);
}

void AHealingAmitArea::DestroyHealingAmitArea() 
{
	Destroy();
}
