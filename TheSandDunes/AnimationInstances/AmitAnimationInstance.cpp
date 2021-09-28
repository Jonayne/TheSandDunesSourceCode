// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "AmitAnimationInstance.h"
#include "GameFramework/Pawn.h"
#include "Amit.h"

void UAmitAnimationInstance::NativeInitializeAnimation() 
{
  if (!Pawn) 
  {
      Pawn = TryGetPawnOwner();
      if(Pawn)
      {
          Amit = Cast<AAmit>(Pawn);
      }
  }
}

void UAmitAnimationInstance::UpdateAnimationVariables() 
{
    if (!Pawn) 
    {
      Pawn = TryGetPawnOwner();
    }

    if (Pawn)
    {
        Amit = Cast<AAmit>(Pawn);
        if(Amit)
        {
          Speed = Amit->GetSpiritSpeed();
          bInAir = Amit->IsInAir();
        } 
        else 
        {
          UE_LOG(LogTemp, Warning, TEXT("No se puede obtener informacion del Anim instance de Amit."));
        }
    }
}
