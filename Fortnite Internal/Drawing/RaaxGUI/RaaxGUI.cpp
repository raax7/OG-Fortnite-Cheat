#include "RaaxGUI.h"
#include "RaaxGUIInput.h"

#include "../../Utilities/Error.h"

// If you are using this library on another project, replace this with your own drawing wrappers (or recreate the ones here)
#include "../Drawing.h"

#include <iomanip>
#include <sstream>

// INTERNAL API

void RaaxGUI::InitContext()
{
    if (GContext)
    {
        THROW_ERROR(std::string(skCrypt("Context already initialized!")), false);
    }

    GContext = new RaaxGUIContext();

    if (GContext == nullptr)
    {
        THROW_ERROR(std::string(skCrypt("Failed to initialize context!")), true);
    }

    // Initialize style
    GContext->Style.InitDarkStyle();
}
RaaxGUI::RaaxGUIContext* RaaxGUI::GetContext()
{
    if (GContext == nullptr)
    {
        THROW_ERROR(std::string(skCrypt("Context not initialized!")), false);
    }

    return GContext;
}
RaaxGUI::Window* RaaxGUI::FindWindowById(const int Id)
{
    for (int i = 0; i < GetContext()->RenderQue.Windows.size(); i++)
    {
        if (GetContext()->RenderQue.Windows[i]->Id == Id)
        {
            return GetContext()->RenderQue.Windows[i];
        }
    }

    return nullptr;
}
RaaxGUI::Window* RaaxGUI::RegisterNewWindow(const int Id)
{
    // Check if window already exists
    for (int i = 0; i < GetContext()->RenderQue.Windows.size(); i++)
    {
        if (GetContext()->RenderQue.Windows[i]->Id == Id)
        {
            // If the window ID matches, return the window
            return GetContext()->RenderQue.Windows[i];
        }
    }

    // If the window doesn't exist, we can safely create a new one
    Window* NewWindow = new Window();
    NewWindow->Id = Id;

    // Register the window with the context
    GetContext()->RegisterWindow(NewWindow);

    return NewWindow;
}
template<typename ElementClass> ElementClass* RaaxGUI::FindElementByIdAndWindow(const int Id, const ElementType Type, Window* ElementWindow)
{
    for (auto& Window : GetContext()->RenderQue.Windows)
    {
        if (Window == ElementWindow)
        {
            for (auto& Element : Window->ElementsLastFrame)
            {
                if (Element->Id == Id && Element->Type == Type)
                {
                    // If the element ID and type matches, return the element
                    return reinterpret_cast<ElementClass*>(Element);
                }
            }
        }
    }

    return nullptr;
}
template<typename ElementClass> ElementClass* RaaxGUI::RegisterNewElement(int Id, ElementType Type, Window* ElementWindow)
{
    // Check if element already exists
    for (auto& Element : ElementWindow->ElementsLastFrame)
    {
        if (Element->Id == Id && Element->Type == Type)
        {
            // If the element ID and type matches, return the element
            return reinterpret_cast<ElementClass*>(Element);
        }
    }

    // If the element doesn't exist, we can safely create a new one
    ElementClass* NewElement = new ElementClass();
    NewElement->Id = Id;
    NewElement->ParentWindow = ElementWindow;

    // Register the element with the window
    ElementWindow->Elements.push_back(NewElement);

    return NewElement;
}


// PUBLIC API

