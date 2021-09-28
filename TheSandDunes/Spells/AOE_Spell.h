// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Spell.h"
#include "AOE_Spell.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API AAOE_Spell : public ASpell
{
	GENERATED_BODY()

public:

	AAOE_Spell();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell AOE")
	float SpellDuration;

	virtual void PlayHitEffects() override;
	
	FTimerHandle AOETimerHandle;

	virtual void TurnOffSpell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
