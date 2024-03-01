#include "../Hooks.h"

#include "../../Game/Actors/Actors.h"
#include "../../Configs/Config.h"

void Hooks::GetPlayerViewpoint::GetPlayerViewpoint(void* this_, SDK::FVector* Location, SDK::FRotator* Rotation) {
	GetPlayerViewpointOriginal(this_, Location, Rotation);
	//spoof_call<void>(GetPlayerViewpointOriginal, this_, Location, Rotation);

	if (!Config::Aimbot::SilentAim) return;
	if (!Actors::MainTarget.LocalInfo.IsTargeting && Config::Aimbot::UseAimKeyForSilent) return;

	if (Actors::MainTarget.GlobalInfo.TargetActor) {
		*Rotation = Actors::MainTarget.LocalInfo.TargetRotationWithSmooth;
	}
}