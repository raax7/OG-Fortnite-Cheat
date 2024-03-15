#include "../Hooks.h"

#include "../../Game/Actors/Actors.h"
#include "../../Configs/Config.h"

#include "../../Game/Features/Aimbot/Aimbot.h"

void Hooks::GetViewpoint::GetViewpoint(void* this_, SDK::FMinimalViewInfo* OutViewInfo, SDK::EStereoscopicPass StereoPass) {
	GetViewpointOriginal(this_, OutViewInfo, StereoPass);
	//spoof_call<void>(GetViewpointOriginal, this_, OutViewInfo, StereoPass);

	Features::Aimbot::GetViewpointCallback(OutViewInfo);
}