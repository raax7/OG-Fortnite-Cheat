#pragma once
#include <Windows.h>
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

	namespace FortWeapon {
		void Tick();

		inline SDK::TArray<SDK::AActor*> CachedWeapons;

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



	// FPS Scaling

	inline std::chrono::high_resolution_clock::time_point LastAimbotFrameTime;
	inline float FPSScale;



	// Functions

	void Tick();
	void Draw();
	void UpdateCaches();
}