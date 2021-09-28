// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "AguaArea.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Enemy.h"
#include "Diana.h"


// Sets default values
AAguaArea::AAguaArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereArea = CreateDefaultSubobject<USphereComponent>(FName("SphereArea"));
	RootComponent = SphereArea;
	SphereArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SphereArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Overlap);

	AguaAreaAudioComponent = CreateDefaultSubobject<UAudioComponent>(FName("AguaAreaAudio"));
	AguaAreaAudioComponent->SetupAttachment(GetRootComponent());

	AguaAreaHitAudioComponent = CreateDefaultSubobject<UAudioComponent>(FName("AguaAreaHitAudioComponent"));
	AguaAreaHitAudioComponent->SetupAttachment(GetRootComponent());
	AguaAreaHitAudioComponent->bAutoActivate = false;

	AguaAreaParticle = CreateDefaultSubobject<UParticleSystemComponent>(FName("AguaAreaParticle"));
	AguaAreaParticle->SetupAttachment(GetRootComponent());
	
	AguaAreaNiagaraParticle = CreateDefaultSubobject<UNiagaraComponent>(FName("AguaAreaNiagaraParticle"));
	AguaAreaNiagaraParticle->SetupAttachment(GetRootComponent());

	AguaAreaDuration = 10.f;
	WaitBeforeDestroy = 1.f;
	StaminaRecoverQuantity = 400.f;
	AguaDashDistance = 2000.f;
}

// Called when the game starts or when spawned
void AAguaArea::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerPawn = GetOwner();
	if(OwnerPawn)
	{
		DianaOwner = Cast<ADiana>(OwnerPawn);
		SphereArea->IgnoreActorWhenMoving(OwnerPawn, true);
	}

	SphereArea->OnComponentBeginOverlap.AddDynamic(this, &AAguaArea::OnOverlapBegin);
	GetWorldTimerManager().SetTimer(AguaAreaTimerHandler, this, &AAguaArea::StopAguaArea, AguaAreaDuration, false);
	if(DianaOwner)
	{
		DianaOwner->MakeSpecialDash(AguaAreaDuration, AguaDashDistance);
	}
}

void AAguaArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
	if(DianaOwner && OtherActor && OtherActor->IsA(AEnemy::StaticClass()))
	{
		ASpirit* SpiritHit = Cast<AEnemy>(OtherActor);

		SpiritHit->SpiritTakeDamage(StaminaRecoverQuantity, TEXT("Agua"), DianaOwner, SpiritHit->GetActorLocation(), false);
		if(AguaAreaHitAudioComponent) AguaAreaHitAudioComponent->Activate(true);
		DianaOwner->ModifyStamina(StaminaRecoverQuantity);
	}
}

void AAguaArea::StopAguaArea() 
{
	if(AguaAreaParticle) AguaAreaParticle->Deactivate();
	if(AguaAreaNiagaraParticle) AguaAreaNiagaraParticle->Deactivate();
	
	SphereArea->OnComponentBeginOverlap.Clear();
	SphereArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWorldTimerManager().SetTimer(AguaAreaDestroyTimerHandler, this, &AAguaArea::DestroyAguaArea, WaitBeforeDestroy, false);
}

void AAguaArea::DestroyAguaArea() 
{
	Destroy();
}
