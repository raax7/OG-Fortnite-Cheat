#include "Aimbot.h"

#include "Target.h"
#include "../../Input/Input.h"
#include "../../../Configs/Config.h"

void Features::Aimbot::AimbotTarget(Target& TargetToAimot) {
	if (Input::IsKeyDown(Input::KeyName::RightMouseButton)) {
		TargetToAimot.LocalInfo.IsTargeting = true;

		if (Config::Aimbot::SilentAim == false) {
			SDK::GetLocalController()->ClientSetRotation(TargetToAimot.LocalInfo.TargetRotationWithSmooth, false);
		}
	}
	else {
		TargetToAimot.LocalInfo.IsTargeting = false;
	}
}