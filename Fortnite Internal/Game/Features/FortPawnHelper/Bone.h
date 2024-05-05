#pragma once
#include <vector>

#include "../../SDK/Classes/Basic.h"

#include "../../Actors/ActorCache.h"

namespace Features {
    namespace FortPawnHelper {
        /* Stores the BoneID enum and is used in aimbot to calculate target bone */
        namespace Bone {
            /* Represents the bone names of a FortPawn */
            struct BoneNames {
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
            };

            /* The bone names of a FortPawn */
            inline BoneNames Names;

            /* Represents the bone IDs of a FortPawn (THE ORDER OF THE ENUM AFFECTS VARIOUS FUNCTIONS, AVOID CHANGING ORDER) */
            enum BoneID_ : uint8_t {
                Head = 1,           // "head"
                Neck = 2,           // "neck_01"

                // Bottom contains the root component position. We have two with the same value for code readability
                Root = 3,           // "Root"

                ChestLeft = 4,	    // "clavicle_l"
                ChestRight = 5,     // "clavicle_r"

                // There is no chest bone, so this is a dummy value used for the hierarchy, chest should be calculated based on left and right chest bones
                Chest = 6,          // none

                LeftShoulder = 7,   // "upperarm_l"
                LeftElbow = 8,	    // "lowerarm_l"
                LeftHand = 9,	    // "Hand_L"
                RightShoulder = 10, // "upperarm_r"
                RightElbow = 11,    // "lowerarm_r"
                RightHand = 12,	    // "hand_r"

                LeftLeg = 13,	    // "thigh_l"
                LeftKnee = 14,	    // "calf_l"
                LeftFoot = 15,	    // "foot_l"
                RightLeg = 16,	    // "thigh_r"
                RightKnee = 17,	    // "calf_r"
                RightFoot = 18,	    // "foot_r"

                Pelvis = 19,	    // "pelvis"

                BONEID_MAX = 20,           // Max value for looping

                None = 0,		    // "None"
            };
            /* Intermediary type for BoneID */
            typedef uint8_t BoneID;

            /* The order of the bone hierarchy, used to determine which bone to aim at based on the visibilities of the bones */
            inline const std::vector<std::pair<Bone::BoneID, Bone::BoneID>> BoneHierarchyOrder{
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

            /* The pairs of bones that are connected to each other in the player skeleton */
            inline const inline std::vector<std::pair<BoneID, BoneID>> SkeletonBonePairs = {
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



            /*
            * @brief Choose the closest bone to crosshair between 2 bones
            *
            * @param BonePosition1 - The screen position of the first bone
            * @param BonePosition2 - The screen position of the second bone
            * @param BoneID1 - The ID of the first bone
            * @param BoneID2 - The ID of the second bone
            */
            BoneID FindClosestBoneBetweenTwo(SDK::FVector2D BonePosition1, SDK::FVector2D BonePosition2, BoneID BoneID1, BoneID BoneID2);

            /*
            * @brief Find the best bone to aim at based on the bone hierarchy and visibilities
            *
            * @param TargetBone - The optimal bone to aim at
            * @param FortPawnCache - The pawn cache of the target
            * @param VisibleCheck - If true, 
            */
            BoneID FindBestBone(BoneID TargetBone, Actors::Caches::FortPawnCache& FortPawnCache, bool VisibleCheck);

            /*
            * @brief Get a cached bone FName from BoneID
            *
            * @param BoneID - The bone ID to get the name of
            *
            * @return The FName of the bone
            */
            SDK::FName GetBoneName(BoneID BoneID);

            /* Initiate bone FNames for GetSocketLocation */
            void Init();
        };
    }
}