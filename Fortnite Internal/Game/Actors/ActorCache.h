#pragma once
#include <vector>
#include "../SDK/Classes/Engine_classes.h"
#include "../SDK/Classes/FortniteGame_classes.h"

namespace Actors {
	namespace Caches {
		/*
		* @brief Cache for FortPaws, stores information like bone registeres, distance, visibilities, etc
		*/
		struct FortPawnCache {
			SDK::AFortPawn* FortPawn;
			SDK::USkeletalMeshComponent* Mesh;

			int								TeamIndex;
			SDK::FString					PlayerName;

			float							DistanceFromLocal;
			bool							AnyBoneVisible;

			std::vector<SDK::FVector>		BoneRegister;
			std::vector<SDK::FVector2D>		BoneRegister2D;
			std::vector<bool>				BoneVisibilities;
		};


	}
}