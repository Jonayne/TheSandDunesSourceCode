// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tornado.generated.h"

UCLASS()
class THESANDDUNES_API ATornado : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATornado();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tornado")
	class UNiagaraComponent* TornadoNiagaraParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tornado")
	class UParticleSystemComponent* TornadoParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tornado")
	class UAudioComponent* TornadoAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tornado")
	float TornadoDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tornado")
	float TornadoGravityModification;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tornado")
	float WaitBeforeDestroy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Tornado")
	float InvulnerabilityExtra;

	FTimerHandle TornadoTimerHandler;
	FTimerHandle TornadoDestroyTimerHandler;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StopTornado();

	void DestroyTornado();

private:
	class ASpirit* Spirit;
};
