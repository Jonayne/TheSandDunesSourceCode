// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Spirit.generated.h"

UCLASS()
class THESANDDUNES_API ASpirit : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpirit();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
	float SpiritMaxBuffSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
	float SpiritWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
	bool bSpellsCanMoveActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
	float SpiritMass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
	float StunnedTimeIfNotMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float DodgeProbabilityIfState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	class UStatesComponent* SpiritStateComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	TMap<FString, float> SpiritResistances;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spells")
	class USpellsComponent* SpellComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
	float WaitBeforeDestroy;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Animation")
	class UAnimMontage* SpiritAnimationMontage;

	class UAIPerceptionStimuliSourceComponent* Stimulus;
	
	/* Esta variable es usada tanto por Diana como por los enemigos. Escencialmente sirve para determinar la rotacion
	   del enemigo al que se lanzara un hechizo. */
	UPROPERTY(BlueprintReadOnly, Category = "Focus Target")
	FVector TargetToHitLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Focus Target")
	ASpirit* TargetToHit;
	
	UPROPERTY(BlueprintReadOnly)
	bool bStunned;

	FTimerHandle DestroyActorTimerHandler;
	FTimerHandle StunnedTimerHandler;
	FTimerHandle InvincibleToStatesHandler;
	FTimerHandle InvincibleStateHandler;

	float DamageMultiplier;

	bool bInvincible;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Nos dice si la vida de este espirtu ya es 0. */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsDead(){ return Health <= 0; };

	/* Reduce la vida de este espiritu basado en las resistencias y el danio recibido. */
	UFUNCTION(BlueprintCallable)
	virtual float SpiritTakeDamage(float DamageAmount, FString TypeOfDamage, AActor* DamageCauser, FVector DamageLocation, bool bIsABeam);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, 
										class AController * EventInstigator, AActor * DamageCauser) override;

	float CalculateDamageRecieved(float DamageAmount, FString TypeOfDamage);
	
	UFUNCTION(BlueprintCallable)
	UAnimMontage* GetSpiritMontage();

	UFUNCTION(BlueprintImplementableEvent, Category="Damage")
	void ShowDamage(float DamageAmount, const FString& TypeOfSpell, FVector DamageLocation, bool bIsABeam);

	/* Reproduce el montage con la accion y velocidad especificadas en el montage especifico */
	UFUNCTION(BlueprintCallable)
	float PlayMontageByName(FName ActionName, UAnimMontage* AnimationMontage, float AnimationSpeed);

	/* Agrega el Spell de la clase especificada a su lista de hechizos. (en runtime) */
	UFUNCTION(BlueprintCallable)
	void AddSpell(FString NewSpellType, TSubclassOf<class ASpell> NewSpellClass);

	/* Agrega el Spell de la clase especificada a su lista de hechizos. (en runtime) */
	UFUNCTION(BlueprintCallable)
	virtual void ModifyHealth(float Heal);
	
	UFUNCTION(BlueprintCallable)
	int32 GetBaseDamage();
	
	UFUNCTION(BlueprintCallable)
	void SetBaseDamage(float NewBaseDamage);

	/* Invoca al spell con el nombre y en el socket especificado. */
	UFUNCTION(BlueprintCallable)
	void InvokeSpell(FString SpellType, FName SocketName);

	/* Invoca un spell aleatorio de la lista de hechizos de este espiritu. */
	UFUNCTION(BlueprintCallable)
	void InvokeRandomSpell(FName SocketName);

	/* Invoca el spell seleccionado actualmente. */
	UFUNCTION(BlueprintCallable)
	void InvokeCurrentSpell(FName SocketName);

	UFUNCTION(BlueprintCallable)
	virtual void ChangeCurrentSpell(FString SpellType);

	UFUNCTION(BlueprintCallable)
	void SetTargetToHitLocation(FVector TargetLocation);

	UFUNCTION(BlueprintCallable)
	void SetTargetToHit(ASpirit* NewTargetToHit);

	UFUNCTION(BlueprintCallable)
	FVector GetTargetToHitLocation();
	
	UFUNCTION(BlueprintCallable)
	ASpirit* GetTargetToHit();

	virtual float StunSpirit();

	virtual void StopStun();
	
	UFUNCTION(BlueprintCallable)
	float GetSpellCastPercentageCompleted();

	UFUNCTION(BlueprintCallable)
	void SetInvincibleMode(float Seconds);

	void StopInvincibleMode();

	// Nos dice si este Spirit esta en el aire o no.
	UFUNCTION()
	bool IsInAir();

	// Regresa la velocidad HORIZONTAL de este Spirit.
	UFUNCTION()
	float GetSpiritSpeed();

	// Aumenta la resistencia a un tipo de danio en un porcentaje.
	UFUNCTION(BlueprintCallable)
	virtual void ModifyResistance(FString StatName, float ResistanceToModify, bool bModifyPermanently);

	UFUNCTION(BlueprintCallable)
	TMap<FString, float> GetSpiritResistances();

	UFUNCTION(BlueprintCallable)
	void SetStunned(bool NewState);

	UFUNCTION(BlueprintCallable)
	void SetResistance(FString StatName, float NewResistance);

	// Nos da la resistencia a un tipo de danio especifico.
	UFUNCTION(BlueprintCallable)
	float GetResistStat(FString StatName);

	// Nos da el nivel de un hechizo a partir de su tipo.
	UFUNCTION(BlueprintCallable)
	int32 GetSpellLevelByType(FString SpellType);

	// Nos da el hechizo actualmente activo.
	UFUNCTION(BlueprintCallable)
	FString GetCurrentSpell();

	UFUNCTION(BlueprintCallable)
	void InvokeSecundarySpell();

	UFUNCTION(BlueprintCallable)
	void InvokeSpecificSecundarySpell(FString SpecificSpellType);

	UFUNCTION(BlueprintCallable)
	virtual void KillThisSpirit();

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth();

	UFUNCTION(BlueprintCallable)
	int32 GetMaxHealth();

	UFUNCTION(BlueprintCallable)
	int32 GetHealth();

	UFUNCTION(BlueprintCallable)
	void ChangeSpiritSpeed(float SpeedToChange);

	UFUNCTION(BlueprintCallable)
	void ChangeSpiritAcceleration(float AccelerationToChange);

	UFUNCTION(BlueprintCallable)
	void ChangeSpiritGravity(float NewGravityScale);

	UFUNCTION(BlueprintCallable)
	bool IsInState(FString StateName, bool bIsDebuf);

	UFUNCTION(BlueprintCallable)
	float SetSpiritState(FString StateName, bool bIsDebuf);

	float GetBuffCooldown(FString StateName);

	virtual void CallDestroyFunction();

};
