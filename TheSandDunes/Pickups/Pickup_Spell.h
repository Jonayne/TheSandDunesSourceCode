// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Pickup_Spell.generated.h"

/**
 * 
 */
UCLASS()
class THESANDDUNES_API APickup_Spell : public APickup
{
	GENERATED_BODY()

public:
	
	APickup_Spell();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Pickup Spell")
	FString SpellName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Pickup Spell")
	TMap<int32, TSubclassOf<class ASpell>> WeakSpells;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Pickup Spell")
	TMap<int32, TSubclassOf<class ASpell>> StrongSpells;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Pickup Spell")
	TMap<int32, TSubclassOf<class ASpiritState>> BuffStates;

	UPROPERTY(BlueprintReadOnly, Category ="Pickup Spell")
	bool bIsNewSpell;

public:

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	void UnlockAchievementNewSpell();
	void CheckMaxSpellAchievement(int32 NewLevel);

	UFUNCTION(BlueprintImplementableEvent, Category="Steam Achievements")
	void SaveAchievementOnSteam(const FString& AchievementName);

};
