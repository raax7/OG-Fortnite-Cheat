#ifndef FORTPAWN_H
#define FORTPAWN_H

#include <vector>
#include "../../SDK/Classes/Engine_classes.h"

namespace Player {
	namespace Bones {
		inline int Head = 66;
		inline int Neck = 64;

		inline int Chest = -1;
		inline int ChestLeft = 8;
		inline int ChestRight = 36;

		inline int LeftShoulder = 9;
		inline int RightShoulder = 37;
		inline int LeftElbow = 10;
		inline int RightElbow = 38;
		inline int LeftHand = 11;
		inline int RightHand = 39;

		inline int LeftLeg = 67;
		inline int RightLeg = 74;
		inline int LeftKnee = 68;
		inline int RightKnee = 75;
		inline int LeftFoot = 71;
		inline int RightFoot = 78;

		inline int Pelvis = 2;
		inline int Bottom = 0;
	}

	bool PopulateBones(SDK::UCanvas* Canvas_, SDK::USkeletalMeshComponent* Mesh, std::vector<SDK::FVector>& BoneRegister, std::vector<SDK::FVector2D>& BoneRegister2D) {
		BoneRegister[Bones::Bottom]				= Mesh->GetBonePosition(Bones::Bottom);
		if (BoneRegister[Bones::Bottom].X == 0 && BoneRegister[Bones::Bottom].Y == 0 && BoneRegister[Bones::Bottom].Z == 0) return false;
		BoneRegister[Bones::Head]				= Mesh->GetBonePosition(Bones::Head);
		BoneRegister[Bones::ChestLeft]			= Mesh->GetBonePosition(Bones::ChestLeft);
		BoneRegister[Bones::ChestRight]			= Mesh->GetBonePosition(Bones::ChestRight);
		BoneRegister[Bones::LeftShoulder]		= Mesh->GetBonePosition(Bones::LeftShoulder);
		BoneRegister[Bones::RightShoulder]		= Mesh->GetBonePosition(Bones::RightShoulder);
		BoneRegister[Bones::LeftElbow]			= Mesh->GetBonePosition(Bones::LeftElbow);
		BoneRegister[Bones::RightElbow]			= Mesh->GetBonePosition(Bones::RightElbow);
		BoneRegister[Bones::LeftHand]			= Mesh->GetBonePosition(Bones::LeftHand);
		BoneRegister[Bones::RightHand]			= Mesh->GetBonePosition(Bones::RightHand);
		BoneRegister[Bones::LeftLeg]			= Mesh->GetBonePosition(Bones::LeftLeg);
		BoneRegister[Bones::RightLeg]			= Mesh->GetBonePosition(Bones::RightLeg);
		BoneRegister[Bones::LeftKnee]			= Mesh->GetBonePosition(Bones::LeftKnee);
		BoneRegister[Bones::RightKnee]			= Mesh->GetBonePosition(Bones::RightKnee);
		BoneRegister[Bones::LeftFoot]			= Mesh->GetBonePosition(Bones::LeftFoot);
		BoneRegister[Bones::RightFoot]			= Mesh->GetBonePosition(Bones::RightFoot);
		BoneRegister[Bones::Pelvis]				= Mesh->GetBonePosition(Bones::Pelvis);

		SDK::FVector2D Temp_W2S;
		BoneRegister2D[Bones::Head]				= Canvas_->K2_Project(BoneRegister[Bones::Head]);
		BoneRegister2D[Bones::ChestLeft]		= Canvas_->K2_Project(BoneRegister[Bones::ChestLeft]);
		BoneRegister2D[Bones::ChestRight]		= Canvas_->K2_Project(BoneRegister[Bones::ChestRight]);
		BoneRegister2D[Bones::LeftShoulder]		= Canvas_->K2_Project(BoneRegister[Bones::LeftShoulder]);
		BoneRegister2D[Bones::RightShoulder]	= Canvas_->K2_Project(BoneRegister[Bones::RightShoulder]);
		BoneRegister2D[Bones::LeftElbow]		= Canvas_->K2_Project(BoneRegister[Bones::LeftElbow]);
		BoneRegister2D[Bones::RightElbow]		= Canvas_->K2_Project(BoneRegister[Bones::RightElbow]);
		BoneRegister2D[Bones::LeftHand]			= Canvas_->K2_Project(BoneRegister[Bones::LeftHand]);
		BoneRegister2D[Bones::RightHand]		= Canvas_->K2_Project(BoneRegister[Bones::RightHand]);
		BoneRegister2D[Bones::LeftLeg]			= Canvas_->K2_Project(BoneRegister[Bones::LeftLeg]);
		BoneRegister2D[Bones::RightLeg]			= Canvas_->K2_Project(BoneRegister[Bones::RightLeg]);
		BoneRegister2D[Bones::LeftKnee]			= Canvas_->K2_Project(BoneRegister[Bones::LeftKnee]);
		BoneRegister2D[Bones::RightKnee]		= Canvas_->K2_Project(BoneRegister[Bones::RightKnee]);
		BoneRegister2D[Bones::LeftFoot]			= Canvas_->K2_Project(BoneRegister[Bones::LeftFoot]);
		BoneRegister2D[Bones::RightFoot]		= Canvas_->K2_Project(BoneRegister[Bones::RightFoot]);
		BoneRegister2D[Bones::Pelvis]			= Canvas_->K2_Project(BoneRegister[Bones::Pelvis]);
		BoneRegister2D[Bones::Bottom]			= Canvas_->K2_Project(BoneRegister[Bones::Bottom]);

		return true;
	}
}

#endif