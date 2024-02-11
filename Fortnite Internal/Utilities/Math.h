#ifndef MATH_H
#define MATH_H

#include <Windows.h>

#define M_PI 3.14159265358979323

namespace Math {
	inline float GetDistance2D(float x1, float y1, float x2, float y2) {
		return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
	}

	inline float Interpolate(float value, float inputMin, float inputMax, float outputMin, float outputMax) {
		return outputMin + (outputMax - outputMin) * ((value - inputMin) / (inputMax - inputMin));
	}

	inline float Clamp(float value, float minValue, float maxValue) {
		return (value < minValue) ? minValue : ((value > maxValue) ? maxValue : value);
	}

	inline SDK::FRotator CalculateRotationAngles(SDK::FVector cameraPosition, SDK::FVector itemPosition) {
		SDK::FVector direction = itemPosition - cameraPosition;

		float yaw = atan2(direction.Y, direction.X) * 180.0f / M_PI;

		float horizontalDistance = sqrt(direction.X * direction.X + direction.Y * direction.Y);
		float pitch = atan2(direction.Z, horizontalDistance) * 180.0f / M_PI;

		SDK::FVector rotationAngles = { pitch, yaw, 0.0f };

		return SDK::FRotator(rotationAngles.X, rotationAngles.Y, rotationAngles.Z);
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

	inline bool IsOnScreen(const SDK::FVector2D& Position) {
		bool OnScreenX = (Position.X >= 0.0f) && (Position.X <= Game::ScreenWidth);
		bool OnScreenY = (Position.Y >= 0.0f) && (Position.Y <= Game::ScreenHeight);

		return OnScreenX && OnScreenY;
	}
}

#endif