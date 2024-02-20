#pragma once
#include <vector>
#include "../../SDK/Classes/Engine_classes.h"
#include "../../SDK/Classes/FortniteGame_classes.h"
#include "../../Actors/Actors.h"
#include "../../Actors/ActorCache.h"
#include "../../../Utilities/Math.h"
#include "../../Features/FortPawn/Bone.h"

namespace Features {
	namespace Aimbot {
		// Helper functions for FortPawns
		class FortPawn {
		public:
			/*
			* @brief Poplate all bone positions used in the skeleton
			*
			* @param FortPawn The FortPawn to populate bones
			*/
			static bool PopulateBones(Actors::Caches::FortPawnCache& FortPawnCache) {
				SDK::FVector2D Temp_W2S;

				FortPawnCache.BoneRegister.resize(100);
				FortPawnCache.BoneRegister2D.resize(100);

				FortPawnCache.BoneRegister[(int)Bone::BoneID::Bottom] = FortPawnCache.Mesh->GetBonePosition((int)Bone::BoneID::Bottom);
				if (FortPawnCache.BoneRegister[(int)Bone::BoneID::Bottom].X == 0 && FortPawnCache.BoneRegister[(int)Bone::BoneID::Bottom].Y == 0 && FortPawnCache.BoneRegister[(int)Bone::BoneID::Bottom].Z == 0) return false;
				FortPawnCache.BoneRegister2D[(int)Bone::BoneID::Bottom] = SDK::Project(FortPawnCache.BoneRegister[(int)Bone::BoneID::Bottom]);
				bool BottomVisible = Math::IsOnScreen(FortPawnCache.BoneRegister2D[(int)Bone::BoneID::Bottom]);

				FortPawnCache.BoneRegister[(int)Bone::BoneID::Head] = FortPawnCache.Mesh->GetBonePosition((int)Bone::BoneID::Head);
				FortPawnCache.BoneRegister2D[(int)Bone::BoneID::Head] = SDK::Project(FortPawnCache.BoneRegister[(int)Bone::BoneID::Head]);
				bool HeadVisible = Math::IsOnScreen(FortPawnCache.BoneRegister2D[(int)Bone::BoneID::Head]);

				FortPawnCache.BoneRegister[(int)Bone::BoneID::Pelvis] = FortPawnCache.Mesh->GetBonePosition((int)Bone::BoneID::Pelvis);
				FortPawnCache.BoneRegister2D[(int)Bone::BoneID::Pelvis] = SDK::Project(FortPawnCache.BoneRegister[(int)Bone::BoneID::Pelvis]);
				bool PelvisVisible = Math::IsOnScreen(FortPawnCache.BoneRegister2D[(int)Bone::BoneID::Pelvis]);

				if (!BottomVisible && !HeadVisible && !PelvisVisible) return false;

				FortPawnCache.BoneRegister[(int)Bone::BoneID::ChestLeft] = FortPawnCache.Mesh->GetBonePosition((int)Bone::BoneID::ChestLeft);
				FortPawnCache.BoneRegister[(int)Bone::BoneID::ChestRight] = FortPawnCache.Mesh->GetBonePosition((int)Bone::BoneID::ChestRight);
				FortPawnCache.BoneRegister[(int)Bone::BoneID::LeftShoulder] = FortPawnCache.Mesh->GetBonePosition((int)Bone::BoneID::LeftShoulder);
				FortPawnCache.BoneRegister[(int)Bone::BoneID::RightShoulder] = FortPawnCache.Mesh->GetBonePosition((int)Bone::BoneID::RightShoulder);
				FortPawnCache.BoneRegister[(int)Bone::BoneID::LeftElbow] = FortPawnCache.Mesh->GetBonePosition((int)Bone::BoneID::LeftElbow);
				FortPawnCache.BoneRegister[(int)Bone::BoneID::RightElbow] = FortPawnCache.Mesh->GetBonePosition((int)Bone::BoneID::RightElbow);
				FortPawnCache.BoneRegister[(int)Bone::BoneID::LeftHand] = FortPawnCache.Mesh->GetBonePosition((int)Bone::BoneID::LeftHand);
				FortPawnCache.BoneRegister[(int)Bone::BoneID::RightHand] = FortPawnCache.Mesh->GetBonePosition((int)Bone::BoneID::RightHand);
				FortPawnCache.BoneRegister[(int)Bone::BoneID::LeftLeg] = FortPawnCache.Mesh->GetBonePosition((int)Bone::BoneID::LeftLeg);
				FortPawnCache.BoneRegister[(int)Bone::BoneID::RightLeg] = FortPawnCache.Mesh->GetBonePosition((int)Bone::BoneID::RightLeg);
				FortPawnCache.BoneRegister[(int)Bone::BoneID::LeftKnee] = FortPawnCache.Mesh->GetBonePosition((int)Bone::BoneID::LeftKnee);
				FortPawnCache.BoneRegister[(int)Bone::BoneID::RightKnee] = FortPawnCache.Mesh->GetBonePosition((int)Bone::BoneID::RightKnee);
				FortPawnCache.BoneRegister[(int)Bone::BoneID::LeftFoot] = FortPawnCache.Mesh->GetBonePosition((int)Bone::BoneID::LeftFoot);
				FortPawnCache.BoneRegister[(int)Bone::BoneID::RightFoot] = FortPawnCache.Mesh->GetBonePosition((int)Bone::BoneID::RightFoot);

				FortPawnCache.BoneRegister2D[(int)Bone::BoneID::ChestLeft] = SDK::Project(FortPawnCache.BoneRegister[(int)Bone::BoneID::ChestLeft]);
				FortPawnCache.BoneRegister2D[(int)Bone::BoneID::ChestRight] = SDK::Project(FortPawnCache.BoneRegister[(int)Bone::BoneID::ChestRight]);
				FortPawnCache.BoneRegister2D[(int)Bone::BoneID::LeftShoulder] = SDK::Project(FortPawnCache.BoneRegister[(int)Bone::BoneID::LeftShoulder]);
				FortPawnCache.BoneRegister2D[(int)Bone::BoneID::RightShoulder] = SDK::Project(FortPawnCache.BoneRegister[(int)Bone::BoneID::RightShoulder]);
				FortPawnCache.BoneRegister2D[(int)Bone::BoneID::LeftElbow] = SDK::Project(FortPawnCache.BoneRegister[(int)Bone::BoneID::LeftElbow]);
				FortPawnCache.BoneRegister2D[(int)Bone::BoneID::RightElbow] = SDK::Project(FortPawnCache.BoneRegister[(int)Bone::BoneID::RightElbow]);
				FortPawnCache.BoneRegister2D[(int)Bone::BoneID::LeftHand] = SDK::Project(FortPawnCache.BoneRegister[(int)Bone::BoneID::LeftHand]);
				FortPawnCache.BoneRegister2D[(int)Bone::BoneID::RightHand] = SDK::Project(FortPawnCache.BoneRegister[(int)Bone::BoneID::RightHand]);
				FortPawnCache.BoneRegister2D[(int)Bone::BoneID::LeftLeg] = SDK::Project(FortPawnCache.BoneRegister[(int)Bone::BoneID::LeftLeg]);
				FortPawnCache.BoneRegister2D[(int)Bone::BoneID::RightLeg] = SDK::Project(FortPawnCache.BoneRegister[(int)Bone::BoneID::RightLeg]);
				FortPawnCache.BoneRegister2D[(int)Bone::BoneID::LeftKnee] = SDK::Project(FortPawnCache.BoneRegister[(int)Bone::BoneID::LeftKnee]);
				FortPawnCache.BoneRegister2D[(int)Bone::BoneID::RightKnee] = SDK::Project(FortPawnCache.BoneRegister[(int)Bone::BoneID::RightKnee]);
				FortPawnCache.BoneRegister2D[(int)Bone::BoneID::LeftFoot] = SDK::Project(FortPawnCache.BoneRegister[(int)Bone::BoneID::LeftFoot]);
				FortPawnCache.BoneRegister2D[(int)Bone::BoneID::RightFoot] = SDK::Project(FortPawnCache.BoneRegister[(int)Bone::BoneID::RightFoot]);

				return true;
			}

