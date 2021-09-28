// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "Diana.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "TimerManager.h"
#include "DianaPlayerController.h"
#include "SpellsComponent.h"
#include "Curves/CurveFloat.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Spell.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/PointLightComponent.h"
#include "StatesComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SpiritState.h"


ADiana::ADiana() 
{
    CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraArm->SetupAttachment(GetRootComponent());
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->TargetArmLength = 600.f;
	CameraArm->TargetOffset = FVector(0.f, 0.f, 330.f);
	CameraArm->bDoCollisionTest = false;
	CameraArm->bEnableCameraLag = true;
	CameraArm->bEnableCameraRotationLag = true;
	CameraArm->CameraRotationLagSpeed = 50.f;

	CameraProxyArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraProxyArm"));
	CameraProxyArm->SetupAttachment(GetRootComponent());
	CameraProxyArm->bUsePawnControlRotation = true;
	CameraProxyArm->TargetArmLength = 600.f;
	CameraProxyArm->TargetOffset = FVector(0.f, 0.f, 330.f);
	CameraProxyArm->bDoCollisionTest = true;
	CameraProxyArm->bEnableCameraLag = false;
	CameraProxyArm->bEnableCameraRotationLag = false;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	ForwardArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ForwardArrow"));
	ForwardArrow->SetupAttachment(GetMesh());

	StrongSpellComponent = CreateDefaultSubobject<USpellsComponent>(FName("StrongSpellComponent"));
	StrongSpellComponent->bAutoActivate = true;

	ShortJumpAudio = CreateDefaultSubobject<UAudioComponent>(FName("ShortJumpAudio"));
	ShortJumpAudio->SetupAttachment(GetMesh());
	ShortJumpAudio->bAutoActivate = false;

	LongJumpAudio = CreateDefaultSubobject<UAudioComponent>(FName("LongJumpAudio"));
	LongJumpAudio->SetupAttachment(GetMesh());
	LongJumpAudio->bAutoActivate = false;

	ShieldActivateAudio = CreateDefaultSubobject<UAudioComponent>(FName("ShieldActivateAudio"));
	ShieldActivateAudio->SetupAttachment(GetMesh());
	ShieldActivateAudio->bAutoActivate = false;

	ShieldHitAudio = CreateDefaultSubobject<UAudioComponent>(FName("ShieldHitAudio"));
	ShieldHitAudio->SetupAttachment(GetMesh());
	ShieldHitAudio->bAutoActivate = false;
	
	ShieldEffect = CreateDefaultSubobject<UNiagaraComponent>(FName("ShieldEffect"));
	ShieldEffect->SetupAttachment(GetMesh());
	ShieldEffect->bAutoActivate = false;
	
	WaterSpellParticle = CreateDefaultSubobject<UNiagaraComponent>(FName("WaterSpellParticle"));
	WaterSpellParticle->SetupAttachment(GetMesh());
	WaterSpellParticle->bAutoActivate = false;

	MovementModeAudio = CreateDefaultSubobject<UAudioComponent>(FName("MovementModeAudio"));
	MovementModeAudio->SetupAttachment(GetMesh());
	MovementModeAudio->bAutoActivate = false;

	EnableMoveModeAudio = CreateDefaultSubobject<UAudioComponent>(FName("EnableMoveModeAudio"));
	EnableMoveModeAudio->SetupAttachment(GetMesh());
	EnableMoveModeAudio->bAutoActivate = false;

	DeactivateMoveModeAudio = CreateDefaultSubobject<UAudioComponent>(FName("DeactivateMoveModeAudio"));
	DeactivateMoveModeAudio->SetupAttachment(GetMesh());
	DeactivateMoveModeAudio->bAutoActivate = false;

	MovementModeNiagParticle = CreateDefaultSubobject<UNiagaraComponent>(FName("MovementModeNiagParticle"));
	MovementModeNiagParticle->SetupAttachment(GetMesh());
	MovementModeNiagParticle->bAutoActivate = false;

	MovementModeNiagParticle2 = CreateDefaultSubobject<UNiagaraComponent>(FName("MovementModeNiagParticle2"));
	MovementModeNiagParticle2->SetupAttachment(GetMesh());
	MovementModeNiagParticle2->bAutoActivate = false;

	StartAttackModeParticle = CreateDefaultSubobject<UNiagaraComponent>(FName("StartAttackModeParticle"));
	StartAttackModeParticle->SetupAttachment(GetMesh(), FName("SocketCast"));
	StartAttackModeParticle->bAutoActivate = false;

	FootstepsEffectToSpawnLeft = CreateDefaultSubobject<UNiagaraComponent>(FName("FootstepsLeftParticles"));
	FootstepsEffectToSpawnLeft->SetupAttachment(GetMesh(), FName("LeftFootSocket"));
	FootstepsEffectToSpawnLeft->bAutoActivate = false;

	FootstepsEffectToSpawnRight = CreateDefaultSubobject<UNiagaraComponent>(FName("FootstepsRightParticles"));
	FootstepsEffectToSpawnRight->SetupAttachment(GetMesh(), FName("RightFootSocket"));
	FootstepsEffectToSpawnRight->bAutoActivate = false;

	FootstepsWalkAudioToSpawnLeft = CreateDefaultSubobject<UAudioComponent>(FName("FootstepsLeftAudio"));
	FootstepsWalkAudioToSpawnLeft->SetupAttachment(GetMesh());
	FootstepsWalkAudioToSpawnLeft->bAutoActivate = false;

	FootstepsWalkAudioToSpawnRight = CreateDefaultSubobject<UAudioComponent>(FName("FootstepsRightAudio"));
	FootstepsWalkAudioToSpawnRight->SetupAttachment(GetMesh());
	FootstepsWalkAudioToSpawnRight->bAutoActivate = false;

	FootstepsRunAudioToSpawnRight = CreateDefaultSubobject<UAudioComponent>(FName("FootstepsRunRightAudio"));
	FootstepsRunAudioToSpawnRight->SetupAttachment(GetMesh());
	FootstepsRunAudioToSpawnRight->bAutoActivate = false;

	FootstepsRunAudioToSpawnLeft = CreateDefaultSubobject<UAudioComponent>(FName("FootstepsRunLeftAudio"));
	FootstepsRunAudioToSpawnLeft->SetupAttachment(GetMesh());
	FootstepsRunAudioToSpawnLeft->bAutoActivate = false;

	DashAudioComponent = CreateDefaultSubobject<UAudioComponent>(FName("DashAudioComponent"));
	DashAudioComponent->SetupAttachment(GetMesh());
	DashAudioComponent->bAutoActivate = false;

	ChangeSpellEffect = CreateDefaultSubobject<UNiagaraComponent>(FName("ChangeSpellEffect"));
	ChangeSpellEffect->SetupAttachment(GetMesh());
	ChangeSpellEffect->bAutoActivate = false;

	ChangeSpellAudio = CreateDefaultSubobject<UAudioComponent>(FName("ChangeSpellAudio"));
	ChangeSpellAudio->SetupAttachment(GetMesh());
	ChangeSpellAudio->bAutoActivate = false;

	NoFightEyesMat = CreateDefaultSubobject<UMaterial>(FName("NoFightEyesMat"));
	NoFightBraceMat = CreateDefaultSubobject<UMaterial>(FName("NoFightBraceMat"));
	FightEyesMat = CreateDefaultSubobject<UMaterial>(FName("FightEyesMat"));
	FightBraceMat = CreateDefaultSubobject<UMaterial>(FName("FightBraceMat"));
	FightEyesCorruptedMat = CreateDefaultSubobject<UMaterial>(FName("FightEyesCorruptedMat"));
	FightBraceCorruptedMat = CreateDefaultSubobject<UMaterial>(FName("FightBraceCorruptedMat"));

	StartAttackModeAudio = CreateDefaultSubobject<UAudioComponent>(FName("StartAttackModeAudio"));
	StartAttackModeAudio->SetupAttachment(GetMesh());
	StartAttackModeAudio->bAutoActivate = false;

	StopAttackModeAudio = CreateDefaultSubobject<UAudioComponent>(FName("StopAttackModeAudio"));
	StopAttackModeAudio->SetupAttachment(GetMesh());
	StopAttackModeAudio->bAutoActivate = false;

	FightLightComponent = CreateDefaultSubobject<UPointLightComponent>(FName("FightLightComponent"));
	FightLightComponent->SetupAttachment(GetMesh(), FName("LightSocket"));
	FightLightComponent->SetVisibility(false);
	FightLightComponent->SetCastShadows(false);
	FightLightComponent->SetMobility(EComponentMobility::Movable);

    RotationRate = 60.f;

    bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true; // mueve en la direccion de input segun la camara
	GetCharacterMovement()->MaxWalkSpeed = 1200.f;

	MinCamaraZOffset = 150.f;
	MinCamaraLength = 100.f;

	BaseMaxStamina = 500.f;
	BaseMaxHealth = 1000.f;
    MaxStamina = 100.f;
	Stamina = MaxStamina;
	StaminaRegenerationRate = 5.f;
	bCanJump = true;
	DashCooldown = 3.f;
	DashDuration = 0.3f;
	DashDistance = 2500.f;
	DashType = TEXT("Default");
	bCanDash = true;
	SecondJumpHeight = 500.f;
	SpiritWalkSpeed = 1200.f;

	bFastStaminaRegeneration = true;
	TimeToWaitForNormalManaRegenerate = .8f;
	DashStaminaCost = 5.f;

	bCanShield = true;
	bShieldFault = false;

	ShieldCooldown = 5.f;
	ShieldDuration = 0.2f;
	ShieldStaminaCost = 100.f;
	bDamageOverlayActivated = false;
	bInBossFight = false;

	SpellColors.Add(TEXT("Arcanos"), FColor::FColor());
	SpellColors.Add(TEXT("Fuego"), FColor::FColor());
	SpellColors.Add(TEXT("Agua"), FColor::FColor());
	SpellColors.Add(TEXT("Rayos"), FColor::FColor());
	SpellColors.Add(TEXT("Sagrado"), FColor::FColor());
	SpellColors.Add(TEXT("Sombras"), FColor::FColor());
	SpellColors.Add(TEXT("Naturaleza"), FColor::FColor());

    TeamId = FGenericTeamId(0);
	Tags.Add(FName("Diana"));
}

