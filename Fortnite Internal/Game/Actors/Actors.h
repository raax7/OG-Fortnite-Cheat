#pragma once
#include <chrono>

#include "../Features/Aimbot/Target.h"
#include "ActorCache.h"

namespace Actors {
	// Actor Loops

	namespace FortPawn {
		void Tick();

		inline std::vector<Actors::Caches::FortPawnCache> CachedPlayers;

		inline const float IntervalSeconds = 0.25f;
		inline std::chrono::steady_clock::time_point LastCacheTime = std::chrono::steady_clock::now();
	}

	namespace FortPickup {
		void Tick();

		inline SDK::TArray<SDK::AActor*> CachedWeapons;

		inline const float IntervalSeconds = 0.10f;
		inline std::chrono::steady_clock::time_point LastCacheTime = std::chrono::steady_clock::now();

		inline std::chrono::steady_clock::time_point LastAutoPickupTime = std::chrono::steady_clock::now();
	}

	namespace FortAthenaVehicle {
		void Tick();

		inline SDK::TArray<SDK::AActor*> CachedVehicles;

		inline const float IntervalSeconds = 0.25f;
		inline std::chrono::steady_clock::time_point LastCacheTime = std::chrono::steady_clock::now();
	}

	namespace BuildingWeakSpot {
		void Tick();

		inline SDK::TArray<SDK::AActor*> CachedBuildingWeakSpot;

		inline const float IntervalSeconds = 0.10f;
		inline std::chrono::steady_clock::time_point LastCacheTime = std::chrono::steady_clock::now();
	}



	// Cache

	inline Features::Aimbot::Target MainTarget;

	inline float CurrentFOVSizeDegrees;
	inline float CurrentFOVSizePixels;



	// FPS Scaling

	inline std::chrono::high_resolution_clock::time_point LastAimbotFrameTime;
	inline float FPSScale;



	// Functions

	void Tick();
	void Draw();
	void UpdateCaches();
}