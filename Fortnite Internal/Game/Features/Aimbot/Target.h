#ifndef TARGET_H
#define TARGET_H

#include <Windows.h>
#include "../../SDK/Classes/CoreUObject_classes.h"
#include "../../../Configs/Config.h"
#include "../../SDK/Classes/Engine_classes.h"

class Target {
private:
public:
	enum TargetType {
		ClosePlayer = 0,
		FarPlayer = 1,
		Weakspot = 2,
		Other = 3,

		MAX = 4,
	};

	struct Target {
		TargetType targetType = TargetType::MAX;

		SDK::FVector PlayerPosition;
		SDK::FVector2D PlayerPosition2D;

		SDK::FVector TargetBonePosition;
		SDK::FVector2D TargetBonePosition2D;
		int TargetBoneId;

		float DistanceFromCrosshair = FLT_MAX;
		float DistanceFromPlayer = FLT_MAX;
		float SmartTargetingDistance = FLT_MAX;

		SDK::AActor* Actor;

		// Local Info on Target
		bool IsTargeting = false;
		SDK::FRotator TargetRotation;
		SDK::FRotator TargetRotationChange;
		float CurrentFOVSize;
		float CurrentSmoothing;
	};



	inline void ResetTarget(Target* target) {
		target->targetType = TargetType::MAX;

		target->PlayerPosition = SDK::FVector(0, 0, 0);
		target->PlayerPosition2D = SDK::FVector2D(0, 0);

		target->TargetBonePosition = SDK::FVector(0, 0, 0);
		target->TargetBonePosition2D = SDK::FVector2D(0, 0);
		target->TargetBoneId = 0;

		target->DistanceFromCrosshair = FLT_MAX;
		target->DistanceFromPlayer = FLT_MAX;
		target->SmartTargetingDistance = FLT_MAX;

		target->Actor = nullptr;

		target->IsTargeting = false;
		target->TargetRotation = SDK::FRotator(0, 0, 0);
		target->TargetRotationChange = SDK::FRotator(0, 0, 0);
	}

	inline void TargetTick(Target* currentTarget, bool SeenTargetLastFrame) {
		if (!SeenTargetLastFrame) {
			// If the player was not seen last frame, then reset the target
			ResetTarget(currentTarget);
		}

		if (currentTarget->DistanceFromCrosshair > currentTarget->CurrentFOVSize) {
			// If the player is outside of the FOV circle, then reset the target
			ResetTarget(currentTarget);
		}

		switch (currentTarget->targetType) {
		case ClosePlayer:
			currentTarget->CurrentFOVSize = Settings::CloseFOVSize;
			currentTarget->CurrentSmoothing = Settings::CloseSmoothing;

			if (currentTarget->DistanceFromPlayer > Settings::CloseRange || !Settings::UseCloseAim) {
				currentTarget->targetType = TargetType::FarPlayer;
			}
			break;
		case FarPlayer:
			currentTarget->CurrentFOVSize = Settings::FOVSize;
			currentTarget->CurrentSmoothing = Settings::Smoothing;

			if (currentTarget->DistanceFromPlayer <= Settings::CloseRange && Settings::UseCloseAim) {
				currentTarget->targetType = TargetType::ClosePlayer;
			}
			break;
		case Weakspot:
			currentTarget->CurrentFOVSize = Settings::CloseFOVSize;
			currentTarget->CurrentSmoothing = Settings::CloseSmoothing;
			break;
		case Other:
			currentTarget->CurrentFOVSize = Settings::FOVSize;
			currentTarget->CurrentSmoothing = Settings::Smoothing;
			break;
		}
	}

	inline bool ShouldSetTarget(Target currentTarget, Target queriedTarget, float FOVSizeForCheck) {
		if (queriedTarget.DistanceFromCrosshair > FOVSizeForCheck) {
			// If the player is outside of the FOV circle, then don't update the target
			return false;
		}

		if (queriedTarget.targetType > currentTarget.targetType) {
			// If the target priority is lower than the current target, then don't update the target
			return false;
		}
		else if (queriedTarget.targetType < currentTarget.targetType) {
			// If the target priority is higher than the current target, then update the target
			return true;
		}
		else if (queriedTarget.targetType == currentTarget.targetType) {
			// If the target priority is the same as the current target, then check the parameters
			if (currentTarget.IsTargeting) {
				// If we are targeting, then don't update
				return false;
			}
			else {
				float CurrentDistance;
				float QueriedDistance;

				// Get the distance based off the target distance types
				// 0 - Smart Distance (player distance & crosshair distance)
				// 1 - Crosshair Distance
				// 2 - Player Distance
				switch (Settings::TargetingType) {
				case 0:
					CurrentDistance = currentTarget.SmartTargetingDistance;
					QueriedDistance = queriedTarget.SmartTargetingDistance;
					break;
				case 1:
					CurrentDistance = currentTarget.DistanceFromCrosshair;
					QueriedDistance = queriedTarget.DistanceFromCrosshair;
					break;
				case 2:
					CurrentDistance = currentTarget.DistanceFromPlayer;
					QueriedDistance = queriedTarget.DistanceFromPlayer;
					break;
				default:
					return false;
				}

				if (QueriedDistance < CurrentDistance) {
					// If the queried target distance is less than the current target distance, then update the target
					return true;
				}
			}
		}

		return false;
	}

	inline void SetTarget(Target* currentTarget, Target newTargetData, bool runTargetChecks = true) {
		switch (newTargetData.targetType) {
		case ClosePlayer:
			newTargetData.CurrentFOVSize = Settings::CloseFOVSize;
			newTargetData.CurrentSmoothing = Settings::CloseSmoothing;
			break;
		case FarPlayer:
			newTargetData.CurrentFOVSize = Settings::FOVSize;
			newTargetData.CurrentSmoothing = Settings::Smoothing;
			break;
		case Weakspot:
			newTargetData.CurrentFOVSize = 350;
			newTargetData.CurrentSmoothing = 3;
			break;
		case Other:
			newTargetData.CurrentFOVSize = Settings::FOVSize;
			newTargetData.CurrentSmoothing = Settings::Smoothing;
			break;
		}

		if (runTargetChecks) {
			if (!ShouldSetTarget(*currentTarget, newTargetData, newTargetData.CurrentFOVSize)) {
				return;
			}
		}

		*currentTarget = newTargetData;
	}
}

#endif