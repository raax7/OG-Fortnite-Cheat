#pragma once
#include "../Features.h"

#include "../../SDK/Classes/Engine_Classes.h"

namespace Features
{
    namespace Aimbot
    {
        /*
        * @brief Aimot a player target
        *
        * @param TargetToAimot The target to aimot
        */
        void AimbotTarget(Aimbot::Target& TargetToAimot);

        /*
        * @brief Callback for the CalculateShot hook (used for silent aim and bullet TP V1)
        *
        * @param ShotTransform The bullet transform
        */
        void CalculateShotCallback(SDK::FTransform* BulletTransform);

        /*
        * @brief Callback for the RaycastMulti hook (used for bullet TP V2)
        *
        * @param World The world (from the RaycastMulti hook)
        * @param OutHits The hit results (from the RaycastMulti hook)
        * @param TraceChannel The trace channel (from the RaycastMulti hook)
        */
        void RaycastMultiCallback(SDK::UWorld* World, SDK::TArray<SDK::FHitResult>& OutHits, const SDK::ECollisionChannel TraceChannel);

        /*
        * @brief Callback for the GetViewpoint hook (used for silent aim)
        *
        * @param OutViewInfo The view info
        */
        void GetViewpointCallback(SDK::FMinimalViewInfo* OutViewInfo);

        /*
        * @brief Callback for the GetPlayerViewpoint hook (used for silent aim)
        *
        * @param Location The location of the camera
        * @param Rotation The rotation of the camera
        */
        void GetPlayerViewpointCallback(SDK::FVector* Location, SDK::FRotator* Rotation);
    };
}