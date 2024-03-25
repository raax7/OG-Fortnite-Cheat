#include "../Hooks.h"

#include "../../Game/Actors/Actors.h"
#include "../../Configs/Config.h"

#include "../../Game/Features/Aimbot/Aimbot.h"

void Hooks::GetPlayerViewpoint::GetPlayerViewpoint(void* this_, SDK::FVector* Location, SDK::FRotator* Rotation) {
	GetPlayerViewpointOriginal(this_, Location, Rotation);
	//spoof_call<void>(GetPlayerViewpointOriginal, this_, Location, Rotation);

	Features::Aimbot::GetPlayerViewpointCallback(Location, Rotation);
}