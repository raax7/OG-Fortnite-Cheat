#pragma once
#include "../../SDK/Classes/Basic.h"

#include "../../Actors/ActorCache.h"

namespace Features {
	namespace Aimbot {
		/* Represents an Actor target */
		class Target {
		public:
			// The type of the target (Affects the priority of the target)
			// The lower number, the higher the priority
			enum class TargetType {
				ClosePlayer = 0,
				FarPlayer = 1,
				Weakspot = 2,
				Other = 3,

				NONE = 4,
			};

		protected:
			// Information about the target, not relative to any player
			struct GlobalTargetInfo {
				// General information
				TargetType Type = TargetType::NONE;											// The type of the target (Affects the priority of the target)

				// The target actor information
				SDK::AActor* TargetActor = nullptr;											// The target actor
				SDK::FVector TargetActorPosition = SDK::FVector(0, 0, 0);					// The position of the actors's root component
				SDK::FVector2D TargetActorPosition2D = SDK::FVector2D(0, 0);				// The position of the actors's root component on the screen

				// The target's bone information
				// If the target doesn't have a bone, then the bone position will be the same as the actor position
				SDK::FVector TargetBonePosition = SDK::FVector(0, 0, 0);					// The position of the target's bone
				SDK::FVector2D TargetBonePosition2D = SDK::FVector2D(0, 0);					// The position of the target's bone on the screen
				uint8_t TargetBoneId = 0;													// The bone id of the target (only if used if the target is a FortPawn)
			};

			// Information about the target, relative to the local player
			struct LocalTargetInfo {
				// Target distance information (FLT_MAX so that any value is less than it)
				float DistanceFromCrosshairPixels = FLT_MAX;								// The distance from the crosshair in pixels
				float DistanceFromCrosshairDegrees = FLT_MAX;								// The distance from the crosshair in degrees
				float DistanceFromPlayer = FLT_MAX;											// The distance from the local player in meters
				float SmartTargetingDistance = FLT_MAX;										// The physical distance and the crosshair distance combined (hence smart targeting)

				// Aimbot information
				bool IsTargeting = false;													// Is the local player currently aimbotting the target
				bool IsOnScreen = false;													// Is the target on the screen

				SDK::FRotator TargetRotation = SDK::FRotator(0, 0, 0);						// The target rotation to aimbot the target
				SDK::FRotator TargetRotationWithSmooth = SDK::FRotator(0, 0, 0);			// The target rotation to aimbot the target with smoothing applied
				SDK::FRotator TargetRotationChange = SDK::FRotator(0, 0, 0);				// The current rotation change required to achieve the target rotation

				int CurrentFOVSizeDegrees = 0;												// The current FOV size in degrees
				int CurrentFOVSizePixels = 0;												// The current FOV size in pixels
				float CurrentSmoothing = 0;													// The current smoothing
			};

		public:
			// Target information
			GlobalTargetInfo GlobalInfo{};		// Information about the target, not relative to any player
			LocalTargetInfo LocalInfo{};		// Information about the target, relative to the local player

		protected:
			/* Update the FOV, smoothing and type of the target */
			static void UpdateLocalInfoAndType(Target& TargetToUpdate);

		public:
			/* Reset the target data to default values */
			virtual void ResetTarget();

			/*
			* @brief Update the target data based off the current target
			*
			* @param SeenTargetLastFrame - Was the target seen this frame
			*/
			virtual void TargetTick(bool SeenTargetThisFrame);

			/*
			* @brief Should the target be set to the new target
			*
			* @param PotentialTarget - The potential new target
			*/
			virtual bool ShouldSetTarget(Target PotentialTarget);

			/*
			* @brief Set the target to the new target
			*
			* @param NewTarget - The new target to set
			* @param ForceSetTarget - Should the target be set regardless
			*/
			virtual void SetTarget(Target NewTarget, bool ForceSetTarget = false);
		};



		/* Represents a player target, inherits from Target with extended functions for dealing with player targets */
		class PlayerTarget : public Target {
		public:
			/*
			 * @brief Propagates a FortPawn target's info to the target
			 *
			 * (If the optional arguments are not provided, then no aimbot calculations are made)
			 *
			 * @param Target - The target to update
			 * @param TargetCache - The target's fort pawn cache
			 * @param MainCamera - The local player's camera (optional)
			 * @param AimbotCamera - The aimbot camera (optional) (used for silent aim)
			 * @param FPSScale - The scale to apply to the smoothing (optional)
			 */
			static void UpdateTargetInfo(Target& Target, Actors::Caches::FortPawnCache& TargetCache, const Actors::Caches::CameraCache& MainCamera = Actors::Caches::CameraCache(), const Actors::Caches::CameraCache& AimbotCamera = Actors::Caches::CameraCache(), const float FPSScale = 0.f);
		};

		/* Represents a building weak spot target, inherits from Target with extended functions for dealing with building weak spot targets */
		class WeakSpotTarget : public Target {
		public:
			/*
			 * @brief Propagates a BuildingWeakSpot target's info to the target
			 *
			 * (If the optional arguments are not provided, then no aimbot calculations are made)
			 *
			 * @param Target - The target to update
			 * @param WeakSpot - The target's building weak spot
			 * @param MainCamera - The local player's camera (optional)
			 * @param AimbotCamera - The aimbot camera (optional) (used for silent aim)
			 * @param FPSScale - The scale to apply to the smoothing (optional)
			 */
			static void UpdateTargetInfo(Target& Target, SDK::ABuildingWeakSpot* WeakSpot, const Actors::Caches::CameraCache& MainCamera = Actors::Caches::CameraCache(), const Actors::Caches::CameraCache& AimbotCamera = Actors::Caches::CameraCache(), const float FPSScale = 0.f);
		};
	}
}