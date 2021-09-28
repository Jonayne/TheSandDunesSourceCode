// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Spirit.h"
#include "GenericTeamAgentInterface.h"
#include "SavableActorInterface.h"
#include "Enemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHasSeenAnEnemy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventStartFightEveryone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyDefeated, AEnemy*, EnemyRef);

/**
 * 
 */
UCLASS()
class THESANDDUNES_API AEnemy : public ASpirit, public IGenericTeamAgentInterface, public ISavableActorInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Savable Actor")
	FSavableActor SavableInfo;

	UPROPERTY(BlueprintReadOnly, Category = "IA Movement")
	FVector NextLocation;

	UPROPERTY(BlueprintReadOnly, Category = "IA Movement")
	FRotator NextRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Simple Enemy")
	int32 NumberOfAttacks;

	// Para los SimpleEnemies. Si se llena esta variable, no es necesaria "AttackToExecute".
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simple Enemy")
	FString ActionNameToExecute;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Simple Enemy")
	int32 AttackToExecute;

	UPROPERTY(BlueprintAssignable)
	FHasSeenAnEnemy OnSeenEnemy;

	UPROPERTY(BlueprintAssignable)
	FEventStartFightEveryone OnStartFightEveryone;

	UPROPERTY(BlueprintAssignable)
	FEnemyDefeated OnEnemyHasBeenDefeated;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Stats")
	class UAudioComponent* DeathEnemySound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Stats")
	class UAudioComponent* TPToDianaSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Stats")
	class UNiagaraComponent* TPEffect;

	// Si esta en 0, no hara TP nunca.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Stats")
	float SecondsForTPingToDiana;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Stats")
	bool bEnemyByEvent;
	
	// Solo puede ser necesario llenar la lista si EnemyByEvent es true.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Stats")
	TArray<AEnemy*> ListOfCompanions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Stats")
	TArray<AEnemy*> ListOfEnemiesThatSouldDieAfterMe;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Stats")
	bool bShouldHear;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Stats")
	bool bIsABoss;

	// si estamos listos para empezar una pelea. Se utiliza para peleas por eventos.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	bool bCanStartFight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IA Movement")
	bool bCanBeTeleportedByOthersEnemies;

	// esta variable solo importa cuando usamos el Task MoveEnemyToLocation.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="IA Movement")
	bool bMoveAroundMyself;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float DamageByTouch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	FString DamageByTouchNature;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	bool bShouldUseDamageByTouchNatureOnSpells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	bool bShouldDamageAllies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IA Movement")
	float RadiusToNextRandomLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IA Movement")
	float VerticalDistanceToKeep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IA Movement")
	FVector AddVectorToRandomLocation;

	UPROPERTY(BlueprintReadOnly, Category="Player Stats")
	class AEnemyController* EnemyController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Enemy Stats")
	class UBoxComponent* BoxVision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	class UWidgetComponent* HealthBarWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	class UWidgetComponent* SpecialHealthBarWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Stats")
	class UAudioComponent* EnemyMusic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
	FText EnemyName;

	bool bSeeingEnemy;
	bool bIsManuallyMovingToNextLocation;

	FTimerHandle ShowHealthBarTimer;
	FTimerHandle TPToDianaTimer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	FSavableActor GetSavableActorInfo_Implementation() const override;
	void ChangeActorState_Implementation() override;
	void UpdateSavableInfo_Implementation(const FSavableActor& NewActorInfo) override;

	virtual float SpiritTakeDamage(float DamageAmount, FString TypeOfDamage, AActor* DamageCauser, FVector DamageLocation, bool bIsABeam) override;
	
	virtual float AttackAnimation();

	virtual float TeleportAnimation();

	void TPToDiana();

	UFUNCTION(BlueprintCallable)
	bool Teleport();

	UFUNCTION(BlueprintCallable)
	void PrepareToFight();

	UFUNCTION()
	virtual void OnHaveSeenAnEnemy();

	UFUNCTION(BlueprintCallable)
	void SetNextLocation(FVector NextLocationToGo);

	// Para enemigos aereos. Toma como base RadiusToNextLocation y AddVector.
	UFUNCTION(BlueprintCallable)
	float MoveEnemyToRandomLocation(AActor* ActorToMoveAround);

	UFUNCTION(BlueprintCallable)
	void SetEnemyRandomLocation(AActor* ActorToMoveAround);

	// Para enemigos terrestres.
	UFUNCTION(BlueprintImplementableEvent, Category="Gameplay Logic")
	void SetRandomPointOnArea(FVector OriginPoint);

	virtual void ModifyHealth(float Heal) override;

	UFUNCTION(BlueprintCallable, Category="UI")
	void HideHealthBar();

	UFUNCTION(BlueprintCallable)
	void StartBossFightEvent();

	UFUNCTION(BlueprintCallable, Category="UI")
	void ShowHealthBar();

	UFUNCTION(BlueprintCallable, Category="UI")
	void ShowHealthBarNoTimer();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Gameplay Logic")
	void StartBossFight();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Gameplay Logic")
	void CallEveryoneToFight();

	virtual void KillThisSpirit() override;
	
	virtual void CallDestroyFunction() override;
	
	UFUNCTION()
	virtual void OnVisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void ShowSpecialHealthBar();
	void HideSpecialHealthBar();

protected:
	float MaxHealthAux;
	bool bExecutedOnce;
	float SpiritMassAux;
	
private:
	// Para los teams
	FGenericTeamId TeamId;
    virtual FGenericTeamId GetGenericTeamId() const override;
	
	void MoveThisEnemy(float DeltaTime);

	int32 GetAttackIndexByName();

};