			/*
			* @brief Poplate all bone visibilities used in the skeleton
			*
			* @param FortPawn The FortPawn to populate visibilities
			*/
			static void PopulateVisibilities(Actors::Caches::FortPawnCache& FortPawnCache) {
				FortPawnCache.BoneVisibilities.resize(100);

				FortPawnCache.BoneVisibilities[(int)Bone::BoneID::Head] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::RealCamera.Position, FortPawnCache.BoneRegister[(int)Bone::BoneID::Head], FortPawnCache.FortPawn);
				FortPawnCache.BoneVisibilities[(int)Bone::BoneID::ChestLeft] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::RealCamera.Position, FortPawnCache.BoneRegister[(int)Bone::BoneID::ChestLeft], FortPawnCache.FortPawn);
				FortPawnCache.BoneVisibilities[(int)Bone::BoneID::ChestRight] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::RealCamera.Position, FortPawnCache.BoneRegister[(int)Bone::BoneID::ChestRight], FortPawnCache.FortPawn);
				FortPawnCache.BoneVisibilities[(int)Bone::BoneID::LeftShoulder] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::RealCamera.Position, FortPawnCache.BoneRegister[(int)Bone::BoneID::LeftShoulder], FortPawnCache.FortPawn);
				FortPawnCache.BoneVisibilities[(int)Bone::BoneID::RightShoulder] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::RealCamera.Position, FortPawnCache.BoneRegister[(int)Bone::BoneID::RightShoulder], FortPawnCache.FortPawn);
				FortPawnCache.BoneVisibilities[(int)Bone::BoneID::LeftElbow] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::RealCamera.Position, FortPawnCache.BoneRegister[(int)Bone::BoneID::LeftElbow], FortPawnCache.FortPawn);
				FortPawnCache.BoneVisibilities[(int)Bone::BoneID::RightElbow] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::RealCamera.Position, FortPawnCache.BoneRegister[(int)Bone::BoneID::RightElbow], FortPawnCache.FortPawn);
				FortPawnCache.BoneVisibilities[(int)Bone::BoneID::LeftHand] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::RealCamera.Position, FortPawnCache.BoneRegister[(int)Bone::BoneID::LeftHand], FortPawnCache.FortPawn);
				FortPawnCache.BoneVisibilities[(int)Bone::BoneID::RightHand] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::RealCamera.Position, FortPawnCache.BoneRegister[(int)Bone::BoneID::RightHand], FortPawnCache.FortPawn);
				FortPawnCache.BoneVisibilities[(int)Bone::BoneID::LeftLeg] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::RealCamera.Position, FortPawnCache.BoneRegister[(int)Bone::BoneID::LeftLeg], FortPawnCache.FortPawn);
				FortPawnCache.BoneVisibilities[(int)Bone::BoneID::RightLeg] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::RealCamera.Position, FortPawnCache.BoneRegister[(int)Bone::BoneID::RightLeg], FortPawnCache.FortPawn);
				FortPawnCache.BoneVisibilities[(int)Bone::BoneID::LeftKnee] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::RealCamera.Position, FortPawnCache.BoneRegister[(int)Bone::BoneID::LeftKnee], FortPawnCache.FortPawn);
				FortPawnCache.BoneVisibilities[(int)Bone::BoneID::RightKnee] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::RealCamera.Position, FortPawnCache.BoneRegister[(int)Bone::BoneID::RightKnee], FortPawnCache.FortPawn);
				FortPawnCache.BoneVisibilities[(int)Bone::BoneID::LeftFoot] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::RealCamera.Position, FortPawnCache.BoneRegister[(int)Bone::BoneID::LeftFoot], FortPawnCache.FortPawn);
				FortPawnCache.BoneVisibilities[(int)Bone::BoneID::RightFoot] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::RealCamera.Position, FortPawnCache.BoneRegister[(int)Bone::BoneID::RightFoot], FortPawnCache.FortPawn);
				FortPawnCache.BoneVisibilities[(int)Bone::BoneID::Pelvis] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::RealCamera.Position, FortPawnCache.BoneRegister[(int)Bone::BoneID::Pelvis], FortPawnCache.FortPawn);
			}
		};
	}
}