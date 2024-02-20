#pragma once
#include <Windows.h>
#include <vector>
#include "../../Actors/Actors.h"
#include "../../../Utilities/Math.h"
#include "../../Actors/ActorCache.h"

namespace Features {
    namespace Aimbot {
        // Stores the BoneID enum. Also used to determine the best bone to aim at based on the bone hierarchy and visibilities
        class Bone {
        public:
            // Represents the bone IDs of a FortPawn
            enum class BoneID : int {
                Head = 66,
                Neck = 64,

                // There is no chest bone, so this is a dummy value used for the hierarchy, chest should be calculated based on left and right chest bones
                Chest = -1,
                ChestLeft = 8,
                ChestRight = 36,

                LeftShoulder = 9,
                LeftElbow = 10,
                LeftHand = 11,
                RightShoulder = 37,
                RightElbow = 38,
                RightHand = 39,

                LeftLeg = 67,
                LeftKnee = 68,
                LeftFoot = 71,
                RightLeg = 74,
                RightKnee = 75,
                RightFoot = 78,

                Pelvis = 2,

                // Bottom contains the root component position. We have two with the same value for code readability
                Bottom = 0,
                None = 0,
            };
        private:
            // The order of the bone hierarchy, used to determine which bone to aim at based on the visibilities of the bones
            static const std::vector<std::pair<BoneID, BoneID>> BoneHierarchyOrder;
        public:
            // The pairs of bones that are connected to each other in the player skeleton
            static const std::vector<std::pair<BoneID, BoneID>> SkeletonBonePairs;
        public:
            /*
            * @brief Choose the closest bone to crosshair between 2 bones
            *
            * @param Bone1 - The screen position of the first bone
            * @param Bone2 - The screen position of the second bone
            * @param BoneID1 - The ID of the first bone
            * @param BoneID2 - The ID of the second bone
            */
            static BoneID ChooseBasedOnPreference(SDK::FVector2D Bone1, SDK::FVector2D Bone2, BoneID BoneID1, BoneID BoneID2) {
                float Bone1Distance = Math::GetDistance2D(Bone1.X, Bone1.Y, (float)Game::ScreenWidth / 2.f, (float)Game::ScreenHeight / 2.f);
                float Bone2Distance = Math::GetDistance2D(Bone2.X, Bone2.Y, (float)Game::ScreenWidth / 2.f, (float)Game::ScreenHeight / 2.f);

                if (Bone1Distance < Bone2Distance) return BoneID1;
                return BoneID2;
            }

            /*
            * @brief Find the best bone to aim at based on the bone hierarchy and visibilities
            *
            * @param TargetBone - The optimal bone to aim at
            * @param FortPawnCache - The pawn cache of the target
            */
            static BoneID FindBestBone(BoneID TargetBone, Actors::Caches::FortPawnCache& FortPawnCache) {
                if (FortPawnCache.BoneVisibilities.size() != 100) {
                    return BoneID::None;
                }
                if (FortPawnCache.BoneRegister2D.size() != 100) {
                    return BoneID::None;
                }

                for (auto& bonePair : BoneHierarchyOrder) {
                    BoneID leftBone = bonePair.first;
                    BoneID rightBone = bonePair.second;

                    if (FortPawnCache.BoneVisibilities[(int)leftBone] && FortPawnCache.BoneVisibilities[(int)rightBone]) {
                        return ChooseBasedOnPreference(FortPawnCache.BoneRegister2D[(int)leftBone], FortPawnCache.BoneRegister2D[(int)rightBone], leftBone, rightBone);
                    }

                    if (FortPawnCache.BoneVisibilities[(int)leftBone]) {
                        return leftBone;
                    }
                    else if (FortPawnCache.BoneVisibilities[(int)rightBone]) {
                        return rightBone;
                    }
                }

                return BoneID::None;
            }
        };

        const inline std::vector<std::pair<Bone::BoneID, Bone::BoneID>> Bone::SkeletonBonePairs = {
            {BoneID::Head, BoneID::Chest},
            {BoneID::Chest, BoneID::LeftShoulder},
            {BoneID::Chest, BoneID::RightShoulder},
            {BoneID::LeftShoulder, BoneID::LeftElbow},
            {BoneID::RightShoulder, BoneID::RightElbow},
            {BoneID::LeftElbow, BoneID::LeftHand},
            {BoneID::RightElbow, BoneID::RightHand},
            {BoneID::Pelvis, BoneID::LeftLeg},
            {BoneID::Pelvis, BoneID::RightLeg},
            {BoneID::LeftLeg, BoneID::LeftKnee},
            {BoneID::RightLeg, BoneID::RightKnee},
            {BoneID::LeftKnee, BoneID::LeftFoot},
            {BoneID::RightKnee, BoneID::RightFoot},
            {BoneID::Chest, BoneID::Pelvis},
        };

        const inline std::vector<std::pair<Bone::BoneID, Bone::BoneID>> Bone::BoneHierarchyOrder = {
            {BoneID::Head,          BoneID::Head},
            {BoneID::Neck,          BoneID::Neck},
            {BoneID::ChestLeft,     BoneID::ChestRight},
            {BoneID::LeftShoulder,  BoneID::RightShoulder},
            {BoneID::LeftElbow,     BoneID::RightElbow},
            {BoneID::LeftHand,      BoneID::RightHand},
            {BoneID::LeftLeg,       BoneID::RightLeg},
            {BoneID::LeftKnee,      BoneID::RightKnee},
            {BoneID::LeftFoot,      BoneID::RightFoot},
            {BoneID::Pelvis,        BoneID::Pelvis}
        };
    }
}