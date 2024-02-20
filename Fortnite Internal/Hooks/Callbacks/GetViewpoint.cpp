#include "../Hooks.h"
#include "../../Game/Actors/Actors.h"
#include "../../Drawing/Drawing.h"

void Hooks::GetViewpoint::GetViewpoint(void* this_, SDK::FMinimalViewInfo* OutViewInfo, SDK::EStereoscopicPass StereoPass) {
	GetViewpointOriginal(this_, OutViewInfo, StereoPass);
	
	if (!Config::Aimbot::SilentAim) return;
	if (!Actors::target.LocalInfo.IsTargeting && Config::Aimbot::UseAimKeyForSilent) return;

	if (Actors::target.GlobalInfo.TargetActor && !Config::Aimbot::ShowSilentRotations) {
		OutViewInfo->SetRotation(Actors::RealCamera.Rotation);
	}
}