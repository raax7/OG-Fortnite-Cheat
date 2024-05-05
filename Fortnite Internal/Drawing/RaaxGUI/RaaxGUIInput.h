#pragma once
#include "RaaxGUI.h"

#include "../../Game/SDK/Classes/Basic.h"

namespace RaaxGUIInput
{
    struct CollisionTraceData
    {
        RaaxGUI::Window* CollidedWindow = nullptr;		// A pointer to the collided window, nullptr if no collision.
        RaaxGUI::Element* CollidedElement = nullptr;	// A pointer to the collided element (will always be in the collided window), nullptr if no collision.
    };

    inline SDK::FVector2D MousePosition;			// The current mouse position.

    inline RaaxGUI::Element* ClickingElement;		// The element that is currently clicked.

    inline RaaxGUI::Window* ResizingWindow;			// The window that is currently being resized.
    inline SDK::FVector2D ResizingWindowOffset;
    inline SDK::FVector2D ResizingWindowOriginal;
    inline SDK::FVector2D ResizingWindowSize;

    inline RaaxGUI::Window* DraggingWindow;			// The window that is currently being dragged.
    inline SDK::FVector2D DraggingWindowOffset;
    inline SDK::FVector2D DraggingWindowOriginal;
    inline SDK::FVector2D DraggingWindowPosition;



    /* Called from the garbage collector in RaaxGUI to clear any dangling window pointers. */
    void NotifyDanglingPointer(RaaxGUI::Window* Window);
    /* Called from the garbage collector in RaaxGUI to clear any dangling element pointers. */
    void NotifyDanglingPointer(RaaxGUI::Element* Element);

    // Everything below is very self-explanatory.

    void SetResizingWindow(RaaxGUI::Window* Window);
    void TickResizingWindow();
    void ClearResizingWindow(const bool DanglingPointer);

    void SetDraggingWindow(RaaxGUI::Window* Window);
    void TickDraggingWindow();
    void ClearDraggingWindow(const bool DanglingPointer);

    void SetClickedElement(RaaxGUI::Element* Element);
    void TickClickingElement();
    void ClearClickedElement(const bool DanglingPointer);

    bool CanClickNewWindow();
    bool CanClickNewElement();

    /* Returns a CollisionTraceData struct containing information about the collision. */
    CollisionTraceData MouseCollisionTrace(const SDK::FVector2D& MousePosition);

    /* Called every frame to update the input system. */
    void Tick();
};