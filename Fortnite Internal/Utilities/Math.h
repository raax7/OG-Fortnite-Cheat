#ifndef MATH_H
#define MATH_H

#include <Windows.h>

#define M_PI 3.14159265358979323

namespace Math {
	inline float GetDistance2D(float x1, float y1, float x2, float y2) {
		return (float)sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
	}

	inline float Interpolate(float Value, float InputMin, float InputMax, float OutputMin, float OutputMax) {
		return OutputMin + (OutputMax - OutputMin) * ((Value - InputMin) / (InputMax - InputMin));
	}

	inline float Clamp(float Value, float MinValue, float MaxValue) {
		return (Value < MinValue) ? MinValue : ((Value > MaxValue) ? MaxValue : Value);
	}

	inline SDK::FRotator CalculateRotationAngles(SDK::FVector CameraPosition, SDK::FVector TargetPosition) {
		SDK::FVector Direction = TargetPosition - CameraPosition;

		float Yaw = (float)(atan2(Direction.Y, Direction.X) * 180.f / M_PI);

		float HorizontalDistance = (float)(sqrt(Direction.X * Direction.X + Direction.Y * Direction.Y));
		float Pitch = (float)(atan2(Direction.Z, HorizontalDistance) * 180.f / M_PI);

		SDK::FVector RotationAngles = { Pitch, Yaw, 0.f };

		return SDK::FRotator(RotationAngles.X, RotationAngles.Y, RotationAngles.Z);
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
		bool OnScreenX = (Position.X >= 0.f) && (Position.X <= Game::ScreenWidth);
		bool OnScreenY = (Position.Y >= 0.f) && (Position.Y <= Game::ScreenHeight);

		return OnScreenX && OnScreenY;
	}
}

#endif