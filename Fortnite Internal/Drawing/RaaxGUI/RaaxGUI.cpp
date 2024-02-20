#include "RaaxGUI.h"
#include "RaaxGUIInput.h"

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

bool RaaxGUI::BeginWindow(const char* Name, bool* Open, SDK::FVector2D InitPosition, SDK::FVector2D InitSize) {
	int Id = HashString(Name);

	Window* ThisWindow = FindWindowById(Id);
	if (ThisWindow == nullptr) {
		DEBUG_LOG(skCrypt("Creating new window").decrypt());
		ThisWindow = RegisterNewWindow(Id);
		ThisWindow->ID = HashString(Name);

		ThisWindow->Size = InitSize;
		ThisWindow->Position = InitPosition;
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