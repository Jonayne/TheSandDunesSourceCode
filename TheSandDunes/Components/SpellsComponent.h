// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpellsComponent.generated.h"

class ASpell;
class ASpirit;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THESANDDUNES_API USpellsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpellsComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Component")
	TMap<FString, TSubclassOf<ASpell>> Spells;

	TMap<FString, FTimerHandle> SpellsSecundaryTimers;

	TMap<FString, FTimerHandle> SpellsTimers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spell Component")
	FString CurrentSpellType;

	bool bIsStrongSpellComponent;

	FName SocketNameForSpawn;
	int32 NumberOfProjectiles;
	bool bIsAOE;
	bool bIsBeam;
	float SecundarySpellCooldown;
	float SpellCooldown;
	float TimeBetweenSpellProjectiles;
	float StaminaCost;

	bool bIsCharged;

	FTimerHandle TimeBetweenSpellsHandler;
	float CurrentProjectileSpawned;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ActivateSecundarySpell();

	void ActivateSecundarySpell(FString SpecificSpellType);

	// Random spell
	void ActivateSpell(FName SocketName);

	// Specified spell.
	void ActivateSpell(FName SocketName, FString SpellType);
	
	// Activa el Spell que este actor tiene como fijado.
	void ActivateCurrentSpell(FName SocketName);

	void AddSpellToComponent(FString NewSpellType, TSubclassOf<class ASpell> NewSpellClass);

	UFUNCTION(BlueprintCallable)
	void SetCurrentSpellByType(FString SpellType);

	TMap<FString, FText> GetSpellDescription(FString SpellType);

	int32 GetSpellTypeLevel(FString SpellType);

	FString GetCurrentSpellType();

	TArray<FString> GetSpells();

	void ReleaseSpell();
	
	float GetSpellPercentage() const;

	bool ValidSpell(FString SpellType);

	float GetSecundarySpellPercentage() const;

	float GetActiveSpellPercentage(FString SpellType) const;

	float GetSpellPercentage(FString SpellType) const;

	float GetSpellCooldownByType(FString SpellType) const;
	
	float GetCurrentSpellStaminaCost();

	void SetSpellComponentAsStrong();

	void SetSpellComponentAsWeak();
	
	float GetSpellCD();

	void UpdateCurrentSpellValues(TSubclassOf<ASpell> ClassToCaptureValues);

	ASpirit* GetOwnerCharacter() const;

	bool CanShotSpell();
	
	TMap<FString, TSubclassOf<ASpell>> GetSpellsList();
	
	void SetSpellsList(TMap<FString, TSubclassOf<ASpell>> NewSpellList);

private:
	void SpawnSpell();

	void CastSpell();

	void StartSpellsComponent();
	
private:
	ASpirit* OwnerCharacter;
	bool bIsDianaOwner;
};