void RaaxGUI::NewFrame()
{
    RaaxGUIInput::Tick();

    // Clear all window seen flags
    for (auto Window : GetContext()->RenderQue.Windows)
    {
        if (Window)
        {
            Window->NewFrame();

            for (auto Element : Window->Elements)
            {
                Element->NewFrame();
            }
        }
    }
}
void RaaxGUI::EndFrame()
{
    for (int i = 0; i < GetContext()->RenderQue.Windows.size(); i++)
    {
        Window* CurrentWindow = GetContext()->RenderQue.Windows[i];
        CurrentWindow->Draw();
    }

    // Loop thru all elements and clear elements that were not seen
    for (auto WindowIt = GetContext()->RenderQue.Windows.begin(); WindowIt != GetContext()->RenderQue.Windows.end();)
    {
        Window* CurrentWindow = *WindowIt;

        for (auto ElementIt = CurrentWindow->ElementsLastFrame.begin(); ElementIt != CurrentWindow->ElementsLastFrame.end();)
        {
            Element* CurrentElement = *ElementIt;

            if (CurrentElement->SeenThisFrame == false)
            {
                ElementIt = CurrentWindow->ElementsLastFrame.erase(ElementIt);
                delete CurrentElement;

                // Tell the input system to null the pointer (if it's being used)
                RaaxGUIInput::NotifyDanglingPointer(CurrentElement);
            }
            else
            {
                ++ElementIt;
            }
        }

        ++WindowIt;
    }

    // Clear all windows that were not seen
    for (auto WindowIt = GetContext()->RenderQue.Windows.begin(); WindowIt != GetContext()->RenderQue.Windows.end();)
    {
        Window* CurrentWindow = *WindowIt;

        if (CurrentWindow == nullptr)
        {
            WindowIt = GetContext()->RenderQue.Windows.erase(WindowIt);
            continue;
        }

        if (CurrentWindow->SeenThisFrame == false)
        {
            WindowIt = GetContext()->RenderQue.Windows.erase(WindowIt);
            delete CurrentWindow;

            // Tell the input system to null the pointer (if it's being used)
            RaaxGUIInput::NotifyDanglingPointer(CurrentWindow);
        }
        else
        {
            ++WindowIt;
        }
    }
}
RaaxGUI::RaaxGUIStyle* RaaxGUI::GetStyle()
{
    return &GetContext()->Style;
}

void RaaxGUI::SetNextWindowSize(const SDK::FVector2D& Size)
{
    GetContext()->UseNextWindowSize = true;
    GetContext()->NextWindowSize = Size;
}
void RaaxGUI::SetNextWindowPos(const SDK::FVector2D& Pos)
{
    GetContext()->UseNextWindowPos = true;
    GetContext()->NextWindowPos = Pos;
}


// RAAXGUICONTEXT

void RaaxGUI::RaaxGUIContext::RegisterWindow(Window* Window)
{
    RenderQue.Windows.push_back(Window);
}
void RaaxGUI::RaaxGUIContext::MakeWindowTopMost(Window* Window)
{
    for (int i = 0; i < RenderQue.Windows.size(); i++)
    {
        if (RenderQue.Windows[i] == Window)
        {
            RenderQue.Windows.erase(RenderQue.Windows.begin() + i);
            RenderQue.Windows.push_back(Window);

            return;
        }
    }
}
RaaxGUI::Window* RaaxGUI::RaaxGUIContext::GetCurrentWindow()
{
    return CurrentWindow;
}


// WINDOW

