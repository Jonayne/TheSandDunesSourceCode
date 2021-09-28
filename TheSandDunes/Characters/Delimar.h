// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Delimar.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API ADelimar : public AEnemy
{
	GENERATED_BODY()
	
public:
	ADelimar();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Delimar")
	float DashDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Delimar")
	float DashDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Delimar")
	class UMaterialInstance* ShortRangeMat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Delimar")
	class UMaterialInstance* MediumRangeMat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Delimar")
	class UMaterialInstance* LongRangeMat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Delimar")
	class UArrowComponent* DashDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Delimar")
	class UAudioComponent* DashSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Delimar")
	class UAudioComponent* ActionSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Delimar")
	class UNiagaraComponent* DashAreaEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Delimar")
	TSubclassOf<class ASummonEnemy> SummonSwordsClass;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Delimar")
	// class UParticleSystemComponent* ActionParticleSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Delimar")
	class UNiagaraComponent* DashEffect;
	
	FTimerHandle TimerToNextAction;
	FTimerHandle DashDurationTimer;
	FTimerHandle TryToMoveTimer;

	bool bExecutedOnce;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:

	virtual void KillThisSpirit() override;
	
	virtual void OnHaveSeenAnEnemy() override;
	
	FVector CalculateDashVelocity(bool bTowardDiana);

	void ChooseNextAction();

	void ExecuteNextAction();

private:

	int32 SetOfAttacks;
	int32 RangePhase;

	void SummonSwords(float TimeBeforeNextAction);
	void ThrowHighSpeedProjectiles(float TimeBeforeNextAction);
	void MakeDash(bool bTowardDiana, float TimeBeforeNextAction);
	void StopDash();
	void ThrowBigProjectiles(float TimeBeforeNextAction);
	void Explode(float TimeBeforeNextAction);
	void MoveDelimar();
};
