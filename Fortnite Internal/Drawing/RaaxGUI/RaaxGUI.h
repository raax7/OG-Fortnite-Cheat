#pragma once
#include <vector>

#include "../../Game/SDK/Classes/Basic.h"

typedef int RaaxGUIWindowFlags;

__forceinline int HashString(const wchar_t* String)
{
    int Hash = 0;

    for (int i = 0; i < wcslen(String); i++)
    {
        Hash += String[i];
    }

    return Hash;
}
__forceinline int HashString(const char* String)
{
    int Hash = 0;

    for (int i = 0; i < strlen(String); i++)
    {
        Hash += String[i];
    }

    return Hash;
}

namespace RaaxGUI
{
    /* FORWARD DECLARATIONS */
    class RaaxGUIContext;
    class Window;
    class Element;
    class CheckboxElement;
    template<typename SliderValueType> class SliderElement;
    struct RenderQue;
    struct RaaxGUIStyle;
    struct WindowStyle;
    struct ElementStyle;
    struct CheckboxStyle;
    struct SliderStyle;
    enum class ElementType;
    enum class TextAlignment;
    enum class ResizeDirection;



    /////////////////////////////////////////////////////////
    // FUNCTIONS
    /* Custom std::clamp as std::clamp loves to throw unneeded errors. */
    template<typename T> T Clamp(T Value, T Minimum, T Maximum) { if (Value < Minimum) { Value = Minimum; } if (Value > Maximum) { Value = Maximum; } return Value; }

    // INTERNAL API FUNCTIONS
    /* Initializes the global context. Should be the first RaaxGUI function called. */
    void InitContext();
    /* Returns the global context. */
    RaaxGUIContext* GetContext();

    /* Finds a window by its ID. Returns nullptr if not found. */
    Window* FindWindowById(const int Id);
    /* Creates and registers a new window. Returns the new window or the existing window if it already exists. */
    Window* RegisterNewWindow(const int Id);

    /* Finds an element by its ID and its parent window. Returns nullptr if not found. */
    template<typename ElementClass> ElementClass* FindElementByIdAndWindow(const int Id, const ElementType Type, Window* ElementWindow);
    /* Creates and registers a new element to a window. Returns the new element or the existing element if it already exists. */
    template<typename ElementClass> ElementClass* RegisterNewElement(const int Id, const ElementType Type, Window* ElementWindow);



    // PUBLIC API FUNCTIONS
    /* Begins a new frame. Must be called to BEFORE any window, element, etc operations are made. */
    void NewFrame();
    /* Ends the current frame. Must be called AFTER all window, element, etc operations are made. */
    void EndFrame();

    /* Sets the current style. */
    RaaxGUIStyle* GetStyle();

    /* Begins a new window. Returns whether the window is open. Must be called BEFORE all window specific operations. */
    bool BeginWindow(const char* Name, bool* Open, const RaaxGUIWindowFlags Flags, const SDK::FVector2D& InitPosition = SDK::FVector2D(), const SDK::FVector2D& InitSize = SDK::FVector2D());
    /* Ends the current window. Must be called AFTER all window specific operations are finished. */
    void EndWindow();

    /* Sets the next window size. */
    void SetNextWindowSize(const SDK::FVector2D& Size);
    /* Sets the next window position. */
    void SetNextWindowPos(const SDK::FVector2D& Pos);

    /* Draws a checkbox. Returns whether the checkbox is checked. */
    bool Checkbox(const char* Name, bool* Value);
    /* Draws a float based slider. Returns whether the slider has been changed this frame. */
    bool SliderFloat(const char* Name, float* Value, const float MinValue, const float MaxValue);
    /* Draws an int based slider. Returns whether the slider has been changed this frame. */
    bool SliderInt(const char* Name, int* Value, const int MinValue, const int MaxValue);
    /////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////
    // STRUCTS AND ENUMS
    /* Contains all the windows to be rendered. */
    struct RenderQue
    {
        std::vector<Window*> Windows;
    };

    /* Window Flags */
    enum RaaxGUIWindowFlags_
    {
        RaaxGUIWindowFlags_None = 0,
        RaaxGUIWindowFlags_NoTitleBar = 1 << 0,	// Disable title-bar
        RaaxGUIWindowFlags_NoResize = 1 << 1,	// Disable user resizing with the lower-right grip
        RaaxGUIWindowFlags_NoMove = 1 << 2,	// Disable user moving the window
        RaaxGUIWindowFlags_NoMainMove = 1 << 2,	// Disable user moving window by dragging on the main window
        RaaxGUIWindowFlags_NoTitleBarMove = 1 << 2,	// Disable user moving window by dragging on the title bar
    };

