#pragma once
#include <Windows.h>
#include "../../SDK/Classes/CoreUObject_classes.h"
#include "../../SDK/Classes/Engine_classes.h"
#include "../../../Configs/Config.h"
#include "../../Features/FortPawn/Bone.h"
#include "../../Actors/ActorCache.h"

namespace Features {
	namespace Aimbot {
		/*
		* @brief Represents an AActor target
		*/
		class Target {
		public:
			/*
			* @brief The type of the target (Affects the priority of the target)
			*/
			enum class TargetType {
				ClosePlayer = 0,
				FarPlayer = 1,
				Weakspot = 2,
				Other = 3,

				TargetType_MAX = 4,
			};
		private:
			/*
			* @brief Information about the target, not relative to any player
			*/
			struct GlobalTargetInfo {
				/*
				* @brief Cached info about the FortPawn's BoneRegisters, etc (only if used if the target is a player)
				*/
				Actors::Caches::FortPawnCache PawnCache = Actors::Caches::FortPawnCache();

				/*
				* @brief The type of the target (Affects the priority of the target)
				*/
				TargetType Type = TargetType::TargetType_MAX;

				/*
				* @brief The target actor
				*/
				SDK::AActor* TargetActor = nullptr;

				/*
				* @brief The position of the actors's root component
				*/
				SDK::FVector TargetActorPosition = SDK::FVector(0, 0, 0);

				/*
				* @brief The position of the actors's root component on the screen
				*/
				SDK::FVector2D TargetActorPosition2D = SDK::FVector2D(0, 0);

				/*
				* @brief The position of the target's bone
				*/
				SDK::FVector TargetBonePosition = SDK::FVector(0, 0, 0);

				/*
				* @brief The position of the target's bone on the screen
				*/
				SDK::FVector2D TargetBonePosition2D = SDK::FVector2D(0, 0);

				/*
				* @brief The bone ID of the target bone
				*/
				Bone::BoneID TargetBoneId = Bone::BoneID::None;
			};

			/*
			* @brief Information about the target, relative to the local player
			*/
			struct LocalTargetInfo {
				// Set to float max so that any target will be less than this
				/*
				* @brief The distance from the crosshair in pixels
				*/
				float DistanceFromCrosshair = FLT_MAX;

				/*
				* @brief The distance between the local player and the target (From rootcomponent)
				*/
				float DistanceFromPlayer = FLT_MAX;

				/*
				* @brief The physical distance and the crosshair distance combined (hence smart targeting)
				*/
				float SmartTargetingDistance = FLT_MAX;



				/*
				* @brief Is the local player currently holding down the aim key
				*/
				bool IsTargeting = false;


				/*
				* @brief The target rotation to aimbot the target
				*/
				SDK::FRotator TargetRotation = SDK::FRotator(0, 0, 0);

				/*
				* @brief The target rotation to aimbot the target with smoothing applied
				*/
				SDK::FRotator TargetRotationWithSmooth = SDK::FRotator(0, 0, 0);

				/*
				* @brief The current rotation change required to achieve the target rotation
				*/
				SDK::FRotator TargetRotationChange = SDK::FRotator(0, 0, 0);



				/*
				* @brief The current FOV size, NOT adjusted for the screen size
				*/
				float CurrentFOVSize = 0;

				/*
				* @brief The current FOV size, adjusted for the screen size
				*/
				float CurrentRealFOVSize = 0;

				/*
				* @brief The current smoothing
				*/
				float CurrentSmoothing = 0;
			};
		public:
			/*
			* @brief Information about the target, not relative to any player
			*/
			GlobalTargetInfo GlobalInfo{};

