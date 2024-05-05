#include "Target.h"

#include "../../../Configs/Config.h"
#include "../../../Utilities/Math.h"

#include "../FortPawnHelper/Bone.h"

#include "../../Game.h"

void Features::Aimbot::Target::UpdateLocalInfoAndType(Target& TargetToUpdate)
{
    switch (TargetToUpdate.GlobalInfo.Type)
    {
    case TargetType::ClosePlayer:
        TargetToUpdate.LocalInfo.CurrentFOVSizePixels = (float)Config::Aimbot::CloseAim::FOV * Game::PixelsPerDegree;
        TargetToUpdate.LocalInfo.CurrentFOVSizeDegrees = Config::Aimbot::CloseAim::FOV;
        TargetToUpdate.LocalInfo.CurrentSmoothing = Config::Aimbot::CloseAim::Smoothing;

        if (TargetToUpdate.LocalInfo.DistanceFromPlayer > Config::Aimbot::CloseAim::Range || !Config::Aimbot::CloseAim::Enabled)
        {
            TargetToUpdate.GlobalInfo.Type = TargetType::FarPlayer;
        }
        break;
    case TargetType::FarPlayer:
        TargetToUpdate.LocalInfo.CurrentFOVSizePixels = (float)Config::Aimbot::Standard::FOV * Game::PixelsPerDegree;
        TargetToUpdate.LocalInfo.CurrentFOVSizeDegrees = Config::Aimbot::Standard::FOV;
        TargetToUpdate.LocalInfo.CurrentSmoothing = Config::Aimbot::Standard::Smoothing;

        if (TargetToUpdate.LocalInfo.DistanceFromPlayer <= Config::Aimbot::CloseAim::Range && Config::Aimbot::CloseAim::Enabled)
        {
            TargetToUpdate.GlobalInfo.Type = TargetType::ClosePlayer;
        }
        break;
    case TargetType::Weakspot:
        TargetToUpdate.LocalInfo.CurrentFOVSizePixels = (float)Config::Aimbot::Weakspot::FOV * Game::PixelsPerDegree;
        TargetToUpdate.LocalInfo.CurrentFOVSizeDegrees = Config::Aimbot::Weakspot::FOV;
        TargetToUpdate.LocalInfo.CurrentSmoothing = Config::Aimbot::Weakspot::Smoothing;
        break;
    case TargetType::Other:
        TargetToUpdate.LocalInfo.CurrentFOVSizePixels = (float)Config::Aimbot::Standard::FOV * Game::PixelsPerDegree;
        TargetToUpdate.LocalInfo.CurrentFOVSizeDegrees = Config::Aimbot::Standard::FOV;
        TargetToUpdate.LocalInfo.CurrentSmoothing = Config::Aimbot::Standard::Smoothing;
        break;
    }
}
void Features::Aimbot::Target::ResetTarget()
{
    // Reset the global info
    {
        GlobalInfo.Type = TargetType::NONE;

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
        LocalInfo.DistanceFromCrosshairDegrees = FLT_MAX;
        LocalInfo.DistanceFromCrosshairPixels = FLT_MAX;
        LocalInfo.DistanceFromPlayer = FLT_MAX;
        LocalInfo.SmartTargetingDistance = FLT_MAX;

        LocalInfo.IsTargeting = false;
        LocalInfo.IsOnScreen = false;

        LocalInfo.TargetRotation = SDK::FRotator(0, 0, 0);
        LocalInfo.TargetRotationChange = SDK::FRotator(0, 0, 0);

        LocalInfo.CurrentFOVSizePixels = 0;
        LocalInfo.CurrentFOVSizeDegrees = 0;
        LocalInfo.CurrentSmoothing = 0.f;
    }
}
void Features::Aimbot::Target::TargetTick(const bool SeenTargetThisFrame)
{
    // Reset if:
    // - The target wasn't seen this frame
    // - The target is outside of the FOV circle in pixels and is on the screen
    // - The target is outside of the FOV circle in degrees and is NOT on the screen
    // - The target pointer is null

    UpdateLocalInfoAndType(*this);

    if (LocalInfo.IsOnScreen)
    {
        if (LocalInfo.DistanceFromCrosshairPixels > LocalInfo.CurrentFOVSizePixels)
        {
            // If the player is on the screen and outside of the FOV circle in pixels, then don't update the target
            ResetTarget();
        }
    }
    else
    {
        if (LocalInfo.DistanceFromCrosshairDegrees > LocalInfo.CurrentFOVSizeDegrees)
        {
            // If the player is on the screen and outside of the FOV circle in degrees, then don't update the target
            ResetTarget();
        }
    }

    if (!SeenTargetThisFrame || GlobalInfo.TargetActor == nullptr)
    {
        ResetTarget();
        return;
    }
}
bool Features::Aimbot::Target::ShouldSetTarget(Target& PotentialTarget)
{
    // We have to do this goofy method to simulate the distance from the crosshair in degrees. It's not perfect, but it's good enough and not noticeable.

    // Update the info so that we use the correct FOV size for verifying if we should update the target
    UpdateLocalInfoAndType(PotentialTarget);

    if (PotentialTarget.LocalInfo.IsOnScreen)
    {
        if (PotentialTarget.LocalInfo.DistanceFromCrosshairPixels > PotentialTarget.LocalInfo.CurrentFOVSizePixels)
        {
            // If the player is on the screen and outside of the FOV circle in pixels, then don't update the target
            return false;
        }
    }
    else
    {
        if (PotentialTarget.LocalInfo.DistanceFromCrosshairDegrees > PotentialTarget.LocalInfo.CurrentFOVSizeDegrees)
        {
            // If the player is on the screen and outside of the FOV circle in degrees, then don't update the target
            return false;
        }
    }

    if (PotentialTarget.GlobalInfo.Type > GlobalInfo.Type)
    {
        // If the target priority is lower than the current target, then don't update the target
        return false;
    }
    else if (PotentialTarget.GlobalInfo.Type < GlobalInfo.Type)
    {
        // If the target priority is higher than the current target, then update the target
        return true;
    }
    else if (PotentialTarget.GlobalInfo.Type == GlobalInfo.Type)
    {
        // If the target priority is the same as the current target, then check the parameters
        if (LocalInfo.IsTargeting && Config::Aimbot::StickyAim)
        {
            // If we are targeting and using sticky aim, then don't update
            return false;
        }

        float CurrentDistance = 0.f;
        float PotentialTargetDistance = 0.f;

        // Get the distance based off the target distance types
        switch (Config::Aimbot::TargettingType)
        {
        case ConfigTypes::AimbotType::Smart:
            CurrentDistance = LocalInfo.SmartTargetingDistance;
            PotentialTargetDistance = PotentialTarget.LocalInfo.SmartTargetingDistance;
            break;
        case ConfigTypes::AimbotType::Crosshair:
            CurrentDistance = LocalInfo.DistanceFromCrosshairDegrees;
            PotentialTargetDistance = PotentialTarget.LocalInfo.DistanceFromCrosshairDegrees;
            break;
        case ConfigTypes::AimbotType::Distance:
            CurrentDistance = LocalInfo.DistanceFromPlayer;
            PotentialTargetDistance = PotentialTarget.LocalInfo.DistanceFromPlayer;
            break;
        default:
            return false;
        }

        if (PotentialTargetDistance < CurrentDistance)
        {
            // If the potential new target distance is less than the current target distance, then update the target
            return true;
        }
    }

    return false;
}
void Features::Aimbot::Target::SetTarget(Target& NewTarget, const bool ForceSetTarget)
{
    UpdateLocalInfoAndType(NewTarget);

    if (ForceSetTarget)
    {
        *this = NewTarget;
        return;
    }
    else if (ShouldSetTarget(NewTarget))
    {
        *this = NewTarget;
        return;
    }
}

