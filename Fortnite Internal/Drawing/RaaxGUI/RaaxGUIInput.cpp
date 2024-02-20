#include "RaaxGUIInput.h"
#include "RaaxGUI.h"
#include "../../Utilities/Logger.h"
#include "../../Game/Input/Input.h"

SDK::FVector2D RaaxGUIInput::MousePosition = SDK::FVector2D();
SDK::FVector2D RaaxGUIInput::MousePositionOnClicked = SDK::FVector2D();

RaaxGUI::Window* RaaxGUIInput::DraggingWindow = nullptr;
SDK::FVector2D RaaxGUIInput::DraggingWindowOffset = SDK::FVector2D();
SDK::FVector2D RaaxGUIInput::DraggingWindowOriginal = SDK::FVector2D();
SDK::FVector2D RaaxGUIInput::DraggingWindowPosition = SDK::FVector2D();

void RaaxGUIInput::SetDraggingWindow(RaaxGUI::Window* Window) {
	RaaxGUIInput::DraggingWindow = Window;
	RaaxGUIInput::DraggingWindowOffset = RaaxGUIInput::MousePosition - Window->Position;
	RaaxGUIInput::DraggingWindowOriginal = Window->Position;
	RaaxGUIInput::DraggingWindowPosition = Window->Position;
}

void RaaxGUIInput::ClearDraggingWindow() {
	RaaxGUIInput::DraggingWindow = nullptr;
	RaaxGUIInput::DraggingWindowOffset = SDK::FVector2D();
	RaaxGUIInput::DraggingWindowOriginal = SDK::FVector2D();
	RaaxGUIInput::DraggingWindowPosition = SDK::FVector2D();
}

void RaaxGUIInput::Tick() {
	MousePosition = Input::GetMousePosition();

	// Handle window order and dragging
	if (Input::IsKeyDown(Input::KeyName::LeftMouseButton)) {
		if (DraggingWindow) {
			DraggingWindow->Position = MousePosition - DraggingWindowOffset;
			DraggingWindowPosition = DraggingWindow->Position;
		}
		else if (Input::WasKeyJustPressed(Input::KeyName::LeftMouseButton)) {
			std::vector ReverseWindowsTemp = RaaxGUI::GetContext()->Windows;
			std::reverse(ReverseWindowsTemp.begin(), ReverseWindowsTemp.end());

			for (auto Window : ReverseWindowsTemp) {
				if (Window->Open && Window->IsInMenuBounds(MousePosition)) {
					DEBUG_LOG(skCrypt("Found window to move up").decrypt());
					Window->OnClick();

					if (Window->IsInDragBounds(MousePosition)) SetDraggingWindow(Window);
					break;
				}
			}
		}
	}
	else {
		if (DraggingWindow) {
			DEBUG_LOG(skCrypt("Clearing dragging window").decrypt());
			ClearDraggingWindow();
		}
	}
}