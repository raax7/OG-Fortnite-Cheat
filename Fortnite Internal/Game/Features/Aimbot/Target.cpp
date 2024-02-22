#include "Target.h"

#include "../../../Utilities/Math.h"
#include "../../../Configs/Config.h"
#include "../FortPawnHelper/Bone.h"

void Features::Aimbot::Target::UpdateLocalInfoAndType(Target& TargetToUpdate) {
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
void Features::Aimbot::Target::ResetTarget() {
	// Reset the global info
	{
		GlobalInfo.Type = TargetType::TargetType_MAX;

		GlobalInfo.TargetActor = nullptr;

		GlobalInfo.TargetActorPosition = SDK::FVector(0, 0, 0);
		GlobalInfo.TargetActorPosition2D = SDK::FVector2D(0, 0);

		GlobalInfo.TargetBonePosition = SDK::FVector(0, 0, 0);
		GlobalInfo.TargetBonePosition2D = SDK::FVector2D(0, 0);
		GlobalInfo.TargetBoneId = Features::FortPawnHelper::Bone::None;
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
void Features::Aimbot::Target::TargetTick(bool SeenTargetThisFrame) {
	if (!SeenTargetThisFrame || LocalInfo.DistanceFromCrosshair > LocalInfo.CurrentRealFOVSize || GlobalInfo.TargetActor == nullptr) {
		// Resets if:
		// - The target wasn't seen this frame
		// - The target is outside of the FOV circle
		// - The target pointer is null
		ResetTarget();
	}

	UpdateLocalInfoAndType(*this);
}
bool Features::Aimbot::Target::ShouldSetTarget(Target PotentialTarget) {
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
void Features::Aimbot::Target::SetTarget(Target NewTarget, bool ForceSetTarget) {
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

void Features::Aimbot::PlayerTarget::UpdateTargetInfo(Target& Target, Actors::Caches::FortPawnCache& TargetCache, const Actors::CameraCache& Camera, const float FPSScale) {
	// Update global information
	Target.GlobalInfo.TargetActor = TargetCache.FortPawn;
	Target.GlobalInfo.TargetBoneId = Features::FortPawnHelper::Bone::FindBestBone(Features::FortPawnHelper::Bone::Head, TargetCache);

	// Update local information
	Target.LocalInfo.DistanceFromCrosshair = Math::GetDistance2D(TargetCache.BoneRegister2D[(int)Target.GlobalInfo.TargetBoneId].X, TargetCache.BoneRegister2D[(int)Target.GlobalInfo.TargetBoneId].Y, Game::ScreenWidth / 2.f, Game::ScreenHeight / 2.f);
	Target.LocalInfo.DistanceFromPlayer = TargetCache.DistanceFromLocal;
	Target.LocalInfo.SmartTargetingDistance = Target.LocalInfo.DistanceFromCrosshair + Target.LocalInfo.DistanceFromPlayer;

	// Determine target type
	Target.GlobalInfo.Type = (Target.LocalInfo.DistanceFromPlayer <= Config::Aimbot::CloseAim::Range && Config::Aimbot::CloseAim::Enabled) ? Target::TargetType::ClosePlayer : Target::TargetType::FarPlayer;

	// Update positions
	Target.GlobalInfo.TargetActorPosition = TargetCache.BoneRegister[Features::FortPawnHelper::Bone::Root];
	Target.GlobalInfo.TargetActorPosition2D = TargetCache.BoneRegister2D[Features::FortPawnHelper::Bone::Root];
	Target.GlobalInfo.TargetBonePosition = TargetCache.BoneRegister[Target.GlobalInfo.TargetBoneId];
	Target.GlobalInfo.TargetBonePosition2D = TargetCache.BoneRegister2D[Target.GlobalInfo.TargetBoneId];

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

void Features::Aimbot::WeakSpotTarget::UpdateTargetInfo(Target& Target, SDK::ABuildingWeakSpot* WeakSpot, const Actors::CameraCache& Camera, const float FPSScale) {
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
	Target.LocalInfo.DistanceFromCrosshair = Math::GetDistance2D(RootComponentPosition2D.X, RootComponentPosition2D.Y, Game::ScreenWidth / 2.f, Game::ScreenHeight / 2.f);
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