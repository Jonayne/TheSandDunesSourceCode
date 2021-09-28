// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/AudioComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "EnemyController.h"
#include "TheSandDunesGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Diana.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"


AEnemy::AEnemy() 
{
	Tags.Add(FName("Enemy"));
	bUseControllerRotationYaw = false;

	RadiusToNextRandomLocation = 1000.f;
	VerticalDistanceToKeep = 100.f;

	SecondsForTPingToDiana = 0.f;

	DamageByTouch = 50.f;
	DamageByTouchNature = TEXT("Arcanos");
	
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(FName("Health Bar"));
	HealthBarWidget->SetupAttachment(GetMesh());
	HealthBarWidget->SetVisibility(false);
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidget->SetDrawSize(FVector2D(200.f));

	SpecialHealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(FName("Special Health Bar"));
	SpecialHealthBarWidget->SetupAttachment(GetMesh());
	SpecialHealthBarWidget->SetVisibility(false);
	SpecialHealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	EnemyMusic = CreateDefaultSubobject<UAudioComponent>(FName("EnemyMusic"));
	EnemyMusic->SetupAttachment(GetRootComponent());
	EnemyMusic->bAutoActivate = false;

	TeamId = FGenericTeamId(1);

	DeathEnemySound = CreateDefaultSubobject<UAudioComponent>(FName("DeathEnemyAudio"));
	DeathEnemySound->SetupAttachment(GetMesh());
	DeathEnemySound->bAutoActivate = false;

	BoxVision = CreateDefaultSubobject<UBoxComponent>(FName("BoxVision"));
	BoxVision->SetupAttachment(GetRootComponent());
	BoxVision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxVision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	BoxVision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);

	TPEffect = CreateDefaultSubobject<UNiagaraComponent>(FName("TPEffect"));
	TPEffect->SetupAttachment(GetMesh());
	TPEffect->bAutoActivate = false;

	TPToDianaSound = CreateDefaultSubobject<UAudioComponent>(FName("TPToDianaSound"));
	TPToDianaSound->SetupAttachment(GetMesh());
	TPToDianaSound->bAutoActivate = false;

	EnemyName = FText();

	bCanStartFight = true;
	bShouldHear = true;
	bCanBeTeleportedByOthersEnemies = false;

	bShouldUseDamageByTouchNatureOnSpells = true;
	bShouldDamageAllies = false;
}

void AEnemy::BeginPlay() 
{
   	Super::BeginPlay();

	MaxHealthAux = MaxHealth;
	Health = MaxHealth;
	GetCharacterMovement()->MaxFlySpeed = SpiritWalkSpeed;
	SpiritMassAux = SpiritMass;

    // Para que la camara no se vuelva loca con los enemigos.
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	EnemyController = Cast<AEnemyController>(GetController());
	OnSeenEnemy.AddDynamic(this, &AEnemy::OnHaveSeenAnEnemy);
	OnStartFightEveryone.AddDynamic(this, &AEnemy::StartBossFightEvent);

	if(bEnemyByEvent)
	{
		if(bIsABoss)
		{
			BoxVision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnVisionOverlap);
		}		

		MaxHealth = 100000000.f;
		Health = MaxHealth;
		SpiritMass = 1.f;
	}
}

void AEnemy::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);

	if(bIsManuallyMovingToNextLocation)
	{
		MoveThisEnemy(DeltaTime);
	}
}

FSavableActor AEnemy::GetSavableActorInfo_Implementation() const 
{
	return SavableInfo;
}

void AEnemy::ChangeActorState_Implementation() 
{
	UWorld* World = GetWorld();
	UTheSandDunesGameInstance* GameInstance = World ? World->GetGameInstance<UTheSandDunesGameInstance>() : nullptr;
	
	if (GameInstance)
	{
		GameInstance->ChangeSavableActorState(UGameplayStatics::GetCurrentLevelName(this, true), SavableInfo);
	}
}

void AEnemy::UpdateSavableInfo_Implementation(const FSavableActor& NewActorInfo) 
{
	SavableInfo = NewActorInfo;
	if(!(SavableInfo.bIsAlive || SavableInfo.bShouldRespawn))
	{
		Destroy();
	}
}

float AEnemy::SpiritTakeDamage(float DamageAmount, FString TypeOfDamage, AActor* DamageCauser, FVector DamageLocation, bool bIsABeam) 
{
	float Damage = Super::SpiritTakeDamage(DamageAmount, TypeOfDamage, DamageCauser, DamageLocation, bIsABeam);
	
	if(DamageCauser)
	{
		ADiana* DianaRef = Cast<ADiana>(DamageCauser);
		if(DianaRef)
		{
			DianaRef->HealDianaDamagePercentage(Damage);
		}
	}

	return Damage;
}

