#pragma once
#include "../Game/SDK/Classes/Basic.h"

#define M_PI 3.14159265358979323f

namespace Math {
	float InvSqrt(float F);

	float GetDistance2D(float x1, float y1, float x2, float y2);

	SDK::FRotator NormalizeAxis(SDK::FRotator Rotation);

	float NormalizeAngle(float Angle);

	float DegreesToRadians(float degrees);

	float RadiansToDegrees(float radians);

	float GetDegreeDistance(SDK::FRotator Rotator1, SDK::FRotator Rotator2);

	float CalculateInterpolatedValue(float CurrentScalar, float MaxScalar, float MinValue, float MaxValue);

	bool IsOnScreen(const SDK::FVector2D& Position);
}