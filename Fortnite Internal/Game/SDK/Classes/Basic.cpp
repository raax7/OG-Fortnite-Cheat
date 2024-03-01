#include "Basic.h"

#include "../../../Utilities/Math.h"

bool SDK::FVector::Normalize(float Tolerance) {
	const float SquareSum = X * X + Y * Y + Z * Z;
	if (SquareSum > Tolerance)
	{
		const float Scale = Math::InvSqrt(SquareSum);
		X *= Scale; Y *= Scale; Z *= Scale;
		return true;
	}
	return false;
}

SDK::FVector SDK::FRotator::Vector() const {
	float CP, SP, CY, SY;
	Math::SinCos(&SP, &CP, Math::DegreesToRadians(Pitch));
	Math::SinCos(&SY, &CY, Math::DegreesToRadians(Yaw));
	FVector V = FVector(CP * CY, CP * SY, SP);

	return V;
}