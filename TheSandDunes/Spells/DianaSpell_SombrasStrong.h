// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Spell.h"
#include "DianaSpell_SombrasStrong.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API ADianaSpell_SombrasStrong : public ASpell
{
	GENERATED_BODY()

public:	
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	void TeleportToTarget(class ASpirit* TargetToTeleport);
};
