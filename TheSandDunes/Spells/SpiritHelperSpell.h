// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Spell.h"
#include "SpiritHelperSpell.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API ASpiritHelperSpell : public ASpell
{
	GENERATED_BODY()

public:
	ASpiritHelperSpell();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Auto AIM")
	class USphereComponent* EnemiesDetectionSphere;

	UPROPERTY(BlueprintReadOnly, Category = "Auto AIM")
	class ASpirit* EnemyTargeted;

public:	
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	UFUNCTION()
	virtual void OnDetectionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable, Category="Spell Movement")
	void MoveSpellToTarget(float DeltaSeconds);

	virtual void PlayHitEffects() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	class ASpiritHelper* HelperOwner;
};
