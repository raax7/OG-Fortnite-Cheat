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

Features::FortPawnHelper::Bone::BoneID Features::FortPawnHelper::Bone::ChooseBasedOnPreference(SDK::FVector2D BonePosition1, SDK::FVector2D BonePosition2, BoneID BoneID1, BoneID BoneID2) {
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
            return ChooseBasedOnPreference(FortPawnCache.BoneRegister2D[LeftBone], FortPawnCache.BoneRegister2D[RightBone], LeftBone, RightBone);
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
    DEBUG_LOG(skCrypt("Initializing bone names...").decrypt());

    // Init Names
    {
        Names.Head = SDK::FName(L"head");
        Names.Neck = SDK::FName(L"neck_01");

        Names.ChestLeft = SDK::FName(L"clavicle_l");
        Names.ChestRight = SDK::FName(L"clavicle_r");

        Names.LeftShoulder = SDK::FName(L"upperarm_l");
        Names.LeftElbow = SDK::FName(L"lowerarm_l");
        Names.LeftHand = SDK::FName(L"Hand_L");
        Names.RightShoulder = SDK::FName(L"upperarm_r");
        Names.RightElbow = SDK::FName(L"lowerarm_r");
        Names.RightHand = SDK::FName(L"hand_r");

        Names.LeftLeg = SDK::FName(L"thigh_l");
        Names.LeftKnee = SDK::FName(L"calf_l");
        Names.LeftFoot = SDK::FName(L"foot_l");
        Names.RightLeg = SDK::FName(L"thigh_r");
        Names.RightKnee = SDK::FName(L"calf_r");
        Names.RightFoot = SDK::FName(L"foot_r");

        Names.Pelvis = SDK::FName(L"pelvis");

        Names.Root = SDK::FName(L"Root");
        Names.None = SDK::FName(L"None");
    }

    DEBUG_LOG(skCrypt("Bone names initialized!").decrypt());
}