bool RaaxGUI::BeginWindow(const char* Name, bool* Open, const RaaxGUIWindowFlags Flags, const SDK::FVector2D& InitPosition, const SDK::FVector2D& InitSize)
{
    int Id = HashString(Name);

    Window* ThisWindow = FindWindowById(Id);
    if (ThisWindow == nullptr)
    {
        ThisWindow = RegisterNewWindow(Id);
        ThisWindow->Id = Id;

        ThisWindow->Size = InitSize;
        ThisWindow->Position = InitPosition;

        ThisWindow->Name = Name;
    }

    ThisWindow->SeenThisFrame = true;

    GetContext()->CurrentWindow = ThisWindow;

    if (GetContext()->UseNextWindowSize)
    {
        ThisWindow->Size = GetContext()->NextWindowSize;
        GetContext()->UseNextWindowSize = false;
    }
    if (GetContext()->UseNextWindowPos)
    {
        ThisWindow->Position = GetContext()->NextWindowPos;
        GetContext()->UseNextWindowPos = false;
    }

    ThisWindow->CurrentElementPosition = SDK::FVector2D(0, ThisWindow->Style.TitleBarSize.Y);

    ThisWindow->Open = *Open;

    return ThisWindow->Open;
}
void RaaxGUI::EndWindow()
{
    GetContext()->CurrentWindow = nullptr;
}
void RaaxGUI::Window::Draw()
{
    if (Open)
    {
        // Draw window
        Drawing::FilledRect(Position, Size, Style.BackgroundColor, false);

        // Draw title bar
        Drawing::FilledRect(Position, SDK::FVector2D(Size.X, Style.TitleBarSize.Y), SDK::FLinearColor(0.2f, 0.2f, 0.2f, 1.f), false);

        // Draw border
        Drawing::Rect(Position, Size, 1.f, SDK::FLinearColor(0.f, 0.f, 0.f, 1.f), true);

        SDK::FVector2D TextPosition = SDK::FVector2D();

        switch (Style.TitleBarTextAlignment)
        {
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
        for (int i = 0; i < Elements.size(); i++)
        {
            Elements[i]->Draw();
        }
    }
}
RaaxGUI::ResizeDirection RaaxGUI::Window::GetResizeDirection(const SDK::FVector2D& MousePosition)
{
    return CurrentResizeDirection;
}
void RaaxGUI::Window::FixWindowPosition(SDK::FVector2D& Position)
{
    // Clamp to screen
    // Require atleast 10 pixels of the windows drag space to be visible

}
void RaaxGUI::Window::FixWindowSize(SDK::FVector2D& Size)
{
    // Clamp to minimum size
    if (Size.X < MinimumSize.X)
    {
        Size.X = MinimumSize.X;
    }
    if (Size.Y < MinimumSize.Y)
    {
        Size.Y = MinimumSize.Y;
    }
}
bool RaaxGUI::Window::IsInResizeBounds(SDK::FVector2D& Position)
{
    // Check if mouse is within 2 pixels of any corner
    SDK::FVector2D TopLeft = this->Position;
    SDK::FVector2D TopRight = SDK::FVector2D(this->Position.X + this->Size.X, this->Position.Y);
    SDK::FVector2D BottomLeft = SDK::FVector2D(this->Position.X, this->Position.Y + this->Size.Y);
    SDK::FVector2D BottomRight = SDK::FVector2D(this->Position.X + this->Size.X, this->Position.Y + this->Size.Y);

    if (Position.Distance(TopLeft) < 10)
    {
        CurrentResizeDirection = ResizeDirection::TopLeft;
        return true;
    }
    else if (Position.Distance(TopRight) < 10)
    {
        CurrentResizeDirection = ResizeDirection::TopRight;
        return true;
    }
    else if (Position.Distance(BottomLeft) < 10)
    {
        CurrentResizeDirection = ResizeDirection::BottomLeft;
        return true;
    }
    else if (Position.Distance(BottomRight) < 10)
    {
        CurrentResizeDirection = ResizeDirection::BottomRight;
        return true;
    }

    return false;
}
bool RaaxGUI::Window::IsInDragBounds(const SDK::FVector2D& Position)
{
    return IsInMenuBounds(Position);
}
bool RaaxGUI::Window::IsInMenuBounds(const SDK::FVector2D& Position)
{
    if (Position.X > this->Position.X && Position.X < this->Position.X + this->Size.X)
    {
        if (Position.Y > this->Position.Y && Position.Y < this->Position.Y + this->Size.Y)
        {
            return true;
        }
    }

    return false;
}
void RaaxGUI::Window::OnClickBegin(const SDK::FVector2D& ClickPosition)
{
    ClickedThisFrame = true;

    GetContext()->MakeWindowTopMost(this);
}
void RaaxGUI::Window::OnClickEnd()
{

}
void RaaxGUI::Window::NewFrame()
{
    // Push the current frame flags to the last frame flags
    ClickedLastFrame = ClickedThisFrame;
    ClickedThisFrame = false;

    // Reset any flags that don't need to be carried over
    SeenThisFrame = false;

    // Move the elements from the current frame to the last frame (using a swap)
    std::swap(Elements, ElementsLastFrame);
    Elements.clear();
}


// ELEMENT

bool RaaxGUI::Element::IsBeingClicked()
{
    if (ParentWindow == nullptr)
    {
        return false;
    }

    return BeingClicked;
}
bool RaaxGUI::Element::Text(const char* RenderText, SDK::FVector2D& ScreenPosition, const float FontSize, const SDK::FLinearColor& RenderColor, const bool CenteredX, const bool CenteredY, const bool Outlined)
{
    if (ParentWindow == nullptr)
    {
        return false;
    }

    // Get the bounds of the text
    SDK::FVector2D TextSize = Drawing::TextSize(RenderText, FontSize);

    // Adjust the position for CenteredX and CenteredY
    if (CenteredX)
    {
        ScreenPosition.X -= TextSize.X / 2;
    }

    if (CenteredY)
    {
        ScreenPosition.Y -= TextSize.Y / 2;
    }

    Drawing::Text(RenderText, ScreenPosition, FontSize, RenderColor, CenteredX, CenteredY, Outlined);
    return true;

#if 0
    // Check if both corners of the text are within the bounds of the element
    if (ParentWindow->IsInMenuBounds(ScreenPosition + TextSize))
    {
        Drawing::Text(RenderText, ScreenPosition, FontSize, RenderColor, CenteredX, CenteredY, Outlined);
        return true;
    }
    else
    {
        // Attempt to draw the text with 1 less character until it fits
        std::string NewText = RenderText;

        while (NewText.size() >= 1 && (ParentWindow->IsInMenuBounds(ScreenPosition + TextSize)) == false)
        {
            NewText.pop_back();

            std::string NewTextTemp = NewText;
            NewTextTemp += skCrypt("...");

            TextSize = Drawing::TextSize(NewTextTemp.c_str(), FontSize);
        }

        if (ParentWindow->IsInMenuBounds(ScreenPosition + TextSize))
        {
            NewText += skCrypt("...");

            Drawing::Text(NewText.c_str(), ScreenPosition, FontSize, RenderColor, CenteredX, CenteredY, Outlined);
            return true;
        }
}

    return false;
#endif
}
bool RaaxGUI::Element::FilledRect(const SDK::FVector2D& ScreenPosition, const SDK::FVector2D& ScreenSize, const SDK::FLinearColor& RenderColor, const bool Outlined)
{
    if (ParentWindow == nullptr)
    {
        return false;
    }

    // Check if both corners of the rect are within the bounds of the element
    if (ParentWindow->IsInMenuBounds(ScreenPosition + ScreenSize))
    {
        Drawing::FilledRect(ScreenPosition, ScreenSize, RenderColor, Outlined);
        return true;
    }

    return false;
}
void RaaxGUI::Element::NewFrame()
{
    SeenThisFrame = false;
}


// CHECKBOX

bool RaaxGUI::Checkbox(const char* Name, bool* Value)
{
    Window* CurrentWindow = GetContext()->GetCurrentWindow();

    if (CurrentWindow == nullptr)
    {
        THROW_ERROR(std::string(skCrypt("Attempted to create a GUI element with no current window!")), false);
        return false;
    }

    int Id = HashString(Name);

    CheckboxElement* ThisElement = FindElementByIdAndWindow<CheckboxElement>(Id, ElementType::Checkbox, CurrentWindow);
    if (ThisElement == nullptr)
    {
        ThisElement = RegisterNewElement<CheckboxElement>(Id, ElementType::Checkbox, CurrentWindow);
        ThisElement->Id = Id;

        ThisElement->Name = Name;
        ThisElement->Type = ElementType::Checkbox;

        // Checkbo exclusive data
        ThisElement->Value = Value;
    }
    else
    {
        CurrentWindow->Elements.push_back(ThisElement);
    }

    ThisElement->SeenThisFrame = true;

    if (ThisElement->Type != ElementType::Checkbox)
    {
        THROW_ERROR(std::string(skCrypt("Found another element in the window with the same name! Please keep element and window names unique.")), false);
        return false;
    }

    return *Value;
}
void RaaxGUI::CheckboxElement::Draw()
{
    WindowStyle& CurrentWindowStyle = ParentWindow->Style;
    CheckboxStyle& CurrentElementStyle = Style;

    // Draw checkbox
    SDK::FVector2D CheckboxPosition = ParentWindow->Position + ParentWindow->CurrentElementPosition;
    SDK::FVector2D CheckboxSize = Style.CheckboxButtonSize;

    CheckboxPosition = CheckboxPosition + Style.CheckboxPadding;
    CheckboxRelativePosition = CheckboxPosition - ParentWindow->Position;

    SDK::FLinearColor CurrentCheckboxColor = Style.CheckboxButtonColor;
    if (IsBeingClicked())
    {
        CurrentCheckboxColor = Style.CheckboxButtonClickHeldColor;
    }

    bool DrewCheckbox = FilledRect(CheckboxPosition, CheckboxSize, CurrentCheckboxColor, true);

    if (DrewCheckbox && *Value)
    {
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
void RaaxGUI::CheckboxElement::OnClickBegin(const SDK::FVector2D& ClickPosition)
{
    GetContext()->MakeWindowTopMost(ParentWindow);

    BeingClicked = true;
}
void RaaxGUI::CheckboxElement::ClickTick(const SDK::FVector2D& Position)
{

}
void RaaxGUI::CheckboxElement::OnClickEnd()
{
    *Value = !*Value;

    BeingClicked = false;
}
bool RaaxGUI::CheckboxElement::IsInElementBounds(const SDK::FVector2D& Position)
{
    if (Position.X > BoundsPosition.X && Position.X < BoundsPosition.X + BoundsSize.X)
    {
        if (Position.Y > BoundsPosition.Y && Position.Y < BoundsPosition.Y + BoundsSize.Y)
        {
            return true;
        }
    }

    return false;
}


// SLIDER

template<typename SliderValueType> bool CreateSliderInternal(const char* Name, SliderValueType* Value, SliderValueType MinValue, SliderValueType MaxValue)
{
    RaaxGUI::Window* CurrentWindow = RaaxGUI::GetContext()->GetCurrentWindow();

    if (CurrentWindow == nullptr)
    {
        THROW_ERROR(std::string(skCrypt("Attempted to create a GUI element with no current window!")), false);
        return false;
    }

    int Id = HashString(Name);

    RaaxGUI::SliderElement<SliderValueType>* ThisElement = RaaxGUI::FindElementByIdAndWindow<RaaxGUI::SliderElement<SliderValueType>>(Id, RaaxGUI::ElementType::Slider, CurrentWindow);
    if (ThisElement == nullptr)
    {
        ThisElement = RaaxGUI::RegisterNewElement<RaaxGUI::SliderElement<SliderValueType>>(Id, RaaxGUI::ElementType::Slider, CurrentWindow);
        ThisElement->Id = Id;

        ThisElement->Name = Name;
        ThisElement->Type = RaaxGUI::ElementType::Slider;

        // Slider exclusive data
        ThisElement->Value = Value;
        ThisElement->MinValue = MinValue;
        ThisElement->MaxValue = MaxValue;
    }
    else
    {
        CurrentWindow->Elements.push_back(ThisElement);
    }

    ThisElement->SeenThisFrame = true;

    if (ThisElement->Type != RaaxGUI::ElementType::Slider)
    {
        THROW_ERROR(std::string(skCrypt("Found another element in the window with the same name! Please keep element and window names unique.")), false);
        return false;
    }

    return ThisElement->BeingClicked;
}

bool RaaxGUI::SliderFloat(const char* Name, float* Value, const float MinValue, const float MaxValue)
{
    return CreateSliderInternal<float>(Name, Value, MinValue, MaxValue);
}
bool RaaxGUI::SliderInt(const char* Name, int* Value, const int MinValue, const int MaxValue)
{
    return CreateSliderInternal<int>(Name, Value, MinValue, MaxValue);
}
template<typename SliderValueType> void RaaxGUI::SliderElement<SliderValueType>::Draw()
{
    WindowStyle& CurrentWindowStyle = ParentWindow->Style;
    SliderStyle& CurrentElementStyle = Style;

    // Setup initial positions and sizes
    SDK::FVector2D SliderPosition = ParentWindow->Position + ParentWindow->CurrentElementPosition + Style.SliderPadding;
    SDK::FVector2D SliderSize = Style.SliderSize;

    // Attempt to draw slider, incrementally decreasing the size until it fits
    bool DrewSlider = false;
    for (; DrewSlider == false && SliderSize.X >= 1; SliderSize.X -= 1)
    {
        DrewSlider = FilledRect(SliderPosition, SliderSize, Style.SliderBarColor, true);
    }

    // Calculate the how far along the slider the value is
    float PercentComplete = ((float)*Value - (float)MinValue) / ((float)MaxValue - (float)MinValue);
    PercentComplete = Clamp<float>(PercentComplete, 0.f, 1.f);

    // Determine the size of the slider based on the current value
    SDK::FVector2D SliderValueSize = SDK::FVector2D(SliderSize.X * PercentComplete, SliderSize.Y);

    // Calculate grabber position and size
    SDK::FVector2D GrabberPosition = SDK::FVector2D(SliderPosition.X + (SliderSize.X * PercentComplete) - (Style.SliderGrabberWidth / 2), SliderPosition.Y);
    SDK::FVector2D GrabberSize = SDK::FVector2D(Style.SliderGrabberWidth, SliderSize.Y);

    // Ensure grabber remains within the slider bounds
    GrabberPosition.X = Clamp<float>(GrabberPosition.X, SliderPosition.X, SliderPosition.X + SliderSize.X - GrabberSize.X);

    // Formatting the value string to only show 2 decimal places
    std::string ValueString = std::to_string(*Value);

    // Calculate text positions
    SDK::FVector2D NameTextPosition = SDK::FVector2D(SliderPosition.X + SliderSize.X + Style.SliderNameTextOffset, SliderPosition.Y);
    SDK::FVector2D ValueTextSize = Drawing::TextSize(ValueString.c_str(), Style.SliderValueTextSize);
    SDK::FVector2D ValueTextPosition = SDK::FVector2D(GrabberPosition.X - (GrabberSize.X / 2), GrabberPosition.Y + Style.SliderValueTextOffset + ValueTextSize.Y);

    // Drawing operations
    if (DrewSlider)
    {
        FilledRect(SliderPosition, SliderValueSize, Style.SliderValueBarColor, true);
        FilledRect(GrabberPosition, GrabberSize, Style.SliderGrabberButtonColor, true);
        Text(ValueString.c_str(), ValueTextPosition, Style.SliderValueTextSize, Style.SliderValueTextColor, false, false, true);
    }

    Text(Name.c_str(), NameTextPosition, Style.SliderNameTextSize, Style.SliderNameTextColor, false, false, true);

    // Update the position for the next element
    ParentWindow->CurrentElementPosition.Y += SliderSize.Y + Style.SliderPadding.Y + ValueTextSize.Y;

    // Set bounds for interaction detection
    BoundsPosition = SliderPosition;
    BoundsSize = SliderSize;
}
template<typename SliderValueType> void RaaxGUI::SliderElement<SliderValueType>::OnClickBegin(const SDK::FVector2D& ClickPosition)
{
    GetContext()->MakeWindowTopMost(ParentWindow);
    HandleSliderBarClick(ClickPosition);

    BeingClicked = true;
}
template<typename SliderValueType> void RaaxGUI::SliderElement<SliderValueType>::ClickTick(const SDK::FVector2D& ClickPosition)
{
    HandleSliderBarClick(ClickPosition);
}
template<typename SliderValueType> void RaaxGUI::SliderElement<SliderValueType>::OnClickEnd()
{
    BeingClicked = false;
}
template<typename SliderValueType> bool RaaxGUI::SliderElement<SliderValueType>::IsInElementBounds(const SDK::FVector2D& Position)
{
    if (Position.X > BoundsPosition.X && Position.X < BoundsPosition.X + BoundsSize.X)
    {
        if (Position.Y > BoundsPosition.Y && Position.Y < BoundsPosition.Y + BoundsSize.Y)
        {
            return true;
        }
    }

    return false;
}
template<typename SliderValueType> void RaaxGUI::SliderElement<SliderValueType>::HandleSliderBarClick(const SDK::FVector2D& ClickPosition)
{
    float CalculatedValue = (ClickPosition.X - BoundsPosition.X) / BoundsSize.X;
    CalculatedValue = Clamp<float>(CalculatedValue, 0.f, 1.f);

    *Value = (SliderValueType)((MaxValue - MinValue) * CalculatedValue + MinValue);
}