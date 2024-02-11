#pragma once
#include "FortniteGame_Classes.h"

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

	enum class EFortItemTier : uint8
	{
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