#include "Aimbot.h"

#include "Target.h"
#include "../../Input/Input.h"
#include "../../../Configs/Config.h"

void Features::Aimbot::AimbotTarget(Target& TargetToAimot) {
	// CONVERT TO ENGINE KEY DETECTION
	if (Input::IsKeyDown(Input::KeyName::RightMouseButton)) {
		TargetToAimot.LocalInfo.IsTargeting = true;

		if (Config::Aimbot::SilentAim == false) {
			SDK::GetLocalController()->ClientSetRotation(TargetToAimot.LocalInfo.TargetRotationWithSmooth, false);
		}
	}
	else {
		TargetToAimot.LocalInfo.IsTargeting = false;
	}

	if (TargetToAimot.GlobalInfo.Type == Target::TargetType::ClosePlayer || TargetToAimot.GlobalInfo.Type == Target::TargetType::FarPlayer) {
		//if (TargetToAimot.GlobalInfo.PawnCache.AnyBoneVisible == false) {
		//	TargetToAimot.ResetTarget();
		//}
	}
}