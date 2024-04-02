#pragma once

extern enum class EFortItemTier : uint8;

namespace ConfigTypes {
	enum class AimbotType {
		Smart,
		Crosshair,
		Distance,
	};

	enum class BoxType {
		Full3D,
		Cornered3D,
		Full2D,
		Cornered2D,
	};

	struct BaseChamsSettings {
		bool Enabled = false;

		bool ShowLocal = false;
		bool Wireframe = false;
		bool ThroughWalls = false;

		float EmissiveIntensity = 1.f;

		float Color[3] = { 1.f, 0.f, 0.75f };
	};

	struct PickupChamsSettings : public BaseChamsSettings {
		EFortItemTier MinimumTier = (EFortItemTier)1;
	};
}