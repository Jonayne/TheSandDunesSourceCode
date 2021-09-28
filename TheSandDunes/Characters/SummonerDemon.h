// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "SummonerDemon.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API ASummonerDemon : public AEnemy
{
	GENERATED_BODY()
	
public:
	ASummonerDemon();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Summoner")
	float TimeBetweenSpawns;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Summoner")
	float TimeBeforeFirstSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Summoner")
	int32 MaxSpawnsBeforeDying;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Summoner")
	TSubclassOf<class ASummonEnemy> SummonEnemiesClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Summoner")
	class UAudioComponent* SpawnEnemySound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Summoner")
	class UNiagaraComponent* SpawnEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Summoner")
	class UNiagaraComponent* AreaEffect;

	FTimerHandle TimerToSpawn;
	FTimerHandle TimerToSpawnFirstTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable, Category="Summoner")
	void SpawnEnemies();

	void StartSpawning();
	
	virtual void OnHaveSeenAnEnemy() override;

private:
	int32 TimesSpawned;
	bool bExecutedOnce;
};
