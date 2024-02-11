#pragma once
#include <Windows.h>
#include <chrono>
#include "../SDK/SDK.h"
#include "../Features/Aimbot/Target.h"
#include "ActorCache.h"

struct LocalPlayer {
	SDK::FVector Position;
	int TeamIndex;
};

namespace Actors {
	namespace FortPawn {
		void Tick();

		inline std::vector<Actors::Caches::FortPawnCache> CachedPlayers;

		inline const float IntervalSeconds = 0.25;
		inline std::chrono::steady_clock::time_point LastTime = std::chrono::steady_clock::now();
	}

	namespace FortWeapon {
		void Tick();

		inline SDK::TArray<SDK::AActor*> CachedWeapons;

		inline const float IntervalSeconds = 0.25;
		inline std::chrono::steady_clock::time_point LastTime = std::chrono::steady_clock::now();
	}

	namespace BuildingWeakSpot {
		void Tick();

		inline SDK::TArray<SDK::AActor*> CachedBuildingWeakSpot;

		inline const float IntervalSeconds = 0.10;
		inline std::chrono::steady_clock::time_point LastTime = std::chrono::steady_clock::now();
	}


	inline Features::Aimbot::Target target{};
	inline Camera RealCamera;
	inline Camera AimbotCamera;
	inline LocalPlayer localPlayer;


	inline SDK::FVector OriginalPosition;
	inline SDK::FRotator OriginalRotation;

	void Tick();
	void UpdateCaches();

	inline std::chrono::high_resolution_clock::time_point lastAimbotFrameTime;
	inline float FPSScale;
}