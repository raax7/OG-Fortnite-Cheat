#include "../../Game/Input/Input.h"
#include "RaaxGUI.h"
#include "RaaxGUIInput.h"

void RaaxGUIInput::NotifyDanglingPointer(RaaxGUI::Window* Window)
{
    if (ResizingWindow == Window) ClearResizingWindow(true);
    if (DraggingWindow == Window) ClearDraggingWindow(true);
}
void RaaxGUIInput::NotifyDanglingPointer(RaaxGUI::Element* Element)
{
    if (ClickingElement == Element) ClearClickedElement(true);
}

void RaaxGUIInput::SetResizingWindow(RaaxGUI::Window* Window)
{
    Window->OnClickBegin(MousePosition);

    RaaxGUIInput::ResizingWindow = Window;
    RaaxGUIInput::ResizingWindowOffset = RaaxGUIInput::MousePosition - Window->Position;
    RaaxGUIInput::ResizingWindowOriginal = Window->Position;
    RaaxGUIInput::ResizingWindowSize = Window->Size;
}
void RaaxGUIInput::TickResizingWindow()
{
    SDK::FVector2D PotentialNewPosition = SDK::FVector2D();
    SDK::FVector2D PotentialNewSize = SDK::FVector2D();

    switch (ResizingWindow->CurrentResizeDirection)
    {
        // TODO: Fix resizing issues when moving the mouuse fast
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
void RaaxGUIInput::ClearResizingWindow(const bool DanglingPointer)
{
    if (DanglingPointer == false)
    {
        ResizingWindow->OnClickEnd();
    }

    RaaxGUIInput::ResizingWindow = nullptr;
    RaaxGUIInput::ResizingWindowOffset = SDK::FVector2D();
    RaaxGUIInput::ResizingWindowOriginal = SDK::FVector2D();
    RaaxGUIInput::ResizingWindowSize = SDK::FVector2D();
}

void RaaxGUIInput::SetDraggingWindow(RaaxGUI::Window* Window)
{
    Window->OnClickBegin(MousePosition);

    RaaxGUIInput::DraggingWindow = Window;
    RaaxGUIInput::DraggingWindowOffset = RaaxGUIInput::MousePosition - Window->Position;
    RaaxGUIInput::DraggingWindowOriginal = Window->Position;
    RaaxGUIInput::DraggingWindowPosition = Window->Position;
}
void RaaxGUIInput::TickDraggingWindow()
{
    SDK::FVector2D PotentialNewPosition = MousePosition - DraggingWindowOffset;
    DraggingWindow->FixWindowPosition(PotentialNewPosition);

    DraggingWindow->Position = PotentialNewPosition;
    DraggingWindowPosition = PotentialNewPosition;
}
void RaaxGUIInput::ClearDraggingWindow(const bool DanglingPointer)
{
    if (DanglingPointer == false)
    {
        DraggingWindow->OnClickEnd();
    }

    RaaxGUIInput::DraggingWindow = nullptr;
    RaaxGUIInput::DraggingWindowOffset = SDK::FVector2D();
    RaaxGUIInput::DraggingWindowOriginal = SDK::FVector2D();
    RaaxGUIInput::DraggingWindowPosition = SDK::FVector2D();
}

void RaaxGUIInput::SetClickedElement(RaaxGUI::Element* Element)
{
    Element->OnClickBegin(MousePosition);

    ClickingElement = Element;
}
void RaaxGUIInput::TickClickingElement()
{
    if (ClickingElement->ParentWindow->Open == false)
    {
        ClearClickedElement(false);
        return;
    }

    ClickingElement->ClickTick(MousePosition);
}
void RaaxGUIInput::ClearClickedElement(const bool DanglingPointer)
{
    if (DanglingPointer == false)
    {
        ClickingElement->OnClickEnd();
    }

    ClickingElement = nullptr;
}

bool RaaxGUIInput::CanClickNewWindow()
{
    if (DraggingWindow) return false;
    if (ResizingWindow) return false;
    if (ClickingElement) return false;

    return true;
}
bool RaaxGUIInput::CanClickNewElement()
{
    if (DraggingWindow) return false;
    if (ResizingWindow) return false;
    if (ClickingElement) return false;

    return true;
}

RaaxGUIInput::CollisionTraceData RaaxGUIInput::MouseCollisionTrace(const SDK::FVector2D& MousePosition)
{
    CollisionTraceData TraceData{};

    // Reverse the windows so the top most window is checked first
    std::vector ReverseWindowsTemp = RaaxGUI::GetContext()->RenderQue.Windows;
    std::reverse(ReverseWindowsTemp.begin(), ReverseWindowsTemp.end());

    for (auto Window : ReverseWindowsTemp)
    {
        if (Window->Open)
        {
            if (Window->IsInMenuBounds(MousePosition))
            {
                TraceData.CollidedWindow = Window;

                for (auto Element : Window->Elements)
                {
                    if (Element->IsInElementBounds(MousePosition))
                    {
                        TraceData.CollidedElement = Element;

                        break;
                    }
                }

                break;
            }
        }
    }

    return TraceData;
}

void RaaxGUIInput::Tick()
{
    // Check for collisions with the mouse position
    MousePosition = Input::GetMousePosition();
    CollisionTraceData TraceData = MouseCollisionTrace(MousePosition);

    // Check for input
    bool LMBDown = Input::IsKeyDown(Input::KeyName::LeftMouseButton);
    bool LMBJustPressed = Input::WasKeyJustPressed(Input::KeyName::LeftMouseButton);

    // If the left mouse button is down, handle clicking events. Otherwise, handle releasing events on any clicked objects.
    if (LMBDown)
    {
        // If the left mouse button was just pressed, check for new objects to click.
        if (LMBJustPressed)
        {
            // Elements take priority over windows, so check for elements first.
            if (TraceData.CollidedElement && CanClickNewElement())
            {
                SetClickedElement(TraceData.CollidedElement);
            }

            // If no elements were clicked, check for windows.
            else if (TraceData.CollidedWindow && CanClickNewWindow())
            {
                // Resizing takes priority over dragging, so check for resizing first.
                if (TraceData.CollidedWindow->IsInResizeBounds(MousePosition))
                {
                    SetResizingWindow(TraceData.CollidedWindow);
                }
                // If the position is not in the resize bounds, check for dragging.
                else if (TraceData.CollidedWindow->IsInMenuBounds(MousePosition))
                {
                    SetDraggingWindow(TraceData.CollidedWindow);
                }
            }
        }
        else
        {
            // If the left mouse button is down, but the left mouse button was not just pressed, process the clicked object.
            if (ClickingElement) TickClickingElement();
            if (ResizingWindow) TickResizingWindow();
            if (DraggingWindow) TickDraggingWindow();
        }
    }
    else
    {
        // If the left mouse button is not down, clear any clicked objects.
        if (ClickingElement) ClearClickedElement(false);
        if (ResizingWindow) ClearResizingWindow(false);
        if (DraggingWindow) ClearDraggingWindow(false);
    }
}