			/*
			* @brief Information about the target, relative to the local player
			*/
			LocalTargetInfo LocalInfo{};
		private:
			/*
			* @brief Update the FOV, smoothing and type of the target
			*/
			void UpdateLocalInfoAndType(Target& TargetToUpdate) {
				switch (TargetToUpdate.GlobalInfo.Type) {
				case TargetType::ClosePlayer:
					TargetToUpdate.LocalInfo.CurrentFOVSize = Config::Aimbot::CloseAim::FOV;
					TargetToUpdate.LocalInfo.CurrentRealFOVSize = Config::Aimbot::CloseAim::RealFOV;
					TargetToUpdate.LocalInfo.CurrentSmoothing = Config::Aimbot::CloseAim::Smoothing;

					if (TargetToUpdate.LocalInfo.DistanceFromPlayer > Config::Aimbot::CloseAim::Range || !Config::Aimbot::CloseAim::Enabled) {
						TargetToUpdate.GlobalInfo.Type = TargetType::FarPlayer;
					}
					break;
				case TargetType::FarPlayer:
					TargetToUpdate.LocalInfo.CurrentFOVSize = Config::Aimbot::Standard::FOV;
					TargetToUpdate.LocalInfo.CurrentRealFOVSize = Config::Aimbot::Standard::RealFOV;
					TargetToUpdate.LocalInfo.CurrentSmoothing = Config::Aimbot::Standard::Smoothing;

					if (TargetToUpdate.LocalInfo.DistanceFromPlayer <= Config::Aimbot::CloseAim::Range && Config::Aimbot::CloseAim::Enabled) {
						TargetToUpdate.GlobalInfo.Type = TargetType::ClosePlayer;
					}
					break;
				case TargetType::Weakspot:
					TargetToUpdate.LocalInfo.CurrentFOVSize = Config::Aimbot::Weakspot::FOV;
					TargetToUpdate.LocalInfo.CurrentRealFOVSize = Config::Aimbot::Weakspot::RealFOV;
					TargetToUpdate.LocalInfo.CurrentSmoothing = Config::Aimbot::Weakspot::Smoothing;
					break;
				case TargetType::Other:
					TargetToUpdate.LocalInfo.CurrentFOVSize = Config::Aimbot::Standard::FOV;
					TargetToUpdate.LocalInfo.CurrentRealFOVSize = Config::Aimbot::Standard::RealFOV;
					TargetToUpdate.LocalInfo.CurrentSmoothing = Config::Aimbot::Standard::Smoothing;
					break;
				}
			}
		public:
			/*
			* @brief Reset the target data to default values
			*/
			void ResetTarget() {
				// Reset the global info
				{
					GlobalInfo.Type = TargetType::TargetType_MAX;
					GlobalInfo.PawnCache = Actors::Caches::FortPawnCache(); // only if used if the target is a player

					GlobalInfo.TargetActor = nullptr;

					GlobalInfo.TargetActorPosition = SDK::FVector(0, 0, 0);
					GlobalInfo.TargetActorPosition2D = SDK::FVector2D(0, 0);

					GlobalInfo.TargetBonePosition = SDK::FVector(0, 0, 0);
					GlobalInfo.TargetBonePosition2D = SDK::FVector2D(0, 0);
					GlobalInfo.TargetBoneId = Bone::BoneID::None;
				}

				// Reset the local info
				{
					// Set to float max so that any target will be less than this
					LocalInfo.DistanceFromCrosshair = FLT_MAX;
					LocalInfo.DistanceFromPlayer = FLT_MAX;
					LocalInfo.SmartTargetingDistance = FLT_MAX;

					LocalInfo.IsTargeting = false;
					LocalInfo.TargetRotation = SDK::FRotator(0, 0, 0);
					LocalInfo.TargetRotationChange = SDK::FRotator(0, 0, 0);
				}
			}

			/*
			* @brief Update the target data based off the current target
			*
			* @param SeenTargetLastFrame - Was the target seen this frame
			*/
			void TargetTick(bool SeenTargetThisFrame) {
				if (!SeenTargetThisFrame || LocalInfo.DistanceFromCrosshair > LocalInfo.CurrentRealFOVSize || GlobalInfo.TargetActor == nullptr) {
					// Resets if:
					// - The target wasn't seen this frame
					// - The target is outside of the FOV circle
					// - The target pointer is null
					ResetTarget();
				}

				UpdateLocalInfoAndType(*this);
			}

			/*
			* @brief Should the target be set to the new target
			*
			* @param PotentialTarget - The potential new target
			*/
			bool ShouldSetTarget(Target PotentialTarget) {
				if (PotentialTarget.LocalInfo.DistanceFromCrosshair > PotentialTarget.LocalInfo.CurrentRealFOVSize) {
					// If the player is outside of the FOV circle, then don't update the target
					return false;
				}

				if (PotentialTarget.GlobalInfo.Type > GlobalInfo.Type) {
					// If the target priority is lower than the current target, then don't update the target
					return false;
				}
				else if (PotentialTarget.GlobalInfo.Type < GlobalInfo.Type) {
					// If the target priority is higher than the current target, then update the target
					return true;
				}
				else if (PotentialTarget.GlobalInfo.Type == GlobalInfo.Type) {
					// If the target priority is the same as the current target, then check the parameters
					if (LocalInfo.IsTargeting) {
						// If we are targeting, then don't update
						return false;
					}
					else {
						float CurrentDistance;
						float PotentialTargetDistance;

						// Get the distance based off the target distance types
						switch (Config::Aimbot::TargettingType) {
						case ConfigTypes::AimbotType::Smart:
							CurrentDistance = LocalInfo.SmartTargetingDistance;
							PotentialTargetDistance = PotentialTarget.LocalInfo.SmartTargetingDistance;
							break;
						case ConfigTypes::AimbotType::Crosshair:
							CurrentDistance = LocalInfo.DistanceFromCrosshair;
							PotentialTargetDistance = PotentialTarget.LocalInfo.DistanceFromCrosshair;
							break;
						case ConfigTypes::AimbotType::Distance:
							CurrentDistance = LocalInfo.DistanceFromPlayer;
							PotentialTargetDistance = PotentialTarget.LocalInfo.DistanceFromPlayer;
							break;
						default:
							return false;
						}

						if (PotentialTargetDistance < CurrentDistance) {
							// If the potential new target distance is less than the current target distance, then update the target
							return true;
						}
					}
				}

				return false;
			}

