#pragma once
#include <vadefs.h>
#include <chrono>

namespace Game {
	inline float GameVersion		= 0.f;
	inline int ScreenWidth			= 0;
	inline int ScreenHeight			= 0;
	inline float ScreenCenterX		= 0.f;
	inline float ScreenCenterY		= 0.f;
	inline float PixelsPerDegree	= 0.f;

	inline unsigned __int64 CurrentFrame = 0;
	inline std::chrono::time_point<std::chrono::steady_clock> CurrentTime;

	inline uintptr_t CurrentCanvas	= 0x0;

	void MenuCallback();
	void DrawCallback();
}