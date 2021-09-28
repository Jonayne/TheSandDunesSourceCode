// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Amit.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API AAmit : public AEnemy
{
	GENERATED_BODY()
	
public:
	AAmit();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Amit")
	class UAudioComponent* ActionSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Amit")
	class UNiagaraComponent* DashEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Amit")
	class UAudioComponent* JumpSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Amit")
	class UNiagaraComponent* JumpExplosionEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Amit")
	class USphereComponent* ExplosionSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Amit")
	class UAudioComponent* ExplosionSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Amit")
	float JumpDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Amit")
	TSubclassOf<class AHealingAmitArea> HealAreaClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Amit")
	TSubclassOf<class ASummonEnemy> ClassToSummon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Amit")
	TSubclassOf<class ASummonEnemy> Class2ToSummon;

	FTimerHandle TimerToNextAction;
	FTimerHandle JumpTimer;

	bool bExecutedOnce;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Landed(const FHitResult& Hit) override;

public:

	virtual void KillThisSpirit() override;
	virtual void OnHaveSeenAnEnemy() override;

	void ExecuteNextAction();

private:

	int32 RangePhase;
	float NextJumpTime;
	bool bCanExplote;
	bool bFightHasBegun;

	void InvokeArcanosProj(float TimeBeforeNextAction);
	void InvokeSombrasProj(float TimeBeforeNextAction);
	void InvokeAguaProj(float TimeBeforeNextAction);
	void InvokeRayo(float TimeBeforeNextAction);
	void MakeJump(float TimeBeforeNextAction);
	void SummonAllies(float TimeBeforeNextAction);
	void SummonOffensiveAllies(float TimeBeforeNextAction);
	void TeleportToDiana();
	void InvokeHealArea(float TimeBeforeNextAction);
	void Explote();
};
