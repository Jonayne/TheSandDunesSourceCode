// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Pickup_Stat.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API APickup_Stat : public APickup
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Pickup Stat")
	float HealthToIncrement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Pickup Stat")
	float StaminaToIncrement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Pickup Stat")
	float BaseDamageToIncrement;

public:

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

};
