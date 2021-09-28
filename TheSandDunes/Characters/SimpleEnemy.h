// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "SimpleEnemy.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API ASimpleEnemy : public AEnemy
{
	GENERATED_BODY()
	
public:
	ASimpleEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Stats")
	float TimeBetweenActions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Stats")
	float TimeBeforeStartingMin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Stats")
	float TimeBeforeStartingMax;

	FTimerHandle TimerToMakeAnim;
	FTimerHandle TimerBeforeAction;

public:

	void MakeAnimation();

	virtual void KillThisSpirit() override;

	virtual void OnHaveSeenAnEnemy() override;

	void StartActions();

private:
	bool bExecutedOnce;
	
};
