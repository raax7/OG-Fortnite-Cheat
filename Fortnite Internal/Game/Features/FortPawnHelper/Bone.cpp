#include "Bone.h"

#include "../../../Utilities/Math.h"
#include "../../../Utilities/Logger.h"

#include "../../Game.h"

Features::FortPawnHelper::Bone::BoneID Features::FortPawnHelper::Bone::FindClosestBoneBetweenTwo(SDK::FVector2D BonePosition1, SDK::FVector2D BonePosition2, BoneID BoneID1, BoneID BoneID2) {
    float Bone1Distance = Math::GetDistance2D(BonePosition1.X, BonePosition1.Y, (float)Game::ScreenCenterX, (float)Game::ScreenCenterY);
    float Bone2Distance = Math::GetDistance2D(BonePosition2.X, BonePosition2.Y, (float)Game::ScreenCenterX, (float)Game::ScreenCenterY);

    if (Bone1Distance < Bone2Distance) return BoneID1;
    else return BoneID2;
}
Features::FortPawnHelper::Bone::BoneID Features::FortPawnHelper::Bone::FindBestBone(BoneID TargetBone, Actors::Caches::FortPawnCache& FortPawnCache, bool VisibleCheck) {
    if (FortPawnCache.BoneVisibilityStates.size() < BONEID_MAX || FortPawnCache.BonePositions2D.size() < BONEID_MAX) {
        return None;
    }

    for (auto& BonePair : BoneHierarchyOrder) {
        BoneID LeftBone = BonePair.first;
        BoneID RightBone = BonePair.second;

        if (FortPawnCache.BoneVisibilityStates[LeftBone] && FortPawnCache.BoneVisibilityStates[RightBone]) {
            return FindClosestBoneBetweenTwo(FortPawnCache.BonePositions2D[LeftBone], FortPawnCache.BonePositions2D[RightBone], LeftBone, RightBone);
        }

        if (FortPawnCache.BoneVisibilityStates[LeftBone]) {
            return LeftBone;
        }
        else if (FortPawnCache.BoneVisibilityStates[RightBone]) {
            return RightBone;
        }
    }

    if (VisibleCheck == false) {
		return TargetBone;
	}
    else {
        return None;
    }
}
SDK::FName Features::FortPawnHelper::Bone::GetBoneName(BoneID BoneID) {
    switch (BoneID) {
    case Head: return Names.Head;
    case Neck: return Names.Neck;
    case LeftShoulder: return Names.LeftShoulder;
    case LeftElbow: return Names.LeftElbow;
    case LeftHand: return Names.LeftHand;
    case RightShoulder: return Names.RightShoulder;
    case RightElbow: return Names.RightElbow;
    case RightHand: return Names.RightHand;
    case LeftLeg: return Names.LeftLeg;
    case LeftKnee: return Names.LeftKnee;
    case LeftFoot: return Names.LeftFoot;
    case RightLeg: return Names.RightLeg;
    case RightKnee: return Names.RightKnee;
    case RightFoot: return Names.RightFoot;
    case Pelvis: return Names.Pelvis;
    case Root: return Names.Root;
    case ChestLeft: return Names.ChestLeft;
    case ChestRight: return Names.ChestRight;
    case None: return Names.None;
    }

    return Names.None;
}
void Features::FortPawnHelper::Bone::Init() {
    DEBUG_LOG(LOG_OFFSET, skCrypt("Initializing bone names..."));

    // Init Names
    {
        Names.Head = SDK::FName(skCrypt(L"head"));
        Names.Neck = SDK::FName(skCrypt(L"neck_01"));

        Names.ChestLeft = SDK::FName(skCrypt(L"clavicle_l"));
        Names.ChestRight = SDK::FName(skCrypt(L"clavicle_r"));

        Names.LeftShoulder = SDK::FName(skCrypt(L"upperarm_l"));
        Names.LeftElbow = SDK::FName(skCrypt(L"lowerarm_l"));
        Names.LeftHand = SDK::FName(skCrypt(L"Hand_L"));
        Names.RightShoulder = SDK::FName(skCrypt(L"upperarm_r"));
        Names.RightElbow = SDK::FName(skCrypt(L"lowerarm_r"));
        Names.RightHand = SDK::FName(skCrypt(L"hand_r"));

        Names.LeftLeg = SDK::FName(skCrypt(L"thigh_l"));
        Names.LeftKnee = SDK::FName(skCrypt(L"calf_l"));
        Names.LeftFoot = SDK::FName(skCrypt(L"foot_l"));
        Names.RightLeg = SDK::FName(skCrypt(L"thigh_r"));
        Names.RightKnee = SDK::FName(skCrypt(L"calf_r"));
        Names.RightFoot = SDK::FName(skCrypt(L"foot_r"));

        Names.Pelvis = SDK::FName(skCrypt(L"pelvis"));

        Names.Root = SDK::FName(skCrypt(L"Root"));
        Names.None = SDK::FName(skCrypt(L"None"));
    }

    DEBUG_LOG(LOG_OFFSET, std::string(skCrypt("Bone names initialized!")));
}