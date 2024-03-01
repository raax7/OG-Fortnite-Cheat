#include "RaaxGUI.h"
#include "RaaxGUIInput.h"

#include "../../Utilities/Error.h"

// If you are using this library on another project, replace this with your own drawing wrappers (or recreate the ones here)
#include "../Drawing.h"
#include "../../Utilities/Logger.h"

/* CONTEXT FUNCTIONS */
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

void RaaxGUI::NewFrame() {
	RaaxGUIInput::Tick();

	// Clear all window seen flags
	for (auto Window : GetContext()->RenderQue.Windows) {
		if (Window) {
			Window->Seen = false;
			for (auto Element : Window->Elements) {
				Element->Seen = false;
			}
			Window->ElementsLastFrame = Window->Elements;
			Window->Elements.clear();
		}
	}
}
void RaaxGUI::EndFrame() {
	for (int i = 0; i < GetContext()->RenderQue.Windows.size(); i++) {
		Window* CurrentWindow = GetContext()->RenderQue.Windows[i];
		CurrentWindow->Draw();
	}

	// Clear all windows that were not seen
	for (auto it = GetContext()->RenderQue.Windows.begin(); it != GetContext()->RenderQue.Windows.end();) {
		Window* CurrentWindow = *it;

		if (CurrentWindow == nullptr) {
			it = GetContext()->RenderQue.Windows.erase(it);
			continue;
		}

		if (CurrentWindow->Seen == false) {
			it = GetContext()->RenderQue.Windows.erase(it);
			delete CurrentWindow;
		}
		else {
			++it;
		}

		// Loop thru all elements and clear elements that were not seen
		for (auto it2 = CurrentWindow->ElementsLastFrame.begin(); it2 != CurrentWindow->ElementsLastFrame.end();) {
			Element* CurrentElement = *it2;

			if (CurrentElement->Seen == false) {
				DEBUG_LOG(skCrypt("Element not seen! - ").decrypt() + CurrentElement->Name);
				it2 = CurrentWindow->ElementsLastFrame.erase(it2);
				delete CurrentElement;
			}
			else {
				++it2;
			}
		}
	}
}

