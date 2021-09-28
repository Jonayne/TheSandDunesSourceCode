// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Yimitir.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API AYimitir : public AEnemy
{
	GENERATED_BODY()
	
public:
	AYimitir();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Yimitir")
	int32 NumberOfExplosions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Yimitir")
	float TimeBeetwenExplosions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Yimitir")
	class UAudioComponent* ActionSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Yimitir")
	TSubclassOf<class ASummonEnemy> ClassToSummon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Yimitir")
	TSubclassOf<AActor> ExplosionClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Yimitir")
	TSubclassOf<class AHealingArea> HealAreaClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Yimitir")
	class UNiagaraComponent* DashEffect;
	
	FTimerHandle TimerToNextAction;
	FTimerHandle DashDurationTimer;
	FTimerHandle ExplosionsHandler;
	FTimerHandle TryToMoveTimer;
	FTimerHandle WaitToMoveAgainHandler;

	bool bExecutedOnce;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:

	virtual void KillThisSpirit() override;
	virtual void OnHaveSeenAnEnemy() override;

	void ChooseNextAction();
	void ExecuteNextAction();

private:

	int32 SetOfAttacks;
	int32 RangePhase;
	int32 NumerOfExplosionsAux;

	void SummonAllies(float TimeBeforeNextAction);
	void SummonExplosionsTime(float TimeBeforeNextAction);
	void SummonExplosions();
	void MakeDash(bool bTowardDiana, float TimeBeforeNextAction);
	void StopDash();
	void InvokeAutiAIMProjectile(float TimeBeforeNextAction);
	void InvokeFastProjectile(float TimeBeforeNextAction);
	void InvokeHealArea(float TimeBeforeNextAction);
	void MoveYimitir();
	void MoveAgain();
};
