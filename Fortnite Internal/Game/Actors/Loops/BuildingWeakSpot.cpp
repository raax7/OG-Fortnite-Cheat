#include "../Actors.h"

#include "../../SDK/Classes/Engine_classes.h"

#include "../../Features/Aimbot/Aimbot.h"
#include "../../../Configs/Config.h"

void Actors::BuildingWeakSpot::Tick() {
	bool SeenTarget = false;

	for (int i = 0; i < CachedBuildingWeakSpot.Num(); i++) {
		if (Config::Aimbot::Weakspot::Enabled == false) break;
		if (reinterpret_cast<SDK::AFortPawn*>(SDK::GetLocalPawn())->CurrentWeapon()->IsPickaxe() == false) break;

		// ADD CHECK FOR IF CURRENT WEAPON IS A GUN OR A PICKAXE
		//if (reinterpret_cast<SDK::AFortPawn*>(SDK::GetLocalController()->AcknowledgedPawn())->CurrentWeapon()) break;
		if (CachedBuildingWeakSpot.IsValidIndex(i) == false) continue;

		SDK::AActor* Actor = CachedBuildingWeakSpot[i];									if (!Actor) continue;
		SDK::ABuildingWeakSpot* WeakSpot = static_cast<SDK::ABuildingWeakSpot*>(Actor);	if (!WeakSpot) continue;

		SDK::FVector RootPosition = Actor->GetRootComponent()->GetPosition();
		float DistanceFromLocal = LocalPawnCache.Position.Distance(RootPosition) / 100.f;

		if (DistanceFromLocal > 5) continue;

		if (WeakSpot->GetWeakSpotInfo() & 0x4 && !(WeakSpot->GetWeakSpotInfo() & 0x2) && !(WeakSpot->GetWeakSpotInfo() & 0x1)) {
			// Aimbot
			if (Config::Aimbot::Enabled && SDK::GetLocalController()->AcknowledgedPawn()) {
				if ((MainTarget.LocalInfo.IsTargeting == false || MainTarget.GlobalInfo.TargetActor == nullptr)) {
					Features::Aimbot::Target PotentialNewTarget{};

					Features::Aimbot::WeakSpotTarget::UpdateTargetInfo(PotentialNewTarget, WeakSpot, AimbotCamera);
					MainTarget.SetTarget(PotentialNewTarget);
				}

				if (MainTarget.GlobalInfo.TargetActor == WeakSpot) {
					SeenTarget = true;

					Features::Aimbot::WeakSpotTarget::UpdateTargetInfo(MainTarget, WeakSpot, MainCamera, AimbotCamera, FPSScale);

					Features::Aimbot::AimbotTarget(MainTarget);
				}
			}
		}
	}

	if (MainTarget.GlobalInfo.Type == Features::Aimbot::Target::TargetType::Weakspot) {
		MainTarget.TargetTick(SeenTarget);
	}
}