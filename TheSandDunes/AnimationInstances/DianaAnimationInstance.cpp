// Copyright Jonathan Suarez Lopez, Jonayne, 2020-2021. Todos los derechos reservados.


#include "DianaAnimationInstance.h"
#include "GameFramework/Pawn.h"
#include "Diana.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

void UDianaAnimationInstance::NativeInitializeAnimation() 
{
  if (!Pawn) 
  {
      Pawn = TryGetPawnOwner();
      if(Pawn)
      {
          Diana = Cast<ADiana>(Pawn);
      }
  }
}

void UDianaAnimationInstance::UpdateAnimationVariables() 
{
    if (!Pawn) 
    {
      Pawn = TryGetPawnOwner();
    }

    if (Pawn)
    {
        Diana = Cast<ADiana>(Pawn);
        if(Diana)
        {
          Speed = Diana->GetSpiritSpeed();
          bInAir = Diana->IsInAir();
          bInAttackMode = Diana->bAttackMode;

          if(bInAttackMode)
          {
            if(!Montage_IsPlaying(Diana->DianaFightMontage))
            {
              Montage_Play(Diana->DianaFightMontage);
            }
          }
        } 
        else 
        {
          UE_LOG(LogTemp, Warning, TEXT("No se puede obtener informacion del Anim instance de Diana."));
        }
    }
}

void UDianaAnimationInstance::HandleDianaFootsteps(FName FootSocket) 
{
  if (!Pawn) 
    {
      Pawn = TryGetPawnOwner();
    }

    if (Pawn)
    {
      Diana = Cast<ADiana>(Pawn);
      if(Diana)
      {
        FHitResult Hit;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(Diana);
        QueryParams.bReturnPhysicalMaterial = true;
        QueryParams.bTraceComplex = true;

        FVector DianaLocation = Diana->GetActorLocation();
        FVector DianaFloorLocation = DianaLocation;
        DianaFloorLocation.Z -= 200.f;
        if(GetWorld()->LineTraceSingleByChannel(Hit, DianaLocation, DianaFloorLocation, ECollisionChannel::ECC_WorldStatic, QueryParams))
        {
           EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

           switch(SurfaceType)
           {
              case SurfaceType_Default:
                Diana->PlayFootstepsAudioAndEffects(TEXT("Default"), FootSocket);
                break;
              case SurfaceType1: // Arena
                Diana->PlayFootstepsAudioAndEffects(TEXT("Arena"), FootSocket);
                break;
              case SurfaceType2: // Tierra
                Diana->PlayFootstepsAudioAndEffects(TEXT("Tierra"), FootSocket);
                break;
              case SurfaceType3: // Piedra
                Diana->PlayFootstepsAudioAndEffects(TEXT("Piedra"), FootSocket);
    	          break;
              case SurfaceType4: // Grava
                Diana->PlayFootstepsAudioAndEffects(TEXT("Grava"), FootSocket);
    	          break;
              case SurfaceType5: // Agua
                Diana->PlayFootstepsAudioAndEffects(TEXT("Agua"), FootSocket);                
    	          break;
              case SurfaceType6: // Nieve
                Diana->PlayFootstepsAudioAndEffects(TEXT("Nieve"), FootSocket);                
    	          break;
              case SurfaceType7: // Nieve
                Diana->PlayFootstepsAudioAndEffects(TEXT("PisoDivino"), FootSocket);                
    	          break;
              default:
                  break;
           }
        }
      }
    }
}
