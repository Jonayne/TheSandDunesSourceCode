// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SavableActorInterface.h"
#include "Pickup.generated.h"

UCLASS()
class THESANDDUNES_API APickup : public AActor, public ISavableActorInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Savable Actor")
	FSavableActor SavableInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup Basics")
	bool bPickupActivated;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup Basics")
	class UCapsuleComponent* PickupCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup Basics")
	float WaitForDestroy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup Basics")
	bool bShouldMakeConstantSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup Basics")
	class UStaticMeshComponent* PickupStaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup Basics")
	class UParticleSystemComponent* PickupConstantParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup Basics")
	class UParticleSystemComponent* PickupParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup Basics")
	class UNiagaraComponent* PickupConstantNiagaraParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup Basics")
	class UNiagaraComponent* PickupNiagaraParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup Basics")
	class UAudioComponent* PickupConstantAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup Basics")
	class UAudioComponent* PickupAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup Basics")
	class UAudioComponent* PickupActivateAudio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup Basics")
	FSlateColor PickupMessageColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup Basics")
	FText PickupMessage;

	FTimerHandle PickupEffectsHandler;

	bool bPicked;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintImplementableEvent, Category="Pickup Functions")
	void ShowPickupWidget();

	UFUNCTION(BlueprintCallable, Category="Pickup Functions")
	void ActivatePickup();

	UFUNCTION(BlueprintCallable, Category="Pickup Functions")
	void DeactivatePickup();

	UFUNCTION(BlueprintCallable, Category="Pickup Functions")
	virtual void PlayPickupEffects();

	void DestroyPickup();

	FSavableActor GetSavableActorInfo_Implementation() const override;
	void ChangeActorState_Implementation() override;
	void UpdateSavableInfo_Implementation(const FSavableActor& NewActorInfo) override;
	
};
