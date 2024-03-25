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