#include "../Actors.h"
#include "../../SDK/SDK.h"
#include "../../SDK/Classes/Engine_classes.h"
#include "../../../Drawing/Drawing.h"
#include "../../../Hooks/Hooks.h"
#include "../../../Utilities/Memory.h"
#include "../../Features/Aimbot/Aimbot.h"

void Actors::BuildingWeakSpot::Tick() {
	if (SDK::GetLocalCanvas() == nullptr) return;

	bool SeenTarget = false;

	for (int i = 0; i < CachedBuildingWeakSpot.Num(); i++) {
		if (Config::Aimbot::Weakspot::Enabled == false) break;

		// ADD CHECK FOR IF CURRENT WEAPON IS A GUN OR A PICKAXE
		//if (reinterpret_cast<SDK::AFortPawn*>(SDK::GetLocalController()->AcknowledgedPawn())->CurrentWeapon()) break;
		if (CachedBuildingWeakSpot.IsValidIndex(i) == false) continue;

		SDK::AActor* Actor = CachedBuildingWeakSpot[i];									if (!Actor) continue;
		SDK::ABuildingWeakSpot* WeakSpot = static_cast<SDK::ABuildingWeakSpot*>(Actor);	if (!WeakSpot) continue;

		SDK::FVector RootPosition = Actor->GetRootComponent()->GetPosition();
		float DistanceFromLocal = localPlayer.Position.Distance(RootPosition) / 100.f;

		if (DistanceFromLocal > 5) continue;

		if (WeakSpot->GetWeakSpotInfo() & 0x4 && !(WeakSpot->GetWeakSpotInfo() & 0x2) && !(WeakSpot->GetWeakSpotInfo() & 0x1)) {
			// Aimbot
			if (Config::Aimbot::Enabled && SDK::GetLocalController()->AcknowledgedPawn()) {
				if ((target.LocalInfo.IsTargeting == false || target.GlobalInfo.TargetActor == nullptr)) {
					Features::Aimbot::Target PotentialNewTarget{};

					Features::Aimbot::WeakSpotTarget::UpdateTargetInfo(PotentialNewTarget, WeakSpot);
					target.SetTarget(PotentialNewTarget);
				}

				if (target.GlobalInfo.TargetActor == WeakSpot) {
					SeenTarget = true;

					Features::Aimbot::WeakSpotTarget::UpdateTargetInfo(target, WeakSpot, AimbotCamera, FPSScale);

					Features::Aimbot::Aimbot::AimbotTarget(target);
				}
			}
		}
	}

	if (target.GlobalInfo.Type == Features::Aimbot::Target::TargetType::Weakspot) {
		target.TargetTick(SeenTarget);
	}
}