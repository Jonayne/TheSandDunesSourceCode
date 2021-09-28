// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.

#pragma once

#include "CoreMinimal.h"
#include "Spirit.h"
#include "GenericTeamAgentInterface.h"
#include "Diana.generated.h"

/**
 * Encapsula a la protagonista del videojuego, Diana.
 */
UCLASS()
class THESANDDUNES_API ADiana : public ASpirit, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	ADiana();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	class USpringArmComponent* CameraProxyArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category =Camera)
	float PercentageTowardsCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Camera)
	float MinCamaraZOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Camera)
	float MinCamaraLength;

	UPROPERTY(BlueprintReadOnly, Category="Player Controller")
	class ADianaPlayerController* DianaPlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spells")
	class USpellsComponent* StrongSpellComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack Mode")
	class UMaterial* NoFightEyesMat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack Mode")
	class UMaterial* NoFightBraceMat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack Mode")
	class UMaterial* FightEyesMat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack Mode")
	class UMaterial* FightBraceMat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack Mode")
	class UMaterial* FightEyesCorruptedMat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack Mode")
	class UMaterial* FightBraceCorruptedMat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack Mode")
	FColor CorruptedFightColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack Mode")
	FColor NonCorruptedFightColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack Mode")
	FColor CorruptedAttackMode1Color;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack Mode")
	FColor CorruptedAttackMode2Color;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack Mode")
	FColor NonCorruptedAttackMode1Color;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack Mode")
	FColor NonCorruptedAttackMode2Color;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack Mode")
	class UAudioComponent* StartAttackModeAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack Mode")
	class UNiagaraComponent* StartAttackModeParticle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack Mode")
	class UAudioComponent* StopAttackModeAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack Mode")
	class UPointLightComponent* FightLightComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Move Mode")
	class UAudioComponent* MovementModeAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Move Mode")
	class UAudioComponent* DeactivateMoveModeAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Move Mode")
	class UAudioComponent* EnableMoveModeAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Move Mode")
	class UNiagaraComponent* MovementModeNiagParticle;

	// Pa cuando ta corrupta la weona
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Move Mode")
	class UNiagaraComponent* MovementModeNiagParticle2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Move Mode")
	float MoveModeExtraSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Move Mode")
	float FOVToChange;

	UPROPERTY(BlueprintReadOnly, Category="Move Mode")
	bool bMoveMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Water Spell")
	class UNiagaraComponent* WaterSpellParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
	float BaseMaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
	float BaseMaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Stats")
	float StaminaRegenerationRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Camera)
	float RotationRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Stats")
	float TimeToWaitForNormalManaRegenerate;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Stats")
	float SecondJumpHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dash Specs")
	TMap<FString, class UNiagaraSystem*> DashEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dash Specs")
	class UAudioComponent* DashAudioComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Stats")
	class UAudioComponent* ShortJumpAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Stats")
	class UAudioComponent* LongJumpAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Shield")
	class UAudioComponent* ShieldActivateAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Shield")
	class UAudioComponent* ShieldHitAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Shield")
	class UNiagaraComponent* ShieldEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Shield")
	float ShieldCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Shield")
	float ShieldDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Shield")
	float ShieldStaminaCost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Footsteps")
	TMap<FString, class UNiagaraSystem*> FootstepsEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Footsteps")
	TMap<FString, class USoundBase*> FootstepsWalkAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Footsteps")
	TMap<FString, class USoundBase*> FootstepsRunAudio;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Footsteps")
 	class UCurveFloat* WalkCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Footsteps")
 	class UCurveFloat* RunCurve;

	UPROPERTY(BlueprintReadOnly, Category="Footsteps")
	class UNiagaraComponent* FootstepsEffectToSpawnLeft;

	UPROPERTY(BlueprintReadOnly, Category="Footsteps")
	class UNiagaraComponent* FootstepsEffectToSpawnRight;

	UPROPERTY(BlueprintReadOnly, Category="Footsteps")
	class UAudioComponent* FootstepsWalkAudioToSpawnRight;

	UPROPERTY(BlueprintReadOnly, Category="Footsteps")
	class UAudioComponent* FootstepsWalkAudioToSpawnLeft;

	UPROPERTY(BlueprintReadOnly, Category="Footsteps")
	class UAudioComponent* FootstepsRunAudioToSpawnRight;

	UPROPERTY(BlueprintReadOnly, Category="Footsteps")
	class UAudioComponent* FootstepsRunAudioToSpawnLeft;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Camera)
	class UArrowComponent* ForwardArrow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Attack Mode")
	TMap<FString, FColor> SpellColors;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spell")
	class UNiagaraComponent* ChangeSpellEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Spell")
	class UAudioComponent* ChangeSpellAudio;

	// Para controlar el hecho de no poder saltar durante un dash.
	UPROPERTY(BlueprintReadWrite, Category="Gameplay Logic")
	bool bCanJump; 

	UPROPERTY(BlueprintReadWrite, Category="Gameplay Logic")
	bool bInBossFight; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dash Specs")
	float DashCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dash Specs")
	float DashDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dash Specs")
	float DashDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dash Specs")
	float DashStaminaCost;

	UPROPERTY(BlueprintReadOnly, Category="Dash Specs")
	FString DashType;

	UPROPERTY(BlueprintReadOnly)
	bool bAttackMode;

	UPROPERTY(BlueprintReadOnly)
	bool bNotInCombat;

	UPROPERTY(BlueprintReadOnly)
	int32 RemainingSaves;

	UPROPERTY(BlueprintReadOnly)
	int32 CorruptionLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Animation")
	class UAnimMontage* DianaFightMontage;

	UPROPERTY(BlueprintReadOnly, Category="Spells")
	int32 CurrentSpellIndex;
	
	FTimerHandle DashCooldownTimerHandle;
	FTimerHandle ShieldCooldownTimerHandle;

	FTimerHandle WeakAttackShootingTimerHandle;
	FTimerHandle StrongAttackShootingTimerHandle;
	FTimerHandle SlowStaminaRecoverTimer;
	
	FTimerHandle RefreshManaTimerHandle;
	FTimerHandle RefreshDashTimerHandle;
	FTimerHandle RefreshShieldTimerHandle;

	FTimerHandle RefreshAbilitiesTimer;

	FTimerHandle ShieldDidnHitHandler;

	FTimerHandle AttackModeHandler;

	FTimerHandle MoveModeHandler;
	FTimerHandle MoveModeFOVHandler;

	FTimerHandle ReceivingDamageHandler;

	FTimerHandle SuperDashDianaHandle;

	bool bIsTPing;
	bool bFastStaminaRegeneration;
	bool bCanDash;
	bool bCanShield;
	bool bShieldFault;

	float LifeStealPercentage;
	bool bNoLimitDash;
	bool bSlippingDiana;

	float MaxTargetArmLength;
	float MaxArmOffSet;

	float BaseMaxDamage;

	float ExtraHealth;
	
	// para cambios de levels.
	FVector DianaNextTPLocation;
	FRotator DianaNextTPRotation;
	FString DianaNextMap;

	TMap<FString, float> DianaBaseResistances;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Landed(const FHitResult& Hit) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent, Category="Gameplay Logic")
	void Dash(float DashDistanceParam, float DashDurationParam, float DashCooldownParam);

	void Shield();

	void EnableShield();
	
	void TurnOnAttackMode();
	void TurnOffAttackMode();

	virtual void ModifyResistance(FString StatName, float ResistanceToModify, bool bModifyPermanently) override;

	virtual void ChangeCurrentSpell(FString SpellType) override;

	virtual float SpiritTakeDamage(float DamageAmount, FString TypeOfDamage, AActor* DamageCauser, FVector DamageLocation, bool bIsABeam) override;

	virtual void ModifyHealth(float Heal) override;

	virtual float StunSpirit() override;
	
	virtual void StopStun() override;

	virtual void KillThisSpirit() override;

	UFUNCTION(BlueprintCallable)
	void HealDianaDamagePercentage(float DamageDone);

	UFUNCTION(BlueprintCallable)
	void EnableDash();

	UFUNCTION(BlueprintCallable)
	void IncrementSavings();

	UFUNCTION(BlueprintCallable)
	void DecrementSavings();

	UFUNCTION(BlueprintCallable)
	void CorruptDiana();

	UFUNCTION(BlueprintCallable)
	void DescorruptDiana();

	UFUNCTION(BlueprintCallable)
	void ReenableJump();

	UFUNCTION(BlueprintCallable)
	void SetDianaResistances(TMap<FString, float> NewResistances);

	UFUNCTION(BlueprintCallable)
	FString GetCurrentDashType();

	UFUNCTION(BlueprintCallable)
	float GetCurrentStamina();

	UFUNCTION(BlueprintCallable)
	float GetCurrentDashCooldown();

	UFUNCTION(BlueprintCallable)
	float GetCurrentShieldCooldown();

	UFUNCTION(BlueprintCallable)
	void SetRotationRate(float NewRotationRate);

	UFUNCTION(BlueprintCallable)
	float GetCurrentSecundarySpellCooldownPercentage();

	UFUNCTION(BlueprintCallable)
	float GetSpellActiveCooldownPercentage(FString SpellType);

	UFUNCTION(BlueprintCallable)
	float GetSpellSecundaryCooldownPercentage(FString SpellType);

	UFUNCTION(BlueprintCallable)
	float GetSpellPrimaryCooldownPercentage(FString SpellType);

	UFUNCTION(BlueprintCallable)
	float GetCurrentStrongSpellCast();

	UFUNCTION(BlueprintCallable)
	int32 GetStamina();

	UFUNCTION(BlueprintCallable)
	int32 GetMaxStamina();

	UFUNCTION(BlueprintCallable)
	void SetNewBaseDamage(float ValueToAdd);

	UFUNCTION(BlueprintCallable)
	void SetNewMaxHealth(float ValueToAdd);

	UFUNCTION(BlueprintCallable)
	void SetNewMaxStamina(float ValueToAdd);

	UFUNCTION(BlueprintCallable)
	void SetNewExactBaseDamage(float Value);

	UFUNCTION(BlueprintCallable)
	void SetNewExactMaxHealth(float Value);

	UFUNCTION(BlueprintCallable)
	void SetNewExactMaxStamina(float Value);

	UFUNCTION(BlueprintCallable)
	void ModifyStamina(float StaminaToModify);

	UFUNCTION(BlueprintCallable)
	void AddHeightToSecondJump(float ExtraJump);

	UFUNCTION(BlueprintCallable)
	TArray<FString> GetListOfSpellsNames();

	UFUNCTION(BlueprintCallable)
	TMap<FString, FText> GetSpellDescription(FString SpellType);

	UFUNCTION(BlueprintCallable)
	void InvokeCurrentStrongSpell(FName SocketName);
	
	UFUNCTION(BlueprintCallable)
	int32 GetSpellTypeLevel(FString SpellType);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Update Diana State")
	void UpdateDianaFOV();

	UFUNCTION(BlueprintCallable, Category="Water Spell")
	void MakeSpecialDash(float Duration, float Distance);

	void StopSpecialDash();

	UFUNCTION(BlueprintImplementableEvent, Category="Interact")
	void CheckDianaInteractions();

	void PlayFootstepsAudioAndEffects(FString MaterialName, FName FootSocket);

	UFUNCTION(BlueprintCallable)
	void SetNextMapTransform(FVector NextLocation, FRotator NextRotation, FString NextMap);

	UFUNCTION(BlueprintCallable)
	void SetNewState(FString NewState, TSubclassOf<ASpiritState> NewStateClass);

	UFUNCTION(BlueprintCallable)
	void ResetDianaGravity();

	UFUNCTION(BlueprintCallable)
	bool IsCorrupted();

	UFUNCTION(BlueprintCallable)
	TSubclassOf<class ASpiritState> GetStateClass(FString StateName);

	UFUNCTION(BlueprintCallable)
	void SetWeakSpellList(TMap<FString, TSubclassOf<ASpell>> NewSpellList);

	UFUNCTION(BlueprintCallable)
	void SetStrongSpellList(TMap<FString, TSubclassOf<ASpell>> NewSpellList);

	void UpdateDianaTargetToHit(float Range);

	void UpdateMaxHealth();

	void UpdateMaxStamina();

	void ChangeSpellEffects(FString SpellType);
	
	void ShowDamageReceivedOverlay();

	void HideDamageReceivedOverlay();

	TMap<FString, TSubclassOf<class ASpell>> GetWeakSpellsList();

	TMap<FString, TSubclassOf<ASpell>> GetStrongSpellsList();

	void SetStatesList(TMap<FString, TSubclassOf<class ASpiritState>> NewStateList);

	void AddSpellToWeakComponentList(FString NewSpellType, TSubclassOf<ASpell> NewSpellClass);

	void AddSpellToStrongComponentList(FString NewSpellType, TSubclassOf<ASpell> NewSpellClass);

	TMap<FString, TSubclassOf<class ASpiritState>> GetDianaStatesList();

	void AdjustCorruptedVisuals();