void ADiana::BeginPlay() 
{
    Super::BeginPlay();

	MaxTargetArmLength = CameraArm->TargetArmLength;
	MaxArmOffSet = CameraArm->TargetOffset.Z;

	AdjustCorruptedVisuals();

	if(!DianaPlayerController) DianaPlayerController = Cast<ADianaPlayerController>(GetController());

	StrongSpellComponent->SetSpellComponentAsStrong();
	SpellComponent->SetSpellComponentAsWeak();
	
	if(!GetWorldTimerManager().IsTimerActive(RefreshManaTimerHandle)) 
	{
		GetWorldTimerManager().SetTimer(RefreshManaTimerHandle, this, &ADiana::UpdateMana, 0.05f, true);
	}

	bNotInCombat = true;
	if(DianaPlayerController) DianaPlayerController->ShowDianaMovementModeIndicator();

	UpdateDianaFOV();
}

void ADiana::Landed(const FHitResult& Hit) 
{
	Super::Landed(Hit);

	JumpCounts = 0;
}

void ADiana::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);
	
	UpdateCameraPosition(DeltaTime);
	UpdateManaNums(DeltaTime);
}

void ADiana::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) 
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ADiana::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ADiana::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ADiana::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &ADiana::LookRightRate);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ADiana::DianaStartJump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ADiana::DianaStopJump);

	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &ADiana::DashPressed);

	PlayerInputComponent->BindAction(TEXT("Shield"), EInputEvent::IE_Pressed, this, &ADiana::Shield);
	
	PlayerInputComponent->BindAction(TEXT("MovementMode"), EInputEvent::IE_Pressed, this, &ADiana::ToggleMovementMode);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ADiana::StartWeakShooting);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Released, this, &ADiana::StopWeakShooting);

	PlayerInputComponent->BindAction(TEXT("StrongAttack"), EInputEvent::IE_Pressed, this, &ADiana::StartStrongShooting);
	PlayerInputComponent->BindAction(TEXT("StrongAttack"), EInputEvent::IE_Released, this, &ADiana::StopStrongShooting);

	PlayerInputComponent->BindAction(TEXT("SecundarySpell"), EInputEvent::IE_Pressed, this, &ADiana::UseSecundarySpell);

	PlayerInputComponent->BindAction(TEXT("Spell1"), EInputEvent::IE_Pressed, this, &ADiana::ChangeToSpell1);
	PlayerInputComponent->BindAction(TEXT("Spell2"), EInputEvent::IE_Pressed, this, &ADiana::ChangeToSpell2);
	PlayerInputComponent->BindAction(TEXT("Spell3"), EInputEvent::IE_Pressed, this, &ADiana::ChangeToSpell3);
	PlayerInputComponent->BindAction(TEXT("Spell4"), EInputEvent::IE_Pressed, this, &ADiana::ChangeToSpell4);
	PlayerInputComponent->BindAction(TEXT("Spell5"), EInputEvent::IE_Pressed, this, &ADiana::ChangeToSpell5);
	PlayerInputComponent->BindAction(TEXT("Spell6"), EInputEvent::IE_Pressed, this, &ADiana::ChangeToSpell6);

	PlayerInputComponent->BindAction(TEXT("ChangeSpellUp"), EInputEvent::IE_Pressed, this, &ADiana::ChangeToSpellDown);
	PlayerInputComponent->BindAction(TEXT("ChangeSpellDown"), EInputEvent::IE_Pressed, this, &ADiana::ChangeToSpellUp);

	PlayerInputComponent->BindAction(TEXT("PauseMenu"), EInputEvent::IE_Pressed, this, &ADiana::PauseMenu).bExecuteWhenPaused = true;
	
	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &ADiana::CheckDianaInteractions);

	PlayerInputComponent->BindAction(TEXT("StatsMenu"), EInputEvent::IE_Pressed, this, &ADiana::StatsMenu).bExecuteWhenPaused = true;
}

