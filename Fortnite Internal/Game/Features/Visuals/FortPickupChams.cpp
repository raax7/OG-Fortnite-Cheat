#include "Chams.h"

#include "../../SDK/Classes/FortniteGame_Classes.h"

std::vector<SDK::UMeshComponent*> Features::Visuals::ChamManagerFortPickup::GetActorMeshes(SDK::AActor* Actor)
{
    std::vector<SDK::UMeshComponent*> ActorMeshes;

    SDK::AFortPickup* Pickup = SDK::Cast<SDK::AFortPickup>(Actor);
    SDK::USkeletalMeshComponent* Skeletal_Mesh_Pickup = SDK::Cast<SDK::AB_Pickups_Parent_C>(Pickup->PickupEffectBlueprint().Get())->Skeletal_Mesh_Pickup();
    SDK::UStaticMeshComponent* Static_Mesh_Pickup = SDK::Cast<SDK::AB_Pickups_Parent_C>(Pickup->PickupEffectBlueprint().Get())->Static_Mesh_Pickup();

    if (SDK::IsValidPointer(Skeletal_Mesh_Pickup))
    {
        ActorMeshes.push_back(Skeletal_Mesh_Pickup);
    }
    if (SDK::IsValidPointer(Static_Mesh_Pickup))
    {
        ActorMeshes.push_back(Static_Mesh_Pickup);
    }

    return ActorMeshes;
}

bool Features::Visuals::ChamManagerFortPickup::ShouldApplyChamsCustom(SDK::AActor* Actor)
{
    if ((uint8)SDK::Cast<SDK::AFortPickup>(Actor)->PickupEffectBlueprint()->ItemDefinition()->Tier() < (uint8)reinterpret_cast<ConfigTypes::PickupChamsSettings&>(ChamSettings).MinimumTier)
    {
        return false;
    }

    return true;
}