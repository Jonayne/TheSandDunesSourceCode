// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "DianaSpell_SagradoStrong.h"
#include "SummonDianaHelp.h"
#include "Spirit.h"


void ADianaSpell_SagradoStrong::BeginPlay() 
{
	Super::BeginPlay();

    if(SummonClass)
    {
        FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwnerPawn;
        if(OwnerPawn)
        {
            ASummonDianaHelp* SummonsRef = GetWorld()->SpawnActor<ASummonDianaHelp>(SummonClass, SpawnParams);
            SummonsRef->AttachToActor(OwnerPawn, FAttachmentTransformRules::KeepRelativeTransform);
        }
    }
}
