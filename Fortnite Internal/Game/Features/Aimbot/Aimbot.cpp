#include "Aimbot.h"

#include "../../../Configs/Config.h"
#include "../../Input/Input.h"
#include "Target.h"

#include "../../Actors/Actors.h"

void Features::Aimbot::AimbotTarget(Target& TargetToAimot) {
	if (Input::IsKeyDown((Input::KeyName)Config::Aimbot::AimKey)) {
		TargetToAimot.LocalInfo.IsTargeting = true;

		if (Config::Aimbot::SilentAim == false) {
			SDK::GetLocalController()->ClientSetRotation(TargetToAimot.LocalInfo.TargetRotationWithSmooth, false);
		}
	}
	else {
		TargetToAimot.LocalInfo.IsTargeting = false;
	}
}

// Moved functionality of GetPlayerViewpoint hook and GetViewpoint hook from "Hooks/Callbacks" to here for better organization

void Features::Aimbot::GetViewpointCallback(SDK::FMinimalViewInfo* OutViewInfo) {
	if (!Config::Aimbot::SilentAim) return;
	if (!Actors::MainTarget.LocalInfo.IsTargeting && Config::Aimbot::UseAimKeyForSilent) return;

	if (Actors::MainTarget.GlobalInfo.TargetActor) {
		OutViewInfo->SetRotation(Actors::MainCamera.Rotation);
		OutViewInfo->SetLocation(Actors::MainCamera.Position);
	}
}

void Features::Aimbot::GetPlayerViewpointCallback(SDK::FRotator* Rotation) {
	if (!Config::Aimbot::SilentAim) return;
	if (!Actors::MainTarget.LocalInfo.IsTargeting && Config::Aimbot::UseAimKeyForSilent) return;

	if (Actors::MainTarget.GlobalInfo.TargetActor) {
		*Rotation = Actors::MainTarget.LocalInfo.TargetRotationWithSmooth;
	}
}