// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Spirit.h"
#include "SpiritHelper.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API ASpiritHelper : public ASpirit
{
	GENERATED_BODY()
	
public:
	ASpiritHelper();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Auto AIM")
	class UBoxComponent* EnemiesDetectionBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Stats")
	bool bWillAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Stats")
	float TimeBetweenActions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Stats")
	float TimeBeforeStartingMin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Stats")
	float TimeBeforeStartingMax;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Stats")
	float TimeBeforeDying;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float VerticalMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	class UNiagaraComponent* SphereEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	class UNiagaraComponent* SphereEndEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	class UNiagaraComponent* AttackEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Stats")
	class UAudioComponent* AttackSound;

	FVector NextLocation;

	FTimerHandle TimerBeforeAction;
	FTimerHandle TimerBeetwenActions;
	FTimerHandle TimerBeforeDying;

	bool bIsManuallyMovingToNextLocation;

	class AEnemy* Target;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	virtual void KillThisSpirit() override;

	void StartChangeTrajectory();

	void StartActions();

	void ThrowSpell();

private:

	bool bWasGoingUp;

	class ADiana* DianaOwner;

	void MoveHelper(float DeltaTime);
	
};
