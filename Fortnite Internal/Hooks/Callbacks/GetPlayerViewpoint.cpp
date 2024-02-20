#include "../Hooks.h"
#include "../../Game/Actors/Actors.h"
#include "../../Drawing/Drawing.h"

void Hooks::GetPlayerViewpoint::GetPlayerViewpoint(void* this_, SDK::FVector* Location, SDK::FRotator* Rotation) {
	GetPlayerViewpointOriginal(this_, Location, Rotation);

	if (!Config::Aimbot::SilentAim) return;
	if (!Actors::target.LocalInfo.IsTargeting && Config::Aimbot::UseAimKeyForSilent) return;

	if (Actors::target.GlobalInfo.TargetActor) {
		*Rotation = Actors::target.LocalInfo.TargetRotationWithSmooth;
	}
}