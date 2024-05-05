#include "Chams.h"

#include "../../Game.h"

Features::Visuals::ChamManagerFortPawn* Features::Visuals::ChamManagerFortPawn::Manager = nullptr;
Features::Visuals::ChamManagerFortPickup* Features::Visuals::ChamManagerFortPickup::Manager = nullptr;

void Features::Visuals::ChamManager::RevertAll()
{
    for (auto& Actor : ChamActorsCache)
    {
        RemoveChams(Actor.first);
    }

    ChamActorsCache.clear();
}

void Features::Visuals::ChamManager::UpdateDynamicMaterialSettings()
{
    // Set the colors
    SDK::FLinearColor Color = SDK::FLinearColor(ChamSettings.Color[0], ChamSettings.Color[1], ChamSettings.Color[2], 1.f);
    for (auto& ColorParameter : MaterialColorParameters)
    {
        GetDynamicMaterial()->SetVectorParameterValue(ColorParameter, Color);
    }

    // Set the emissive brightness
    float EmissiveIntensity = ChamSettings.EmissiveIntensity;
    for (auto& EmissiveParameter : MaterialEmissiveParameters)
    {
        GetDynamicMaterial()->SetScalarParameterValue(EmissiveParameter, EmissiveIntensity);
    }
}

void Features::Visuals::ChamManager::UpdateMaterialSettings()
{
    GetMaterial()->SetbDisableDepthTest(ChamSettings.ThroughWalls ? true : false, &ChamSettings.Enabled);
    GetMaterial()->SetBlendMode(SDK::EBlendMode::BLEND_Additive, &ChamSettings.Enabled);
    GetMaterial()->SetWireFrame(ChamSettings.Wireframe, &ChamSettings.Enabled);
}

void Features::Visuals::ChamManager::Tick(SDK::AActor* Actor)
{
    // Update the material settings every tick if chams are enabled (they often get reverted by the game)
    if (ChamSettings.Enabled)
    {
        UpdateMaterialSettings();
    }

    bool ActorInList = ChamActorsCache.find(Actor) != ChamActorsCache.end();

    if (ShouldApplyChams(Actor))
    {
        if (ActorInList == false)
        {
            // Apply the chams
            if (ApplyChams(Actor, GetDynamicMaterial()))
            {
                ChamActorsCache[Actor] = std::make_unique<ChamActor>(Actor, Game::CurrentTime);
            }
        }
        else
        {
            // Re-apply the chams if the delay has passed
            auto& ChamActor = ChamActorsCache[Actor];
            if (Game::CurrentTime - ChamActor->LastApplyTime > std::chrono::seconds(ReapplyChamsDelay))
            {
                if (ApplyChams(Actor, GetDynamicMaterial()))
                {
                    ChamActor->LastApplyTime = Game::CurrentTime;
                }
            }
        }
    }
    else if (ActorInList)
    {
        // If the pawn is not valid, remove it and the chams
        RemoveChams(Actor);
        ChamActorsCache.erase(Actor);
    }
}

void Features::Visuals::ChamManager::Init(const std::vector<SDK::FName> MaterialColorParameters, const std::vector<SDK::FName> MaterialEmissiveParameters, const std::string MaterialName)
{
    // Initialize the FName's
    this->MaterialColorParameters = MaterialColorParameters;
    this->MaterialEmissiveParameters = MaterialEmissiveParameters;

    // Set the material name
    this->MaterialName = MaterialName;

    // Mark the ChamManager as initiated
    Initiated = true;
}

bool Features::Visuals::ChamManager::ShouldApplyChams(SDK::AActor* Actor)
{
    if (SDK::IsValidPointer(Actor) == false)
    {
        return false;
    }

    if (ChamSettings.Enabled == false)
    {
        return false;
    }

    if (ShouldApplyChamsCustom(Actor) == false)
    {
        return false;
    }

    return true;
}

bool Features::Visuals::ChamManager::ApplyChams(SDK::AActor* Actor, SDK::UMaterialInstanceDynamic* DynamicMaterial, const bool SendingNullptrMaterial)
{
    // You can send a nullptr material to revert materials
    if (SendingNullptrMaterial == false && IsDynamicMaterialValid(DynamicMaterial) == false)
    {
        return false;
    }

    std::vector<SDK::UMeshComponent*> ActorMeshes = GetActorMeshes(Actor);

    for (auto Mesh : ActorMeshes)
    {
        if (SDK::IsValidPointer(Mesh) == false) continue;
        SDK::TArray<SDK::UMaterialInterface*> Materials = Mesh->GetMaterials();

        for (int i = 0; i < Materials.Num(); i++)
        {
            if (Materials.IsValidIndex(i) == false) continue;

            if (SendingNullptrMaterial == true || IsDynamicMaterialValid(DynamicMaterial))
            {
                Mesh->SetMaterial(i, DynamicMaterial);
            }
        }
    }

    return true;
}

void Features::Visuals::ChamManager::RemoveChams(SDK::AActor* Actor)
{
    ApplyChams(Actor, nullptr, true);
}

bool Features::Visuals::ChamManager::IsDynamicMaterialValid(SDK::UMaterialInstanceDynamic* DynamicMaterial)
{
    // This handles the case of the dynamic material being a dangling pointer

    if (SDK::IsValidPointer(DynamicMaterial) == false
        || SDK::IsValidPointer(DynamicMaterial->Class) == false
        || DynamicMaterial->IsA(SDK::UMaterialInstanceDynamic::StaticClass()) == false)
    {
        return false;
    }

    return true;
}

SDK::UMaterial* Features::Visuals::ChamManager::GetMaterial()
{
    if (!ChamsMaterial)
        ChamsMaterial = SDK::UObject::FindObject<SDK::UMaterial>(MaterialName);

    return ChamsMaterial;
}

SDK::UMaterialInstanceDynamic* Features::Visuals::ChamManager::GetDynamicMaterial()
{
    // When switching levels, the dynamic material can sometimes become invalid. So we have to do this to re-init the material
    if (IsDynamicMaterialValid(ChamsMaterialInstance) == false)
    {
        // Create a new dynamic material instance
        ChamsMaterialInstance = SDK::UKismetMaterialLibrary::CreateDynamicMaterialInstance(SDK::GetWorld(), GetMaterial(), SDK::FName());

        // Update the material settings (since we made a new material)
        UpdateDynamicMaterialSettings();
    }

    return ChamsMaterialInstance;
}