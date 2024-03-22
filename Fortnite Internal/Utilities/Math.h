#pragma once
#include <Windows.h>
#include <cmath>
#include <emmintrin.h>
#include <algorithm>

#include "../Game/SDK/Classes/Engine_Classes.h"

#include "../Game/Game.h"

#define INV_PI 0.31830988618f
#define HALF_PI 1.57079632679f
#define M_PI 3.14159265358979323f

namespace Math {
	inline float InvSqrt(float F) {
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
	inline void SinCos(float* ScalarSin, float* ScalarCos, float  Value)
	{
		// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
		float quotient = (INV_PI * 0.5f) * Value;
		if (Value >= 0.0f)
		{
			quotient = (float)((int)(quotient + 0.5f));
		}
		else
		{
			quotient = (float)((int)(quotient - 0.5f));
		}
		float y = Value - (2.0f * M_PI) * quotient;

		// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
		float sign;
		if (y > HALF_PI)
		{
			y = M_PI - y;
			sign = -1.0f;
		}
		else if (y < -HALF_PI)
		{
			y = -M_PI - y;
			sign = -1.0f;
		}
		else
		{
			sign = +1.0f;
		}

		float y2 = y * y;

		// 11-degree minimax approximation
		*ScalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

		// 10-degree minimax approximation
		float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
		*ScalarCos = sign * p;
	}

	inline float GetDistance2D(float x1, float y1, float x2, float y2) {
		return (float)sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
	}

	inline float Interpolate(float Value, float InputMin, float InputMax, float OutputMin, float OutputMax) {
		return OutputMin + (OutputMax - OutputMin) * ((Value - InputMin) / (InputMax - InputMin));
	}

	inline float Clamp(float Value, float MinValue, float MaxValue) {
		return (Value < MinValue) ? MinValue : ((Value > MaxValue) ? MaxValue : Value);
	}

	inline SDK::FRotator NormalizeAxis(SDK::FRotator Rotation) {
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

	inline float NormalizeAngle(float Angle) {
		while (Angle > 180.f) {
			Angle -= 360.f;
		}
		while (Angle < -180.f) {
			Angle += 360.f;
		}
		return Angle;
	}

	inline float DegreesToRadians(float degrees) {
		return degrees * (M_PI / 180.0f);
	}

	inline SDK::FQuat FRotatorToQuat(SDK::FRotator Rotator) {
		float SP, SY, SR, CP, CY, CR;
		Math::SinCos(&SP, &CP, Math::DegreesToRadians(Rotator.Pitch));
		Math::SinCos(&SY, &CY, Math::DegreesToRadians(Rotator.Yaw));
		Math::SinCos(&SR, &CR, Math::DegreesToRadians(Rotator.Roll));

		SDK::FQuat RotationQuat;
		RotationQuat.X = CR * SP * SY - SR * CP * CY;
		RotationQuat.Y = -CR * SP * CY - SR * CP * SY;
		RotationQuat.Z = CR * CP * SY - SR * SP * CY;
		RotationQuat.W = CR * CP * CY + SR * SP * SY;

		return RotationQuat;
	}

	inline float RadiansToDegrees(float radians) {
		return radians * (180.0f / M_PI);
	}

	inline float GetDegreeDistance(SDK::FRotator Rotator1, SDK::FRotator Rotator2) {
		SDK::FVector ForwardVector1 = SDK::UKismetMathLibrary::StaticClass()->GetForwardVector(Rotator1);
		SDK::FVector ForwardVector2 = SDK::UKismetMathLibrary::StaticClass()->GetForwardVector(Rotator2);

		ForwardVector1.Normalize();
		ForwardVector2.Normalize();

		float DotProduct = ForwardVector1.Dot(ForwardVector2);
		DotProduct = Clamp(DotProduct, -1.0f, 1.0f);

		float AngleBetween = RadiansToDegrees(acos(DotProduct));

		return AngleBetween;
	}

	inline float CalculateInterpolatedValue(float CurrentScalar, float MaxScalar, float MinValue, float MaxValue) {
		MaxScalar = min(MaxScalar, CurrentScalar);

		float InterpolatedValue = MaxValue - (MaxValue - MinValue) * (MaxScalar / CurrentScalar);

		InterpolatedValue = std::clamp(InterpolatedValue, MinValue, MaxValue);

		return InterpolatedValue;
	}

	inline bool IsOnScreen(const SDK::FVector2D& Position) {
		bool OnScreenX = (Position.X >= 0.f) && (Position.X <= Game::ScreenWidth);
		bool OnScreenY = (Position.Y >= 0.f) && (Position.Y <= Game::ScreenHeight);

		return OnScreenX && OnScreenY;
	}
}