private:
	// Para los teams
	FGenericTeamId TeamId;
    virtual FGenericTeamId GetGenericTeamId() const override;

	int32 JumpCounts;
	float CamaraProxyAux;
	bool bDamageOverlayActivated;
	float NextFOVAux;
	bool bHasAlreadyDied;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUpRate(float Value);
	void LookRightRate(float Value);
	void DashPressed();
	void DecreaseWalkSpeed();
	void DianaStartJump();
	void DianaStopJump();
	void StartWeakShooting();
	void StartStrongShooting();
	void InvokeWeakShoot();
	void StopWeakShooting();
	void InvokeStrongShoot();
	void StopStrongShooting();

	void UpdateSpellsCDTimers();

	void ShieldDidntHit();

	void ChangeToSpellUp();
	void ChangeToSpellDown();

	void ChangeToSpell1();
	void ChangeToSpell2();
	void ChangeToSpell3();
	void ChangeToSpell4();
	void ChangeToSpell5();
	void ChangeToSpell6();

	void TurnOnMoveMode();
	void TurnOffMoveMode();
	void EnableMovementMode();
	void ToggleMovementMode();

	void ChangeDianaFOV(float FOVToModify);
	void ModifyActualFOV();

	void UseSecundarySpell();

	bool HaveEnoughStamina(float StaminaCost);
	void EnableStaminaRegeneration();
	bool WasteStamina(float StaminaToUse);

	void UpdateMana();
	void UpdateManaNums(float DeltaTime);

	void PauseMenu();

	void StatsMenu();
	
	void UpdateCameraPosition(float DeltaTime);
};
