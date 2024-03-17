#pragma once
#include "FortniteGame_Classes.h"

#include "../../Features/Features.h"

typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

namespace SDK {
	struct FVehiclePawnState {
	public:
		// VALUES

		class AFortAthenaVehicle* GetVehicle() {
			if (SDK::IsValidPointer(this) == false) return nullptr;
			return (AFortAthenaVehicle*)(*(uintptr_t*)((uintptr_t)this + SDK::Cached::Offsets::VehiclePawnState::Vehicle));
		}
	};

	struct FFortRechargingActionTimer {
	public:
		// VALUES

		void SetChargeRate(float NewChargeRate, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false) return;

			if (AutoRevertFeature) {
				Features::CreateAutoRevertFeature<float>((float*)((uintptr_t)this + SDK::Cached::Offsets::FortRechargingActionTimer::ChargeRate), AutoRevertFeature);
			}

			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRechargingActionTimer::ChargeRate) = NewChargeRate;
		}
	
		void SetActiveExpenseRate(float NewActiveExpenseRate, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false) return;

			if (AutoRevertFeature) {
				Features::CreateAutoRevertFeature<float>((float*)((uintptr_t)this + SDK::Cached::Offsets::FortRechargingActionTimer::ActiveExpenseRate), AutoRevertFeature);
			}

			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRechargingActionTimer::ActiveExpenseRate) = NewActiveExpenseRate;
		}

		void SetPassiveExpenseRate(float NewPassiveExpenseRate, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false) return;

			if (AutoRevertFeature) {
				Features::CreateAutoRevertFeature<float>((float*)((uintptr_t)this + SDK::Cached::Offsets::FortRechargingActionTimer::PassiveExpenseRate), AutoRevertFeature);
			}

			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRechargingActionTimer::PassiveExpenseRate) = NewPassiveExpenseRate;
		}

		void SetCharge(float NewCharge) {
			if (SDK::IsValidPointer(this) == false) return;
			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRechargingActionTimer::Charge) = NewCharge;
		}
	};

	struct FFortBaseWeaponStats {
	public:
		// VALUES

		void SetReloadTime(float NewReloadTime, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false) return;

			if (AutoRevertFeature) {
				Features::CreateAutoRevertFeature<float>((float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::ReloadTime), AutoRevertFeature);
			}

			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::ReloadTime) = NewReloadTime;
		}
	};

	struct FFortRangedWeaponStats : public FFortBaseWeaponStats {
	public:
		// VALUES

		void SetSpread(float NewSpread, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false) return;

			if (AutoRevertFeature) {
				Features::CreateAutoRevertFeature<float>((float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::Spread), AutoRevertFeature);
			}

			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::Spread) = NewSpread;
		}

		void SetSpreadDownsights(float NewSpreadDownsights, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false) return;

			if (AutoRevertFeature) {
				Features::CreateAutoRevertFeature<float>((float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::SpreadDownsights), AutoRevertFeature);
			}

			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::SpreadDownsights) = NewSpreadDownsights;
		}

		void SetStandingStillSpreadMultiplier(float NewStandingStillSpreadMultiplier, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false) return;

			if (AutoRevertFeature) {
				Features::CreateAutoRevertFeature<float>((float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::StandingStillSpreadMultiplier), AutoRevertFeature);
			}

			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::StandingStillSpreadMultiplier) = NewStandingStillSpreadMultiplier;
		}

		void SetAthenaCrouchingSpreadMultiplier(float NewAthenaCrouchingSpreadMultiplier, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false) return;

			if (AutoRevertFeature) {
				Features::CreateAutoRevertFeature<float>((float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::AthenaCrouchingSpreadMultiplier), AutoRevertFeature);
			}

			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::AthenaCrouchingSpreadMultiplier) = NewAthenaCrouchingSpreadMultiplier;
		}

		void SetAthenaJumpingFallingSpreadMultiplier(float NewAthenaJumpingFallingSpreadMultiplier, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false) return;

			if (AutoRevertFeature) {
				Features::CreateAutoRevertFeature<float>((float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::AthenaJumpingFallingSpreadMultiplier), AutoRevertFeature);
			}

			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::AthenaJumpingFallingSpreadMultiplier) = NewAthenaJumpingFallingSpreadMultiplier;
		}

		void SetAthenaSprintingSpreadMultiplier(float NewAthenaSprintingSpreadMultiplier, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false) return;

			if (AutoRevertFeature) {
				Features::CreateAutoRevertFeature<float>((float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::AthenaSprintingSpreadMultiplier), AutoRevertFeature);
			}

			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::AthenaSprintingSpreadMultiplier) = NewAthenaSprintingSpreadMultiplier;
		}

		void SetMinSpeedForSpreadMultiplier(float NewMinSpeedForSpreadMultiplier, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false) return;

			if (AutoRevertFeature) {
				Features::CreateAutoRevertFeature<float>((float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::MinSpeedForSpreadMultiplier), AutoRevertFeature);
			}

			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::MinSpeedForSpreadMultiplier) = NewMinSpeedForSpreadMultiplier;
		}

		void SetMaxSpeedForSpreadMultiplier(float NewMaxSpeedForSpreadMultiplier, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false) return;

			if (AutoRevertFeature) {
				Features::CreateAutoRevertFeature<float>((float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::MaxSpeedForSpreadMultiplier), AutoRevertFeature);
			}

			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::MaxSpeedForSpreadMultiplier) = NewMaxSpeedForSpreadMultiplier;
		}

		void SetRecoilVert(float NewRecoilVert, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false) return;

			if (AutoRevertFeature) {
				Features::CreateAutoRevertFeature<float>((float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::RecoilVert), AutoRevertFeature);
			}

			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::RecoilVert) = NewRecoilVert;
		}

		void SetRecoilHoriz(float NewRecoilHoriz, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false) return;

			if (AutoRevertFeature) {
				Features::CreateAutoRevertFeature<float>((float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::RecoilHoriz), AutoRevertFeature);
			}

			*(float*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::RecoilHoriz) = NewRecoilHoriz;
		}

		void SetBulletsPerCartridge(int32 NewBulletsPerCartridge, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false) return;

			if (AutoRevertFeature) {
				Features::CreateAutoRevertFeature<int32>((int32*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::BulletsPerCartridge), AutoRevertFeature);
			}

			*(int32*)((uintptr_t)this + SDK::Cached::Offsets::FortRangedWeaponStats::BulletsPerCartridge) = NewBulletsPerCartridge;
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

		void SetSwingPlaySpeed(float NewSwingPlaySpeed, bool* AutoRevertFeature = nullptr) {
			if (SDK::IsValidPointer(this) == false) return;

			if (AutoRevertFeature) {
				Features::CreateAutoRevertFeature<float>((float*)((uintptr_t)this + SDK::Cached::Offsets::FortMeleeWeaponStats::SwingPlaySpeed), AutoRevertFeature);
			}

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