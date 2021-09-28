// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "SummonEnemy.h"
#include "NiagaraSystem.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"
#include "TimerManager.h"
#include "Enemy.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ASummonEnemy::ASummonEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SummonEnemyAudio = CreateDefaultSubobject<USoundBase>(FName("SummonEnemyAudio"));
	SummonEnemyParticle = CreateDefaultSubobject<UParticleSystem>(FName("SummonEnemyParticle"));
	SummonEnemyNiagaraParticle = CreateDefaultSubobject<UNiagaraSystem>(FName("SummonEnemyNiagaraParticle"));

	SecondsBetweenSummons = 2.f;
	EnemiesMaxScale = 1.f;
	EnemiesMinScale = 1.f;
	SpawnAreaRadius = 1000.f;
}

// Called when the game starts or when spawned
void ASummonEnemy::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();
	if(OwnerActor)
	{
		OwnerSpirit = Cast<ASpirit>(OwnerActor);
	}

	SummonAnEnemy();
	GetWorldTimerManager().SetTimer(SummonEnemyTimerHandler, this, &ASummonEnemy::SummonAnEnemy, SecondsBetweenSummons, true);
}

void ASummonEnemy::SummonAnEnemy() 
{
	if(EnemiesToSpawn.IsValidIndex(NumberOfEnemiesSpawned))
	{
		FVector SpawnRandLocation = GetActorLocation();
		FRotator SpawnRandRotation = GetActorRotation();

		if(SpawnAtLocation.IsZero())
		{
			FVector RandomUnitVector = UKismetMathLibrary::RandomUnitVector();
			FVector RandomPoint = RandomUnitVector * (FMath::RandRange(SpawnAreaRadius*.2f, SpawnAreaRadius));
			RandomPoint.Z = 0.f;

			if(OwnerSpirit && !OwnerSpirit->IsDead())
			{
				SpawnRandLocation = OwnerSpirit->GetActorLocation() + RandomPoint;
				SpawnRandRotation = OwnerSpirit->GetActorRotation();
			}
		}
		else
		{
			SpawnRandLocation = SpawnAtLocation;
			SpawnRandRotation = SpawnAtRotation;
		}
		
		FTransform SpawnTransform =  UKismetMathLibrary::MakeTransform(SpawnRandLocation, SpawnRandRotation, FVector(FMath::RandRange(EnemiesMinScale, EnemiesMaxScale)));
		PlaySpawnVisualsAtCharacter(SpawnTransform);
		
		AEnemy* SpawnedActor = GetWorld()->SpawnActor<AEnemy>(EnemiesToSpawn[NumberOfEnemiesSpawned], SpawnTransform);
		if(SpawnedActor)
		{
			SpawnedActor->StartBossFightEvent();
		}
		
		NumberOfEnemiesSpawned++;
	}
	else
	{
		Destroy();
	}
}

void ASummonEnemy::PlaySpawnVisualsAtCharacter(FTransform SpawnTransform) 
{
	if(SummonEnemyParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SummonEnemyParticle, SpawnTransform);
	}
	if(SummonEnemyNiagaraParticle)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, SummonEnemyNiagaraParticle, SpawnTransform.GetLocation(), SpawnTransform.GetRotation().Rotator(), SpawnTransform.GetScale3D());
	}
	if(SummonEnemyAudio)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SummonEnemyAudio, SpawnTransform.GetLocation());
	}
}
