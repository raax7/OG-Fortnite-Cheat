#pragma once
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

		inline KeyName AimKey = (KeyName)7;

		inline bool BulletTP = false;
		inline bool BulletTPV2 = false;

		inline bool SilentAim = false;
		inline bool UseAimKeyForSilent = false;

		inline bool StickyAim = true;
		inline bool VisibleCheck = true;

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
			inline float Smoothing = 5.f;
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

			namespace OffScreenIndicators {
				inline bool Enabled = true;

				//inline bool ShowDistance = true;
				//inline bool ShowName = true;

				inline bool CopyAimbotFOV = true;

				inline float Height = 10.f;
				inline float Width = 10.f;

				inline int FOV = 20;
			}

			inline ConfigTypes::BoxType BoxType = ConfigTypes::BoxType::Cornered2D;

			inline bool Box = true;

			inline bool Skeleton = true;
			inline bool IndividualBoneVisibilities = true;

			inline bool Name = true;
			inline bool Distance = true;
			inline bool CurrentWeapon = true;

			// Chams
			inline bool Chams = false;

			inline bool SelfChams = false;

			inline bool Wireframe = true;

			inline float GlowAmount = 1.f;
		}

		namespace Weapons {
			inline bool Enabled = false;

			inline int MaxDistance = 300;
		}

		namespace Vehicles {
			inline bool Enabled = false;

			inline int MaxDistance = 300;
		}
	}

	namespace Exploits {
		namespace Pickaxe {
			inline bool FastPickaxe = false;
			inline float SpeedMultiplier = 1.f;
		}

		namespace Player {
			inline bool ZiplineFly = false;

			inline bool EditEnemyBuilds = false;

			inline bool ADSWhileNotOnGround = false;
			inline bool DoublePump = false;
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