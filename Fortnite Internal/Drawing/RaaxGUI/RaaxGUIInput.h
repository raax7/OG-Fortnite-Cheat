#pragma once
#include <Windows.h>
#include <string>
#include "../../Game/SDK/Classes/Basic.h"
#include "RaaxGUI.h"

class RaaxGUIInput {
public:
	static SDK::FVector2D MousePosition;
	static SDK::FVector2D MousePositionOnClicked;

	static RaaxGUI::Window* ResizingWindow;
	static SDK::FVector2D ResizingWindowOffset;
	static SDK::FVector2D ResizingWindowOriginal;
	static SDK::FVector2D ResizingWindowSize;

	static RaaxGUI::Window* DraggingWindow;
	static SDK::FVector2D DraggingWindowOffset;
	static SDK::FVector2D DraggingWindowOriginal;
	static SDK::FVector2D DraggingWindowPosition;
public:
	static void SetResizingWindow(RaaxGUI::Window* Window);
	static void ClearResizingWindow();
	static void ProcessResizingWindow();

	static void SetDraggingWindow(RaaxGUI::Window* Window);
	static void ClearDraggingWindow();
	static void ProcessDraggingWindow();

	static void Tick();
};