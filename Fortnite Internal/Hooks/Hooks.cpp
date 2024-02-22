#include "Hooks.h"

#include "../Game/SDK/SDK.h"
#include "../Game/SDK/Classes/Engine_Classes.h"

#include "../Utilities/Logger.h"
#include "../Utilities/LazyImporter.h"
#include "../Configs/Config.h"

template <typename T>
Hooks::VFTHook::VFTHook(void** VFT, const uintptr_t VFTIndex, T& Original, void* Hook) {
	DEBUG_LOG(skCrypt("Create VFTHook called").decrypt());

	DWORD OldProtection{};
	LI_FN(VirtualProtect, &VFT[VFTIndex], sizeof(void*), PAGE_EXECUTE_READWRITE, &OldProtection).safe();

	Original = reinterpret_cast<T>(VFT[VFTIndex]);
	VFT[VFTIndex] = Hook;

	LI_FN(VirtualProtect, &VFT[VFTIndex], sizeof(void*), OldProtection, &OldProtection).safe();

	this->VFT = VFT;
	this->VFTIndex = VFTIndex;
	this->Original = Original;
}
Hooks::VFTHook::~VFTHook() {
	DEBUG_LOG(skCrypt("Destroy VFTHook called").decrypt());

	if (!VFT || !Original) {
		DEBUG_LOG(skCrypt("Failed to destroy hook! VFT or Original is nullptr").decrypt());
		return;
	}

	DWORD OldProtection{};
	LI_FN(VirtualProtect, &VFT[VFTIndex], sizeof(void*), PAGE_EXECUTE_READWRITE, &OldProtection).safe();

	VFT[VFTIndex] = Original;

	LI_FN(VirtualProtect, &VFT[VFTIndex], sizeof(void*), OldProtection, &OldProtection).safe();
}

void Hooks::Init() {
	PostRender::Hook = new Hooks::VFTHook(
		*(void***)(SDK::GetEngine()->GameViewport()),
		SDK::Cached::VFT::PostRender,
		Hooks::PostRender::PostRenderOriginal,
		Hooks::PostRender::PostRender);
}
void Hooks::Tick() {
	if (Config::Aimbot::SilentAim) {
		SDK::APlayerController* PlayerController = SDK::GetLocalController();
		if (!Hooks::GetPlayerViewpoint::Hook || (Hooks::GetPlayerViewpoint::PlayerControllerHooked != PlayerController && PlayerController)) {
			if (Hooks::GetPlayerViewpoint::Hook) delete Hooks::GetPlayerViewpoint::Hook;

			Hooks::GetPlayerViewpoint::Hook = new Hooks::VFTHook(
				*(void***)(PlayerController),
				SDK::Cached::VFT::GetPlayerViewpoint,
				Hooks::GetPlayerViewpoint::GetPlayerViewpointOriginal,
				Hooks::GetPlayerViewpoint::GetPlayerViewpoint);

			Hooks::GetPlayerViewpoint::PlayerControllerHooked = PlayerController;
		}

		SDK::ULocalPlayer* LocalPlayer = SDK::GetLocalPlayer();
		if (!Hooks::GetViewpoint::Hook || (Hooks::GetViewpoint::LocalPlayerHooked != LocalPlayer && LocalPlayer)) {
			if (Hooks::GetViewpoint::Hook) delete Hooks::GetViewpoint::Hook;

			Hooks::GetViewpoint::Hook = new Hooks::VFTHook(
				*(void***)(LocalPlayer),
				SDK::Cached::VFT::GetViewpoint,
				Hooks::GetViewpoint::GetViewpointOriginal,
				Hooks::GetViewpoint::GetViewpoint);

			Hooks::GetViewpoint::LocalPlayerHooked = LocalPlayer;
		}
	}
	else {
		if (Hooks::GetPlayerViewpoint::Hook) {
			delete Hooks::GetPlayerViewpoint::Hook;
			Hooks::GetPlayerViewpoint::Hook = nullptr;
			Hooks::GetPlayerViewpoint::PlayerControllerHooked = nullptr;
		}

		if (Hooks::GetViewpoint::Hook) {
			delete Hooks::GetViewpoint::Hook;
			Hooks::GetViewpoint::Hook = nullptr;
			Hooks::GetViewpoint::LocalPlayerHooked = nullptr;
		}
	}
}