#include "Math.h"

#include <cmath>
#include <emmintrin.h>
#include <algorithm>

#include "../Game/SDK/Classes/Engine_Classes.h"

#include "../Game/Game.h"

float Math::InvSqrt(float F) {
	// Performs two passes of Newton-Raphson iteration on the hardware estimate
	//    v^-0.5 = x
	// => x^2 = v^-1
	// => 1/(x^2) = v
	// => F(x) = x^-2 - v
	//    F'(x) = -2x^-3

	//    x1 = x0 - F(x0)/F'(x0)
	// => x1 = x0 + 0.5 * (x0^-2 - Vec) * x0^3
	// => x1 = x0 + 0.5 * (x0 - Vec * x0^3)
	// => x1 = x0 + x0 * (0.5 - 0.5 * Vec * x0^2)
	//
	// This final form has one more operation than the legacy factorization (X1 = 0.5*X0*(3-(Y*X0)*X0)
	// but retains better accuracy (namely InvSqrt(1) = 1 exactly).

	const __m128 fOneHalf = _mm_set_ss(0.5f);
	__m128 Y0, X0, X1, X2, FOver2;
	float temp;

	Y0 = _mm_set_ss(F);
	X0 = _mm_rsqrt_ss(Y0);	// 1/sqrt estimate (12 bits)
	FOver2 = _mm_mul_ss(Y0, fOneHalf);

	// 1st Newton-Raphson iteration
	X1 = _mm_mul_ss(X0, X0);
	X1 = _mm_sub_ss(fOneHalf, _mm_mul_ss(FOver2, X1));
	X1 = _mm_add_ss(X0, _mm_mul_ss(X0, X1));

	// 2nd Newton-Raphson iteration
	X2 = _mm_mul_ss(X1, X1);
	X2 = _mm_sub_ss(fOneHalf, _mm_mul_ss(FOver2, X2));
	X2 = _mm_add_ss(X1, _mm_mul_ss(X1, X2));

	_mm_store_ss(&temp, X2);
	return temp;
}

float Math::GetDistance2D(float x1, float y1, float x2, float y2) {
	return (float)sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

SDK::FRotator Math::NormalizeAxis(SDK::FRotator Rotation) {
	while (Rotation.Yaw > 180.f)
		Rotation.Yaw -= 360.f;
	while (Rotation.Yaw < -180.f)
		Rotation.Yaw += 360.f;

	while (Rotation.Roll > 180.f)
		Rotation.Roll -= 360.f;
	while (Rotation.Roll < -180.f)
		Rotation.Roll += 360.f;

	while (Rotation.Pitch > 180.f)
		Rotation.Pitch -= 360.f;
	while (Rotation.Pitch < -180.f)
		Rotation.Pitch += 360.f;

	return Rotation;
}

float Math::NormalizeAngle(float Angle) {
	while (Angle > 180.f) {
		Angle -= 360.f;
	}
	while (Angle < -180.f) {
		Angle += 360.f;
	}
	return Angle;
}

float Math::DegreesToRadians(float degrees) {
	return degrees * (M_PI / 180.0f);
}

float Math::RadiansToDegrees(float radians) {
	return radians * (180.0f / M_PI);
}

float Math::GetDegreeDistance(SDK::FRotator Rotator1, SDK::FRotator Rotator2) {
	SDK::FVector ForwardVector1 = SDK::UKismetMathLibrary::GetForwardVector(Rotator1);
	SDK::FVector ForwardVector2 = SDK::UKismetMathLibrary::GetForwardVector(Rotator2);

	ForwardVector1.Normalize();
	ForwardVector2.Normalize();

	float DotProduct = ForwardVector1.Dot(ForwardVector2);
	DotProduct = std::clamp(DotProduct, -1.0f, 1.0f);

	float AngleBetween = RadiansToDegrees(acos(DotProduct));

	return AngleBetween;
}

float Math::CalculateInterpolatedValue(float CurrentScalar, float MaxScalar, float MinValue, float MaxValue) {
	MaxScalar = min(MaxScalar, CurrentScalar);

	float InterpolatedValue = MaxValue - (MaxValue - MinValue) * (MaxScalar / CurrentScalar);

	InterpolatedValue = std::clamp(InterpolatedValue, MinValue, MaxValue);

	return InterpolatedValue;
}

bool Math::IsOnScreen(const SDK::FVector2D& Position) {
	bool OnScreenX = (Position.X >= 0.f) && (Position.X <= Game::ScreenWidth);
	bool OnScreenY = (Position.Y >= 0.f) && (Position.Y <= Game::ScreenHeight);

	return OnScreenX && OnScreenY;
}