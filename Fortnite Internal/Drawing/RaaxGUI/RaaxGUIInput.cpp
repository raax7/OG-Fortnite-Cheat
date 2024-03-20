#include "RaaxGUIInput.h"
#include "RaaxGUI.h"
#include "../../Utilities/Logger.h"
#include "../../Game/Input/Input.h"

void RaaxGUIInput::SetResizingWindow(RaaxGUI::Window* Window) {
	RaaxGUIInput::ResizingWindow = Window;
	RaaxGUIInput::ResizingWindowOffset = RaaxGUIInput::MousePosition - Window->Position;
	RaaxGUIInput::ResizingWindowOriginal = Window->Position;
	RaaxGUIInput::ResizingWindowSize = Window->Size;
}
void RaaxGUIInput::ClearResizingWindow() {
	RaaxGUIInput::ResizingWindow = nullptr;
	RaaxGUIInput::ResizingWindowOffset = SDK::FVector2D();
	RaaxGUIInput::ResizingWindowOriginal = SDK::FVector2D();
	RaaxGUIInput::ResizingWindowSize = SDK::FVector2D();
}

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

void RaaxGUIInput::ProcessDraggingWindow() {
	SDK::FVector2D PotentialNewPosition = MousePosition - DraggingWindowOffset;
	DraggingWindow->FixWindowPosition(PotentialNewPosition);

	DraggingWindow->Position = PotentialNewPosition;
	DraggingWindowPosition = PotentialNewPosition;
}

void RaaxGUIInput::ProcessResizingWindow() {
	SDK::FVector2D PotentialNewPosition = SDK::FVector2D();
	SDK::FVector2D PotentialNewSize = SDK::FVector2D();

	switch (ResizingWindow->CurrentResizeDirection) {
// TODO: Fix resizing
#if 0
	case RaaxGUI::ResizeDirection::TopLeft:
		PotentialNewPosition = MousePosition - ResizingWindowOffset;
		PotentialNewSize = ResizingWindowSize + (ResizingWindowOriginal - MousePosition);

		ResizingWindow->FixWindowPosition(PotentialNewPosition);
		ResizingWindow->FixWindowSize(PotentialNewSize);

		ResizingWindow->Position = PotentialNewPosition;
		ResizingWindow->Size = PotentialNewSize;
		break;
	case RaaxGUI::ResizeDirection::TopRight:
		PotentialNewPosition = SDK::FVector2D(ResizingWindowOriginal.X, MousePosition.Y - ResizingWindowOffset.Y);
		PotentialNewSize = SDK::FVector2D(MousePosition.X - ResizingWindowOriginal.X, ResizingWindowSize.Y + (ResizingWindowOriginal.Y - MousePosition.Y));

		ResizingWindow->FixWindowPosition(PotentialNewPosition);
		ResizingWindow->FixWindowSize(PotentialNewSize);

		ResizingWindow->Position = PotentialNewPosition;
		ResizingWindow->Size = PotentialNewSize;

		break;
	case RaaxGUI::ResizeDirection::BottomLeft:
		PotentialNewPosition = SDK::FVector2D(MousePosition.X - ResizingWindowOffset.X, ResizingWindowOriginal.Y);
		PotentialNewSize = SDK::FVector2D(ResizingWindowSize.X + (ResizingWindowOriginal.X - MousePosition.X), MousePosition.Y - ResizingWindowOriginal.Y);

		ResizingWindow->FixWindowPosition(PotentialNewPosition);
		ResizingWindow->FixWindowSize(PotentialNewSize);

		ResizingWindow->Position = PotentialNewPosition;
		ResizingWindow->Size = PotentialNewSize;

		break;
#endif
	case RaaxGUI::ResizeDirection::BottomRight:
		PotentialNewSize = MousePosition - ResizingWindowOriginal;

		ResizingWindow->FixWindowSize(PotentialNewSize);

		ResizingWindow->Size = PotentialNewSize;

		break;
	}
}

void RaaxGUIInput::Tick() {
	MousePosition = Input::GetMousePosition();

	// Handle window order and dragging
	if (Input::IsKeyDown(Input::KeyName::LeftMouseButton)) {
		if (Input::WasKeyJustPressed(Input::KeyName::LeftMouseButton)) {
			std::vector ReverseWindowsTemp = RaaxGUI::GetContext()->RenderQue.Windows;
			std::reverse(ReverseWindowsTemp.begin(), ReverseWindowsTemp.end());

			for (auto Window : ReverseWindowsTemp) {
				if (Window->Open) {
					if (Window->IsInResizeBounds(MousePosition)) {
						Window->OnClickBegin(MousePosition);

						SetResizingWindow(Window);

						ClickedWindow = Window;

						break;
					}
					else if (Window->IsInMenuBounds(MousePosition)) {
						Window->OnClickBegin(MousePosition);

						if (Window->ShouldDrag(MousePosition)) {
							SetDraggingWindow(Window);
						}

						ClickedWindow = Window;

						break;
					}
				}
			}
		}
		else {
			if (ClickedWindow) ClickedWindow->OnClickTick(MousePosition);
			if (DraggingWindow) ProcessDraggingWindow();
			if (ResizingWindow) ProcessResizingWindow();
		}
	}
	else {
		if (DraggingWindow) ClearDraggingWindow();
		if (ResizingWindow) ClearResizingWindow();
		if (ClickedWindow) ClickedWindow->OnClickEnd();
	}

	if (Input::IsKeyDown(Input::KeyName::MouseX) || Input::IsKeyDown(Input::KeyName::MouseY)) {

	}
}