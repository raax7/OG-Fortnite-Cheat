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

	};

	struct FFortMeleeWeaponStats : public FFortBaseWeaponStats {
		// VALUES

		float GetSwingPlaySpeed() {
			if (!this) return 0;

		}

		void SetSwingPlaySpeed(float NewSwingPlaySpeed) {
			if (!this) return;

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