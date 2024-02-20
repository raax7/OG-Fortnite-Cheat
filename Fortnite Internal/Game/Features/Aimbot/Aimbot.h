#pragma once
#include "../../Actors/Actors.h"
#include "Target.h"
#include "../../Features/FortPawn/Bone.h"
#include "../../SDK/Classes/FortniteGame_Classes.h"
#include "../../Input/Input.h"

namespace Features {
	namespace Aimbot {
		// Used to aimot a Target
		class Aimbot {
		public:
			/*
			* @brief Aimot a player target
			*
			* @param TargetToAimot The target to aimot
			*/
			static void AimbotTarget(Target& TargetToAimot) {
				// CONVERT TO ENGINE KEY DETECTION
				if (Input::IsKeyDown(Input::KeyName::RightMouseButton)) {
					TargetToAimot.LocalInfo.IsTargeting = true;

					if (Config::Aimbot::SilentAim == false) {
						SDK::GetLocalController()->ClientSetRotation(TargetToAimot.LocalInfo.TargetRotationWithSmooth, false);
					}
				}
				else {
					TargetToAimot.LocalInfo.IsTargeting = false;
				}

				if (TargetToAimot.GlobalInfo.Type == Target::TargetType::ClosePlayer || TargetToAimot.GlobalInfo.Type == Target::TargetType::FarPlayer) {
					if (TargetToAimot.GlobalInfo.PawnCache.AnyBoneVisible == false) {
						TargetToAimot.ResetTarget();
					}
				}
			}
		};
	}
}