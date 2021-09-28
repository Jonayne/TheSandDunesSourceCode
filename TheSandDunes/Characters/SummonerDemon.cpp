// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "SummonerDemon.h"
#include "Spirit.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SummonEnemy.h"
#include "NiagaraComponent.h"


ASummonerDemon::ASummonerDemon() 
{
    SpiritMaxBuffSpeed = 2200.f;
	SpiritWalkSpeed = 1700.f;

    WaitBeforeDestroy = 5.f;

    TimeBetweenSpawns = 6.f;

    Tags.Add(FName("SummonerDemon"));

    SpawnEnemySound = CreateDefaultSubobject<UAudioComponent>(FName("SpawnEnemySound"));
	SpawnEnemySound->SetupAttachment(GetMesh());
	SpawnEnemySound->bAutoActivate = false;
    
    SpawnEffect = CreateDefaultSubobject<UNiagaraComponent>(FName("SpawnEffect"));
	SpawnEffect->SetupAttachment(GetMesh());
	SpawnEffect->bAutoActivate = false;

    AreaEffect = CreateDefaultSubobject<UNiagaraComponent>(FName("AreaEffect"));
	AreaEffect->SetupAttachment(GetMesh());

    TimeBeforeFirstSpawn = 3.f;
    MaxSpawnsBeforeDying = 3;
    bSpellsCanMoveActor = false;
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

void ASummonerDemon::BeginPlay() 
{
    Super::BeginPlay();
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

void ASummonerDemon::SpawnEnemies()
{
    if(TimesSpawned < MaxSpawnsBeforeDying && !IsDead())
    {
        if(SummonEnemiesClass)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;

            ASummonEnemy* Spawner = GetWorld()->SpawnActor<ASummonEnemy>(SummonEnemiesClass, SpawnParams);
            Spawner->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

            TimesSpawned++;

            if(SpawnEnemySound) SpawnEnemySound->Activate(true);
            if(SpawnEffect) SpawnEffect->Activate(true);
        }
    }
    else
    {
        GetWorldTimerManager().ClearTimer(TimerToSpawn);
        KillThisSpirit();
    }
}

void ASummonerDemon::StartSpawning() 
{
    SpawnEnemies();
    GetWorldTimerManager().SetTimer(TimerToSpawn, this, &ASummonerDemon::SpawnEnemies, TimeBetweenSpawns, true);
}

void ASummonerDemon::OnHaveSeenAnEnemy() 
{
    Super::OnHaveSeenAnEnemy();
    if(!bExecutedOnce && bCanStartFight)
    {
        bExecutedOnce = true;
        GetWorldTimerManager().SetTimer(TimerToSpawnFirstTime, this, &ASummonerDemon::StartSpawning, TimeBeforeFirstSpawn, false);
    }
}
