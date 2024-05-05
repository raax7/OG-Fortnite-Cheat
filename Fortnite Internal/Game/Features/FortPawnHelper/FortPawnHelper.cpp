#include "FortPawnHelper.h"

#include "Bone.h"

#include "../../../Utilities/Math.h"

bool Features::FortPawnHelper::PopulateBones(Actors::Caches::FortPawnCache& FortPawnCache)
{
    // Resize the bone register to avoid out of range errors
    FortPawnCache.BonePositions3D.resize(Features::FortPawnHelper::Bone::BONEID_MAX);
    FortPawnCache.BonePositions2D.resize(Features::FortPawnHelper::Bone::BONEID_MAX);
    FortPawnCache.BoneVisibilityStates.resize(Features::FortPawnHelper::Bone::BONEID_MAX);

    bool FoundBoneOnScreen = false;

    for (int i = Bone::Head; i < Bone::BONEID_MAX; i++)
    {
        FortPawnCache.BonePositions3D[i] = FortPawnCache.Mesh->GetBonePosition(i);

        if (i == Bone::Head)
        {
            if (FortPawnCache.BonePositions3D[Bone::Head] == SDK::FVector(0, 0, 0))
            {
                return false;
            }
        }

        if (i == Bone::Chest)
        {
            FortPawnCache.BonePositions3D[Bone::Chest] = (FortPawnCache.BonePositions3D[Bone::ChestLeft] + FortPawnCache.BonePositions3D[Bone::ChestRight]) / 2;
        }

        // To avoid W2Sing players that aren't on the screen
        if (i <= 5 || FoundBoneOnScreen)
        {
            FortPawnCache.BonePositions2D[i] = SDK::Project(FortPawnCache.BonePositions3D[i]);

            if (Math::IsOnScreen(FortPawnCache.BonePositions2D[i]))
            {
                FoundBoneOnScreen = true;
            }
        }
    }

    return FoundBoneOnScreen;
}
void Features::FortPawnHelper::PopulateVisibilities(Actors::Caches::FortPawnCache& FortPawnCache)
{
    FortPawnCache.BonePositions3D.resize(Features::FortPawnHelper::Bone::BONEID_MAX);
    FortPawnCache.BonePositions2D.resize(Features::FortPawnHelper::Bone::BONEID_MAX);
    FortPawnCache.BoneVisibilityStates.resize(Features::FortPawnHelper::Bone::BONEID_MAX);

    SDK::APawn* LocalPawn = SDK::GetLocalPawn();

    for (int i = Bone::Head; i < Bone::BONEID_MAX; i++)
    {
        FortPawnCache.BoneVisibilityStates[i] = true;
    }

    return;

    FortPawnCache.BoneVisibilityStates[Bone::Head] = SDK::IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[Bone::Head], FortPawnCache.FortPawn, LocalPawn);
    FortPawnCache.BoneVisibilityStates[Bone::Chest] = SDK::IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[Bone::Chest], FortPawnCache.FortPawn, LocalPawn);
    FortPawnCache.BoneVisibilityStates[Bone::LeftShoulder] = SDK::IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[Bone::LeftShoulder], FortPawnCache.FortPawn, LocalPawn);
    FortPawnCache.BoneVisibilityStates[Bone::RightShoulder] = SDK::IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[Bone::RightShoulder], FortPawnCache.FortPawn, LocalPawn);
    FortPawnCache.BoneVisibilityStates[Bone::LeftElbow] = SDK::IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[Bone::LeftElbow], FortPawnCache.FortPawn, LocalPawn);
    FortPawnCache.BoneVisibilityStates[Bone::RightElbow] = SDK::IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[Bone::RightElbow], FortPawnCache.FortPawn, LocalPawn);
    FortPawnCache.BoneVisibilityStates[Bone::LeftHand] = SDK::IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[Bone::LeftHand], FortPawnCache.FortPawn, LocalPawn);
    FortPawnCache.BoneVisibilityStates[Bone::RightHand] = SDK::IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[Bone::RightHand], FortPawnCache.FortPawn, LocalPawn);
    FortPawnCache.BoneVisibilityStates[Bone::LeftLeg] = SDK::IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[Bone::LeftLeg], FortPawnCache.FortPawn, LocalPawn);
    FortPawnCache.BoneVisibilityStates[Bone::RightLeg] = SDK::IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[Bone::RightLeg], FortPawnCache.FortPawn, LocalPawn);
    FortPawnCache.BoneVisibilityStates[Bone::LeftKnee] = SDK::IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[Bone::LeftKnee], FortPawnCache.FortPawn, LocalPawn);
    FortPawnCache.BoneVisibilityStates[Bone::RightKnee] = SDK::IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[Bone::RightKnee], FortPawnCache.FortPawn, LocalPawn);
    FortPawnCache.BoneVisibilityStates[Bone::LeftFoot] = SDK::IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[Bone::LeftFoot], FortPawnCache.FortPawn, LocalPawn);
    FortPawnCache.BoneVisibilityStates[Bone::RightFoot] = SDK::IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[Bone::RightFoot], FortPawnCache.FortPawn, LocalPawn);
    FortPawnCache.BoneVisibilityStates[Bone::Pelvis] = SDK::IsPositionVisible(FortPawnCache.FortPawn, FortPawnCache.BonePositions3D[Bone::Pelvis], FortPawnCache.FortPawn, LocalPawn);
}
void Features::FortPawnHelper::PopulateBoundCorners(Actors::Caches::FortPawnCache& FortPawnCache, SDK::FVector2D& TopLeft, SDK::FVector2D& BottomRight)
{
    TopLeft = SDK::FVector2D(FLT_MAX, FLT_MAX);
    BottomRight = SDK::FVector2D(-FLT_MAX, -FLT_MAX);

    for (int i = Bone::Head; i < Bone::BONEID_MAX; i++)
    {
        TopLeft.X = min(TopLeft.X, FortPawnCache.BonePositions2D[i].X);
        TopLeft.Y = min(TopLeft.Y, FortPawnCache.BonePositions2D[i].Y);

        BottomRight.X = max(BottomRight.X, FortPawnCache.BonePositions2D[i].X);
        BottomRight.Y = max(BottomRight.Y, FortPawnCache.BonePositions2D[i].Y);
    }

    // Adjust the bounding box to make it more visually appealing
    float BoxSizeMultiplier = Math::CalculateInterpolatedValue(FortPawnCache.DistanceFromLocalPawn, 100.f, 1.0f, 4.0f);

    // Increase the size of the bounding box by a percentage of the original size. This is to make the bounding box more visually appealing.
    float LeftRightOffset = (BottomRight.X - TopLeft.X) * (0.36f * BoxSizeMultiplier);
    float TopBottomOffset = (BottomRight.Y - TopLeft.Y) * (0.14f * BoxSizeMultiplier);

    TopLeft.X -= LeftRightOffset;
    TopLeft.Y -= TopBottomOffset;

    BottomRight.X += LeftRightOffset;
    BottomRight.Y += TopBottomOffset;
}