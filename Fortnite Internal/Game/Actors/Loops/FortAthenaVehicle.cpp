#include "../Actors.h"

#include "../../SDK/Classes/Engine_classes.h"
#include "../../SDK/Classes/FortniteGame_classes.h"

#include "../../../Drawing/Drawing.h"

#include "../../../Configs/Config.h"

#include "../../../Utilities/Math.h"

void Actors::FortAthenaVehicle::Tick()
{
    for (int i = 0; i < CachedVehicles.Num(); i++)
    {
        if (Config::Visuals::Vehicles::Enabled)
        {
            if (CachedVehicles.IsValidIndex(i) == false) continue;

            SDK::AActor* Actor = CachedVehicles[i]; if (SDK::IsValidPointer(Actor) == false) continue;
            SDK::AFortAthenaVehicle* FortAthenaVehicle = SDK::Cast<SDK::AFortAthenaVehicle>(Actor); if (SDK::IsValidPointer(FortAthenaVehicle) == false) continue;

            SDK::FVector RootPosition = Actor->RootComponent()->RelativeLocation();
            if (RootPosition.Distance(Actors::LocalPawnCache.Position) / 100 > Config::Visuals::Vehicles::MaxDistance)
            {
                continue;
            }

            if (SDK::Cast<SDK::AFortPlayerPawn>(SDK::GetLocalPawn())->VehicleStateLocal()->Vehicle() == FortAthenaVehicle)
            {
                continue;
            }

            SDK::FVector2D Project = SDK::Project(RootPosition);
            if (Math::IsOnScreen(Project) == false) continue;

            std::string VehicleName = FortAthenaVehicle->Class->GetName();
            SDK::FLinearColor VehicleColor = SDK::FLinearColor(1.f, 1.f, 1.f, 1.f);

            Drawing::Text(VehicleName.c_str(), Project, 16.f, VehicleColor, true, true, true);
        }
    }
}