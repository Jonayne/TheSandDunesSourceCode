// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "TeleportArea.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Enemy.h"


// Sets default values
ATeleportArea::ATeleportArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TeleportAreaAudioComponent = CreateDefaultSubobject<UAudioComponent>(FName("TeleportAreaAudio"));
	RootComponent = TeleportAreaAudioComponent;

	SphereDetectionArea = CreateDefaultSubobject<USphereComponent>(FName("SphereDetectionArea"));
	SphereDetectionArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereDetectionArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SphereDetectionArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);
	SphereDetectionArea->SetupAttachment(GetRootComponent());

	TeleportAreaParticle = CreateDefaultSubobject<UParticleSystemComponent>(FName("TeleportAreaParticle"));
	TeleportAreaParticle->SetupAttachment(GetRootComponent());
	
	TeleportAreaNiagaraParticle = CreateDefaultSubobject<UNiagaraComponent>(FName("TeleportAreaNiagaraParticle"));
	TeleportAreaNiagaraParticle->SetupAttachment(GetRootComponent());

	WaitBeforeDestroy = 5.f;
	WaitBeforeTeleporting = 2.f;
}

// Called when the game starts or when spawned
void ATeleportArea::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerPawn = GetOwner();
	if(OwnerPawn)
	{
		SpiritOwner = Cast<AEnemy>(OwnerPawn);
	}

	GetWorldTimerManager().SetTimer(WaitToTeleportTimerHandler, this, &ATeleportArea::TeleportToArea, WaitBeforeTeleporting, false);
	GetWorldTimerManager().SetTimer(TeleportAreaDestroyTimerHandler, this, &ATeleportArea::DestroyTeleportArea, WaitBeforeDestroy+WaitBeforeTeleporting, false);
}

void ATeleportArea::TeleportToArea() 
{
	TArray<AActor*> EnemiesIn;
	SphereDetectionArea->GetOverlappingActors(EnemiesIn, AEnemy::StaticClass());
	
	if(SpiritOwner)
	{
		EnemiesIn.Remove(SpiritOwner);
		SpiritOwner->SetRandomPointOnArea(SpiritOwner->GetTargetToHitLocation());
		SpiritOwner->SetActorLocation(SpiritOwner->NextLocation);
	}

	for (auto& ActorHit : EnemiesIn)
	{
		AEnemy* EnemyInArea = Cast<AEnemy>(ActorHit);
		if(EnemyInArea && SpiritOwner && !EnemyInArea->IsDead() && EnemyInArea->bCanBeTeleportedByOthersEnemies)
		{
			EnemyInArea->RadiusToNextRandomLocation = FMath::FRandRange(300.f, 600.f);
			EnemyInArea->AddVectorToRandomLocation = FVector(0.f, 0.f, FMath::FRandRange(100.f, 300.f));
			EnemyInArea->SetRandomPointOnArea(SpiritOwner->NextLocation);
			EnemyInArea->SetActorLocation(EnemyInArea->NextLocation);
		}
	}
}

void ATeleportArea::DestroyTeleportArea() 
{
	Destroy();
}
