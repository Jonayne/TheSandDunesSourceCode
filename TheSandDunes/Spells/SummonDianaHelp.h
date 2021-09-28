// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SummonDianaHelp.generated.h"

UCLASS()
class THESANDDUNES_API ASummonDianaHelp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASummonDianaHelp();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SummonDianaHelp")
	class UNiagaraSystem* SummonDianaHelpNiagaraParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SummonDianaHelp")
	class UParticleSystem* SummonDianaHelpParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="SummonDianaHelp")
	class USoundBase* SummonDianaHelpAudio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SummonDianaHelp")
	float SpawnAreaRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SummonDianaHelp")
	TArray<TSubclassOf<class ASpiritHelper>> SpiritsToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SummonDianaHelp")
	float SecondsBetweenSummons;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SummonDianaHelp")
	float SpiritsMaxScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="SummonDianaHelp")
	float SpiritsMinScale;

	FTimerHandle SummonDianaHelpTimerHandler;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	void SummonASpirit();

	void PlaySpawnVisualsAtCharacter(FTransform SpawnTransform);

private:
	int32 NumberOfSpiritsSpawned;
	class ADiana* OwnerDiana;
};
