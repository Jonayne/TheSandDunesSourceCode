// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AguaArea.generated.h"

UCLASS()
class THESANDDUNES_API AAguaArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAguaArea();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AguaArea")
	class USphereComponent* SphereArea;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AguaArea")
	class UNiagaraComponent* AguaAreaNiagaraParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AguaArea")
	class UParticleSystemComponent* AguaAreaParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AguaArea")
	class UAudioComponent* AguaAreaAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AguaArea")
	class UAudioComponent* AguaAreaHitAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AguaArea")
	float AguaAreaDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AguaArea")
	float AguaDashDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AguaArea")
	float WaitBeforeDestroy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AguaArea")
	float StaminaRecoverQuantity;

	FTimerHandle AguaAreaTimerHandler;
	FTimerHandle AguaAreaDestroyTimerHandler;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void StopAguaArea();

	void DestroyAguaArea();

private:
	class ADiana* DianaOwner;
};
