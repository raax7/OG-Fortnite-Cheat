#pragma once
#include <Windows.h>

namespace Game {
	inline float GameVersion;
	inline int ScreenWidth;
	inline int ScreenHeight;

	inline uintptr_t CurrentCanvas;

	void DrawCallback();
}