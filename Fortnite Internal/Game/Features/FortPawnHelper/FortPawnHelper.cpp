#include "FortPawnHelper.h"

#include "Bone.h"
#include "../../../Utilities/Math.h"

bool Features::FortPawnHelper::PopulateBones(Actors::Caches::FortPawnCache& FortPawnCache) {
	SDK::FVector2D Temp_W2S;

	FortPawnCache.BoneRegister.resize(100);
	FortPawnCache.BoneRegister2D.resize(100);

	FortPawnCache.BoneRegister[Bone::Root] = FortPawnCache.Mesh->GetBonePosition(Bone::Root);
	if (FortPawnCache.BoneRegister[Bone::Root].X == 0 && FortPawnCache.BoneRegister[Bone::Root].Y == 0 && FortPawnCache.BoneRegister[Bone::Root].Z == 0) return false;
	FortPawnCache.BoneRegister2D[Bone::Root] = SDK::Project(FortPawnCache.BoneRegister[Bone::Root]);
	bool BottomVisible = Math::IsOnScreen(FortPawnCache.BoneRegister2D[Bone::Root]);

	FortPawnCache.BoneRegister[Bone::Head] = FortPawnCache.Mesh->GetBonePosition(Bone::Head);
	FortPawnCache.BoneRegister2D[Bone::Head] = SDK::Project(FortPawnCache.BoneRegister[Bone::Head]);
	bool HeadVisible = Math::IsOnScreen(FortPawnCache.BoneRegister2D[Bone::Head]);

	FortPawnCache.BoneRegister[Bone::Pelvis] = FortPawnCache.Mesh->GetBonePosition(Bone::Pelvis);
	FortPawnCache.BoneRegister2D[Bone::Pelvis] = SDK::Project(FortPawnCache.BoneRegister[Bone::Pelvis]);
	bool PelvisVisible = Math::IsOnScreen(FortPawnCache.BoneRegister2D[Bone::Pelvis]);

	if (!BottomVisible && !HeadVisible && !PelvisVisible) return false;

	SDK::FVector ChestLeft = FortPawnCache.Mesh->GetBonePosition(Bone::ChestLeft);
	SDK::FVector ChestRight = FortPawnCache.Mesh->GetBonePosition(Bone::ChestRight);

	FortPawnCache.BoneRegister[Bone::Chest] = (ChestLeft + ChestRight) / 2;

	FortPawnCache.BoneRegister[Bone::LeftShoulder] = FortPawnCache.Mesh->GetBonePosition(Bone::LeftShoulder);
	FortPawnCache.BoneRegister[Bone::RightShoulder] = FortPawnCache.Mesh->GetBonePosition(Bone::RightShoulder);
	FortPawnCache.BoneRegister[Bone::LeftElbow] = FortPawnCache.Mesh->GetBonePosition(Bone::LeftElbow);
	FortPawnCache.BoneRegister[Bone::RightElbow] = FortPawnCache.Mesh->GetBonePosition(Bone::RightElbow);
	FortPawnCache.BoneRegister[Bone::LeftHand] = FortPawnCache.Mesh->GetBonePosition(Bone::LeftHand);
	FortPawnCache.BoneRegister[Bone::RightHand] = FortPawnCache.Mesh->GetBonePosition(Bone::RightHand);
	FortPawnCache.BoneRegister[Bone::LeftLeg] = FortPawnCache.Mesh->GetBonePosition(Bone::LeftLeg);
	FortPawnCache.BoneRegister[Bone::RightLeg] = FortPawnCache.Mesh->GetBonePosition(Bone::RightLeg);
	FortPawnCache.BoneRegister[Bone::LeftKnee] = FortPawnCache.Mesh->GetBonePosition(Bone::LeftKnee);
	FortPawnCache.BoneRegister[Bone::RightKnee] = FortPawnCache.Mesh->GetBonePosition(Bone::RightKnee);
	FortPawnCache.BoneRegister[Bone::LeftFoot] = FortPawnCache.Mesh->GetBonePosition(Bone::LeftFoot);
	FortPawnCache.BoneRegister[Bone::RightFoot] = FortPawnCache.Mesh->GetBonePosition(Bone::RightFoot);

	FortPawnCache.BoneRegister2D[Bone::Chest] = SDK::Project(FortPawnCache.BoneRegister[Bone::Chest]);
	FortPawnCache.BoneRegister2D[Bone::LeftShoulder] = SDK::Project(FortPawnCache.BoneRegister[Bone::LeftShoulder]);
	FortPawnCache.BoneRegister2D[Bone::RightShoulder] = SDK::Project(FortPawnCache.BoneRegister[Bone::RightShoulder]);
	FortPawnCache.BoneRegister2D[Bone::LeftElbow] = SDK::Project(FortPawnCache.BoneRegister[Bone::LeftElbow]);
	FortPawnCache.BoneRegister2D[Bone::RightElbow] = SDK::Project(FortPawnCache.BoneRegister[Bone::RightElbow]);
	FortPawnCache.BoneRegister2D[Bone::LeftHand] = SDK::Project(FortPawnCache.BoneRegister[Bone::LeftHand]);
	FortPawnCache.BoneRegister2D[Bone::RightHand] = SDK::Project(FortPawnCache.BoneRegister[Bone::RightHand]);
	FortPawnCache.BoneRegister2D[Bone::LeftLeg] = SDK::Project(FortPawnCache.BoneRegister[Bone::LeftLeg]);
	FortPawnCache.BoneRegister2D[Bone::RightLeg] = SDK::Project(FortPawnCache.BoneRegister[Bone::RightLeg]);
	FortPawnCache.BoneRegister2D[Bone::LeftKnee] = SDK::Project(FortPawnCache.BoneRegister[Bone::LeftKnee]);
	FortPawnCache.BoneRegister2D[Bone::RightKnee] = SDK::Project(FortPawnCache.BoneRegister[Bone::RightKnee]);
	FortPawnCache.BoneRegister2D[Bone::LeftFoot] = SDK::Project(FortPawnCache.BoneRegister[Bone::LeftFoot]);
	FortPawnCache.BoneRegister2D[Bone::RightFoot] = SDK::Project(FortPawnCache.BoneRegister[Bone::RightFoot]);

	return true;
}
void Features::FortPawnHelper::PopulateVisibilities(Actors::Caches::FortPawnCache& FortPawnCache) {
	FortPawnCache.BoneVisibilities.resize(100);

	FortPawnCache.BoneVisibilities[Bone::Head] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::Head], FortPawnCache.FortPawn);
	FortPawnCache.BoneVisibilities[Bone::Chest] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::Chest], FortPawnCache.FortPawn);
	FortPawnCache.BoneVisibilities[Bone::LeftShoulder] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::LeftShoulder], FortPawnCache.FortPawn);
	FortPawnCache.BoneVisibilities[Bone::RightShoulder] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::RightShoulder], FortPawnCache.FortPawn);
	FortPawnCache.BoneVisibilities[Bone::LeftElbow] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::LeftElbow], FortPawnCache.FortPawn);
	FortPawnCache.BoneVisibilities[Bone::RightElbow] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::RightElbow], FortPawnCache.FortPawn);
	FortPawnCache.BoneVisibilities[Bone::LeftHand] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::LeftHand], FortPawnCache.FortPawn);
	FortPawnCache.BoneVisibilities[Bone::RightHand] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::RightHand], FortPawnCache.FortPawn);
	FortPawnCache.BoneVisibilities[Bone::LeftLeg] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::LeftLeg], FortPawnCache.FortPawn);
	FortPawnCache.BoneVisibilities[Bone::RightLeg] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::RightLeg], FortPawnCache.FortPawn);
	FortPawnCache.BoneVisibilities[Bone::LeftKnee] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::LeftKnee], FortPawnCache.FortPawn);
	FortPawnCache.BoneVisibilities[Bone::RightKnee] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::RightKnee], FortPawnCache.FortPawn);
	FortPawnCache.BoneVisibilities[Bone::LeftFoot] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::LeftFoot], FortPawnCache.FortPawn);
	FortPawnCache.BoneVisibilities[Bone::RightFoot] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::RightFoot], FortPawnCache.FortPawn);
	FortPawnCache.BoneVisibilities[Bone::Pelvis] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::Pelvis], FortPawnCache.FortPawn);
}