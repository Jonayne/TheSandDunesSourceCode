// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Spell.h"
#include "Beam_Spell.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API ABeam_Spell : public ASpell
{
	GENERATED_BODY()

public:
	ABeam_Spell();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Beam")
	float SpellDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Beam")
	float BeamRange;

	void TurnOffEffects();

	FTimerHandle SpawnedTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void ChangeBeamEnd();

	class ADiana* Diana;

	void ShotBeamTick();

	FVector TargetLocationToHit;

	bool bPlayingHitParticles;

	void PlayBeamParticles();
};
