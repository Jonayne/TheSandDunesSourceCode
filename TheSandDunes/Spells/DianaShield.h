// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Shield.h"
#include "DianaShield.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API ADianaShield : public AShield
{
	GENERATED_BODY()

public:	
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	class ADiana* Diana;
};
