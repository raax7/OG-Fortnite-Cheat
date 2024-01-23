#ifndef MATH_H
#define MATH_H

#include <Windows.h>
#include <corecrt_math.h>

#define M_PI 3.14159265358979323

namespace Math {
	inline float GetCrosshairDistance(float x1, float y1, float x2, float y2) {
		return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
	}
}

#endif