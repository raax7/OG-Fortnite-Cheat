#include "Bone.h"

#include "../../../Utilities/Math.h"
#include "../../../Utilities/Logger.h"
#include "../../Game.h"

inline Features::FortPawnHelper::Bone::BoneNames Features::FortPawnHelper::Bone::Names;
const inline std::vector<std::pair<Features::FortPawnHelper::Bone::BoneID, Features::FortPawnHelper::Bone::BoneID>> Features::FortPawnHelper::Bone::BoneHierarchyOrder = {
    {Head,          Head},
    {Neck,          Neck},
    {Chest,         Chest},
    {LeftShoulder,  RightShoulder},
    {LeftElbow,     RightElbow},
    {LeftHand,      RightHand},
    {LeftLeg,       RightLeg},
    {LeftKnee,      RightKnee},
    {LeftFoot,      RightFoot},
    {Pelvis,        Pelvis}
};
const inline std::vector<std::pair<Features::FortPawnHelper::Bone::BoneID, Features::FortPawnHelper::Bone::BoneID>> Features::FortPawnHelper::Bone::SkeletonBonePairs = {
    {Head,            Chest},
    {Chest,           LeftShoulder},
    {Chest,           RightShoulder},
    {LeftShoulder,    LeftElbow},
    {RightShoulder,   RightElbow},
    {LeftElbow,       LeftHand},
    {RightElbow,      RightHand},
    {Pelvis,          LeftLeg},
    {Pelvis,          RightLeg},
    {LeftLeg,         LeftKnee},
    {RightLeg,        RightKnee},
    {LeftKnee,        LeftFoot},
    {RightKnee,       RightFoot},
    {Chest,           Pelvis},
};

Features::FortPawnHelper::Bone::BoneID Features::FortPawnHelper::Bone::FindClosestBoneBetweenTwo(SDK::FVector2D BonePosition1, SDK::FVector2D BonePosition2, BoneID BoneID1, BoneID BoneID2) {
    float Bone1Distance = Math::GetDistance2D(BonePosition1.X, BonePosition1.Y, (float)Game::ScreenWidth / 2.f, (float)Game::ScreenHeight / 2.f);
    float Bone2Distance = Math::GetDistance2D(BonePosition2.X, BonePosition2.Y, (float)Game::ScreenWidth / 2.f, (float)Game::ScreenHeight / 2.f);

    if (Bone1Distance < Bone2Distance) return BoneID1;
    else return BoneID2;
}
Features::FortPawnHelper::Bone::BoneID Features::FortPawnHelper::Bone::FindBestBone(BoneID TargetBone, Actors::Caches::FortPawnCache& FortPawnCache) {
    if (FortPawnCache.BoneVisibilities.size() != 100 || FortPawnCache.BoneRegister2D.size() != 100) {
        return None;
    }

    for (auto& BonePair : BoneHierarchyOrder) {
        BoneID LeftBone = BonePair.first;
        BoneID RightBone = BonePair.second;

        if (FortPawnCache.BoneVisibilities[LeftBone] && FortPawnCache.BoneVisibilities[RightBone]) {
            return FindClosestBoneBetweenTwo(FortPawnCache.BoneRegister2D[LeftBone], FortPawnCache.BoneRegister2D[RightBone], LeftBone, RightBone);
        }

        if (FortPawnCache.BoneVisibilities[LeftBone]) {
            return LeftBone;
        }
        else if (FortPawnCache.BoneVisibilities[RightBone]) {
            return RightBone;
        }
    }

    return None;
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
    DEBUG_LOG(LOG_OFFSET, skCrypt("Initializing bone names...").decrypt());

    // Init Names
    {
        Names.Head = SDK::FName(skCrypt(L"head").decrypt());
        Names.Neck = SDK::FName(skCrypt(L"neck_01").decrypt());

        Names.ChestLeft = SDK::FName(skCrypt(L"clavicle_l").decrypt());
        Names.ChestRight = SDK::FName(skCrypt(L"clavicle_r").decrypt());

        Names.LeftShoulder = SDK::FName(skCrypt(L"upperarm_l").decrypt());
        Names.LeftElbow = SDK::FName(skCrypt(L"lowerarm_l").decrypt());
        Names.LeftHand = SDK::FName(skCrypt(L"Hand_L").decrypt());
        Names.RightShoulder = SDK::FName(skCrypt(L"upperarm_r").decrypt());
        Names.RightElbow = SDK::FName(skCrypt(L"lowerarm_r").decrypt());
        Names.RightHand = SDK::FName(skCrypt(L"hand_r").decrypt());

        Names.LeftLeg = SDK::FName(skCrypt(L"thigh_l").decrypt());
        Names.LeftKnee = SDK::FName(skCrypt(L"calf_l").decrypt());
        Names.LeftFoot = SDK::FName(skCrypt(L"foot_l").decrypt());
        Names.RightLeg = SDK::FName(skCrypt(L"thigh_r").decrypt());
        Names.RightKnee = SDK::FName(skCrypt(L"calf_r").decrypt());
        Names.RightFoot = SDK::FName(skCrypt(L"foot_r").decrypt());

        Names.Pelvis = SDK::FName(skCrypt(L"pelvis").decrypt());

        Names.Root = SDK::FName(skCrypt(L"Root").decrypt());
        Names.None = SDK::FName(skCrypt(L"None").decrypt());
    }

    DEBUG_LOG(LOG_OFFSET, skCrypt("Bone names initialized!").decrypt());
}