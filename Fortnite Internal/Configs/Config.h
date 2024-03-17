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
			inline float Smoothing = 8.f;
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
		namespace Pickaxe {
			inline bool FastPickaxe = false;
			inline float SpeedMultiplier = 1.f;
		}

		namespace Player {
			inline bool EditEnemyBuilds = false;

			inline bool InfiniteBuilds = false;
			inline bool InfiniteAmmo = false;
		}

		namespace Weapon {
			inline bool NoSpread = false;
			inline bool NoRecoil = false;
			inline bool NoReload = false;

			inline bool RapidFire = false;

			inline bool Recoil = false;
			inline float RecoilMultiplier = 1.f; // 0.f is none

			inline bool UseDamageMultiplier = false;
			inline int DamageMultiplier = 1;
		}

		namespace Vehicle {
			inline bool InfiniteBoost = false;
			inline bool Fly = false;
			inline bool FlyThroughWalls = false;
			inline bool FreezeInAir = false;
			inline bool NoTilting = false;
			inline float FlySpeed = 35.f;
		}

		namespace Pickup {
			inline bool PrioritizeFarthestWeapons = false;

			inline bool AutoPickup = false;
			inline float AutoPickupDelaySecs = 1.f;

			inline int MaxDistance = 300;
			inline int MaxItems = 5;

			inline KeyName PickupAllKey;
		}
	}
}