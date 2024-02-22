#pragma once
#include <vector>
#include "../../SDK/Classes/Basic.h"

#include "../../Actors/Actors.h"

namespace Features {
    namespace FortPawnHelper {
        /* @brief Stores the BoneID enum and is used in aimbot to calculate target bone */
        class Bone {
        public:
            /* Represents the bone names of a FortPawn */
            static struct BoneNames {
                SDK::FName Head;
                SDK::FName Neck;

                SDK::FName ChestLeft;
                SDK::FName ChestRight;

                SDK::FName LeftShoulder;
                SDK::FName LeftElbow;
                SDK::FName LeftHand;
                SDK::FName RightShoulder;
                SDK::FName RightElbow;
                SDK::FName RightHand;

                SDK::FName LeftLeg;
                SDK::FName LeftKnee;
                SDK::FName LeftFoot;
                SDK::FName RightLeg;
                SDK::FName RightKnee;
                SDK::FName RightFoot;

                SDK::FName Pelvis;

                SDK::FName Root;
                SDK::FName None;
            };;

            /* The bone names of a FortPawn */
            static BoneNames Names;

            /* Represents the bone IDs of a FortPawn */
            enum BoneID_ : uint8_t {
                Head = 1,           // "head"
                Neck = 2,           // "neck_01"

                // There is no chest bone, so this is a dummy value used for the hierarchy, chest should be calculated based on left and right chest bones
                Chest = 3,          // none

                LeftShoulder = 4,   // "upperarm_l"
                LeftElbow = 5,	    // "lowerarm_l"
                LeftHand = 6,	    // "Hand_L"
                RightShoulder = 7,  // "upperarm_r"
                RightElbow = 8,	    // "lowerarm_r"
                RightHand = 9,	    // "hand_r"

                LeftLeg = 10,	    // "thigh_l"
                LeftKnee = 11,	    // "calf_l"
                LeftFoot = 12,	    // "foot_l"
                RightLeg = 13,	    // "thigh_r"
                RightKnee = 14,	    // "calf_r"
                RightFoot = 15,	    // "foot_r"

                Pelvis = 16,	        // "pelvis"

                // Bottom contains the root component position. We have two with the same value for code readability
                Root = 17,          // "Root"

                ChestLeft = 18,
                ChestRight = 19,

                None = 0,		    // "None"
            };
            /* Intermediary type for BoneID */
            typedef uint8_t BoneID;
        private:
            /* The order of the bone hierarchy, used to determine which bone to aim at based on the visibilities of the bones */
            static const std::vector<std::pair<Bone::BoneID, Bone::BoneID>> BoneHierarchyOrder;
        public:
            /* The pairs of bones that are connected to each other in the player skeleton */
            static const std::vector<std::pair<Bone::BoneID, Bone::BoneID>> SkeletonBonePairs;
        public:
            /*
            * @brief Choose the closest bone to crosshair between 2 bones
            *
            * @param BonePosition1 - The screen position of the first bone
            * @param BonePosition2 - The screen position of the second bone
            * @param BoneID1 - The ID of the first bone
            * @param BoneID2 - The ID of the second bone
            */
            static BoneID ChooseBasedOnPreference(SDK::FVector2D BonePosition1, SDK::FVector2D BonePosition2, BoneID BoneID1, BoneID BoneID2);

            /*
            * @brief Find the best bone to aim at based on the bone hierarchy and visibilities
            *
            * @param TargetBone - The optimal bone to aim at
            * @param FortPawnCache - The pawn cache of the target
            */
            static BoneID FindBestBone(BoneID TargetBone, Actors::Caches::FortPawnCache& FortPawnCache);

            /*
            * @brief Get a cached bone FName from BoneID
            *
            * @param BoneID - The bone ID to get the name of
            *
            * @return The FName of the bone
            */
            static SDK::FName GetBoneName(BoneID BoneID);

            /* @brief Initiate bone FNames for GetSocketLocation */
            static void Init();
        };
    }
}