    /* Used to distinguish between different Element child classes */
    enum class ElementType
    {
        None = 0,
        Checkbox,
        Slider,
        Combo,
        Button,
        Seperator,
    };

    /* Text Alignment */
    enum class TextAlignment
    {
        Left = 0,
        Center,
        Right,
    };
    /* Resize Direction */
    enum class ResizeDirection
    {
        None = 0,
        Top,
        TopRight,
        Right,
        BottomRight,
        Bottom,
        BottomLeft,
        Left,
        TopLeft,
    };

    /* Checkbox Style */
    struct CheckboxStyle
    {
        SDK::FVector2D CheckboxButtonSize;					// The size of the checkbox button

        SDK::FLinearColor CheckboxButtonColor;				// The color of the checkbox button
        SDK::FLinearColor CheckboxButtonHoverColor;			// The color of the checkbox button when hovered
        SDK::FLinearColor CheckboxButtonClickHeldColor;		// The color of the checkbox button when the click is held

        SDK::FLinearColor CheckboxEnabledInnerBoxColor;		// The color of the inner box displayed when enabled

        SDK::FLinearColor CheckboxTextColor;				// The color of the text
        float CheckboxTextSize;								// The size of the text
        float CheckboxTextOffset;							// The offset of the text

        SDK::FVector2D CheckboxPadding;						// The padding of the checkbox
    };
    /* Slider Style */
    struct SliderStyle
    {
        SDK::FVector2D SliderSize;							// The size of the slider

        float SliderGrabberWidth;							// The size of the slider button
        SDK::FLinearColor SliderGrabberButtonColor;			// The color of the slider drag button
        SDK::FLinearColor SliderGrabberButtonHoverColor;	// The color of the slider drag button when hovered
        SDK::FLinearColor SliderGrabberButtonClickHeldColor;// The color of the slider drag button when the click is held

        SDK::FLinearColor SliderValueBarColor;				// The color of the slider value bar

        SDK::FLinearColor SliderBarColor;					// The color of the slider bar
        SDK::FLinearColor SliderBarHoverColor;				// The color of the slider bar when hovered
        SDK::FLinearColor SliderBarActiveColor;				// The color of the slider bar when active
        SDK::FLinearColor SliderBarClickHeldColor;			// The color of the slider bar when the click is held

        SDK::FLinearColor SliderValueTextColor;				// The color of the value text
        float SliderValueTextSize;							// The size of the value text
        float SliderValueTextOffset;						// The offset of the value text

        SDK::FLinearColor SliderNameTextColor;				// The color of the slider name text
        float SliderNameTextSize;							// The size of the slider name text
        float SliderNameTextOffset;							// The offset of the slider name text

        SDK::FVector2D SliderPadding;						// The padding of the slider
    };
    /* Element Style */
    struct ElementStyle
    {
        CheckboxStyle CheckboxStyle;						// The style of the checkbox
        SliderStyle SliderStyle;							// The style of the slider
    };
    /* Window Style */
    struct WindowStyle
    {
        SDK::FLinearColor BackgroundColor;

        // Title bar
        SDK::FVector2D TitleBarSize;
        SDK::FLinearColor TitleBarColor;
        SDK::FLinearColor TitleBarTextColor;
        TextAlignment TitleBarTextAlignment;
        bool AutoTitleBarTextSize;
        float TitleBarTextSize;
    };
    /* Contains all the default style information */
    struct RaaxGUIStyle
    {
        WindowStyle DefaultWindowStyle;
        ElementStyle DefaultElementStyle;