void RaaxGUI::RaaxGUIContext::MakeWindowTopMost(Window* Window) {
	for (int i = 0; i < RenderQue.Windows.size(); i++) {
		if (RenderQue.Windows[i] == Window) {
			RenderQue.Windows.erase(RenderQue.Windows.begin() + i);
			RenderQue.Windows.push_back(Window);
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

RaaxGUI::Window* RaaxGUI::RaaxGUIContext::GetCurrentWindow() {
	return CurrentWindow;
}
RaaxGUI::RaaxGUIContext* RaaxGUI::GetContext() {
	if (GContext == nullptr) {
		THROW_ERROR(skCrypt("Context not initialized!").decrypt(), false);
	}

	return GContext;
}
RaaxGUI::RaaxGUIStyle* RaaxGUI::GetStyle() {
	return &GetContext()->Style;
}



/* WINDOW FUNCTIONS */
bool RaaxGUI::BeginWindow(const char* Name, bool* Open, RaaxGUIWindowFlags Flags, SDK::FVector2D InitPosition, SDK::FVector2D InitSize) {
	int Id = HashString(Name);

	Window* ThisWindow = FindWindowById(Id);
	if (ThisWindow == nullptr) {
		ThisWindow = RegisterNewWindow(Id);
		ThisWindow->ID = Id;

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

	ThisWindow->CurrentElementPosition = SDK::FVector2D(0, ThisWindow->Style.TitleBarSize.Y);

	ThisWindow->Open = *Open;

	return Open;
}
void RaaxGUI::EndWindow() {
	GetContext()->CurrentWindow = nullptr;
}

RaaxGUI::Window* RaaxGUI::RegisterNewWindow(int ID) {
	Window* NewWindow = new Window();
	NewWindow->ID = ID;

	GetContext()->RenderQue.Windows.push_back(NewWindow);

	return NewWindow;
}
RaaxGUI::Window* RaaxGUI::FindWindowById(int ID) {
	for (int i = 0; i < GetContext()->RenderQue.Windows.size(); i++) {
		if (GetContext()->RenderQue.Windows[i]->ID == ID) {
			return GetContext()->RenderQue.Windows[i];
		}
	}

	return nullptr;
}

void RaaxGUI::Window::Draw() {
	if (Open) {
		// Draw window
		Drawing::FilledRect(Position, Size, Style.BackgroundColor, false);

		// Draw title bar
		Drawing::FilledRect(Position, { Size.X, Style.TitleBarSize.Y }, { 0.2f, 0.2f, 0.2f, 1.f }, false);

		SDK::FVector2D TextPosition = SDK::FVector2D();

		switch (Style.TitleBarTextAlignment) {
		case TextAlignment::Left:
			TextPosition = SDK::FVector2D(Position.X + 5, Position.Y + 5);
			break;
		case TextAlignment::Center:
			TextPosition = SDK::FVector2D(Position.X + (Size.X / 2), Position.Y + 5);
			break;
		case TextAlignment::Right:
			TextPosition = SDK::FVector2D(Position.X + Size.X - 5, Position.Y + 5);
			break;
		}

		Drawing::Text(Name, TextPosition, 12.f, { 1.f, 1.f, 1.f, 1.f }, Style.TitleBarTextAlignment == TextAlignment::Center ? true : false, false, false);

		// Draw elements
		for (int i = 0; i < Elements.size(); i++) {
			Elements[i]->Draw();
		}
	}
}

void RaaxGUI::Window::OnClickBegin(const SDK::FVector2D ClickPosition) {
	SDK::FVector2D RelativePosition = ClickPosition - Position;

	GetContext()->MakeWindowTopMost(this);

	for (int i = 0; i < Elements.size(); i++) {
		if (Elements[i]->IsInElementBounds(RelativePosition)) {
			Elements[i]->OnClickBegin(RelativePosition);

			// There should only ever be one element clicked at a time
			continue;
		}
	}
}
void RaaxGUI::Window::OnClickEnd() {
	for (int i = 0; i < Elements.size(); i++) {
		if (Elements[i]->IsBeingClicked()) {
			Elements[i]->OnClickEnd();

			// There should only ever be one element clicked at a time
			continue;
		}
	}
}
bool RaaxGUI::Window::ShouldDrag(SDK::FVector2D Position) {
	SDK::FVector2D RelativePosition = Position - this->Position;

	if (IsInDragBounds(Position)) {
		for (int i = 0; i < Elements.size(); i++) {
			if (Elements[i]->IsInElementBounds(RelativePosition)) {
				return false;
			}
		}

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

RaaxGUI::ResizeDirection RaaxGUI::Window::GetResizeDirection(SDK::FVector2D Position) {
	return CurrentResizeDirection;
}



/* ELEMENT FUNCTIONS */
template<typename ElementType> ElementType* RaaxGUI::RegisterNewElement(int ID, Window* ElementWindow) {
	// Check if T inherits from Element
	if (std::is_base_of<Element, ElementType>::value == false) {
		THROW_ERROR(skCrypt("Attempted to register an element that does not inherit from Element class!").decrypt(), true);
		return nullptr;
	}

	ElementType* NewElement = new ElementType();
	NewElement->ID = ID;
	NewElement->ParentWindow = ElementWindow;

	ElementWindow->Elements.push_back(NewElement);

	return NewElement;
}
template<typename ElementType> ElementType* RaaxGUI::FindElementByIdAndWindow(int ID, Window* ElementWindow) {
	for (int i = 0; i < GetContext()->RenderQue.Windows.size(); i++) {
		if (GetContext()->RenderQue.Windows[i] == ElementWindow) {
			for (int j = 0; j < ElementWindow->ElementsLastFrame.size(); j++) {
				if (ElementWindow->ElementsLastFrame[j]->ID == ID) {
					// Check if ElementWindow->Elements[j] is the same type as T
					//if (std::is_<Element, ElementWindow->Elements[j]>::value == false) {
					//	continue;
					//}

					return reinterpret_cast<ElementType*>(ElementWindow->ElementsLastFrame[j]);
				}
			}
		}
	}

	return nullptr;
}

void RaaxGUI::Element::Draw() {
	if (ParentWindow == nullptr) {
		return;
	}

	switch (Type) {
	case ElementType::Checkbox:
		reinterpret_cast<CheckboxElement*>(this)->DrawCheckbox();
		break;
	}
}

void RaaxGUI::Element::OnClickBegin(SDK::FVector2D ClickPosition) {
	if (ParentWindow == nullptr) {
		return;
	}

	switch (Type) {
	case ElementType::Checkbox:
		reinterpret_cast<CheckboxElement*>(this)->OnClickBeginCheckbox();
		break;
	}
}
void RaaxGUI::Element::OnClickEnd() {
	if (ParentWindow == nullptr) {
		return;
	}

	BeingClicked = false;

	switch (Type) {
	case ElementType::Checkbox:
		reinterpret_cast<CheckboxElement*>(this)->OnClickEndCheckbox();
		break;
	}
}
bool RaaxGUI::Element::IsBeingClicked() {
	if (ParentWindow == nullptr) {
		return false;
	}

	return BeingClicked;
}

bool RaaxGUI::Element::IsInElementBounds(const SDK::FVector2D& Position) {
	if (ParentWindow == nullptr) {
		return false;
	}

	switch (Type) {
	case ElementType::Checkbox:
		return reinterpret_cast<CheckboxElement*>(this)->IsInElementBoundsCheckbox(Position);
		break;
	}
}


/* CHECKBOX */
bool RaaxGUI::Checkbox(const char* Name, bool* Value) {
	Window* CurrentWindow = GetContext()->GetCurrentWindow();

	if (CurrentWindow == nullptr) {
		THROW_ERROR(skCrypt("Attempted to create a GUI element with no current window!").decrypt(), false);
		return false;
	}

	int Id = HashString(Name);

	CheckboxElement* ThisElement = FindElementByIdAndWindow<CheckboxElement>(Id, CurrentWindow);
	if (ThisElement == nullptr) {
		ThisElement = RegisterNewElement<CheckboxElement>(Id, CurrentWindow);
		ThisElement->ID = Id;

		ThisElement->Name = Name;
		ThisElement->Type = ElementType::Checkbox;

		// Checkbo exclusive data
		ThisElement->Value = Value;
	}
	else {
		CurrentWindow->Elements.push_back(ThisElement);
	}

	ThisElement->Seen = true;

	if (ThisElement->Type != ElementType::Checkbox) {
		THROW_ERROR(skCrypt("Found another element in the window with the same name! Please keep element and window names unique.").decrypt(), false);
		return false;
	}

	return *Value;
}
bool RaaxGUI::CheckboxElement::IsInElementBoundsCheckbox(const SDK::FVector2D& Position) {
	if (Position.X > CheckboxRelativePosition.X && Position.X < CheckboxRelativePosition.X + Style.CheckboxButtonSize.X) {
		if (Position.Y > CheckboxRelativePosition.Y && Position.Y < CheckboxRelativePosition.Y + Style.CheckboxButtonSize.Y) {
			return true;
		}
	}

	return false;
}
void RaaxGUI::CheckboxElement::OnClickBeginCheckbox() {
	BeingClicked = true;
}
void RaaxGUI::CheckboxElement::OnClickEndCheckbox() {
	BeingClicked = false;
	*Value = !*Value;
}
void RaaxGUI::CheckboxElement::DrawCheckbox() {
	WindowStyle& CurrentWindowStyle = ParentWindow->Style;
	ElementStyle& CurrentElementStyle = Style;

	// Draw checkbox
	SDK::FVector2D CheckboxPosition = ParentWindow->Position + ParentWindow->CurrentElementPosition;
	SDK::FVector2D CheckboxSize = Style.CheckboxButtonSize;

	CheckboxPosition = CheckboxPosition + Style.CheckboxPadding;
	CheckboxRelativePosition = CheckboxPosition - ParentWindow->Position;

	Drawing::FilledRect(CheckboxPosition, CheckboxSize, Style.CheckboxButtonColor, false);

	if (*Value) {
		Drawing::FilledRect(CheckboxPosition + (CheckboxSize / 4), CheckboxSize / 2, Style.CheckboxEnabledInnerBoxColor, false);
	}

	// Draw text
	SDK::FVector2D TextPosition = CheckboxPosition;
	TextPosition.X += CheckboxSize.X + Style.CheckboxTextOffset;

	Drawing::Text(Name.c_str(), TextPosition, Style.CheckboxTextSize, Style.CheckboxTextColor, false, false, false);
	
	// CheckboxPadding * 2 because of the padding above and below the checkbox
	ParentWindow->CurrentElementPosition.Y += Style.CheckboxButtonSize.Y + (Style.CheckboxPadding.Y * 2);
}