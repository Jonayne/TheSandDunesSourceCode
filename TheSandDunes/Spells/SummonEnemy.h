// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SummonEnemy.generated.h"

UCLASS()
class THESANDDUNES_API ASummonEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASummonEnemy();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SummonEnemy")
	class UNiagaraSystem* SummonEnemyNiagaraParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SummonEnemy")
	class UParticleSystem* SummonEnemyParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SummonEnemy")
	class USoundBase* SummonEnemyAudio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SummonEnemy")
	float SpawnAreaRadius;

	// Si es distinto al vector 0, spawneara los enemigos ahi.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SummonEnemy")
	FVector SpawnAtLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SummonEnemy")
	FRotator SpawnAtRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SummonEnemy")
	TArray<TSubclassOf<class AEnemy>> EnemiesToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SummonEnemy")
	float SecondsBetweenSummons;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SummonEnemy")
	float EnemiesMaxScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SummonEnemy")
	float EnemiesMinScale;

	FTimerHandle SummonEnemyTimerHandler;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	void SummonAnEnemy();

	void PlaySpawnVisualsAtCharacter(FTransform SpawnTransform);

private:
	int32 NumberOfEnemiesSpawned;
	class ASpirit* OwnerSpirit;
};
