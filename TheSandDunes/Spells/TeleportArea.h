// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeleportArea.generated.h"

UCLASS()
class THESANDDUNES_API ATeleportArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleportArea();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="TeleportArea")
	class USphereComponent* SphereDetectionArea;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TeleportArea")
	class UNiagaraComponent* TeleportAreaNiagaraParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TeleportArea")
	class UParticleSystemComponent* TeleportAreaParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TeleportArea")
	class UAudioComponent* TeleportAreaAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TeleportArea")
	float WaitBeforeTeleporting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TeleportArea")
	float WaitBeforeDestroy;

	FTimerHandle TeleportAreaDestroyTimerHandler;
	FTimerHandle WaitToTeleportTimerHandler;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	void TeleportToArea();

	void DestroyTeleportArea();

private:
	class AEnemy* SpiritOwner;
};