        void InitDarkStyle()
        {
            // Window
            DefaultWindowStyle.BackgroundColor = { 0.03f, 0.03f, 0.03f, 1.f }; // Darker grey background
            DefaultWindowStyle.TitleBarSize = { 0.f, 15.f };
            DefaultWindowStyle.TitleBarColor = { 0.1f, 0.1f, 0.1f, 1.f }; // Even darker grey for title bar
            DefaultWindowStyle.TitleBarTextColor = { 0.7f, 0.7f, 0.7f, 1.f }; // Dark grey for title text
            DefaultWindowStyle.TitleBarTextAlignment = TextAlignment::Center;
            DefaultWindowStyle.AutoTitleBarTextSize = true;
            DefaultWindowStyle.TitleBarTextSize = 16.f;

            // Checkbox
            DefaultElementStyle.CheckboxStyle.CheckboxButtonSize = SDK::FVector2D(15.f, 15.f);
            DefaultElementStyle.CheckboxStyle.CheckboxButtonColor = { 0.12f, 0.12f, 0.12f, 1.f }; // Darker grey
            DefaultElementStyle.CheckboxStyle.CheckboxButtonHoverColor = { 0.16f, 0.16f, 0.16f, 1.f }; // Dark grey hover
            DefaultElementStyle.CheckboxStyle.CheckboxButtonClickHeldColor = { 0.08f, 0.08f, 0.08f, 1.f }; // Darkest grey for click

            DefaultElementStyle.CheckboxStyle.CheckboxEnabledInnerBoxColor = { 0.0f, 0.6f, 0.7f, 1.f }; // Adjusted aqua for enabled state

            DefaultElementStyle.CheckboxStyle.CheckboxTextColor = { 0.7f, 0.7f, 0.7f, 1.f }; // Dark grey text
            DefaultElementStyle.CheckboxStyle.CheckboxTextSize = 14.f;
            DefaultElementStyle.CheckboxStyle.CheckboxTextOffset = 5.f;

            DefaultElementStyle.CheckboxStyle.CheckboxPadding = { 5.f, 5.f };

            // Slider
            DefaultElementStyle.SliderStyle.SliderSize = SDK::FVector2D(200.f, 15.f);

            DefaultElementStyle.SliderStyle.SliderGrabberWidth = 8.f;
            DefaultElementStyle.SliderStyle.SliderGrabberButtonColor = { 0.12f, 0.12f, 0.18f, 1.f }; // Dark grey with a hint of blue
            DefaultElementStyle.SliderStyle.SliderGrabberButtonHoverColor = { 0.0f, 0.6f, 0.7f, 1.f }; // Aqua for hover
            DefaultElementStyle.SliderStyle.SliderGrabberButtonClickHeldColor = { 0.0f, 0.4f, 0.5f, 1.f }; // Darker aqua for click

            DefaultElementStyle.SliderStyle.SliderValueBarColor = { 0.0f, 0.6f, 0.7f, 1.f }; // Adjusted aqua for value bar

            DefaultElementStyle.SliderStyle.SliderValueTextColor = { 0.7f, 0.7f, 0.7f, 1.f }; // Dark grey for value text
            DefaultElementStyle.SliderStyle.SliderValueTextSize = 10.f;
            DefaultElementStyle.SliderStyle.SliderValueTextOffset = 2.f;

            DefaultElementStyle.SliderStyle.SliderBarColor = { 0.12f, 0.12f, 0.12f, 1.f }; // Darker grey
            DefaultElementStyle.SliderStyle.SliderBarHoverColor = { 0.16f, 0.16f, 0.16f, 1.f }; // Dark grey hover
            DefaultElementStyle.SliderStyle.SliderBarActiveColor = { 0.20f, 0.20f, 0.20f, 1.f }; // Even darker grey for active
            DefaultElementStyle.SliderStyle.SliderBarClickHeldColor = { 0.24f, 0.24f, 0.24f, 1.f }; // Darkest grey for click held

            DefaultElementStyle.SliderStyle.SliderNameTextColor = { 0.7f, 0.7f, 0.7f, 1.f }; // Dark grey for the slider name text
            DefaultElementStyle.SliderStyle.SliderNameTextSize = 12.f;
            DefaultElementStyle.SliderStyle.SliderNameTextOffset = 5.f;

            DefaultElementStyle.SliderStyle.SliderPadding = { 5.f, 5.f };
        }
    };
    /////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////
    // CLASSES
    /* Contains global info about the RaaxGUI state, default styles, render que, etc. */
    class RaaxGUIContext
    {
    public:
        virtual ~RaaxGUIContext() {}

        /* Registers a window to the render que. Does not check if the window already exists. */
        void RegisterWindow(Window* Window);
        /* Pushes a window to the front of the render que. */
        void MakeWindowTopMost(Window* Window);
        /* Returns the window that is currently being drawn. */
        Window* GetCurrentWindow();

    public:
        /* Style */
        RaaxGUIStyle Style;				// The default styles.

