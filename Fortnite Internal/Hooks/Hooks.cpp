#include "Hooks.h"

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