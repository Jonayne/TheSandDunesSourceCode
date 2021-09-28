// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Spell.h"
#include "DianaSpell_Sagrado.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API ADianaSpell_Sagrado : public ASpell
{
	GENERATED_BODY()

public:
	ADianaSpell_Sagrado();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Auto AIM")
	class USphereComponent* EnemiesDetectionSphere;

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
	class ADiana* Diana;	
	class ASpirit* EnemyTargeted;
};
