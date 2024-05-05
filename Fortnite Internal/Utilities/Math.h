#pragma once
#include "../Game/SDK/Classes/Basic.h"

#define M_PI 3.14159265358979323f

namespace Math
{
    float InvSqrt(const float F);

    float GetDistance2D(const float x1, const float y1, const float x2, const float y2);

    SDK::FRotator NormalizeAxis(SDK::FRotator& Rotation);

    float NormalizeAngle(float Angle);

    float DegreesToRadians(const float degrees);

    float RadiansToDegrees(const float radians);

    float GetDegreeDistance(const SDK::FRotator& Rotator1, const SDK::FRotator& Rotator2);

    float CalculateInterpolatedValue(const float CurrentScalar, float MaxScalar, const float MinValue, const float MaxValue);

    bool IsOnScreen(const SDK::FVector2D& Position);
}