			/*
			* @brief Set the target to the new target
			*
			* @param NewTarget - The new target to set
			* @param ForceSetTarget - Should the target be set regardless
			*/
			void SetTarget(Target NewTarget, bool ForceSetTarget = false) {
				UpdateLocalInfoAndType(NewTarget);

				if (ForceSetTarget) {
					*this = NewTarget;
					return;
				}
				else {
					if (ShouldSetTarget(NewTarget)) {
						*this = NewTarget;
						return;
					}
				}
			}
		};



		/*
		* @brief Represents a player target, inherits from Target and adds functions for dealing with player targets
		*/
		class PlayerTarget : public Target {
		public:
			/*
			 * @brief Propagates a FortPawn target's info to the target
			 *
			 * (If the optional arguments are not provided, then no aimbot calculations are made)
			 *
			 * @param Target - The target to update
			 * @param TargetCache - The target's fort pawn cache
			 * @param Camera - The local player's camera (optional)
			 * @param FPSScale - The scale to apply to the smoothing (optional)
			 */
			static void UpdateTargetInfo(Target& Target, Actors::Caches::FortPawnCache& TargetCache, const Camera& Camera = Camera(), const float FPSScale = float()) {
				// Update global information
				Target.GlobalInfo.PawnCache = TargetCache;
				Target.GlobalInfo.TargetActor = TargetCache.FortPawn;
				Target.GlobalInfo.TargetBoneId = Bone::FindBestBone(Bone::BoneID::Head, TargetCache);

				// Update local information
				Target.LocalInfo.DistanceFromCrosshair = Math::GetDistance2D(TargetCache.BoneRegister2D[(int)Target.GlobalInfo.TargetBoneId].X, TargetCache.BoneRegister2D[(int)Target.GlobalInfo.TargetBoneId].Y, SDK::GetLocalCanvas()->SizeX() / 2, SDK::GetLocalCanvas()->SizeY() / 2);
				Target.LocalInfo.DistanceFromPlayer = TargetCache.DistanceFromLocal;
				Target.LocalInfo.SmartTargetingDistance = Target.LocalInfo.DistanceFromCrosshair + Target.LocalInfo.DistanceFromPlayer;

				// Determine target type
				Target.GlobalInfo.Type = (Target.LocalInfo.DistanceFromPlayer <= Config::Aimbot::CloseAim::Range && Config::Aimbot::CloseAim::Enabled) ? Target::TargetType::ClosePlayer : Target::TargetType::FarPlayer;

				// Update positions
				Target.GlobalInfo.TargetActorPosition = TargetCache.BoneRegister[(int)Bone::BoneID::Bottom];
				Target.GlobalInfo.TargetActorPosition2D = TargetCache.BoneRegister2D[(int)Bone::BoneID::Bottom];
				Target.GlobalInfo.TargetBonePosition = TargetCache.BoneRegister[(int)Target.GlobalInfo.TargetBoneId];
				Target.GlobalInfo.TargetBonePosition2D = TargetCache.BoneRegister2D[(int)Target.GlobalInfo.TargetBoneId];

				// Apply FPS scaling for smoothing
				if (FPSScale) {
					float AimbotSpeed;
					if (Target.LocalInfo.CurrentSmoothing <= 1) {
						AimbotSpeed = Target.LocalInfo.CurrentSmoothing;
					}
					else {
						AimbotSpeed = Target.LocalInfo.CurrentSmoothing * FPSScale;
					}
					Target.LocalInfo.TargetRotation = Math::CalculateRotationAngles(Camera.Position, TargetCache.BoneRegister[(int)Target.GlobalInfo.TargetBoneId]);

					// Calculate smoothed rotation
					Target.LocalInfo.TargetRotationChange = SDK::FRotator(Target.LocalInfo.TargetRotation.Pitch - Camera.Rotation.Pitch, Target.LocalInfo.TargetRotation.Yaw - Camera.Rotation.Yaw, 0.f);
					Target.LocalInfo.TargetRotationChange = Math::NormalizeAxis(Target.LocalInfo.TargetRotationChange);

					SDK::FRotator RotationAfterSmooth = Target.LocalInfo.TargetRotationChange / AimbotSpeed;
					RotationAfterSmooth = Math::NormalizeAxis(RotationAfterSmooth);

					Target.LocalInfo.TargetRotationWithSmooth = SDK::FRotator(Camera.Rotation.Pitch + RotationAfterSmooth.Pitch, Camera.Rotation.Yaw + RotationAfterSmooth.Yaw, 0.f);
					Target.LocalInfo.TargetRotationWithSmooth = Math::NormalizeAxis(Target.LocalInfo.TargetRotationWithSmooth);
					Target.LocalInfo.TargetRotationWithSmooth = SDK::FRotator(Target.LocalInfo.TargetRotationWithSmooth.Pitch, Target.LocalInfo.TargetRotationWithSmooth.Yaw, 0.f); // 0 on the roll so the camera doesn't get stuck tilted
				}
			}
		};

