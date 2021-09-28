// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Pickup_Resistance.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API APickup_Resistance : public APickup
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Pickup Resistance")
	FString StatName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Pickup Resistance")
	float StatToIncrement;

public:

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

};