        /* SetNext Info */
        bool UseNextWindowSize;			// Whether to use the next window size.
        SDK::FVector2D NextWindowSize;	// The next window size.
        bool UseNextWindowPos;			// Whether to use the next window position.
        SDK::FVector2D NextWindowPos;	// The next window position.

        /* Render Que */
        RenderQue RenderQue;			// The current render que. (windows, elements, etc)

        /* Window Management */
        Window* CurrentWindow;			// The current window being drawn.

    };

    /* A window that contains elements. */
    class Window
    {
    public:
        /* Draws the window and it's elements. */
        void Draw();

        /* Returns the evaluated resize direction based off the mouse position. */
        ResizeDirection GetResizeDirection(const SDK::FVector2D& MousePosition);

        /* Corrects a window position to one that matches the windows settings. */
        void FixWindowPosition(SDK::FVector2D& Position);
        /* Corrects a window size to one that matches the windows settings. */
        void FixWindowSize(SDK::FVector2D& Size);

        /* Returns wether a position is inside any resize bound. */
        bool IsInResizeBounds(SDK::FVector2D& Position);
        /* Returns wether a position is inside any drag bound. */
        bool IsInDragBounds(const SDK::FVector2D& Position);
        /* Returns wether a position is inside the menu bounds. */
        bool IsInMenuBounds(const SDK::FVector2D& Position);

        /* Called when the mouse begins clicking the window. */
        void OnClickBegin(const SDK::FVector2D& ClickPosition);
        /* Called when the mouse ends clicking the window. */
        void OnClickEnd();

        /* Called every time there is a new frame to adjust frame specific data. */
        void NewFrame();

    public:
        Window()
        {
            Open = false;
            Id = 0;
            Name = "";

            Style = GetContext()->Style.DefaultWindowStyle;

            Position = SDK::FVector2D();
            Size = SDK::FVector2D();
            MinimumSize = SDK::FVector2D(50, 50);

            CurrentElementPosition = SDK::FVector2D();

            CurrentResizeDirection = ResizeDirection::None;

            SeenThisFrame = false;
            ClickedLastFrame = false;
            ClickedThisFrame = false;

            Elements = std::vector<Element*>();
            ElementsLastFrame = std::vector<Element*>();
        }
        virtual ~Window()
        {
            for (int i = 0; i < Elements.size(); i++)
            {
                if (Elements[i] != nullptr)
                {
                    delete Elements[i];
                }
            }
            Elements.clear();

            for (int i = 0; i < ElementsLastFrame.size(); i++)
            {
                if (ElementsLastFrame[i] != nullptr)
                {
                    delete ElementsLastFrame[i];
                }
            }
            ElementsLastFrame.clear();
        }

    public:
        /* Window Info */
        bool Open;							// Whether the window is open.
        int Id;								// The window's ID. (hashed from the name. used for differentiating between windows)
        std::string Name;					// The window's name.

        /* Style */
        WindowStyle Style;					// The window's style.

        /* Window Size and Position Info */
        SDK::FVector2D Position;			// The window's position.
        SDK::FVector2D Size;				// The window's size.
        SDK::FVector2D MinimumSize;			// The window's minimum size.

        /* Element Info */
        SDK::FVector2D CurrentElementPosition;	// The position of the current element being drawn.

        /* Resize Info */
        ResizeDirection CurrentResizeDirection; // The current resize direction

        /* Frame Specific Flags*/
        bool SeenThisFrame;					// Whether the BeginWindow function has been called yet this frame. (used for garbage collection)
        bool ClickedLastFrame;				// Whether the window was clicked last frame.
        bool ClickedThisFrame;				// Whether the window was clicked this frame.

        /* Elements */
        std::vector<Element*> Elements;			// All the elements in the window
        std::vector<Element*> ElementsLastFrame;// All the elements rendered last frame

    };

    /* A base class for all elements. */
    class Element
    {
    public:
        /* Draws the element. (Override) */
        virtual void Draw() = 0;

        /* Called when the mouse begins clicking the element. (Override) */
        virtual void OnClickBegin(const SDK::FVector2D& ClickPosition) = 0;
        /* Called every frame the mouse is clicking the element. (Override) */
        virtual void ClickTick(const SDK::FVector2D& ClickPosition) = 0;
        /* Called when the mouse ends clicking the element. (Override) */
        virtual void OnClickEnd() = 0;

        /* Returns whether a position is inside the element bounds. (Override) */
        virtual bool IsInElementBounds(const SDK::FVector2D& Position) = 0;

