#pragma once
#include "RaaxGUI.h"

#include "../../Game/SDK/Classes/Basic.h"

namespace RaaxGUIInput {
	inline SDK::FVector2D MousePosition;
	inline SDK::FVector2D MousePositionOnClicked;

	inline RaaxGUI::Window* ClickedWindow;

	inline RaaxGUI::Window* ResizingWindow;
	inline SDK::FVector2D ResizingWindowOffset;
	inline SDK::FVector2D ResizingWindowOriginal;
	inline SDK::FVector2D ResizingWindowSize;

	inline RaaxGUI::Window* DraggingWindow;
	inline SDK::FVector2D DraggingWindowOffset;
	inline SDK::FVector2D DraggingWindowOriginal;
	inline SDK::FVector2D DraggingWindowPosition;



	void SetResizingWindow(RaaxGUI::Window* Window);
	void ClearResizingWindow();
	void ProcessResizingWindow();

	void SetDraggingWindow(RaaxGUI::Window* Window);
	void ClearDraggingWindow();
	void ProcessDraggingWindow();

	void Tick();
};