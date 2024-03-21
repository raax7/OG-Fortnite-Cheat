#include "../Hooks.h"

#include "../../Game/Features/Aimbot/Aimbot.h"

SDK::FTransform* Hooks::CalculateShot::CalculateShotHook(void** FortPlayerPawnCalculateShotVFT, uintptr_t arg1, uintptr_t arg2) {
	SDK::FTransform* ReturnValue = CalculateShotOriginal(FortPlayerPawnCalculateShotVFT, arg1, arg2);

	if (SDK::IsValidPointer(ReturnValue)) {
		Features::Aimbot::CalculateShotCallback(ReturnValue);
	}

	return ReturnValue;
}