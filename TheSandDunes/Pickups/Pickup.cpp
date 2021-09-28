// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "Pickup.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "NiagaraComponent.h"
#include "TheSandDunesGameInstance.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PickupCapsule = CreateDefaultSubobject<UCapsuleComponent>(FName("PickupCapsule"));
	RootComponent = PickupCapsule;

	PickupStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("PickupStaticMesh"));
	PickupStaticMesh->SetupAttachment(GetRootComponent());

	PickupConstantParticle = CreateDefaultSubobject<UParticleSystemComponent>(FName("PickupConstantParticle"));
	PickupConstantParticle->SetupAttachment(GetRootComponent());

	PickupConstantAudio = CreateDefaultSubobject<UAudioComponent>(FName("PickupConstantAudio"));
	PickupConstantAudio->SetupAttachment(GetRootComponent());
	PickupConstantAudio->bAutoActivate = false;	

	PickupConstantNiagaraParticle = CreateDefaultSubobject<UNiagaraComponent>(FName("PickupConstantNiagaraParticle"));
	PickupConstantNiagaraParticle->SetupAttachment(GetRootComponent());

	PickupParticle = CreateDefaultSubobject<UParticleSystemComponent>(FName("PickupParticle"));
	PickupParticle->SetupAttachment(GetRootComponent());
	PickupParticle->bAutoActivate = false;	

	PickupAudio = CreateDefaultSubobject<UAudioComponent>(FName("PickupAudio"));
	PickupAudio->SetupAttachment(GetRootComponent());
	PickupAudio->bAutoActivate = false;

	PickupNiagaraParticle = CreateDefaultSubobject<UNiagaraComponent>(FName("PickupNiagaraParticle"));
	PickupNiagaraParticle->SetupAttachment(GetRootComponent());
	PickupNiagaraParticle->bAutoActivate = false;

	PickupActivateAudio = CreateDefaultSubobject<UAudioComponent>(FName("PickupActivateAudio"));
	PickupActivateAudio->SetupAttachment(GetRootComponent());
	PickupActivateAudio->bAutoActivate = false;

	PickupStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); 

	bShouldMakeConstantSound = false;
	WaitForDestroy = 3.f;

	bPickupActivated = true;
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	if(!bPickupActivated)
	{
		SetActorHiddenInGame(true);
	}
	else if(bShouldMakeConstantSound)
	{
		if(PickupConstantAudio) PickupConstantAudio->Activate();
	}

	PickupCapsule->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnOverlapBegin);
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
}

void APickup::ActivatePickup() 
{
	if(!bPickupActivated)
	{
		if(PickupActivateAudio) PickupActivateAudio->Activate();
		bPickupActivated = true;
		SetActorHiddenInGame(false);
		if(bShouldMakeConstantSound)
		{
			if(PickupConstantAudio) PickupConstantAudio->Activate();
		}
	}
}

void APickup::DeactivatePickup() 
{
	if(bPickupActivated)
	{
		bPickupActivated = false;
		SetActorHiddenInGame(true);
		if(PickupConstantAudio) PickupConstantAudio->Deactivate();
	}
}

void APickup::PlayPickupEffects() 
{
	if(PickupConstantNiagaraParticle) PickupConstantNiagaraParticle->Deactivate();
	if(PickupNiagaraParticle) PickupNiagaraParticle->Deactivate();
	if(PickupStaticMesh) PickupStaticMesh->SetHiddenInGame(true);
	if(PickupNiagaraParticle) PickupNiagaraParticle->Activate(true);
	if(PickupAudio) PickupAudio->Activate(true);
	if(PickupParticle) PickupParticle->Activate(true);

	ShowPickupWidget();

	SavableInfo.bIsAlive = false;
	ISavableActorInterface::Execute_ChangeActorState(this);	

	if(PickupAudio) PickupAudio->FadeOut(WaitForDestroy-(WaitForDestroy*.1f), 0.f, EAudioFaderCurve::Linear);
	if(PickupConstantAudio && bShouldMakeConstantSound) PickupConstantAudio->FadeOut(WaitForDestroy-(WaitForDestroy*.1f), 0.f, EAudioFaderCurve::Linear);
	GetWorldTimerManager().SetTimer(PickupEffectsHandler, this, &APickup::DestroyPickup, WaitForDestroy+1.f, false);
}

void APickup::DestroyPickup() 
{
	Destroy();
}

FSavableActor APickup::GetSavableActorInfo_Implementation() const 
{
	return SavableInfo;
}

void APickup::ChangeActorState_Implementation() 
{
	UWorld* World = GetWorld();
	UTheSandDunesGameInstance* GameInstance = World ? World->GetGameInstance<UTheSandDunesGameInstance>() : nullptr;
	
	if (GameInstance)
	{
		GameInstance->ChangeSavableActorState(UGameplayStatics::GetCurrentLevelName(this, true), SavableInfo);
	}
}

void APickup::UpdateSavableInfo_Implementation(const FSavableActor& NewActorInfo) 
{
	SavableInfo = NewActorInfo;
	if(!(SavableInfo.bIsAlive || SavableInfo.bShouldRespawn))
	{
		Destroy();
	}
}
