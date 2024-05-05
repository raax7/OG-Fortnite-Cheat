#include "Chams.h"

#include "../../SDK/Classes/FortniteGame_Classes.h"

std::vector<SDK::UMeshComponent*> Features::Visuals::ChamManagerFortPawn::GetActorMeshes(SDK::AActor* Actor)
{
    std::vector<SDK::USkeletalMeshComponentBudgeted*> CharacterParts = SDK::Cast<SDK::AFortPlayerPawn>(Actor)->GetCharacterPartSkeletalMeshComponents();

    std::vector<SDK::UMeshComponent*> ActorMeshes;
    for (SDK::USkeletalMeshComponentBudgeted* Part : CharacterParts)
    {
        if (SDK::IsValidPointer(Part))
        {
            ActorMeshes.push_back(Part);
        }
    }

    return ActorMeshes;
}

bool Features::Visuals::ChamManagerFortPawn::ShouldApplyChamsCustom(SDK::AActor* Actor)
{
    if (ChamSettings.ShowLocal == false && (Actor == SDK::GetLocalPawn()))
    {
        return false;
    }

    return true;
}