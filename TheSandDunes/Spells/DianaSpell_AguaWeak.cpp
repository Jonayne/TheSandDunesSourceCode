// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "DianaSpell_AguaWeak.h"
#include "Diana.h"
#include "AguaArea.h"

void ADianaSpell_AguaWeak::BeginPlay() 
{
    Super::BeginPlay();

	if(StaminaAreaClass)
    {
        FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwnerPawn;
        if(OwnerPawn)
        {
            AAguaArea* AreaRef = GetWorld()->SpawnActor<AAguaArea>(StaminaAreaClass, SpawnParams);
            AreaRef->AttachToActor(OwnerPawn, FAttachmentTransformRules::KeepRelativeTransform);
        }
    }
}
