// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "TSDTypes.generated.h"


USTRUCT(BlueprintType)
struct THESANDDUNES_API FDianaStats
{
	GENERATED_BODY()

	FDianaStats()
		: MaxHealth(0.f)
		, Health(0.f)
		, Stamina(0.f)
		, MaxStamina(0.f)
		, BaseDamage(0.f)
		, DianaResistances(TMap<FString, float>())
		, DianaStates(TMap<FString, TSubclassOf<class ASpiritState>>())
		, DianaWeakSpells(TMap<FString, TSubclassOf<class ASpell>>())
		, DianaStrongSpells(TMap<FString, TSubclassOf<class ASpell>>())
		, CurrentSpell(TEXT("Arcanos"))
		, CurrentSpellIndex(1)
		, SpellsOrder(TMap<FString, int32>())
		, CurrentLevelName(TEXT("TSD_MundoDivino"))
		, Location(FVector(0.f))
		, Rotation(FRotator(0.f))
		, bIsTPing(false)
		, NextMapLocation(FVector(0.f))
		, NextMapRotation(FRotator(0.f))
		, RemainingSaves(0)
		, CorruptionLevel(0)
	{}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Diana SaveGame Data")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Diana SaveGame Data")
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Diana SaveGame Data")
	float Stamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Diana SaveGame Data")
	float MaxStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Diana SaveGame Data")
	float BaseDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Diana SaveGame Data")
	TMap<FString, float> DianaResistances;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Diana SaveGame Data")
	TMap<FString, TSubclassOf<class ASpiritState>> DianaStates;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Diana SaveGame Data")
	TMap<FString, TSubclassOf<class ASpell>> DianaWeakSpells;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Diana SaveGame Data")
	TMap<FString, TSubclassOf<ASpell>> DianaStrongSpells;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Diana SaveGame Data")
	FString CurrentSpell;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Diana SaveGame Data")
	int32 CurrentSpellIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Diana SaveGame Data")
	TMap<FString, int32> SpellsOrder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Diana SaveGame Data")
	FString CurrentLevelName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Diana SaveGame Data")
	FVector Location;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Diana SaveGame Data")
	FRotator Rotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Diana SaveGame Data")
	bool bIsTPing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Diana SaveGame Data")
	FVector NextMapLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Diana SaveGame Data")
	FRotator NextMapRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Diana SaveGame Data")
	int32 RemainingSaves;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Diana SaveGame Data")
	int32 CorruptionLevel;
};

USTRUCT(BlueprintType)
struct THESANDDUNES_API FSavableActor
{
	GENERATED_BODY()

	FSavableActor()
		: SavableId(FName())
		, bIsAlive(true)
		, bShouldRespawn(true)
		, State(FName())
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MapActors SaveGame Data")
	FName SavableId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MapActors SaveGame Data")
	bool bIsAlive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MapActors SaveGame Data")
	bool bShouldRespawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MapActors SaveGame Data")
	FName State;

};

USTRUCT(BlueprintType)
struct THESANDDUNES_API FSavableActorMap
{
	GENERATED_BODY()

	FSavableActorMap()
	{}

	FSavableActorMap(TMap<FName, FSavableActor> NewSavableMap)
	: SavableMap(NewSavableMap)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Array SaveGame Data")
	TMap<FName, FSavableActor> SavableMap;
};
