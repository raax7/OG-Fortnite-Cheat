#include "RaaxGUI.h"
#include "RaaxGUIInput.h"

#include "../../Utilities/Error.h"

// If you are using this library on another project, replace this with your own drawing wrappers (or recreate the ones here)
#include "../Drawing.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

/* CONTEXT FUNCTIONS */
void RaaxGUI::InitContext() {
	if (GContext) {
		THROW_ERROR(std::string(skCrypt("Context already initialized!")), false);
	}

	GContext = new RaaxGUIContext();

	if (GContext == nullptr) {
		THROW_ERROR(std::string(skCrypt("Failed to initialize context!")), true);
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
		THROW_ERROR(std::string(skCrypt("Context not initialized!")), false);
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

	return ThisWindow->Open;
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
		Drawing::FilledRect(Position, SDK::FVector2D(Size.X, Style.TitleBarSize.Y), SDK::FLinearColor(0.2f, 0.2f, 0.2f, 1.f), false);

		// Draw border
		Drawing::Rect(Position, Size, 1.f, SDK::FLinearColor(0.f, 0.f, 0.f, 1.f), true);

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

		Drawing::Text(Name.c_str(), TextPosition, Style.TitleBarTextSize, SDK::FLinearColor(1.f, 1.f, 1.f, 1.f), Style.TitleBarTextAlignment == TextAlignment::Center ? true : false, false, true);

		// Draw elements
		for (int i = 0; i < Elements.size(); i++) {
			Elements[i]->Draw();
		}
	}
}