void ADiana::Shield() 
{
	if(bCanShield && WasteStamina(ShieldStaminaCost))
	{
		bCanShield = false;
		SetInvincibleMode(ShieldDuration);

		if(ShieldEffect)
		{
			ShieldEffect->SetVariableFloat(FName("Duration"), ShieldDuration+.5f);
			ShieldEffect->Activate(true);
		}
		
		if(ShieldActivateAudio) ShieldActivateAudio->Activate(true);

		GetWorld()->GetTimerManager().SetTimer(ShieldDidnHitHandler, this, &ADiana::ShieldDidntHit, ShieldDuration+.4f, false);
		GetWorld()->GetTimerManager().SetTimer(ShieldCooldownTimerHandle, this, &ADiana::EnableShield, ShieldCooldown, false);

		if(DianaPlayerController)
		{
			GetWorldTimerManager().SetTimer(RefreshShieldTimerHandle, DianaPlayerController, &ADianaPlayerController::UpdateDianaShieldBar, 0.1f, true);
		}
		GetWorldTimerManager().SetTimer(SlowStaminaRecoverTimer, this, &ADiana::EnableStaminaRegeneration, TimeToWaitForNormalManaRegenerate, false);
	}
}

void ADiana::ChangeCurrentSpell(FString SpellType) 
{
	Super::ChangeCurrentSpell(SpellType);

	if(SpellComponent->ValidSpell(SpellType))
	{
		if(StrongSpellComponent->ValidSpell(SpellType))
		{
			StrongSpellComponent->SetCurrentSpellByType(SpellType);
		}

		UpdateSpellsCDTimers();

		if(!DashType.Equals(SpellType))
		{
			ChangeSpellEffects(SpellType);
			DashType = SpellType;
		}

	}
	else
	{
		DashType = TEXT("Default");
	}
}

float ADiana::SpiritTakeDamage(float DamageAmount, FString TypeOfDamage, AActor* DamageCauser, FVector DamageLocation, bool bIsABeam) 
{
	float Damage = Super::SpiritTakeDamage(DamageAmount, TypeOfDamage, DamageCauser, DamageLocation, bIsABeam);

	// Si teniamos el escudo activado y esquivamos un ataque
	if(bInvincible && !bCanShield && Damage == 0.f && !bShieldFault)
	{
		GetWorldTimerManager().ClearTimer(ShieldDidnHitHandler);
		GetWorldTimerManager().ClearTimer(DashCooldownTimerHandle);
		GetWorldTimerManager().ClearTimer(ShieldCooldownTimerHandle);
		EnableDash();
		EnableShield();

		if(ShieldHitAudio) ShieldHitAudio->Activate(true);

		if(bIsABeam)
		{
			ModifyHealth(MaxHealth*.009f);
			ModifyStamina(1.05f*ShieldStaminaCost);
		}
		else
		{
			ModifyHealth(MaxHealth*.07f);
			ModifyStamina(1.5f*ShieldStaminaCost);
		}

		return Damage;
	}
	else if(bInvincible && !bCanDash && Damage == 0.f)
	{
		GetWorldTimerManager().ClearTimer(DashCooldownTimerHandle);
		EnableDash();

		return Damage;
	}

	if(bNotInCombat && FMath::IsWithinInclusive(CorruptionLevel, -2, 0))
	{
		if(DianaPlayerController) DianaPlayerController->HideDianaMovementModeIndicator();
		TurnOffMoveMode();
		bNotInCombat = false;
		GetWorldTimerManager().SetTimer(MoveModeHandler, this, &ADiana::EnableMovementMode, 5.f, false);
	}

	ShowDamageReceivedOverlay();
	if(DianaPlayerController) DianaPlayerController->UpdateDianaHealthBar();
	
	return Damage;
}

void ADiana::ModifyHealth(float Heal) 
{
	float PrevHealth = Health;
	Super::ModifyHealth(Heal);

	// Si tras el ModifyHealth nos quitaron vida.
	if(PrevHealth > Health)
	{
		ShowDamageReceivedOverlay();
	}

	if(CorruptionLevel > 0)
	{
		float NextMaxHealth = MaxHealth * (1.f - (CorruptionLevel * .20f));
		if(Health > NextMaxHealth)
		{
			Health = NextMaxHealth;
		}
	}

	if(DianaPlayerController) DianaPlayerController->UpdateDianaHealthBar();
}

float ADiana::StunSpirit() 
{
	float StunTime = Super::StunSpirit();

	if(DianaPlayerController)
	{
		DianaPlayerController->SetIgnoreMoveInput(true);
	} 
	
	return StunTime;
}

void ADiana::StopStun() 
{
	Super::StopStun();

	if(DianaPlayerController)
	{
		DianaPlayerController->SetIgnoreMoveInput(false);
	} 
}

void ADiana::KillThisSpirit() 
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
	Health = 0.f;
	Stamina = 0.f;
	StaminaRegenerationRate = 0.f;

	if(DianaPlayerController && !bHasAlreadyDied)
	{
		TArray<AActor*> DianaStatesRef;
		UGameplayStatics::GetAllActorsOfClass(this, ASpiritState::StaticClass(), DianaStatesRef);
		for(auto& StatesRef : DianaStatesRef)
		{
			StatesRef->Destroy();
		}

		DianaPlayerController->ShowDeathMenu();
		DianaPlayerController->UpdateDianaHealthBar();
		bHasAlreadyDied = true;
	}

	Camera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	GetMesh()->bPauseAnims = true;
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADiana::HealDianaDamagePercentage(float DamageDone) 
{
	if(LifeStealPercentage)
	{
		ModifyHealth(DamageDone*LifeStealPercentage);
	}
}

void ADiana::SetDianaResistances(TMap<FString, float> NewResistances) 
{
	DianaBaseResistances = NewResistances;
	SpiritResistances = NewResistances;
}

