#include "Aimbot.h"

#include "../../../Configs/Config.h"
#include "../../Input/Input.h"
#include "Target.h"

#include "../../Actors/Actors.h"

void Features::Aimbot::AimbotTarget(Target& TargetToAimot)
{
    if (Input::IsKeyDown((Input::KeyName)Config::Aimbot::AimKey))
    {
        TargetToAimot.LocalInfo.IsTargeting = true;

        // We don't need to aimbot the target if using SilentAim or BulletTP
        if (Config::Aimbot::SilentAim == false && Config::Aimbot::BulletTP == false && Config::Aimbot::BulletTPV2 == false)
        {
            SDK::GetLocalController()->AddPitchInput(TargetToAimot.LocalInfo.TargetRotationChangeWithSmooth.Pitch / SDK::GetLocalController()->InputPitchScale());
            SDK::GetLocalController()->AddYawInput(TargetToAimot.LocalInfo.TargetRotationChangeWithSmooth.Yaw / SDK::GetLocalController()->InputYawScale());
        }
    }
    else
    {
        TargetToAimot.LocalInfo.IsTargeting = false;
    }
}

void Features::Aimbot::CalculateShotCallback(SDK::FTransform* BulletTransform)
{
    if (Config::Aimbot::BulletTP && Actors::MainTarget.GlobalInfo.TargetActor)
    {
        SDK::FVector Position = Actors::MainTarget.GlobalInfo.TargetBonePosition;
        Position.Z += 10.f; // Add 0.01 meters to the Z axis to make sue it always hits the correct bone

        BulletTransform->Translation = Position;
    }
}

void Features::Aimbot::RaycastMultiCallback(SDK::UWorld* World, SDK::TArray<SDK::FHitResult>& OutHits, const SDK::ECollisionChannel TraceChannel)
{
    if (Actors::MainTarget.GlobalInfo.TargetActor && Config::Aimbot::BulletTPV2)
    {
        // Only modify the line trace for the bullet
        if (TraceChannel != SDK::ECollisionChannel::ECC_GameTraceChannel7) return;

        for (int i = 0; i < OutHits.Num(); i++)
        {
            SDK::FHitResult OutHit = OutHits[i];

            // Prepare data for our own line trace
            SDK::TArray<SDK::AActor*> ActorsToIgnore;
            SDK::FVector Position = Actors::MainTarget.GlobalInfo.TargetBonePosition;
            Position.Z += 100.f; // Add 0.1 meters to the Z axis to make sure it always hits the correct bone

            // Save the original start position of the RaycastMulti
            SDK::FVector OriginalStart = OutHits[i].TraceStart();

            // Run our own line trace that is guaranteed to hit the target bone
            SDK::UKismetSystemLibrary::LineTraceSingle(World, Position, Actors::MainTarget.GlobalInfo.TargetBonePosition, (SDK::ETraceTypeQuery)TraceChannel, true, ActorsToIgnore, SDK::EDrawDebugTrace::None, OutHit, false, SDK::FLinearColor(), SDK::FLinearColor(), 0.f);

            // Revert some data back to the original RaycastMulti (to avoid some possible detections)
            OutHit.SetTraceStart(OriginalStart);
            OutHit.SetDistance(OriginalStart.Distance(Actors::MainTarget.GlobalInfo.TargetBonePosition));

            // Set the new hit result
            OutHits[i] = OutHit;
        }
    }
}

void Features::Aimbot::GetViewpointCallback(SDK::FMinimalViewInfo* OutViewInfo)
{
    if (Config::Aimbot::SilentAim == false) return;
    if (!Actors::MainTarget.LocalInfo.IsTargeting == false && Config::Aimbot::UseAimKeyForSilent) return;

    if (Actors::MainTarget.GlobalInfo.TargetActor)
    {
        // This function is called when gettting the camera viewpoint for rendering
        // Revert the cameras location and rotation to the original values
        OutViewInfo->SetRotation(Actors::MainCamera.Rotation);
        OutViewInfo->SetLocation(Actors::MainCamera.Position);
    }
}

void Features::Aimbot::GetPlayerViewpointCallback(SDK::FVector* Location, SDK::FRotator* Rotation)
{
    if (Config::Aimbot::SilentAim == false) return;
    if (Actors::MainTarget.LocalInfo.IsTargeting == false && Config::Aimbot::UseAimKeyForSilent) return;

    if (Actors::MainTarget.GlobalInfo.TargetActor)
    {
        // This function is called when calculating bullet trajectories
        // Set the rotation to the target aimbot rotation
        *Rotation = Actors::MainTarget.LocalInfo.TargetRotationWithSmooth;
    }
}