		/*
		* @brief Represents a building weak spot target, inherits from Target and adds functions for dealing with building weak spot targets
		*/
		class WeakSpotTarget : public Target {
		public:
			/*
			 * @brief Propagates a BuildingWeakSpot target's info to the target
			 *
			 * (If the optional arguments are not provided, then no aimbot calculations are made)
			 *
			 * @param Target - The target to update
			 * @param WeakSpot - The target's building weak spot
			 * @param Camera - The local player's camera (optional)
			 * @param FPSScale - The scale to apply to the smoothing (optional)
			 */
			static void UpdateTargetInfo(Target& Target, SDK::ABuildingWeakSpot* WeakSpot, const Camera& Camera = Camera(), const float FPSScale = float()) {
				// Update global information
				Target.GlobalInfo.TargetActor = WeakSpot;

				// Update positions
				SDK::FVector RootComponentPosition = WeakSpot->GetRootComponent()->GetPosition();
				SDK::FVector2D RootComponentPosition2D = SDK::Project(RootComponentPosition);

				Target.GlobalInfo.TargetActorPosition = RootComponentPosition;
				Target.GlobalInfo.TargetActorPosition2D = RootComponentPosition2D;
				Target.GlobalInfo.TargetBonePosition = RootComponentPosition;
				Target.GlobalInfo.TargetBonePosition2D = RootComponentPosition2D;

				// Update local information
				Target.LocalInfo.DistanceFromCrosshair = Math::GetDistance2D(RootComponentPosition2D.X, RootComponentPosition2D.Y, SDK::GetLocalCanvas()->SizeX() / 2, SDK::GetLocalCanvas()->SizeY() / 2);
				Target.LocalInfo.DistanceFromPlayer = 0;
				Target.LocalInfo.SmartTargetingDistance = Target.LocalInfo.DistanceFromCrosshair + Target.LocalInfo.DistanceFromPlayer;

				// Determine target type
				Target.GlobalInfo.Type = Target::TargetType::Weakspot;

				// Apply FPS scaling for smoothing
				if (FPSScale) {
					float AimbotSpeed;
					if (Target.LocalInfo.CurrentSmoothing <= 1) {
						AimbotSpeed = Target.LocalInfo.CurrentSmoothing;
					}
					else {
						AimbotSpeed = Target.LocalInfo.CurrentSmoothing * FPSScale;
					}
					Target.LocalInfo.TargetRotation = Math::CalculateRotationAngles(Camera.Position, RootComponentPosition);

					// Calculate smoothed rotation
					Target.LocalInfo.TargetRotationChange = SDK::FRotator(Target.LocalInfo.TargetRotation.Pitch - Camera.Rotation.Pitch, Target.LocalInfo.TargetRotation.Yaw - Camera.Rotation.Yaw, 0.f);
					Target.LocalInfo.TargetRotationChange = Math::NormalizeAxis(Target.LocalInfo.TargetRotationChange);

					SDK::FRotator RotationAfterSmooth = Target.LocalInfo.TargetRotationChange / AimbotSpeed;
					RotationAfterSmooth = Math::NormalizeAxis(RotationAfterSmooth);

					Target.LocalInfo.TargetRotationWithSmooth = SDK::FRotator(Camera.Rotation.Pitch + RotationAfterSmooth.Pitch, Camera.Rotation.Yaw + RotationAfterSmooth.Yaw, 0.f);
					Target.LocalInfo.TargetRotationWithSmooth = Math::NormalizeAxis(Target.LocalInfo.TargetRotationWithSmooth);
					Target.LocalInfo.TargetRotationWithSmooth = SDK::FRotator(Target.LocalInfo.TargetRotationWithSmooth.Pitch, Target.LocalInfo.TargetRotationWithSmooth.Yaw, 0.f); // 0 on the roll so the camera doesn't get stuck tilted
				}
			}
		};
	}
}