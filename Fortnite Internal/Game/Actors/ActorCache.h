#pragma once
#include <vector>

#include "../SDK/Classes/Basic.h"
#include "../SDK/Classes/Engine_classes.h"
#include "../SDK/Classes/FortniteGame_Classes.h"

namespace Actors {
	namespace Caches {
		/* Cache for FortPaws, stores information like bone registeres, distance, visibilities, etc */
		struct FortPawnCache {
			SDK::AFortPawn*					FortPawn = nullptr;
			SDK::USkeletalMeshComponent*	Mesh = nullptr;

			int								TeamIndex{};
			SDK::FString					PlayerName;

			float							DistanceFromLocal{};
			bool							AnyBoneVisible;

			std::vector<SDK::FVector>		BoneRegister;
			std::vector<SDK::FVector2D>		BoneRegister2D;
			std::vector<bool>				BoneVisibilities;
		};
	}

	// Structs

	struct LocalCache {
		SDK::FVector Position;
		uint8_t TeamIndex;
	};
	struct CameraCache {
		SDK::FVector Position;
		SDK::FRotator Rotation;
		float FOV;
	};



	// Cache

	inline CameraCache MainCamera;
	inline CameraCache AimbotCamera;

	inline LocalCache LocalPawnCache;
}