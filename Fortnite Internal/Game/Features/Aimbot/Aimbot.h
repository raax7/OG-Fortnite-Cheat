#pragma once
#include "../Features.h"

#include "../../SDK/Classes/Engine_Classes.h"

namespace Features {
	namespace Aimbot {
		/*
		* @brief Aimot a player target
		*
		* @param TargetToAimot The target to aimot
		*/
		void AimbotTarget(Aimbot::Target& TargetToAimot);

		/*
		* @brief Callback for the CalculateShot hook (used for silent aim and bullet TP)
		* 
		* @param ShotTransform The bullet transform
		*/
		void CalculateShotCallback(SDK::FTransform* BulletTransform);

		/*
		* @brief Callback for the GetViewpoint hook (used for silent aim)
		* 
		* @param OutViewInfo The view info
		*/
		void GetViewpointCallback(SDK::FMinimalViewInfo* OutViewInfo);

		/*
		* @brief Callback for the GetPlayerViewpoint hook (used for silent aim)
		* 
		* @param this_ The player controller
		*/
		void GetPlayerViewpointCallback(SDK::FRotator* Rotation);
	};
}