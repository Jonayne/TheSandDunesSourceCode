// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "SummonDianaHelp.h"
#include "NiagaraSystem.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"
#include "TimerManager.h"
#include "Diana.h"
#include "SpiritHelper.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ASummonDianaHelp::ASummonDianaHelp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SummonDianaHelpAudio = CreateDefaultSubobject<USoundBase>(FName("SummonDianaHelpAudio"));
	SummonDianaHelpParticle = CreateDefaultSubobject<UParticleSystem>(FName("SummonDianaHelpParticle"));
	SummonDianaHelpNiagaraParticle = CreateDefaultSubobject<UNiagaraSystem>(FName("SummonDianaHelpNiagaraParticle"));

	SecondsBetweenSummons = 2.f;
	SpiritsMaxScale = 1.f;
	SpiritsMinScale = 1.f;
	SpawnAreaRadius = 500.f;
}

// Called when the game starts or when spawned
void ASummonDianaHelp::BeginPlay()
{
	Super::BeginPlay();

	OwnerDiana = GetOwner<ADiana>();

	SummonASpirit();
	GetWorldTimerManager().SetTimer(SummonDianaHelpTimerHandler, this, &ASummonDianaHelp::SummonASpirit, SecondsBetweenSummons, true);
}

void ASummonDianaHelp::SummonASpirit() 
{
	if(SpiritsToSpawn.IsValidIndex(NumberOfSpiritsSpawned))
	{
		FVector SpawnRandLocation = GetActorLocation();
		FRotator SpawnRandRotation = GetActorRotation();

		FVector RandomUnitVector = UKismetMathLibrary::RandomUnitVector();
		FVector RandomPoint = RandomUnitVector * (FMath::RandRange(SpawnAreaRadius*.2f, SpawnAreaRadius));
		RandomPoint.Z = 0.f;

		if(OwnerDiana && !OwnerDiana->IsDead())
		{
			SpawnRandLocation = OwnerDiana->GetActorLocation() + RandomPoint;
			SpawnRandLocation.Z = OwnerDiana->GetActorLocation().Z + 200.f;
			SpawnRandRotation = OwnerDiana->GetActorRotation();
		}
		
		FTransform SpawnTransform =  UKismetMathLibrary::MakeTransform(SpawnRandLocation, SpawnRandRotation, FVector(FMath::RandRange(SpiritsMinScale, SpiritsMaxScale)));
		PlaySpawnVisualsAtCharacter(SpawnTransform);
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwnerDiana;

		ASpiritHelper* SpawnedActor = GetWorld()->SpawnActor<ASpiritHelper>(SpiritsToSpawn[NumberOfSpiritsSpawned], SpawnTransform, SpawnParams);
		if(SpawnedActor)
		{
			SpawnedActor->StartActions();
		}
		
		NumberOfSpiritsSpawned++;
	}
	else
	{
		Destroy();
	}
}

void ASummonDianaHelp::PlaySpawnVisualsAtCharacter(FTransform SpawnTransform) 
{
	if(SummonDianaHelpParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SummonDianaHelpParticle, SpawnTransform);
	}
	if(SummonDianaHelpNiagaraParticle)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, SummonDianaHelpNiagaraParticle, SpawnTransform.GetLocation(), SpawnTransform.GetRotation().Rotator(), SpawnTransform.GetScale3D());
	}
	if(SummonDianaHelpAudio)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SummonDianaHelpAudio, SpawnTransform.GetLocation());
	}
}