void Features::Aimbot::PlayerTarget::UpdateTargetInfo(Target& Target, Actors::Caches::FortPawnCache& TargetCache, const Actors::Caches::CameraCache& MainCamera, const Actors::Caches::CameraCache& AimbotCamera, const float FPSScale)
{
    // Update global information
    Target.GlobalInfo.TargetActor = TargetCache.FortPawn;
    Target.GlobalInfo.TargetBoneId = Features::FortPawnHelper::Bone::FindBestBone(Features::FortPawnHelper::Bone::Head, TargetCache, (Config::Aimbot::VisibleCheck && Config::Aimbot::BulletTP == false && Config::Aimbot::BulletTPV2 == false));

    // Determine target type
    Target.GlobalInfo.Type = (Target.LocalInfo.DistanceFromPlayer <= Config::Aimbot::CloseAim::Range && Config::Aimbot::CloseAim::Enabled) ? Target::TargetType::ClosePlayer : Target::TargetType::FarPlayer;

    // Update positions
    Target.GlobalInfo.TargetActorPosition = TargetCache.BonePositions3D[Features::FortPawnHelper::Bone::Root];
    Target.GlobalInfo.TargetActorPosition2D = TargetCache.BonePositions2D[Features::FortPawnHelper::Bone::Root];
    Target.GlobalInfo.TargetBonePosition = TargetCache.BonePositions3D[Target.GlobalInfo.TargetBoneId];
    Target.GlobalInfo.TargetBonePosition2D = TargetCache.BonePositions2D[Target.GlobalInfo.TargetBoneId];

    SDK::FRotator TargetCameraRotation = SDK::UKismetMathLibrary::FindLookAtRotation(AimbotCamera.Position, Target.GlobalInfo.TargetBonePosition);

    // Update local information
    Target.LocalInfo.DistanceFromCrosshairDegrees = Math::GetDegreeDistance(MainCamera.Rotation, TargetCameraRotation);
    Target.LocalInfo.DistanceFromCrosshairPixels = Math::GetDistance2D(Target.GlobalInfo.TargetBonePosition2D.X, Target.GlobalInfo.TargetBonePosition2D.Y, Game::ScreenCenterX, Game::ScreenCenterY);
    Target.LocalInfo.DistanceFromPlayer = TargetCache.DistanceFromLocalPawn;
    Target.LocalInfo.SmartTargetingDistance = (Target.LocalInfo.DistanceFromCrosshairDegrees * 20) + Target.LocalInfo.DistanceFromPlayer;
    Target.LocalInfo.IsOnScreen = TargetCache.IsPlayerVisibleOnScreen;

    // Apply FPS scaling for smoothing
    if (FPSScale)
    {
        UpdateLocalInfoAndType(Target);

        float AimbotSpeed;
        if (Target.LocalInfo.CurrentSmoothing <= 1.f)
        {
            AimbotSpeed = Target.LocalInfo.CurrentSmoothing;
        }
        else
        {
            AimbotSpeed = Target.LocalInfo.CurrentSmoothing * FPSScale;
        }
        Target.LocalInfo.TargetRotation = TargetCameraRotation;

        // Calculate smoothed rotation
        Target.LocalInfo.TargetRotationChange = SDK::FRotator(Target.LocalInfo.TargetRotation.Pitch - AimbotCamera.Rotation.Pitch, Target.LocalInfo.TargetRotation.Yaw - AimbotCamera.Rotation.Yaw, 0.f);
        Target.LocalInfo.TargetRotationChange = Math::NormalizeAxis(Target.LocalInfo.TargetRotationChange);

        Target.LocalInfo.TargetRotationChangeWithSmooth = Target.LocalInfo.TargetRotationChange / AimbotSpeed;
        Target.LocalInfo.TargetRotationChangeWithSmooth = Math::NormalizeAxis(Target.LocalInfo.TargetRotationChangeWithSmooth);

        Target.LocalInfo.TargetRotationWithSmooth = SDK::FRotator(AimbotCamera.Rotation.Pitch + Target.LocalInfo.TargetRotationChangeWithSmooth.Pitch, AimbotCamera.Rotation.Yaw + Target.LocalInfo.TargetRotationChangeWithSmooth.Yaw, 0.f);
        Target.LocalInfo.TargetRotationWithSmooth = Math::NormalizeAxis(Target.LocalInfo.TargetRotationWithSmooth);
        Target.LocalInfo.TargetRotationWithSmooth = SDK::FRotator(Target.LocalInfo.TargetRotationWithSmooth.Pitch, Target.LocalInfo.TargetRotationWithSmooth.Yaw, 0.f); // 0 on the roll so the camera doesn't get stuck tilted
    }
}