float AEnemy::AttackAnimation() 
{
	int32 Attack = 0;

	if(ActionNameToExecute.IsEmpty())
	{
		Attack = AttackToExecute;

		if(Attack == 0)
		{
			Attack = FMath::RandRange(1, NumberOfAttacks);	
		}
	}
	else
	{
		Attack = GetAttackIndexByName();
	}

	FString AttackName = TEXT("Attack_");
	AttackName.AppendInt(Attack);
	float RemainingTime = PlayMontageByName(FName(*AttackName), SpiritAnimationMontage, 1.f);

	return RemainingTime;
}

float AEnemy::TeleportAnimation() 
{
	float RemainingTime = PlayMontageByName(FName("Teleport"), SpiritAnimationMontage, 1.f);

	return RemainingTime;
}

void AEnemy::TPToDiana() 
{
	ADiana* DianaRef = Cast<ADiana>(UGameplayStatics::GetPlayerPawn(this, 0));
	if(DianaRef)
	{
		FVector DianaLoc = DianaRef->GetActorLocation();
		FVector RandomAreaVec = UKismetMathLibrary::RandomUnitVector() * (FMath::RandRange(RadiusToNextRandomLocation*.5f, RadiusToNextRandomLocation));
		DianaLoc.Z += FMath::FRandRange(VerticalDistanceToKeep*.5f, VerticalDistanceToKeep);
		RandomAreaVec.Z = 0.f;

		bool bSuccess = TeleportTo(DianaLoc + RandomAreaVec, GetActorRotation());
		if(bSuccess)
		{
			if(TPEffect) TPEffect->Activate(true);
			if(TPToDianaSound) TPToDianaSound->Activate(true);
		}	
	}
}

bool AEnemy::Teleport() 
{
	bool bSuccess = TeleportTo(NextLocation, NextRotation, false, false);

	return bSuccess;
}

void AEnemy::PrepareToFight() 
{
	SetStunned(true);
}

void AEnemy::OnHaveSeenAnEnemy() 
{
	if(bCanStartFight)
	{
		bSeeingEnemy = true;
		if(EnemyController)
		{
			EnemyController->StartEnemyBehaviorTree();
		}
		
		if(SecondsForTPingToDiana)
		{
			GetWorldTimerManager().SetTimer(TPToDianaTimer, this, &AEnemy::TPToDiana, SecondsForTPingToDiana, true);
		}
	}
}

void AEnemy::SetNextLocation(FVector NextLocationToGo)
{
    NextLocation = NextLocationToGo;
	NextRotation = GetActorRotation();
}

float AEnemy::MoveEnemyToRandomLocation(AActor* ActorToMoveAround) 
{
	if(!bIsManuallyMovingToNextLocation)
	{
		SetEnemyRandomLocation(ActorToMoveAround);

		// aproximacion del tiempo que tomara en llegar de un lado a otro.
		FVector DistanceToMove = NextLocation - GetActorLocation();
		float Distance = DistanceToMove.Size();
		float TimeThatWillTake = Distance/SpiritWalkSpeed;

		bIsManuallyMovingToNextLocation = true;

		return TimeThatWillTake;
	}

	return 0.f;
}

void AEnemy::SetEnemyRandomLocation(AActor* ActorToMoveAround) 
{
	FVector RandomUnitVector = UKismetMathLibrary::RandomUnitVector();
	FVector NewPoint = RandomUnitVector * (FMath::RandRange(RadiusToNextRandomLocation*.5f, RadiusToNextRandomLocation));
	NewPoint.Z = 0.f;

	FVector ActorToMoveAroundLocation;
	
	if(ActorToMoveAround)
	{
		ActorToMoveAroundLocation = ActorToMoveAround->GetActorLocation();
		ActorToMoveAroundLocation.Z += FMath::FRandRange(VerticalDistanceToKeep*.5f, VerticalDistanceToKeep);
	}
	else
	{
		ActorToMoveAroundLocation = GetActorLocation();
	}

	SetNextLocation(ActorToMoveAroundLocation + NewPoint);
}

void AEnemy::ModifyHealth(float Heal) 
{
	Super::ModifyHealth(Heal);

	ShowDamage(-Heal, TEXT("Sombras"), GetActorLocation(), false);	
}

void AEnemy::MoveThisEnemy(float DeltaTime) 
{
	FVector ActualLocation = GetActorLocation();
	if(!bStunned && !FVector::PointsAreNear(ActualLocation, NextLocation, 500.f))
	{
		NextLocation.Z = GetTargetToHitLocation().Z + FMath::FRandRange(VerticalDistanceToKeep*.5f, VerticalDistanceToKeep);;
		FVector NextLerpLocation = FMath::VInterpConstantTo(ActualLocation, NextLocation, DeltaTime, SpiritWalkSpeed);
		SetActorLocation(NextLerpLocation);
	}
	else
	{
		bIsManuallyMovingToNextLocation = false;
	}
}

void AEnemy::HideHealthBar() 
{
	HealthBarWidget->SetVisibility(false);
	GetWorldTimerManager().ClearTimer(ShowHealthBarTimer);
}

