#include "../Hooks.h"
#include "../../Game/Actors/Actors.h"
#include "../../Drawing/Drawing.h"

void Hooks::PostRender::PostRender(uintptr_t this_, uintptr_t Canvas) {
	if (!Canvas) {
		return PostRender(this_, Canvas);
	}

	Game::CurrentCanvas = Canvas;
	Game::ScreenWidth = reinterpret_cast<SDK::UCanvas*>(Canvas)->SizeX();
	Game::ScreenHeight = reinterpret_cast<SDK::UCanvas*>(Canvas)->SizeY();

	Hooks::Tick();

	Actors::Tick();
	Actors::UpdateCaches();

	Game::DrawCallback();

	return PostRenderOriginal(this_, Canvas);
}

float Hooks::ReloadSpeed::ReloadSpeed(void* Weapon) {
	if (Config::Exploits::InstantReload::Enabled) {
		if (Config::Exploits::InstantReload::SpeedMultiplier == 5.0f) return FLT_MAX;

		float Real = ReloadSpeedOriginal(Weapon);

		return Real * Config::Exploits::InstantReload::SpeedMultiplier;
	}
	else {
		return Hooks::ReloadSpeed::ReloadSpeedOriginal(Weapon);
	}
}

float Hooks::FiringRate::FiringRate(void* Weapon) {
	if (Config::Exploits::RapidFire::Enabled) {
		if (Config::Exploits::RapidFire::SpeedMultiplier == 5.0f) return FLT_MAX;

		float Real = FiringRateOriginal(Weapon);

		return Real * Config::Exploits::RapidFire::SpeedMultiplier;
	}
	else {
		return Hooks::FiringRate::FiringRateOriginal(Weapon);
	}
}

void Hooks::SilentGun::SilentGun1(void* Weapon) {
	if (Config::Exploits::SilentGun::Enabled && Weapon == Hooks::SilentGun::WeaponHooked1) {
		return;
	}
	else {
		return Hooks::SilentGun::SilentGunOriginal1(Weapon);
	}
}

void Hooks::SilentGun::SilentGun2(void* Weapon) {
	if (Config::Exploits::SilentGun::Enabled && Weapon == Hooks::SilentGun::WeaponHooked2) {
		return;
	}
	else {
		return Hooks::SilentGun::SilentGunOriginal2(Weapon);
	}
}

void Hooks::SilentGun::SilentGun3(void* Weapon) {
	if (Config::Exploits::SilentGun::Enabled && Weapon == Hooks::SilentGun::WeaponHooked3) {
		return;
	}
	else {
		return Hooks::SilentGun::SilentGunOriginal3(Weapon);
	}
}