void Features::Aimbot::WeakSpotTarget::UpdateTargetInfo(Target& Target, SDK::ABuildingWeakSpot* WeakSpot, const Actors::Caches::CameraCache& MainCamera, const Actors::Caches::CameraCache& AimbotCamera, const float FPSScale)
{
    // Update global information
    Target.GlobalInfo.TargetActor = WeakSpot;

    // Set target type
    Target.GlobalInfo.Type = Target::TargetType::Weakspot;

    // Update positions
    SDK::FVector RootComponentPosition = WeakSpot->RootComponent()->RelativeLocation();
    SDK::FVector2D RootComponentPosition2D = SDK::Project(RootComponentPosition);

    Target.GlobalInfo.TargetActorPosition = RootComponentPosition;
    Target.GlobalInfo.TargetActorPosition2D = RootComponentPosition2D;
    Target.GlobalInfo.TargetBonePosition = RootComponentPosition;
    Target.GlobalInfo.TargetBonePosition2D = RootComponentPosition2D;

    SDK::FRotator TargetCameraRotation = SDK::UKismetMathLibrary::FindLookAtRotation(AimbotCamera.Position, Target.GlobalInfo.TargetActorPosition);

    // Update local information
    Target.LocalInfo.DistanceFromCrosshairDegrees = Math::GetDegreeDistance(MainCamera.Rotation, TargetCameraRotation);
    Target.LocalInfo.DistanceFromCrosshairPixels = Math::GetDistance2D(Target.GlobalInfo.TargetActorPosition2D.X, Target.GlobalInfo.TargetActorPosition2D.Y, Game::ScreenCenterX, Game::ScreenCenterY);
    Target.LocalInfo.DistanceFromPlayer = Actors::LocalPawnCache.Position.Distance(Target.GlobalInfo.TargetActorPosition) / 100.f;
    Target.LocalInfo.SmartTargetingDistance = (Target.LocalInfo.DistanceFromCrosshairDegrees * 20) + Target.LocalInfo.DistanceFromPlayer;
    Target.LocalInfo.IsOnScreen = Math::IsOnScreen(Target.GlobalInfo.TargetActorPosition2D);

    // Apply FPS scaling for smoothing
    if (FPSScale)
    {
        UpdateLocalInfoAndType(Target);

        float AimbotSpeed;
        if (Target.LocalInfo.CurrentSmoothing <= 1)
        {
            AimbotSpeed = Target.LocalInfo.CurrentSmoothing;
        }
        else
        {
            AimbotSpeed = Target.LocalInfo.CurrentSmoothing * FPSScale;
        }
        Target.LocalInfo.TargetRotation = TargetCameraRotation;

        // Calculate smoothed rotation
        Target.LocalInfo.TargetRotationChange = SDK::FRotator(Target.LocalInfo.TargetRotation.Pitch - AimbotCamera.Rotation.Pitch, Target.LocalInfo.TargetRotation.Yaw - AimbotCamera.Rotation.Yaw, 0.f);
        Target.LocalInfo.TargetRotationChange = Math::NormalizeAxis(Target.LocalInfo.TargetRotationChange);

        Target.LocalInfo.TargetRotationChangeWithSmooth = Target.LocalInfo.TargetRotationChange / AimbotSpeed;
        Target.LocalInfo.TargetRotationChangeWithSmooth = Math::NormalizeAxis(Target.LocalInfo.TargetRotationChangeWithSmooth);

        Target.LocalInfo.TargetRotationWithSmooth = SDK::FRotator(AimbotCamera.Rotation.Pitch + Target.LocalInfo.TargetRotationChangeWithSmooth.Pitch, AimbotCamera.Rotation.Yaw + Target.LocalInfo.TargetRotationChangeWithSmooth.Yaw, 0.f);
        Target.LocalInfo.TargetRotationWithSmooth = Math::NormalizeAxis(Target.LocalInfo.TargetRotationWithSmooth);
        Target.LocalInfo.TargetRotationWithSmooth = SDK::FRotator(Target.LocalInfo.TargetRotationWithSmooth.Pitch, Target.LocalInfo.TargetRotationWithSmooth.Yaw, 0.f); // 0 on the roll so the camera doesn't get stuck tilted
    }
}