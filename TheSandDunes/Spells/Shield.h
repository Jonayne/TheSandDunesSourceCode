// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shield.generated.h"

UCLASS()
class THESANDDUNES_API AShield : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShield();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shield")
	class UBoxComponent* Box;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Shield")
	class UNiagaraComponent* ShieldNiagaraParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Shield")
	class UAudioComponent* ShieldAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Shield")
	float ShieldDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Shield")
	float ShieldWaitBeforeDestroy;

	FTimerHandle ShieldOnTimerHandle;
	FTimerHandle ShieldDestroyTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void DestroyShield();

	class ASpirit* GetSpiritOwner();
	
protected:
	ASpirit* Spirit;

	class AEnemy* EnemyOwner;
};
