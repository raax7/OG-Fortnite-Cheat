#pragma once
#include <Windows.h>
#include <string>
#include "../../Game/SDK/Classes/Basic.h"
#include "RaaxGUI.h"

class RaaxGUIInput {
public:
	static SDK::FVector2D MousePosition;
	static SDK::FVector2D MousePositionOnClicked;

	static RaaxGUI::Window* DraggingWindow;
	static SDK::FVector2D DraggingWindowOffset;
	static SDK::FVector2D DraggingWindowOriginal;
	static SDK::FVector2D DraggingWindowPosition;
public:
	static void SetDraggingWindow(RaaxGUI::Window* Window);
	static void ClearDraggingWindow();
	static void Tick();
};