        /* Returns whether the element is being clicked. */
        bool IsBeingClicked();

        /* Wrapper to safely draw text only inside the parent window bounds. */
        bool Text(const char* RenderText, SDK::FVector2D& ScreenPosition, const float FontSize, const SDK::FLinearColor& RenderColor, const bool CenteredX, const bool CenteredY, const bool Outlined);
        /* Wrapper to safely draw a filled rect only inside the parent window bounds. */
        bool FilledRect(const SDK::FVector2D& ScreenPosition, const SDK::FVector2D& ScreenSize, const SDK::FLinearColor& RenderColor, const bool Outlined);

        /* Called every time there is a new frame to adjust frame specific data. */
        void NewFrame();

    public:
        Element()
        {
            Type = ElementType::None;
            Id = 0;
            Name = "";

            ParentWindow = GetContext()->CurrentWindow;

            BoundsPosition = SDK::FVector2D();
            BoundsSize = SDK::FVector2D();

            BeingClicked = false;

            SeenThisFrame = false;
        }
        virtual ~Element() {}

    public:
        /* Element Info */
        ElementType Type;                   // The type of element.
        int Id;								// The element's ID. (hashed from the name. used for differentiating between elements)
        std::string Name;					// The element's name.

        /* Parent Window */
        Window* ParentWindow;				// The parent window of the element.

        /* Element Bounds */
        SDK::FVector2D BoundsPosition;		// The starting position of the element bounds.
        SDK::FVector2D BoundsSize;			// The size of the element bounds.

        /* Element Flags */
        bool BeingClicked;					// Whether the element is being clicked.

        /* Frame Specific Flags */
        bool SeenThisFrame;					// Whether the element has been drawn this frame.

    };

    /* A checkbox element. */
    class CheckboxElement : public Element
    {
    public:
        /* Draws the checkbox. */
        void Draw() override;

        /* Called when the mouse begins clicking the checkbox. */
        void OnClickBegin(const SDK::FVector2D& ClickPosition) override;
        /* Called every frame the mouse is clicking the checkbox. */
        void ClickTick(const SDK::FVector2D& ClickPosition) override;
        /* Called when the mouse ends clicking the checkbox. */
        void OnClickEnd() override;

        /* Returns whether a position is inside the checkbox bounds. */
        bool IsInElementBounds(const SDK::FVector2D& Position) override;

    public:
        CheckboxElement()
        {
            Type = ElementType::Checkbox;

            Style = GetContext()->Style.DefaultElementStyle.CheckboxStyle;

            Value = nullptr;

            CheckboxRelativePosition = SDK::FVector2D();
        }

    public:
        /* Style */
        CheckboxStyle Style;				// The style of the checkbox.

        /* Value */
        bool* Value;						// A pointer to the value of the checkbox.

        /* Position */
        SDK::FVector2D CheckboxRelativePosition; // The relative position of the checkbox to the parent window.

    };

    /* A slider element. */
    template<typename SliderValueType> class SliderElement : public Element
    {
    public:
        /* Draws the slider. */
        void Draw() override;

        /* Called when the mouse begins clicking the slider. */
        void OnClickBegin(const SDK::FVector2D& ClickPosition) override;
        /* Called every frame the mouse is clicking the slider. */
        void ClickTick(const SDK::FVector2D& ClickPosition) override;
        /* Called when the mouse ends clicking the slider. */
        void OnClickEnd() override;

        /* Returns whether a position is inside the slider bounds. */
        bool IsInElementBounds(const SDK::FVector2D& Position) override;

        /* Handles updating the value based on the clicked position */
        void HandleSliderBarClick(const SDK::FVector2D& ClickPosition);

    public:
        SliderElement()
        {
            Type = ElementType::Slider;

            Style = GetContext()->Style.DefaultElementStyle.SliderStyle;

            Value = nullptr;
            MinValue = SliderValueType();
            MaxValue = SliderValueType();

            SliderRelativePosition = SDK::FVector2D();
        }

    public:
        /* Style */
        SliderStyle Style;					// The style of the slider.

        /* Value */
        SliderValueType* Value;				// A pointer to the value of the slider. (type is defined by the template)
        SliderValueType MinValue;			// The minimum value of the slider.
        SliderValueType MaxValue;			// The maximum value of the slider.

        /* Position */
        SDK::FVector2D SliderRelativePosition; // The relative position of the slider to the parent window.

    };
}

inline RaaxGUI::RaaxGUIContext* GContext;