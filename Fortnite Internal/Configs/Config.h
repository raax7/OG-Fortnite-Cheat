#pragma once
#include <Windows.h>
#include "ConfigTypes.h"

extern enum class KeyName;

namespace Config {
	inline bool rapidfiresex = false;

	inline float test = 0;
	inline int test2 = 5;
	inline float testfloat = 1.f;
	inline float testfloat2 = 0;

	namespace Aimbot {
		inline bool Enabled = true;

		inline KeyName AimKey;

		inline bool SilentAim = false;
		inline bool UseAimKeyForSilent = false;

		inline bool ShowAimLine = true;
		inline bool ShowFOV = true;

		inline ConfigTypes::AimbotType TargettingType = ConfigTypes::AimbotType::Smart;
		// 0: smart
		// 1: crosshair distance
		// 2: player distance

		namespace TriggerBot {
			inline bool Enabled = true;
			inline bool DisableVisibleCheck = false;
			inline bool ShowFOV = false;
			inline int FOV = 175;
			inline float Range = 10.f;

			inline int TriggerBotType = 0;
			// 0: hold
			// 1: toggle
		}



		namespace CloseAim {
			inline bool Enabled = true;
			inline int FOV = 30;
			inline float Smoothing = 4.f;
			inline int Range = 15;
		}

		namespace Standard {
			inline bool Enabled = true;
			inline int FOV = 20;
			inline float Smoothing = 1.f;
		}

		namespace Weakspot {
			inline bool Enabled = true;
			inline int FOV = 40;
			inline float Smoothing = 1.5f;
		}
	}

	namespace Visuals {
		namespace Players {
			inline bool Enabled = true;

			inline bool Box = true;
			inline bool Skeleton = true;
			inline bool Name = true;
			inline bool Distance = true;
			inline bool Weapon = true;
		}

		namespace Weapons {
			inline bool Enabled = false;

			inline int WeaponMaxDistance = 300;
		}
	}

	namespace Exploits {
		namespace RapidFire {
			inline bool Enabled = false;
			inline float SpeedMultiplier = 1.f;
		}

		namespace InstantReload {
			inline bool Enabled = false;
			inline float SpeedMultiplier = 1.f;
		}

		namespace SilentGun {
			inline bool Enabled = false;
		}

		namespace Pickaxe {
			inline bool Enabled = false;
			inline float SpeedMultiplier = 1.f;
		}

		namespace Weapon {
			inline bool Enabled = false;

			inline bool ReloadSpeed = false;
			inline float ReloadSpeedMultiplier = 1.f; // 10.f is instant

			inline bool FireRate = false;
			inline float FireRateMultiplier = 1.f; // 10.f is instant

			inline bool NoSpread = false;

			inline bool Recoil = false;
			inline float RecoilMultiplier = 1.f; // 0.f is none

			inline int CartridgePerFire = 1;
		}

		namespace Vehicle {
			inline bool Enabled = false;
			
			inline bool InfiniteBoost = false;
			inline bool Fly = false;
			inline float FlySpeed = 1.f;
		}
	}
}