void AEnemy::StartBossFightEvent() 
{	
	MaxHealth = MaxHealthAux;
	Health = MaxHealthAux;
	SetStunned(false);
	SpiritMass = SpiritMassAux;
	bSeeingEnemy = true;
	ModifyHealth(0.f);

	ADiana* DianaRef = Cast<ADiana>(UGameplayStatics::GetPlayerPawn(this, 0));

	if(EnemyController)
	{
		EnemyController->EnemyPerceptionComponent->SetSenseEnabled(UAISense_Sight::StaticClass(), true);
		EnemyController->EnemyPerceptionComponent->SetSenseEnabled(UAISense_Hearing::StaticClass(), true);
		EnemyController->SeenPlayer = DianaRef;
		SetTargetToHit(EnemyController->SeenPlayer);
		EnemyController->SetFocus(EnemyController->SeenPlayer);
		OnSeenEnemy.Broadcast();
	}

	if(bIsABoss)
	{
		if(EnemyMusic)
		{
			EnemyMusic->Activate();
		}

		ShowSpecialHealthBar();

		for (auto& Ally : ListOfCompanions)
		{
			Ally->bCanStartFight = true;
			Ally->CallEveryoneToFight();
		}

		if(DianaRef && !DianaRef->IsDead())
		{
			DianaRef->bInBossFight = true;
		}
	}
}

void AEnemy::ShowHealthBar() 
{
	if(!HealthBarWidget->IsVisible() && !bIsABoss)
	{
		HealthBarWidget->SetVisibility(true);
		GetWorldTimerManager().SetTimer(ShowHealthBarTimer, this, &AEnemy::HideHealthBar, 15.f, false);
	}
}

void AEnemy::ShowHealthBarNoTimer() 
{
	if(!bIsABoss)
	{
		HealthBarWidget->SetVisibility(true);
		GetWorldTimerManager().ClearTimer(ShowHealthBarTimer);
	}
}

void AEnemy::CallEveryoneToFight_Implementation() 
{
	if(!bExecutedOnce)
	{
		bExecutedOnce = true;
		OnStartFightEveryone.Broadcast();
	}
}

void AEnemy::StartBossFight_Implementation() 
{
	StartBossFightEvent();
}

void AEnemy::KillThisSpirit() 
{
	OnEnemyHasBeenDefeated.Broadcast(this);
	
	for (auto& OtherEnemie : ListOfEnemiesThatSouldDieAfterMe)
	{
		if(OtherEnemie && !OtherEnemie->IsDead())
		{
			OtherEnemie->KillThisSpirit();
		}
	}

	GetWorldTimerManager().ClearTimer(TPToDianaTimer);
	
	SavableInfo.bIsAlive = false;
	ISavableActorInterface::Execute_ChangeActorState(this);

	if(bIsABoss)
	{
		HideSpecialHealthBar();
		if(EnemyMusic)
		{
			EnemyMusic->FadeOut(WaitBeforeDestroy-(WaitBeforeDestroy*.1f), 0.f, EAudioFaderCurve::Linear);
		}
		
		ADiana* DianaRef = Cast<ADiana>(UGameplayStatics::GetPlayerPawn(this, 0));
		if(DianaRef && !DianaRef->IsDead())
		{
			DianaRef->bInBossFight = false;
		}
	}
	else
	{
		HideHealthBar();
	}
	
	if(DeathEnemySound) DeathEnemySound->Activate();

	Super::KillThisSpirit();
}

void AEnemy::CallDestroyFunction() 
{
	Destroy();
}

void AEnemy::OnVisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
	if(OtherActor && OtherActor->IsA(ADiana::StaticClass()))
	{
		if(bCanStartFight)
		{
			StartBossFight();
		}
	}
}

void AEnemy::ShowSpecialHealthBar() 
{
	if(!SpecialHealthBarWidget->IsVisible())
	{
		SpecialHealthBarWidget->SetVisibility(true);
	}
}

void AEnemy::HideSpecialHealthBar() 
{
	SpecialHealthBarWidget->SetVisibility(false);
}

FGenericTeamId AEnemy::GetGenericTeamId() const 
{
	return TeamId;
}

int32 AEnemy::GetAttackIndexByName() 
{
	if(ActionNameToExecute.Equals(TEXT("Shield")))
	{
		return 5;
	}
	else if(ActionNameToExecute.Equals(TEXT("Heal")))
	{
		return 1;
	}
	else if(ActionNameToExecute.Equals(TEXT("Teleport")))
	{
		return 6;
	}
	else if(ActionNameToExecute.Equals(TEXT("Basico")))
	{
		return 2;
	}
	else if(ActionNameToExecute.Equals(TEXT("ZigZag")))
	{
		return 3;
	}
	else if(ActionNameToExecute.Equals(TEXT("Rapido")))
	{
		return 4;
	}
	else if(ActionNameToExecute.Equals(TEXT("AOE")))
	{
		return 7;
	}
	else if(ActionNameToExecute.Equals(TEXT("Explotador")))
	{
		return 8;
	}
	else if(ActionNameToExecute.Equals(TEXT("AutoAIM")))
	{
		return 9;
	}

	return 2;
}
