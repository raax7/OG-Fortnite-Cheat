#include "../Hooks.h"

#include "../../Game/Actors/Actors.h"
#include "../../Configs/Config.h"

void Hooks::GetViewpoint::GetViewpoint(void* this_, SDK::FMinimalViewInfo* OutViewInfo, SDK::EStereoscopicPass StereoPass) {
	GetViewpointOriginal(this_, OutViewInfo, StereoPass);
	//spoof_call<void>(GetViewpointOriginal, this_, OutViewInfo, StereoPass);
	
	if (!Config::Aimbot::SilentAim) return;
	if (!Actors::MainTarget.LocalInfo.IsTargeting && Config::Aimbot::UseAimKeyForSilent) return;

	if (Actors::MainTarget.GlobalInfo.TargetActor) {
		OutViewInfo->SetRotation(Actors::MainCamera.Rotation);
		OutViewInfo->SetLocation(Actors::MainCamera.Position);
	}
}