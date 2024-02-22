#include "RaaxGUI.h"
#include "RaaxGUIInput.h"

#include "../Drawing.h"

RaaxGUI::Window* RaaxGUI::RegisterNewWindow(int ID) {
	Window* NewWindow = new Window();
	NewWindow->ID = ID;

	GetContext()->Windows.push_back(NewWindow);

	return NewWindow;
}

RaaxGUI::Window* RaaxGUI::FindWindowById(int ID) {
	for (int i = 0; i < GetContext()->Windows.size(); i++) {
		if (GetContext()->Windows[i]->ID == ID) {
			return GetContext()->Windows[i];
		}
	}

	return nullptr;
}

bool RaaxGUI::BeginWindow(const char* Name, bool* Open, RaaxGUIWindowFlags Flags, SDK::FVector2D InitPosition, SDK::FVector2D InitSize) {
	int Id = HashString(Name);

	Window* ThisWindow = FindWindowById(Id);
	if (ThisWindow == nullptr) {
		DEBUG_LOG(skCrypt("Creating new window").decrypt());
		ThisWindow = RegisterNewWindow(Id);
		ThisWindow->ID = HashString(Name);

		ThisWindow->Size = InitSize;
		ThisWindow->Position = InitPosition;

		ThisWindow->Name = Name;
	}

	ThisWindow->Seen = true;

	GetContext()->CurrentWindow = ThisWindow;

	if (GetContext()->UseNextWindowSize) {
		ThisWindow->Size = GetContext()->NextWindowSize;
		GetContext()->UseNextWindowSize = false;
	}
	if (GetContext()->UseNextWindowPos) {
		ThisWindow->Position = GetContext()->NextWindowPos;
		GetContext()->UseNextWindowPos = false;
	}

	ThisWindow->Open = *Open;

	return Open;
}

void RaaxGUI::EndWindow() {
	GetContext()->CurrentWindow = nullptr;
}

void RaaxGUI::InitContext() {
	if (GContext) {
		THROW_ERROR(skCrypt("Context already initialized!").decrypt(), false);
	}

	GContext = new RaaxGUIContext();

	if (GContext == nullptr) {
		THROW_ERROR(skCrypt("Failed to initialize context!").decrypt(), true);
	}

	// Initialize style
	GContext->Style.InitDarkStyle();
}

RaaxGUI::RaaxGUIContext* RaaxGUI::GetContext() {
	if (GContext == nullptr) {
		THROW_ERROR(skCrypt("Context not initialized!").decrypt(), false);
	}

	return GContext;
}

void RaaxGUI::NewFrame() {
	// Clear all window seen flags
	for (auto Window : GetContext()->Windows) {
		if (Window) {
			Window->Seen = false;
		}
	}
}

void RaaxGUI::Render() {
	RaaxGUIInput::Tick();

	for (int i = 0; i < GetContext()->Windows.size(); i++) {
		Window* CurrentWindow = GetContext()->Windows[i];

		if (CurrentWindow->Open) {
			// Draw window
			Drawing::FilledRect(CurrentWindow->Position, CurrentWindow->Size, CurrentWindow->Style.BackgroundColor, false);

			// Draw title bar
			Drawing::FilledRect(CurrentWindow->Position, { CurrentWindow->Size.X, 20 }, { 0.2f, 0.2f, 0.2f, 1.f }, false);	
			
			SDK::FVector2D TextPosition = SDK::FVector2D();

			switch (CurrentWindow->Style.TitleBarTextAlignment) {
			case TextAlignment::Left:
				TextPosition = SDK::FVector2D(CurrentWindow->Position.X + 5, CurrentWindow->Position.Y + 5);
				break;
			case TextAlignment::Center:
				TextPosition = SDK::FVector2D(CurrentWindow->Position.X + (CurrentWindow->Size.X / 2), CurrentWindow->Position.Y + 5);
				break;
			case TextAlignment::Right:
				TextPosition = SDK::FVector2D(CurrentWindow->Position.X + CurrentWindow->Size.X - 5, CurrentWindow->Position.Y + 5);
				break;
			}

			Drawing::Text(CurrentWindow->Name, TextPosition, 12.f, { 1.f, 1.f, 1.f, 1.f }, false, false, false);
		}
	}

	// Clear all windows that were not seen
	for (auto it = GetContext()->Windows.begin(); it != GetContext()->Windows.end();) {
		Window* CurrentWindow = *it;

		if (CurrentWindow->Seen == false) {
			it = GetContext()->Windows.erase(it);
			delete CurrentWindow;
		}
		else {
			++it;
		}
	}
}

