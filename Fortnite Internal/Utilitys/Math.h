#ifndef MATH_H
#define MATH_H

#include <Windows.h>
#include <corecrt_math.h>

namespace Math {
	inline double GetCrosshairDistance(double x1, double y1, double x2, double y2) {
		return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
	}
}

#endif