void RaaxGUI::Window::OnClickTick(const SDK::FVector2D ClickPosition) {
	SDK::FVector2D RelativePosition = ClickPosition - Position;

	for (int i = 0; i < Elements.size(); i++) {
		Elements[i]->ClickTick(RelativePosition);
	}
}
void RaaxGUI::Window::OnClickBegin(const SDK::FVector2D ClickPosition) {
	SDK::FVector2D RelativePosition = ClickPosition - Position;

	GetContext()->MakeWindowTopMost(this);

	for (int i = 0; i < Elements.size(); i++) {
		Elements[i]->OnClickBegin(RelativePosition);
	}
}
void RaaxGUI::Window::OnClickEnd() {
	for (int i = 0; i < Elements.size(); i++) {
		Elements[i]->OnClickEnd();
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
	// Require atleast 10 pixels of the windows drag space to be visible

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
	ElementType* NewElement = new ElementType();
	NewElement->ID = ID;
	NewElement->ParentWindow = ElementWindow;

	ElementWindow->Elements.push_back(NewElement);

	return NewElement;
}
template<typename ElementType> ElementType* RaaxGUI::FindElementByIdAndWindow(int ID, Window* ElementWindow) {
	for (auto& Window : GetContext()->RenderQue.Windows) {
		if (Window == ElementWindow) {
			for (auto& Element : Window->ElementsLastFrame) {
				if (Element->ID == ID) {
					return reinterpret_cast<ElementType*>(Element);
				}
			}
		}
	}

	return nullptr;
}

bool RaaxGUI::Element::IsBeingClicked() {
	if (ParentWindow == nullptr) {
		return false;
	}

	return BeingClicked;
}
bool RaaxGUI::Element::IsBeingHovered() {
	if (ParentWindow == nullptr) {
		return false;
	}

	return BeingHovered;
}

bool RaaxGUI::Element::Text(const char* RenderText, SDK::FVector2D ScreenPosition, float FontSize, SDK::FLinearColor RenderColor, bool CenteredX, bool CenteredY, bool Outlined) {
	if (ParentWindow == nullptr) {
		return false;
	}

	// Get the bounds of the text
	SDK::FVector2D TextSize = Drawing::TextSize(RenderText, FontSize);

	// Adjust the position for CenteredX and CenteredY
	if (CenteredX) {
		ScreenPosition.X -= TextSize.X / 2;
	}

	if (CenteredY) {
		ScreenPosition.Y -= TextSize.Y / 2;
	}

	// Check if both corners of the text are within the bounds of the element
	if (ParentWindow->IsInMenuBounds(ScreenPosition) && ParentWindow->IsInMenuBounds(ScreenPosition + TextSize)) {
		Drawing::Text(RenderText, ScreenPosition, FontSize, RenderColor, CenteredX, CenteredY, Outlined);
		return true;
	}
	else {
		// Attempt to draw the text with 1 less character until it fits
		std::string NewText = RenderText;

		while (NewText.size() >= 1 && (ParentWindow->IsInMenuBounds(ScreenPosition) && ParentWindow->IsInMenuBounds(ScreenPosition + TextSize)) == false) {
			NewText.pop_back();

			std::string NewTextTemp = NewText;
			NewTextTemp += skCrypt("...");

			TextSize = Drawing::TextSize(NewTextTemp.c_str(), FontSize);
		}

		if (ParentWindow->IsInMenuBounds(ScreenPosition) && ParentWindow->IsInMenuBounds(ScreenPosition + TextSize)) {
			NewText += skCrypt("...");

			Drawing::Text(NewText.c_str(), ScreenPosition, FontSize, RenderColor, CenteredX, CenteredY, Outlined);
			return true;
		}
	}

	return false;
}
bool RaaxGUI::Element::FilledRect(SDK::FVector2D ScreenPosition, SDK::FVector2D ScreenSize, SDK::FLinearColor RenderColor, bool Outlined) {
	if (ParentWindow == nullptr) {
		return false;
	}

	// Check if both corners of the rect are within the bounds of the element
	if (ParentWindow->IsInMenuBounds(ScreenPosition) && ParentWindow->IsInMenuBounds(ScreenPosition + ScreenSize)) {
		Drawing::FilledRect(ScreenPosition, ScreenSize, RenderColor, Outlined);
		return true;
	}

	return false;
}


/* CHECKBOX */
bool RaaxGUI::Checkbox(const char* Name, bool* Value) {
	Window* CurrentWindow = GetContext()->GetCurrentWindow();

	if (CurrentWindow == nullptr) {
		THROW_ERROR(std::string(skCrypt("Attempted to create a GUI element with no current window!")), false);
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
		THROW_ERROR(std::string(skCrypt("Found another element in the window with the same name! Please keep element and window names unique.")), false);
		return false;
	}

	return *Value;
}
bool RaaxGUI::CheckboxElement::IsInElementBounds(const SDK::FVector2D Position) {
	if (Position.X > CheckboxRelativePosition.X && Position.X < CheckboxRelativePosition.X + Style.CheckboxButtonSize.X) {
		if (Position.Y > CheckboxRelativePosition.Y && Position.Y < CheckboxRelativePosition.Y + Style.CheckboxButtonSize.Y) {
			return true;
		}
	}

	return false;
}
void RaaxGUI::CheckboxElement::ClickTick(SDK::FVector2D Position) {
	return;
}
void RaaxGUI::CheckboxElement::OnClickBegin(const SDK::FVector2D ClickPosition) {
	if (IsInElementBounds(ClickPosition)) {
		BeingClicked = true;
	}
}
void RaaxGUI::CheckboxElement::OnClickEnd() {
	if (BeingClicked) {
		*Value = !*Value;
	}

	BeingClicked = false;
}
void RaaxGUI::CheckboxElement::Draw() {
	WindowStyle& CurrentWindowStyle = ParentWindow->Style;
	CheckboxStyle& CurrentElementStyle = Style;

	// Draw checkbox
	SDK::FVector2D CheckboxPosition = ParentWindow->Position + ParentWindow->CurrentElementPosition;
	SDK::FVector2D CheckboxSize = Style.CheckboxButtonSize;

	CheckboxPosition = CheckboxPosition + Style.CheckboxPadding;
	CheckboxRelativePosition = CheckboxPosition - ParentWindow->Position;

	SDK::FLinearColor CurrentCheckboxColor = Style.CheckboxButtonColor;
	if (IsBeingClicked()) {
		CurrentCheckboxColor = Style.CheckboxButtonClickHeldColor;
	}
	else if (IsBeingHovered()) {
		CurrentCheckboxColor = Style.CheckboxButtonHoverColor;
	}

	bool DrewCheckbox = FilledRect(CheckboxPosition, CheckboxSize, CurrentCheckboxColor, true);

	if (DrewCheckbox && *Value) {
		FilledRect(CheckboxPosition + (CheckboxSize / 4.f), CheckboxSize / 2.f, Style.CheckboxEnabledInnerBoxColor, false);
	}

	// Draw text
	SDK::FVector2D TextPosition = CheckboxPosition;
	TextPosition.X += CheckboxSize.X + Style.CheckboxTextOffset;

	if (DrewCheckbox) Text(Name.c_str(), TextPosition, Style.CheckboxTextSize, Style.CheckboxTextColor, false, false, true);
	
	// CheckboxPadding * 2 because of the padding above and below the checkbox
	ParentWindow->CurrentElementPosition.Y += Style.CheckboxButtonSize.Y + (Style.CheckboxPadding.Y * 2);

	BoundsPosition = CheckboxPosition;
	BoundsSize = CheckboxSize;
}



/* SLIDER */
template<typename SliderValueType> void CreateSliderInternal(const char* Name, SliderValueType* Value, SliderValueType MinValue, SliderValueType MaxValue) {
	RaaxGUI::Window* CurrentWindow = RaaxGUI::GetContext()->GetCurrentWindow();

	if (CurrentWindow == nullptr) {
		THROW_ERROR(std::string(skCrypt("Attempted to create a GUI element with no current window!")), false);
		return;
	}

	int Id = HashString(Name);

	RaaxGUI::SliderElement<SliderValueType>* ThisElement = RaaxGUI::FindElementByIdAndWindow<RaaxGUI::SliderElement<SliderValueType>>(Id, CurrentWindow);
	if (ThisElement == nullptr) {
		ThisElement = RaaxGUI::RegisterNewElement<RaaxGUI::SliderElement<SliderValueType>>(Id, CurrentWindow);
		ThisElement->ID = Id;

		ThisElement->Name = Name;
		ThisElement->Type = RaaxGUI::ElementType::Slider;

		// Slider exclusive data
		ThisElement->Value = Value;
		ThisElement->MinValue = MinValue;
		ThisElement->MaxValue = MaxValue;
	}
	else {
		CurrentWindow->Elements.push_back(ThisElement);
	}

	ThisElement->Seen = true;

	if (ThisElement->Type != RaaxGUI::ElementType::Slider) {
		THROW_ERROR(std::string(skCrypt("Found another element in the window with the same name! Please keep element and window names unique.")), false);
		return;
	}
}

void RaaxGUI::SliderFloat(const char* Name, float* Value, float MinValue, float MaxValue) {
	CreateSliderInternal<float>(Name, Value, MinValue, MaxValue);
}
void RaaxGUI::SliderInt(const char* Name, int* Value, int MinValue, int MaxValue) {
	CreateSliderInternal<int>(Name, Value, MinValue, MaxValue);
}

template<typename SliderValueType> bool RaaxGUI::SliderElement<SliderValueType>::IsInElementBounds(const SDK::FVector2D Position) {
	if (Position.X > BoundsPosition.X && Position.X < BoundsPosition.X + BoundsSize.X) {
		if (Position.Y > BoundsPosition.Y && Position.Y < BoundsPosition.Y + BoundsSize.Y) {
			return true;
		}
	}

	return false;
}
template<typename SliderValueType> void RaaxGUI::SliderElement<SliderValueType>::ClickTick(const SDK::FVector2D ClickPosition) {
	if (BeingClicked) {
		float CalculatedValue = (ClickPosition.X - BoundsPosition.X) / BoundsSize.X;
		CalculatedValue = std::clamp(CalculatedValue, 0.f, 1.f);

		*Value = (SliderValueType)((MaxValue - MinValue) * CalculatedValue + MinValue);
	}
}
template<typename SliderValueType> void RaaxGUI::SliderElement<SliderValueType>::OnClickBegin(const SDK::FVector2D ClickPosition) {
	if (IsInElementBounds(ClickPosition)) {
		float CalculatedValue = (ClickPosition.X - BoundsPosition.X) / BoundsSize.X;
		CalculatedValue = std::clamp(CalculatedValue, 0.f, 1.f);

		*Value = (SliderValueType)((MaxValue - MinValue) * CalculatedValue + MinValue);

		BeingClicked = true;
	}
}
template<typename SliderValueType> void RaaxGUI::SliderElement<SliderValueType>::OnClickEnd() {
	BeingClicked = false;
}
template<typename SliderValueType> void RaaxGUI::SliderElement<SliderValueType>::Draw() {
	WindowStyle& CurrentWindowStyle = ParentWindow->Style;
	SliderStyle& CurrentElementStyle = Style;

	// Setup initial positions and sizes
	SDK::FVector2D SliderPosition = ParentWindow->Position + ParentWindow->CurrentElementPosition + Style.SliderPadding;
	SDK::FVector2D SliderSize = Style.SliderSize;

	// Attempt to draw slider, adjusting size if necessary
	bool DrewSlider = false;
	for (; DrewSlider == false && SliderSize.X >= 1; SliderSize.X -= 1) {
		DrewSlider = FilledRect(SliderPosition, SliderSize, Style.SliderBarColor, true);
	}

	// Calculate the how far along the slider the value is
	float PercentComplete = ((float)*Value - (float)MinValue) / ((float)MaxValue - (float)MinValue);
	PercentComplete = std::clamp(PercentComplete, 0.f, 1.f);

	// Determine the size of the slider based on the current value
	SDK::FVector2D SliderValueSize = SDK::FVector2D(SliderSize.X * PercentComplete, SliderSize.Y);

	// Calculate grabber position and size
	SDK::FVector2D GrabberPosition = SDK::FVector2D(SliderPosition.X + (SliderSize.X * PercentComplete) - (Style.SliderGrabberWidth / 2), SliderPosition.Y);
	SDK::FVector2D GrabberSize = SDK::FVector2D(Style.SliderGrabberWidth, SliderSize.Y);

	// Ensure grabber remains within the slider bounds
	GrabberPosition.X = std::clamp(GrabberPosition.X, SliderPosition.X, SliderPosition.X + SliderSize.X - GrabberSize.X);

	// Formatting the value string with precision
	std::stringstream ValueStream;
	ValueStream << std::fixed << std::setprecision(2) << *Value;
	std::string ValueString = ValueStream.str();

	// Calculate text positions
	SDK::FVector2D NameTextPosition = SDK::FVector2D(SliderPosition.X + SliderSize.X + Style.SliderNameTextOffset, SliderPosition.Y);
	SDK::FVector2D ValueTextSize = Drawing::TextSize(ValueString.c_str(), Style.SliderValueTextSize);
	SDK::FVector2D ValueTextPosition = SDK::FVector2D(GrabberPosition.X - (GrabberSize.X / 2), GrabberPosition.Y + Style.SliderValueTextOffset + ValueTextSize.Y);

	// Drawing operations
	if (DrewSlider) {
		FilledRect(SliderPosition, SliderValueSize, Style.SliderValueBarColor, true);
		FilledRect(GrabberPosition, GrabberSize, Style.SliderGrabberButtonColor, true);
		Text(ValueString.c_str(), ValueTextPosition, Style.SliderValueTextSize, Style.SliderValueTextColor, false, false, true);
	}

	Text(Name.c_str(), NameTextPosition, Style.SliderNameTextSize, Style.SliderNameTextColor, false, false, true);

	// Update the position for the next element
	ParentWindow->CurrentElementPosition.Y += SliderSize.Y + Style.SliderPadding.Y + ValueTextSize.Y;

	// Set bounds for interaction detection
	BoundsPosition = SliderPosition - ParentWindow->Position;
	BoundsSize = SliderSize;
}