void ADiana::MoveForward(float Value) 
{
    if (Controller && Value != 0)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ADiana::MoveRight(float Value) 
{
    if (Controller && Value != 0)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ADiana::UpdateCameraPosition(float DeltaTime) 
{
	FTransform CameraTranform = CameraProxyArm->GetSocketTransform(FName(), ERelativeTransformSpace::RTS_Component);
	float CameraProxyLength = CameraTranform.GetLocation().Size();
	
	// Si estamos a 300 unidades de colisionar.
	if(CameraProxyLength <= 335.f)
	{	
		CamaraProxyAux = FMath::FInterpTo(CamaraProxyAux, MinCamaraLength, DeltaTime, 4.5f);
	}
	else
	{
		CamaraProxyAux = CameraProxyLength;
	}

	// Actualizamos el tamanio del Arm Length.
	float NextArmLength = FMath::FInterpTo(CameraArm->TargetArmLength, CamaraProxyAux, DeltaTime, 2.f);
	CameraArm->TargetArmLength = NextArmLength;

	// Actualizamos el Socket Offset para acercanos a nuestro pj hacia abajo.
	float NextSocketOffset = (NextArmLength * MaxArmOffSet) / MaxTargetArmLength;
	NextSocketOffset = FMath::Max(NextSocketOffset, MinCamaraZOffset);
	float NextSmoothOffset = FMath::FInterpTo(CameraArm->TargetOffset.Z, NextSocketOffset, DeltaTime, 3.f);
	
	CameraArm->TargetOffset.Z = NextSmoothOffset;
}

void ADiana::LookUpRate(float Value) 
{
    AddControllerPitchInput(Value * RotationRate * GetWorld()->GetDeltaSeconds());
}

void ADiana::LookRightRate(float Value) 
{
    AddControllerYawInput(Value * RotationRate * GetWorld()->GetDeltaSeconds());
}

void ADiana::DashPressed() 
{
	if(!bStunned && bCanDash && (bNoLimitDash || WasteStamina(DashStaminaCost)))
	{
		bCanDash = false;
		if(DashAudioComponent) DashAudioComponent->Activate(true);

		Dash(DashDistance, DashDuration, DashCooldown);
		SetInvincibleMode(.3f);
		GetWorldTimerManager().SetTimer(DashCooldownTimerHandle, this, &ADiana::EnableDash, DashCooldown, false);
		
		if(DianaPlayerController)
		{
			GetWorldTimerManager().SetTimer(RefreshDashTimerHandle, DianaPlayerController, &ADianaPlayerController::UpdateDianaDashBar, 0.1f, true);
		}
		GetWorldTimerManager().SetTimer(SlowStaminaRecoverTimer, this, &ADiana::EnableStaminaRegeneration, TimeToWaitForNormalManaRegenerate, false);
	}
}

void ADiana::DianaStartJump() 
{
	if(!bStunned && (bCanJump || JumpCounts == 2)){
		if(JumpCounts == 0)
		{	
			Jump();
			JumpCounts = 1;
			if(ShortJumpAudio) ShortJumpAudio->Activate(true);
		}
		else if (JumpCounts == 1)
		{
			ACharacter::LaunchCharacter(FVector(0.f, 0.f, SecondJumpHeight), false, true);
			JumpCounts = 2;
			if(LongJumpAudio) LongJumpAudio->Activate(true);
			PlayFootstepsAudioAndEffects(TEXT("Special"), FName("LeftFoot"));
			PlayFootstepsAudioAndEffects(TEXT("Special"), FName("RightFoot"));
		}
	} 
}

void ADiana::DianaStopJump() 
{
	StopJumping();
}

void ADiana::StartWeakShooting() 
{
	if(!bStunned)
	{
		InvokeWeakShoot();
		float TimeBetweenShots = SpellComponent->GetSpellCD();
		GetWorldTimerManager().SetTimer(WeakAttackShootingTimerHandle, this, &ADiana::InvokeWeakShoot, TimeBetweenShots, true);
	}
}

void ADiana::StartStrongShooting() 
{
	if(!bStunned)
	{
		InvokeStrongShoot();
		float TimeBetweenShots = StrongSpellComponent->GetSpellCD();
		GetWorldTimerManager().SetTimer(StrongAttackShootingTimerHandle, this, &ADiana::InvokeStrongShoot, TimeBetweenShots, true);
	}
}

void ADiana::InvokeWeakShoot() 
{	
	if(SpellComponent->CanShotSpell() && WasteStamina(SpellComponent->GetCurrentSpellStaminaCost()))
	{	
		TurnOnAttackMode();
		InvokeCurrentSpell(FName("SocketSpells"));	
	}
}

void ADiana::InvokeStrongShoot() 
{
	if(StrongSpellComponent->CanShotSpell() && WasteStamina(StrongSpellComponent->GetCurrentSpellStaminaCost()))
	{	
		TurnOnAttackMode();
		InvokeCurrentStrongSpell(FName("SocketSpells"));
	}
}

void ADiana::StopStrongShooting() 
{
	GetWorldTimerManager().ClearTimer(StrongAttackShootingTimerHandle);
	GetWorldTimerManager().SetTimer(SlowStaminaRecoverTimer, this, &ADiana::EnableStaminaRegeneration, TimeToWaitForNormalManaRegenerate, false);
}

void ADiana::UpdateSpellsCDTimers() 
{
	// Actualizamos el CD de las habilidades.
	if(DianaPlayerController && !GetWorldTimerManager().IsTimerActive(RefreshAbilitiesTimer))
	{
		GetWorldTimerManager().SetTimer(RefreshAbilitiesTimer, DianaPlayerController, &ADianaPlayerController::UpdateAbilityBars, 0.2f, true);
	}
	else if(DianaPlayerController)
	{
		DianaPlayerController->UpdateAbilityBars();
	}
}

void ADiana::StopWeakShooting() 
{	
	GetWorldTimerManager().ClearTimer(WeakAttackShootingTimerHandle);
	GetWorldTimerManager().SetTimer(SlowStaminaRecoverTimer, this, &ADiana::EnableStaminaRegeneration, TimeToWaitForNormalManaRegenerate, false);
}

void ADiana::EnableDash() 
{
	bCanDash = true;
	GetWorldTimerManager().ClearTimer(DashCooldownTimerHandle);
	GetWorldTimerManager().ClearTimer(RefreshDashTimerHandle);
	if(DianaPlayerController) DianaPlayerController->UpdateDianaDashBar();	
}

void ADiana::IncrementSavings() 
{
	RemainingSaves++;
	if(DianaPlayerController)
	{
		DianaPlayerController->ChangeRemainingSaves(RemainingSaves);
	}
}

void ADiana::DecrementSavings() 
{
	RemainingSaves--;
	if(DianaPlayerController)
	{
		DianaPlayerController->ChangeRemainingSaves(RemainingSaves);
	}
}

void ADiana::CorruptDiana() 
{
	if(CorruptionLevel < 0)
	{
		MaxHealth = MaxHealth - (BaseMaxHealth * ((-CorruptionLevel) * .05f));

		float ResistanceStat = CorruptionLevel * .025f;
		ModifyResistance(TEXT("Arcanos"), ResistanceStat, false);
		ModifyResistance(TEXT("Agua"), ResistanceStat, false);
		ModifyResistance(TEXT("Rayos"), ResistanceStat, false);
		ModifyResistance(TEXT("Sombras"), ResistanceStat, false);
		ModifyResistance(TEXT("Sagrado"), ResistanceStat, false);

		CorruptionLevel = 0;
	}
	
	CorruptionLevel = CorruptionLevel+1 > 3 ? 3 : CorruptionLevel+1;
	BaseDamage = (BaseDamage - (CorruptionLevel-1) * 15.f) + (CorruptionLevel * 15.f);

	float NextMaxHealth = MaxHealth * (1.f - (CorruptionLevel * .20f));
	if(Health > NextMaxHealth)
	{
		Health = NextMaxHealth;
	}

	MaxStamina = BaseMaxStamina + (BaseMaxStamina * (CorruptionLevel * .20f));

	if(DianaPlayerController)
	{
		DianaPlayerController->UpdateDianaHealthBar();
		DianaPlayerController->UpdateDianaManaBar();
		WasteStamina(1.f); // pa' que se actualice el mana.
		DianaPlayerController->DianaGotCorrupted();
	}
	if(bMoveMode)
	{
		if(MovementModeNiagParticle2) MovementModeNiagParticle2->Activate(true);
		if(MovementModeNiagParticle) MovementModeNiagParticle->Deactivate();
	}
	
	AdjustCorruptedVisuals();
}

void ADiana::DescorruptDiana() 
{
	if(CorruptionLevel > 0)
	{
		BaseDamage = BaseDamage - (CorruptionLevel * 15.f);

		MaxStamina = MaxStamina - (BaseMaxStamina * (CorruptionLevel * .20f));
		Stamina = FMath::Min(Stamina, MaxStamina);

		CorruptionLevel = 0;
		if(DianaPlayerController)
		{
			DianaPlayerController->DianaIsNotCorrupted();
		}
		if(bMoveMode)
		{
			if(MovementModeNiagParticle2) MovementModeNiagParticle2->Deactivate();
			if(MovementModeNiagParticle) MovementModeNiagParticle->Activate(true);
		}
	}

	bool bCanImproveResistanes = true;
	if(CorruptionLevel == -6)
	{
		bCanImproveResistanes = false;
	}
	
	CorruptionLevel = CorruptionLevel-1 < -6 ? -6 : CorruptionLevel-1;

	MaxHealth = (BaseMaxHealth + (BaseMaxHealth * ((-CorruptionLevel) * .05f))) + ExtraHealth;
	Health = MaxHealth;

	if(bCanImproveResistanes)
	{
		ModifyResistance(TEXT("Arcanos"), .025f, false);
		ModifyResistance(TEXT("Agua"), .025f, false);
		ModifyResistance(TEXT("Rayos"), .025f, false);
		ModifyResistance(TEXT("Sombras"), .025f, false);
		ModifyResistance(TEXT("Sagrado"), .025f, false);
	}

	if(DianaPlayerController)
	{
		DianaPlayerController->UpdateDianaHealthBar();
	}

	AdjustCorruptedVisuals();
}

void ADiana::ReenableJump() 
{
	JumpCounts = 0;
}

void ADiana::TurnOnAttackMode() 
{
	if(!GetWorldTimerManager().IsTimerActive(AttackModeHandler))
	{
		if(CorruptionLevel > 0)
		{
			GetMesh()->SetMaterialByName(FName("M_PupilasInteriores"), FightEyesCorruptedMat);
			GetMesh()->SetMaterialByName(FName("Brazalete"), FightBraceCorruptedMat);
			FightLightComponent->SetLightColor(CorruptedFightColor);
		}
		else
		{
			GetMesh()->SetMaterialByName(FName("M_PupilasInteriores"), FightEyesMat);
			GetMesh()->SetMaterialByName(FName("Brazalete"), FightBraceMat);
			FightLightComponent->SetLightColor(NonCorruptedFightColor);
		}
		
		if(StartAttackModeAudio)
		{
			StartAttackModeAudio->Activate(true);
		}
		if(FightLightComponent)
		{
			FightLightComponent->SetVisibility(true);
		}
		if(StartAttackModeParticle)
		{
			if(CorruptionLevel > 0)
			{
				StartAttackModeParticle->SetColorParameter(FName("Color"), CorruptedAttackMode1Color);
				StartAttackModeParticle->SetColorParameter(FName("Color_02"), CorruptedAttackMode2Color);
			}
			else
			{
				StartAttackModeParticle->SetColorParameter(FName("Color"), NonCorruptedAttackMode1Color);
				StartAttackModeParticle->SetColorParameter(FName("Color_02"), NonCorruptedAttackMode2Color);
			}
			StartAttackModeParticle->Activate(true);
		}
	}

	bAttackMode = true;
	GetWorldTimerManager().SetTimer(AttackModeHandler, this, &ADiana::TurnOffAttackMode, 3.f, false);
}

void ADiana::TurnOffAttackMode() 
{
	GetWorldTimerManager().ClearTimer(AttackModeHandler);
	AdjustCorruptedVisuals();
	
	if(StopAttackModeAudio)
	{
		StopAttackModeAudio->Activate(true);
	}
	if(FightLightComponent)
	{
		FightLightComponent->SetVisibility(false);
	}
	if(StartAttackModeParticle)
	{
		StartAttackModeParticle->Deactivate();
	}
	bAttackMode = false;

}

void ADiana::TurnOnMoveMode() 
{
	if(!bMoveMode)
	{
		bMoveMode = true;
		StaminaRegenerationRate += 100.f;
		if(MovementModeAudio) MovementModeAudio->Activate(true);
		if(CorruptionLevel > 0)
		{
			if(MovementModeNiagParticle2) MovementModeNiagParticle2->Activate(true);
		}
		else
		{
			if(MovementModeNiagParticle) MovementModeNiagParticle->Activate(true);
		}

		ChangeSpiritSpeed(MoveModeExtraSpeed);
		ChangeDianaFOV(FOVToChange);
		if(DianaPlayerController) DianaPlayerController->MovementModeChanged(true);
	}
}

void ADiana::TurnOffMoveMode()
{
	if(bMoveMode)
	{
		bMoveMode = false;
		StaminaRegenerationRate -= 100.f;
		if(MovementModeNiagParticle) MovementModeNiagParticle->Deactivate();
		if(MovementModeNiagParticle2) MovementModeNiagParticle2->Deactivate();
		if(DeactivateMoveModeAudio) DeactivateMoveModeAudio->Activate(true);

		ChangeSpiritSpeed(-MoveModeExtraSpeed);
		ChangeDianaFOV(-FOVToChange);
		if(DianaPlayerController) DianaPlayerController->MovementModeChanged(false);
	}
}

void ADiana::EnableMovementMode() 
{
	GetWorldTimerManager().ClearTimer(MoveModeHandler);
	bNotInCombat = true;
	if(DianaPlayerController && FMath::IsWithinInclusive(CorruptionLevel, -2, 0))
	{
		DianaPlayerController->ShowDianaMovementModeIndicator();
	}
	if(EnableMoveModeAudio && FMath::IsWithinInclusive(CorruptionLevel, -2, 0)) EnableMoveModeAudio->Activate(true);
}

void ADiana::ToggleMovementMode() 
{
	if(bNotInCombat || !(FMath::IsWithinInclusive(CorruptionLevel, -2, 0)))
	{
		if(!bMoveMode)
		{
			TurnOnMoveMode();
		}
		else
		{
			TurnOffMoveMode();
		}
	}
}

void ADiana::ChangeDianaFOV(float FOVToModify) 
{
	NextFOVAux = Camera->FieldOfView + FOVToModify;
	GetWorldTimerManager().SetTimer(MoveModeFOVHandler, this, &ADiana::ModifyActualFOV, 0.01f, true);
}

void ADiana::ModifyActualFOV() 
{
	float ActualFOV = Camera->FieldOfView;
	if(!FMath::IsNearlyEqual(ActualFOV, NextFOVAux))
	{
		float NextFOV = FMath::FInterpTo(ActualFOV, NextFOVAux, GetWorld()->GetDeltaSeconds(), 5.f);
		Camera->SetFieldOfView(NextFOV);
	}
	else
	{
		Camera->SetFieldOfView(NextFOVAux);
		GetWorldTimerManager().ClearTimer(MoveModeFOVHandler);
	}
}

void ADiana::ModifyResistance(FString StatName, float ResistanceToModify, bool bModifyPermanently=false) 
{
	Super::ModifyResistance(StatName, ResistanceToModify, bModifyPermanently);

	if(bModifyPermanently)
	{
		if(DianaBaseResistances.Contains(StatName))
		{
			float Resistance = DianaBaseResistances[StatName];
			float NextResistance = FMath::Min(1.f, Resistance+ResistanceToModify);
			
			DianaBaseResistances[StatName] = NextResistance;
		}
	}
}

void ADiana::ChangeToSpellUp() 
{
	if(DianaPlayerController && !bStunned)
	{	
		CurrentSpellIndex = DianaPlayerController->GetNextValidSpellIndex(CurrentSpellIndex, true);
		DianaPlayerController->UpdateChoosenSpellByIndex(CurrentSpellIndex);
	}	
}

void ADiana::ChangeToSpellDown() 
{
	if(DianaPlayerController && !bStunned) 
	{
		CurrentSpellIndex = DianaPlayerController->GetNextValidSpellIndex(CurrentSpellIndex, false);
		DianaPlayerController->UpdateChoosenSpellByIndex(CurrentSpellIndex);
	}	
}

void ADiana::ChangeToSpell1() 
{
	if(CurrentSpellIndex != 1 && DianaPlayerController && !bStunned) 
	{
		if(DianaPlayerController->ExistSpellIndex(1))
		{
			CurrentSpellIndex = 1;
			DianaPlayerController->UpdateChoosenSpellByIndex(CurrentSpellIndex);
		}
	}
}

void ADiana::ChangeToSpell2() 
{
	if(CurrentSpellIndex != 2 && DianaPlayerController && !bStunned) 
	{
		if(DianaPlayerController->ExistSpellIndex(2))
		{
			CurrentSpellIndex = 2;
			DianaPlayerController->UpdateChoosenSpellByIndex(CurrentSpellIndex);
		}
	}
}

void ADiana::ChangeToSpell3() 
{
	if(CurrentSpellIndex != 3 && DianaPlayerController && !bStunned) 
	{
		if(DianaPlayerController->ExistSpellIndex(3))
		{
			CurrentSpellIndex = 3;
			DianaPlayerController->UpdateChoosenSpellByIndex(CurrentSpellIndex);
		}
	}
}

void ADiana::ChangeToSpell4() 
{
	if(CurrentSpellIndex != 4 && DianaPlayerController && !bStunned) 
	{
		if(DianaPlayerController->ExistSpellIndex(4))
		{
			CurrentSpellIndex = 4;
			DianaPlayerController->UpdateChoosenSpellByIndex(CurrentSpellIndex);
		}
	}
}

void ADiana::ChangeToSpell5() 
{
	if(CurrentSpellIndex != 5 && DianaPlayerController && !bStunned) 
	{
		if(DianaPlayerController->ExistSpellIndex(5))
		{
			CurrentSpellIndex = 5;
			DianaPlayerController->UpdateChoosenSpellByIndex(CurrentSpellIndex);
		}
	}
}

void ADiana::ChangeToSpell6() 
{
	if(CurrentSpellIndex != 6 && DianaPlayerController && !bStunned) 
	{
		if(DianaPlayerController->ExistSpellIndex(6))
		{
			CurrentSpellIndex = 6;
			DianaPlayerController->UpdateChoosenSpellByIndex(CurrentSpellIndex);
		}
	}
}

void ADiana::UseSecundarySpell() 
{
	if(!bStunned)
	{
		InvokeSecundarySpell();
		if(DianaPlayerController) DianaPlayerController->UpdateAbilityBars();
	}
}

bool ADiana::WasteStamina(float StaminaToUse) 
{
	if(HaveEnoughStamina(StaminaToUse))	
	{
		Stamina -= StaminaToUse;
		bFastStaminaRegeneration = false; 
		GetWorldTimerManager().ClearTimer(SlowStaminaRecoverTimer); 
		if(!GetWorldTimerManager().IsTimerActive(RefreshManaTimerHandle)) 
		{
			GetWorldTimerManager().SetTimer(RefreshManaTimerHandle, this, &ADiana::UpdateMana, 0.05f, true);
		}
		return true;
	}
	else
	{
		if(DianaPlayerController && !IsDead()) DianaPlayerController->ShowNoManaMessage();
	}

	return false;
}

void ADiana::UpdateMana() 
{
	if(Stamina < MaxStamina)
	{
		if(DianaPlayerController)
		{
			DianaPlayerController->UpdateDianaManaBar();
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(RefreshManaTimerHandle);
		if(DianaPlayerController)
		{
			DianaPlayerController->UpdateDianaManaBar();
		}
	}
}

void ADiana::UpdateManaNums(float DeltaTime) 
{
	if(Stamina < MaxStamina)
	{
		if(bFastStaminaRegeneration)
		{
			// rapida regeneracion, mientras menos tengamos, mas rapido regeneramos
			Stamina = Stamina + ((DeltaTime * StaminaRegenerationRate) + ((MaxStamina - Stamina) * .003f));
		}
		else
		{
			// mas lenta, cuando hacemos algo que nos gaste mana.
			Stamina = Stamina + ((DeltaTime * StaminaRegenerationRate));
		}
		Stamina = FMath::Min(Stamina, MaxStamina);
	}
}

void ADiana::PauseMenu() 
{
	if(DianaPlayerController)
	{
		if(!DianaPlayerController->bGamePaused)
		{
			DianaPlayerController->ShowPauseMenu();
		}
	}
}

void ADiana::StatsMenu() 
{
	if(DianaPlayerController)
	{
		if(!DianaPlayerController->bGamePaused)
		{
			DianaPlayerController->ShowStatsMenu();
		}
	}
}

bool ADiana::HaveEnoughStamina(float StaminaCost) 
{
	return Stamina >= StaminaCost;
}

void ADiana::EnableStaminaRegeneration() 
{
	bFastStaminaRegeneration = true;
	GetWorldTimerManager().ClearTimer(SlowStaminaRecoverTimer); 
}

void ADiana::UpdateDianaTargetToHit(float Range) 
{
	if(DianaPlayerController)
	{
		FVector Location;
		FRotator Rotation;
		DianaPlayerController->GetPlayerViewPoint(Location, Rotation);
		
		FVector End = Location + Rotation.Vector() * Range;
		SetTargetToHitLocation(End);
	}
}

void ADiana::UpdateMaxHealth() 
{
	if(Health > MaxHealth) Health = MaxHealth;

	if(DianaPlayerController) DianaPlayerController->UpdateDianaHealthBar();
}

void ADiana::UpdateMaxStamina() 
{
	Stamina = FMath::Min(MaxStamina, Stamina);	
	if(!GetWorldTimerManager().IsTimerActive(RefreshManaTimerHandle)) 
	{
		GetWorldTimerManager().SetTimer(RefreshManaTimerHandle, this, &ADiana::UpdateMana, 0.05f, true);
	}
	if(DianaPlayerController) DianaPlayerController->UpdateDianaManaBar();
}

void ADiana::ChangeSpellEffects(FString SpellType) 
{
	if(SpellColors.Contains(SpellType))
	{
		if(ChangeSpellEffect)
		{
			ChangeSpellEffect->SetColorParameter(FName("Color"), SpellColors[SpellType]);
			ChangeSpellEffect->Activate(true);
		}
	}
	if(ChangeSpellAudio)
	{
		ChangeSpellAudio->Activate(true);
	}
}

void ADiana::ShowDamageReceivedOverlay() 
{
	GetWorldTimerManager().SetTimer(ReceivingDamageHandler, this, &ADiana::HideDamageReceivedOverlay, 2.f, false);
	if(!bDamageOverlayActivated)
	{
		if(DianaPlayerController) DianaPlayerController->ShowBloodOverlay();
		bDamageOverlayActivated = true;
	}
}

void ADiana::HideDamageReceivedOverlay() 
{
	if(DianaPlayerController) DianaPlayerController->HideBloodOverlay();
	bDamageOverlayActivated = false;
}

void ADiana::EnableShield() 
{
	bCanShield = true;
	bShieldFault = false;
	GetWorldTimerManager().ClearTimer(ShieldCooldownTimerHandle);
	GetWorldTimerManager().ClearTimer(RefreshShieldTimerHandle);
	if(DianaPlayerController) DianaPlayerController->UpdateDianaShieldBar();
}

void ADiana::ShieldDidntHit() 
{
	GetWorldTimerManager().ClearTimer(ShieldDidnHitHandler);
	bShieldFault = true;
}

TMap<FString, TSubclassOf<ASpell>> ADiana::GetWeakSpellsList() 
{
	return SpellComponent->GetSpellsList();
}

TMap<FString, TSubclassOf<ASpell>> ADiana::GetStrongSpellsList() 
{
	return StrongSpellComponent->GetSpellsList();
}

void ADiana::SetWeakSpellList(TMap<FString, TSubclassOf<ASpell>> NewSpellList) 
{
	SpellComponent->SetSpellsList(NewSpellList);
	if(DianaPlayerController)
	{
		DianaPlayerController->DianaSpellsOrder = DianaPlayerController->ResetSpellList();
		DianaPlayerController->UpdateSpellListOrder();
		DianaPlayerController->OnSpellListChanged.Broadcast();
	}
}

void ADiana::SetStrongSpellList(TMap<FString, TSubclassOf<ASpell>> NewSpellList) 
{
	StrongSpellComponent->SetSpellsList(NewSpellList);
}

void ADiana::SetStatesList(TMap<FString, TSubclassOf<class ASpiritState>> NewStateList) 
{
	SpiritStateComponent->SetStatesList(NewStateList);
}

void ADiana::AddSpellToWeakComponentList(FString NewSpellType, TSubclassOf<ASpell> NewSpellClass) 
{
	SpellComponent->AddSpellToComponent(NewSpellType, NewSpellClass);
	
	if(DianaPlayerController)
	{
		DianaPlayerController->UpdateSpellOrderForNewSpell(NewSpellType);
		DianaPlayerController->UpdateSpellListOrder();
	}
}

void ADiana::AddSpellToStrongComponentList(FString NewSpellType, TSubclassOf<ASpell> NewSpellClass) 
{
	StrongSpellComponent->AddSpellToComponent(NewSpellType, NewSpellClass);
}

TMap<FString, TSubclassOf<ASpiritState>> ADiana::GetDianaStatesList() 
{
	if(SpiritStateComponent)
	{
		return SpiritStateComponent->GetSpiritStates();
	}

	return TMap<FString, TSubclassOf<ASpiritState>>();
}

void ADiana::AdjustCorruptedVisuals() 
{
	if(CorruptionLevel <= 0)
	{
		GetMesh()->SetMaterialByName(FName("M_PupilasInteriores"), NoFightEyesMat);
		GetMesh()->SetMaterialByName(FName("Brazalete"), NoFightBraceMat);

		if(StartAttackModeParticle)
		{
			StartAttackModeParticle->SetColorParameter(FName("Color"), NonCorruptedAttackMode1Color);
			StartAttackModeParticle->SetColorParameter(FName("Color_02"), NonCorruptedAttackMode2Color);
		}
		
	}
	else
	{
		GetMesh()->SetMaterialByName(FName("M_PupilasInteriores"), FightEyesCorruptedMat);
		GetMesh()->SetMaterialByName(FName("Brazalete"), FightBraceCorruptedMat);

		if(StartAttackModeParticle)
		{
			StartAttackModeParticle->SetColorParameter(FName("Color"), CorruptedAttackMode1Color);
			StartAttackModeParticle->SetColorParameter(FName("Color_02"), CorruptedAttackMode2Color);
		}
	}
}

void ADiana::SetNewState(FString NewState, TSubclassOf<ASpiritState> NewStateClass) 
{
	if(SpiritStateComponent)
	{
		SpiritStateComponent->SetSpiritState(NewState, NewStateClass);
	}
}

void ADiana::ResetDianaGravity() 
{
	if(GetCharacterMovement()) GetCharacterMovement()->GravityScale = 3.f;
}

bool ADiana::IsCorrupted() 
{
	return CorruptionLevel > 0;
}

TSubclassOf<ASpiritState> ADiana::GetStateClass(FString StateName) 
{
	if(SpiritStateComponent)
	{
		return SpiritStateComponent->GetSpiritStateClass(StateName);
	}

	return TSubclassOf<ASpiritState>();
}

FGenericTeamId ADiana::GetGenericTeamId() const 
{
    return TeamId;
}

FString ADiana::GetCurrentDashType() 
{
	return DashType;
}

float ADiana::GetCurrentStamina() 
{
	if(MaxStamina) return Stamina/MaxStamina;

	return 0.f;
}

float ADiana::GetCurrentDashCooldown() 
{	
	float TimeElapsed = GetWorldTimerManager().GetTimerElapsed(DashCooldownTimerHandle);
	if(TimeElapsed != -1.f)
	{
		if(DashCooldown) return TimeElapsed/DashCooldown;
	}
	
	return 1.f;
}

float ADiana::GetCurrentShieldCooldown() 
{
	float TimeElapsed = GetWorldTimerManager().GetTimerElapsed(ShieldCooldownTimerHandle);
	if(TimeElapsed != -1.f)
	{
		if(ShieldCooldown) return TimeElapsed/ShieldCooldown;
	}
	
	return 1.f;
}

void ADiana::SetRotationRate(float NewRotationRate) 
{
	RotationRate = NewRotationRate;
}

float ADiana::GetCurrentSecundarySpellCooldownPercentage() 
{
	return SpellComponent->GetSecundarySpellPercentage();
}

float ADiana::GetSpellActiveCooldownPercentage(FString SpellType) 
{
	return SpellComponent->GetActiveSpellPercentage(SpellType);
}

float ADiana::GetSpellSecundaryCooldownPercentage(FString SpellType) 
{
	return StrongSpellComponent->GetSpellPercentage(SpellType);
}

float ADiana::GetSpellPrimaryCooldownPercentage(FString SpellType) 
{
	return SpellComponent->GetSpellPercentage(SpellType);
}

float ADiana::GetCurrentStrongSpellCast() 
{
	return StrongSpellComponent->GetSpellPercentage();
}

int32 ADiana::GetStamina() 
{
	return FMath::FloorToInt(Stamina);
}

int32 ADiana::GetMaxStamina() 
{
	return FMath::FloorToInt(MaxStamina);
}

void ADiana::SetNewBaseDamage(float ValueToAdd) 
{
	BaseMaxDamage += ValueToAdd;
	BaseDamage += ValueToAdd;
}

void ADiana::SetNewMaxHealth(float ValueToAdd) 
{
	BaseMaxHealth += ValueToAdd;
	MaxHealth += ValueToAdd;
	ModifyHealth(100000.f);
}

void ADiana::SetNewMaxStamina(float ValueToAdd) 
{
	BaseMaxStamina += ValueToAdd;
	MaxStamina += ValueToAdd;
	ModifyStamina(100000.f); 
}

void ADiana::SetNewExactBaseDamage(float Value) 
{
	BaseMaxDamage = Value;
	BaseDamage = Value;
}

void ADiana::SetNewExactMaxHealth(float Value) 
{
	BaseMaxHealth = Value;
	MaxHealth = Value;
	ModifyHealth(100000.f);
}

void ADiana::SetNewExactMaxStamina(float Value) 
{
	BaseMaxStamina = Value;
	MaxStamina = Value;
	ModifyStamina(100000.f);
}

void ADiana::ModifyStamina(float StaminaToModify) 
{
	float NextStamina = FMath::Min(MaxStamina, Stamina+StaminaToModify);
	NextStamina = FMath::Max(0.f, NextStamina);

	Stamina = NextStamina;

	if(!GetWorldTimerManager().IsTimerActive(RefreshManaTimerHandle)) 
	{
		GetWorldTimerManager().SetTimer(RefreshManaTimerHandle, this, &ADiana::UpdateMana, 0.05f, true);
	}
}

void ADiana::AddHeightToSecondJump(float ExtraJump) 
{
	SecondJumpHeight += ExtraJump;
}

TArray<FString> ADiana::GetListOfSpellsNames() 
{
	return SpellComponent->GetSpells();
}

TMap<FString, FText> ADiana::GetSpellDescription(FString SpellType) 
{
	return SpellComponent->GetSpellDescription(SpellType);
}

void ADiana::InvokeCurrentStrongSpell(FName SocketName) 
{
	StrongSpellComponent->ActivateCurrentSpell(SocketName);
}

int32 ADiana::GetSpellTypeLevel(FString SpellType) 
{
	return SpellComponent->GetSpellTypeLevel(SpellType);
}

void ADiana::MakeSpecialDash(float Duration, float Distance) 
{
	if(WaterSpellParticle) WaterSpellParticle->Activate(true);

	bSlippingDiana = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	SetInvincibleMode(Duration);

	LaunchCharacter(GetActorForwardVector()*Distance, false, false);
	GetCharacterMovement()->GroundFriction = 0.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 0.f;
	GetMesh()->SetVisibility(false, false);
	if(StartAttackModeParticle) StartAttackModeParticle->SetVisibility(false, false);

	GetWorldTimerManager().SetTimer(SuperDashDianaHandle, this, &ADiana::StopSpecialDash, Duration, false);
}

void ADiana::StopSpecialDash() 
{
	GetWorldTimerManager().ClearTimer(SuperDashDianaHandle);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	GetCharacterMovement()->GroundFriction = 8.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2048.f;
	GetMesh()->SetVisibility(true, false);
	if(StartAttackModeParticle) StartAttackModeParticle->SetVisibility(true, false);

	bSlippingDiana = false;

	if(WaterSpellParticle) WaterSpellParticle->Deactivate();
}

void ADiana::PlayFootstepsAudioAndEffects(FString MaterialName, FName FootSocket) 
{
	if(FootstepsEffects.Contains(MaterialName) &&
		FootstepsWalkAudio.Contains(MaterialName) && 
		FootstepsRunAudio.Contains(MaterialName))
	{
		USoundBase* WalkSound = FootstepsWalkAudio[MaterialName];
		USoundBase* RunSound = FootstepsRunAudio[MaterialName];
		UNiagaraSystem* NiagEffect = FootstepsEffects[MaterialName];

		if(WalkSound && RunSound && NiagEffect)
		{
			if(FootSocket.IsEqual(FName("LeftFoot")))
			{
				FootstepsWalkAudioToSpawnLeft->SetSound(WalkSound);
				FootstepsRunAudioToSpawnLeft->SetSound(RunSound);
				FootstepsEffectToSpawnLeft->SetAsset(NiagEffect);

				FootstepsEffectToSpawnLeft->Activate(true);
				FootstepsWalkAudioToSpawnLeft->Activate(true);
				FootstepsRunAudioToSpawnLeft->Activate(true);
				
				float SpeedNormalizedToRange = UKismetMathLibrary::NormalizeToRange(GetSpiritSpeed(), -1.f, SpiritWalkSpeed);
				if(WalkCurve)
				{
					float NewWalkVolume = WalkCurve->GetFloatValue(SpeedNormalizedToRange);
					FootstepsWalkAudioToSpawnLeft->SetVolumeMultiplier(NewWalkVolume);
				}

				if(RunCurve)
				{
					float NewRunVolume = RunCurve->GetFloatValue(SpeedNormalizedToRange);
					FootstepsRunAudioToSpawnLeft->SetVolumeMultiplier(NewRunVolume);
				}
			}
			else
			{
				FootstepsWalkAudioToSpawnRight->SetSound(WalkSound);
				FootstepsRunAudioToSpawnRight->SetSound(RunSound);
				FootstepsEffectToSpawnRight->SetAsset(NiagEffect);

				FootstepsEffectToSpawnRight->Activate(true);
				FootstepsWalkAudioToSpawnRight->Activate(true);
				FootstepsRunAudioToSpawnRight->Activate(true);

				float SpeedNormalizedToRange = UKismetMathLibrary::NormalizeToRange(GetSpiritSpeed(), -1.f, SpiritWalkSpeed);
				if(WalkCurve)
				{
					float NewWalkVolume = WalkCurve->GetFloatValue(SpeedNormalizedToRange);
					FootstepsWalkAudioToSpawnRight->SetVolumeMultiplier(NewWalkVolume);
				}

				if(RunCurve)
				{
					float NewRunVolume = RunCurve->GetFloatValue(SpeedNormalizedToRange);
					FootstepsRunAudioToSpawnRight->SetVolumeMultiplier(NewRunVolume);
				}
			}
		}
	}
}

void ADiana::SetNextMapTransform(FVector NextLocation, FRotator NextRotation, FString NextMap) 
{
	bIsTPing = true;
	DianaNextTPLocation = NextLocation;
	DianaNextTPRotation = NextRotation;
	DianaNextMap = NextMap;
}
