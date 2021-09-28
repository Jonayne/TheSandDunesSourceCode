// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "Shield.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "Spell.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"


// Sets default values
AShield::AShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ShieldAudioComponent = CreateDefaultSubobject<UAudioComponent>(FName("ShieldAudio"));
	RootComponent = ShieldAudioComponent;

	Box = CreateDefaultSubobject<UBoxComponent>(FName("ShieldBox"));
	Box->SetupAttachment(GetRootComponent());
	Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	
	ShieldNiagaraParticle = CreateDefaultSubobject<UNiagaraComponent>(FName("ShieldNiagaraParticle"));
	ShieldNiagaraParticle->SetupAttachment(GetRootComponent());

	ShieldDuration = 0.5f;
	ShieldWaitBeforeDestroy = 0.2f;
}

// Called when the game starts or when spawned
void AShield::BeginPlay()
{
	Super::BeginPlay();
	
	Spirit = Cast<ASpirit>(GetOwner());
	
	Box->IgnoreActorWhenMoving(Spirit, true);
	Box->IgnoreActorWhenMoving(this, true);

	Box->OnComponentBeginOverlap.AddDynamic(this, &AShield::OnOverlapBegin);

	EnemyOwner = Cast<AEnemy>(Spirit);

	ShieldNiagaraParticle->SetVariableFloat(FName("Duration"), ShieldDuration);
	GetWorldTimerManager().SetTimer(ShieldOnTimerHandle, this, &AShield::DestroyShield, ShieldDuration+0.5f, false); 

}

// Called every frame
void AShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShield::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
	if (OtherActor && OtherActor->IsA(ASpell::StaticClass()))
	{
		ASpell* OtherSpell = Cast<ASpell>(OtherActor);
		ASpirit* SpiritOwner = OtherSpell->GetOwnerSpirit();
		if(SpiritOwner && !SpiritOwner->IsA(AEnemy::StaticClass()))
		{
			OtherSpell->PlayHitEffects();
		
			if(Spirit)
			{
				Spirit->ModifyHealth(OtherSpell->GetSpellDamage() * .5f);
				Spirit->SpiritTakeDamage(0.f, TEXT("Arcanos"), SpiritOwner, Spirit->GetActorLocation(), true);

				if(EnemyOwner)
				{
					for (auto& Ally : EnemyOwner->ListOfCompanions)
					{
						if(Ally && !Ally->IsDead())
						{
							Ally->ModifyHealth(OtherSpell->GetSpellDamage() * .45f);
						}
					}
				}
			}
		}	
	}
}

void AShield::DestroyShield() 
{
	Destroy();
}

ASpirit* AShield::GetSpiritOwner() 
{
	return Spirit;
}
