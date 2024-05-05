#include "../Actors.h"

#include "../../SDK/Classes/Engine_classes.h"

#include "../../../Configs/Config.h"
#include "../../Features/Aimbot/Aimbot.h"

void Actors::BuildingWeakSpot::Tick()
{
    bool SeenTarget = false;

    for (int i = 0; i < CachedBuildingWeakSpot.Num(); i++)
    {
        if (Config::Aimbot::Weakspot::Enabled == false) break;
        if (SDK::Cast<SDK::AFortPawn>(SDK::GetLocalPawn())->CurrentWeapon()->IsPickaxe() == false) break;

        SDK::AActor* Actor = CachedBuildingWeakSpot[i];									if (SDK::IsValidPointer(Actor) == false) continue;
        SDK::ABuildingWeakSpot* WeakSpot = SDK::Cast<SDK::ABuildingWeakSpot>(Actor);	if (SDK::IsValidPointer(WeakSpot) == false) continue;

        SDK::FVector RootPosition = Actor->RootComponent()->RelativeLocation();
        float DistanceFromLocal = LocalPawnCache.Position.Distance(RootPosition) / 100.f;

        // Max distance from local player
        if (DistanceFromLocal > 5.f) continue;

        if (WeakSpot->bActive() && WeakSpot->bFadeOut() == false && WeakSpot->bHit() == false)
        {
            // Aimbot
            if (Config::Aimbot::Enabled && SDK::GetLocalController()->AcknowledgedPawn())
            {
                if (((MainTarget.LocalInfo.IsTargeting == false || Config::Aimbot::StickyAim == false) || MainTarget.GlobalInfo.TargetActor == nullptr))
                {
                    Features::Aimbot::Target PotentialNewTarget{};

                    Features::Aimbot::WeakSpotTarget::UpdateTargetInfo(PotentialNewTarget, WeakSpot, MainCamera, AimbotCamera);
                    MainTarget.SetTarget(PotentialNewTarget);
                }

                if (MainTarget.GlobalInfo.TargetActor == WeakSpot)
                {
                    SeenTarget = true;

                    Features::Aimbot::WeakSpotTarget::UpdateTargetInfo(MainTarget, WeakSpot, MainCamera, AimbotCamera, FPSScale);

                    Features::Aimbot::AimbotTarget(MainTarget);
                }
            }
        }
    }

    if (MainTarget.GlobalInfo.Type == Features::Aimbot::Target::TargetType::Weakspot)
    {
        MainTarget.TargetTick(SeenTarget);
    }
}