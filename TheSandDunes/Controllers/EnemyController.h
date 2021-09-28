// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyController.generated.h"

/**
 * Clase base para el controlador de los Enemigos.
 */
UCLASS()
class THESANDDUNES_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyController();
	
	UPROPERTY(EditAnywhere, Category="Enemy Controller")
	class UBehaviorTree* AIBehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy Controller")
	bool bAttackEveryone;

	UPROPERTY()	
	class UAIPerceptionComponent* EnemyPerceptionComponent;

	class ASpirit* SeenPlayer;

	class UAISenseConfig_Sight* SightConfig;
	class UAISenseConfig_Damage* DamageConfig;
	class UAISenseConfig_Hearing* HearingConfig;

	class AEnemy* ControlledEnemy;

protected:
	virtual void OnPossess(APawn* InPawn) override;

public:
	virtual void Tick(float DeltaSeconds) override;

	bool IsDead() const;

	UFUNCTION()
	void OnActorUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION(BlueprintCallable)
	void StartEnemyBehaviorTree();

	ASpirit* GetSeenPlayer();

	bool IsSeeingPlayer();

	ASpirit* GetSpiritControlled();
};
