// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spell.generated.h"

UCLASS()
class THESANDDUNES_API ASpell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpell();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Basics")
	FString SpellType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Description")
	FText SpellWeakDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Description")
	FText SpellStrongDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Description")
	FText SpellSecundaryDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Basics")
	bool bShouldHitWorld;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Basics")
	class USphereComponent* Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Basics")
	class UParticleSystemComponent* Particle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Basics")
	class UNiagaraComponent* NiagaraParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Basics")
	class UAudioComponent* Audio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Basics")
	class UNiagaraComponent* MuzzleParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Basics")
	UParticleSystemComponent* HitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Basics")
	UNiagaraComponent* NiagaraHitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Basics")
	UAudioComponent* HitAudio;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Basics")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Basics")
	float SpellStaminaCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Basics")
	float SpellLifeSpan;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Projectile")
	float TimeBetweenProjectiles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Basics")
	float WaitBeforeDestroy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Basics")
	float SpellCooldown;

	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Projectile")
	int32 NumberOfProjectilesPerSpell;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Basics")
	int32 SpellLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Movement")
	float SpellSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Movement")
	float SpellAcelerationRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Movement")
	float SpellZigZagFrecuency;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Movement")
	float SpellZigZagAmplitud;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Movement")
	float TimeShouldUseAutoAIM;

	FVector HitPoint;
	FVector TargetLocationToHit;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable, Category="Spell")
	virtual void PlayHitEffects();

	UFUNCTION(BlueprintCallable, Category="Spell Movement")
	void MoveStraightLine(float DeltaSeconds);

	// Por default, el ZigZag es en el eje Y.
	UFUNCTION(BlueprintCallable, Category="Spell Movement")
	void MoveZigZagLine(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category="Spell Movement")
	void MoveZigZagLineXAxis(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category="Spell Movement")
	void MoveZigZagLineZAxis(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category="Spell Movement")
	void MoveZigZagLineAllAxis(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category="Spell Movement")
	void AddSpellRotation(FRotator AddRotator);

	UFUNCTION(BlueprintCallable, Category="Spell Movement")
	void MoveStraightLineWithAceleration(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category="Spell Movement")
	void MoveWithAutoAIM(float DeltaSeconds);

	void PlayHitEffectsAtLocation(class ASpirit* SpiritHit);

	void PlayHitEffectsAtLocation(FVector SpiritHitLocation);

	void DestroySpell();
	
	float GetSpellCooldown() const;

	float GetTimeBetweenProjectiles() const;

	float GetStaminaCost() const;

	// Nos da el numero de proyectiles que se lanzan simultaneamente (por default, no se cuentan los extras)
	int32 GetNumberOfProjectilesPerSpell() const;

	FString GetSpellType();

	TMap<FString, FText> GetSpellDescription() const;

	TSubclassOf<class UStatusEffectBuffBase> GetSecundarySpellEffect() const;

	UFUNCTION(BlueprintCallable)
	int32 GetSpellLevel() const;
	
	float GetSpellDamage();
	
	UFUNCTION(BlueprintCallable, Category="Spell")
	class ASpirit* GetOwnerSpirit();
	
	FVector GetPredictedPositionToAttack();

protected:
	ASpirit* OwnerPawn;
	ASpirit* Target;
	float MaxSpellSpeed;
	float Diameter;
	float AutoAimTimeAux;
	class AEnemy* OwnerEnemy;
	
	FString FinalTypeOfDamage;
	
	void InitializeSpellRotation();

	void UpdateHitPoint();
};
