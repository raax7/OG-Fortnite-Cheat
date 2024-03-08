#include "FortPawnHelper.h"

#include "Bone.h"

#include "../../../Utilities/Math.h"
#include "../../../Utilities/Logger.h"

bool Features::FortPawnHelper::PopulateBones(Actors::Caches::FortPawnCache& FortPawnCache) {
	SDK::FVector2D Temp_W2S;

	// Resize the bone register to avoid out of range errors
	FortPawnCache.BoneRegister.resize(100);
	FortPawnCache.BoneRegister2D.resize(100);
	FortPawnCache.BoneVisibilities.resize(100);

	bool BoneOnScreen = false;

	for (int i = Bone::Head; i < Bone::MAX; i++) {
		FortPawnCache.BoneRegister[i] = FortPawnCache.Mesh->GetBonePosition(i);

		if (i == Bone::Chest) {
			FortPawnCache.BoneRegister[Bone::Chest] = (FortPawnCache.BoneRegister[Bone::ChestLeft] + FortPawnCache.BoneRegister[Bone::ChestRight]) / 2;
		}

		// To avoid W2Sing players that aren't on the screen
		if (i <= 5 || BoneOnScreen) {
			FortPawnCache.BoneRegister2D[i] = SDK::Project(FortPawnCache.BoneRegister[i]);

			if (Math::IsOnScreen(FortPawnCache.BoneRegister2D[i])) {
				BoneOnScreen = true;
			}
		}
	}

	return BoneOnScreen;
}
void Features::FortPawnHelper::PopulateVisibilities(Actors::Caches::FortPawnCache& FortPawnCache) {
	FortPawnCache.BoneRegister.resize(100);
	FortPawnCache.BoneRegister2D.resize(100);
	FortPawnCache.BoneVisibilities.resize(100);

	FortPawnCache.BoneVisibilities[Bone::Head] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::Head], FortPawnCache.FortPawn, SDK::GetLocalPawn());
	FortPawnCache.BoneVisibilities[Bone::Chest] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::Chest], FortPawnCache.FortPawn, SDK::GetLocalPawn());
	FortPawnCache.BoneVisibilities[Bone::LeftShoulder] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::LeftShoulder], FortPawnCache.FortPawn, SDK::GetLocalPawn());
	FortPawnCache.BoneVisibilities[Bone::RightShoulder] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::RightShoulder], FortPawnCache.FortPawn, SDK::GetLocalPawn());
	FortPawnCache.BoneVisibilities[Bone::LeftElbow] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::LeftElbow], FortPawnCache.FortPawn, SDK::GetLocalPawn());
	FortPawnCache.BoneVisibilities[Bone::RightElbow] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::RightElbow], FortPawnCache.FortPawn, SDK::GetLocalPawn());
	FortPawnCache.BoneVisibilities[Bone::LeftHand] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::LeftHand], FortPawnCache.FortPawn, SDK::GetLocalPawn());
	FortPawnCache.BoneVisibilities[Bone::RightHand] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::RightHand], FortPawnCache.FortPawn, SDK::GetLocalPawn());
	FortPawnCache.BoneVisibilities[Bone::LeftLeg] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::LeftLeg], FortPawnCache.FortPawn, SDK::GetLocalPawn());
	FortPawnCache.BoneVisibilities[Bone::RightLeg] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::RightLeg], FortPawnCache.FortPawn, SDK::GetLocalPawn());
	FortPawnCache.BoneVisibilities[Bone::LeftKnee] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::LeftKnee], FortPawnCache.FortPawn, SDK::GetLocalPawn());
	FortPawnCache.BoneVisibilities[Bone::RightKnee] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::RightKnee], FortPawnCache.FortPawn, SDK::GetLocalPawn());
	FortPawnCache.BoneVisibilities[Bone::LeftFoot] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::LeftFoot], FortPawnCache.FortPawn, SDK::GetLocalPawn());
	FortPawnCache.BoneVisibilities[Bone::RightFoot] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::RightFoot], FortPawnCache.FortPawn, SDK::GetLocalPawn());
	FortPawnCache.BoneVisibilities[Bone::Pelvis] = SDK::IsPositionVisible(FortPawnCache.FortPawn, Actors::MainCamera.Position, FortPawnCache.BoneRegister[Bone::Pelvis], FortPawnCache.FortPawn, SDK::GetLocalPawn());
}