RaaxGUI::RaaxGUIStyle* RaaxGUI::GetStyle() {
	return &GetContext()->Style;
}

bool RaaxGUI::Window::IsInMenuBounds(SDK::FVector2D Position) {
	if (Position.X > this->Position.X && Position.X < this->Position.X + this->Size.X) {
		if (Position.Y > this->Position.Y && Position.Y < this->Position.Y + this->Size.Y) {
			return true;
		}
	}

	return false;
}

bool RaaxGUI::Window::IsInDragBounds(SDK::FVector2D Position) {
	return IsInMenuBounds(Position);
}

bool RaaxGUI::Window::IsInResizeBounds(SDK::FVector2D Position) {
	// Check if mouse is within 2 pixels of any corner
	SDK::FVector2D TopLeft = this->Position;
	SDK::FVector2D TopRight = SDK::FVector2D(this->Position.X + this->Size.X, this->Position.Y);
	SDK::FVector2D BottomLeft = SDK::FVector2D(this->Position.X, this->Position.Y + this->Size.Y);
	SDK::FVector2D BottomRight = SDK::FVector2D(this->Position.X + this->Size.X, this->Position.Y + this->Size.Y);

	if (Position.Distance(TopLeft) < 10) {
		CurrentResizeDirection = ResizeDirection::TopLeft;
		return true;
	}
	else if (Position.Distance(TopRight) < 10) {
		CurrentResizeDirection = ResizeDirection::TopRight;
		return true;
	}
	else if (Position.Distance(BottomLeft) < 10) {
		CurrentResizeDirection = ResizeDirection::BottomLeft;
		return true;
	}
	else if (Position.Distance(BottomRight) < 10) {
		CurrentResizeDirection = ResizeDirection::BottomRight;
		return true;
	}

	return false;
}

void RaaxGUI::Window::FixWindowPosition(SDK::FVector2D& Position) {
	// Clamp to screen
	// Require ateast 10 pixels of the windows drag space to be visible

}
void RaaxGUI::Window::FixWindowSize(SDK::FVector2D& Size) {
	// Clamp to minimum size
	if (Size.X < MinimumSize.X) {
		Size.X = MinimumSize.X;
	}
	if (Size.Y < MinimumSize.Y) {
		Size.Y = MinimumSize.Y;
	}
}

RaaxGUI::ResizeDirection RaaxGUI::Window::GetResizeDirection(SDK::FVector2D Position) {
	return CurrentResizeDirection;
}

void RaaxGUI::Window::OnClick() {
	DEBUG_LOG("Clicked on window");

	GetContext()->MakeWindowTopMost(this);
}

void RaaxGUI::RaaxGUIContext::MakeWindowTopMost(Window* Window) {
	for (int i = 0; i < Windows.size(); i++) {
		if (Windows[i] == Window) {
			Windows.erase(Windows.begin() + i);
			Windows.push_back(Window);
			break;
		}
	}
}

void RaaxGUI::SetNextWindowSize(SDK::FVector2D Size) {
	GetContext()->UseNextWindowSize = true;
	GetContext()->NextWindowSize = Size;
}

void RaaxGUI::SetNextWindowPos(SDK::FVector2D Pos) {
	GetContext()->UseNextWindowPos = true;
	GetContext()->NextWindowPos = Pos;
}