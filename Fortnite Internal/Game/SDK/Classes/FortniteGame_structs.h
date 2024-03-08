#pragma once
#include "FortniteGame_Classes.h"

typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

namespace SDK {
	struct FFortBaseWeaponStats {
	public:
		// VALUES

		void SetCartridgePerFire(float NewCartridgePerFire) {
			if (SDK::IsValidPointer(this) == false) return;
			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::CartridgePerFire) = NewCartridgePerFire;
		}
	};

	struct FFortRangedWeaponStats : public FFortBaseWeaponStats {
	public:
		// VALUES

		void SetSpread(float NewSpread) {
			if (SDK::IsValidPointer(this) == false) return;
			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::Spread) = NewSpread;
		}

		void SetSpreadDownsights(float NewSpreadDownsights) {
			if (SDK::IsValidPointer(this) == false) return;
			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::SpreadDownsights) = NewSpreadDownsights;
		}

		void SetStandingStillSpreadMultiplier(float NewStandingStillSpreadMultiplier) {
			if (SDK::IsValidPointer(this) == false) return;
			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::StandingStillSpreadMultiplier) = NewStandingStillSpreadMultiplier;
		}

		void SetAthenaCrouchingSpreadMultiplier(float NewAthenaCrouchingSpreadMultiplier) {
			if (SDK::IsValidPointer(this) == false) return;
			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::AthenaCrouchingSpreadMultiplier) = NewAthenaCrouchingSpreadMultiplier;
		}

		void SetAthenaJumpingFallingSpreadMultiplier(float NewAthenaJumpingFallingSpreadMultiplier) {
			if (SDK::IsValidPointer(this) == false) return;
			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::AthenaJumpingFallingSpreadMultiplier) = NewAthenaJumpingFallingSpreadMultiplier;
		}

		void SetAthenaSprintingSpreadMultiplier(float NewAthenaSprintingSpreadMultiplier) {
			if (SDK::IsValidPointer(this) == false) return;
			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::AthenaSprintingSpreadMultiplier) = NewAthenaSprintingSpreadMultiplier;
		}

		void SetMinSpeedForSpreadMultiplier(float NewMinSpeedForSpreadMultiplier) {
			if (SDK::IsValidPointer(this) == false) return;
			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::MinSpeedForSpreadMultiplier) = NewMinSpeedForSpreadMultiplier;
		}

		void SetMaxSpeedForSpreadMultiplier(float NewMaxSpeedForSpreadMultiplier) {
			if (SDK::IsValidPointer(this) == false) return;
			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::MaxSpeedForSpreadMultiplier) = NewMaxSpeedForSpreadMultiplier;
		}
	};

	struct FFortMeleeWeaponStats : public FFortBaseWeaponStats
	{
	public:
		// VALUES

		float GetSwingPlaySpeed() {
			if (SDK::IsValidPointer(this) == false) return 0;
			return *(float*)((uintptr_t)this + SDK::Cached::Offsets::FortMeleeWeaponStats::SwingPlaySpeed);
		}

		void SetSwingPlaySpeed(float NewSwingPlaySpeed) {
			if (SDK::IsValidPointer(this) == false) return;
			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortMeleeWeaponStats::SwingPlaySpeed) = NewSwingPlaySpeed;
		}
	};

	enum class EFortItemTier : uint8 {
		No_Tier = 0,
		I = 1,
		II = 2,
		III = 3,
		IV = 4,
		V = 5,
		VI = 6,
		VII = 7,
		VIII = 8,
		IX = 9,
		X = 10,
		NumItemTierValues = 11,
		EFortItemTier_MAX = 12,
	};
}