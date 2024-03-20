#pragma once
#include <vector>

#include "../SDK/Classes/Basic.h"
#include "../SDK/Classes/Engine_classes.h"
#include "../SDK/Classes/FortniteGame_Classes.h"

namespace Actors {
	namespace Caches {
		/* Cache for FortPawns, stores information like bone positions, distance from local pawn, visibilities etc */
		struct FortPawnCache {
			SDK::AFortPawn*					FortPawn = nullptr;					// FortPawn
			SDK::USkeletalMeshComponent*	Mesh = nullptr;						// Player mesh

			int								TeamIndex = -1;						// Team index
			SDK::FString					PlayerName = SDK::FString();		// Player name

			float							DistanceFromLocalPawn = 0.f;		// Distance from the local player (meters)
			bool							IsAnyBoneVisible = false;			// If any bones were visible from LineTraceSingle
			bool							IsPlayerVisibleOnScreen = false;	// If the player is on the screen
			bool							IsBoneRegister2DPopulated = false;	// If the 2D bone register was fully populated

			std::vector<SDK::FVector>		BonePositions3D;					// Contains all bones
			std::vector<SDK::FVector2D>		BonePositions2D;					// Contains all bones in 2D (world to screen)
			std::vector<bool>				BoneVisibilityStates;				// Contains all bones visibilities (from LineTraceSingle)
		};

		/* Minimal Cache for FortPawns, stores position and team index */
		struct MinimalFortPawnCache {
			SDK::FVector Position = SDK::FVector();
			uint8_t TeamIndex = 0;
		};

		/* Cache for the camera, stores position, rotation and FOV */
		struct CameraCache {
			SDK::FVector Position = SDK::FVector();
			SDK::FRotator Rotation = SDK::FRotator();
			float FOV = 0.f;
		};
	}



	// Cache

	inline Caches::CameraCache MainCamera;					// Cache for the main camera
	inline Caches::CameraCache AimbotCamera;				// Cache for the aimbot camera (used for silent aim)

	inline Caches::MinimalFortPawnCache LocalPawnCache;		// Cache for the local pawn (position, team index)
}