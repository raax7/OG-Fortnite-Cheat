#pragma once
#include <vector>

#include "../../Game/SDK/Classes/Basic.h"

#ifndef RAAXGUI_API
#define RAAXGUI_API // Used for public functions
#endif
#ifndef RAAXGUI_INTERNAL_API
#define RAAXGUI_INTERNAL_API // Used for internal functions
#endif

inline int HashString(const wchar_t* String) {
	int Hash = 0;

	for (int i = 0; i < wcslen(String); i++) {
		Hash += String[i];
	}

	return Hash;
}
inline int HashString(const char* String) {
	int Hash = 0;

	for (int i = 0; i < strlen(String); i++) {
		Hash += String[i];
	}

	return Hash;
}

typedef int RaaxGUIWindowFlags;

namespace RaaxGUI {
	/* FUNCTIONS */
	/* INTERNAL API FUNCTIONS */

	RAAXGUI_INTERNAL_API void InitContext();															// Initializes the context
	RAAXGUI_INTERNAL_API class RaaxGUIContext* GetContext();											// Returns the current context

	RAAXGUI_INTERNAL_API class Window* FindWindowById(int ID);											// Finds a window by its ID
	RAAXGUI_INTERNAL_API class Window* RegisterNewWindow(int ID);										// Registers a new window

	template<typename ElementType>
	RAAXGUI_INTERNAL_API ElementType* FindElementByIdAndWindow(int ID, class Window* ElementWindow);	// Finds an element by its ID and its parent window
	template<typename ElementType>
	RAAXGUI_INTERNAL_API ElementType* RegisterNewElement(int ID, class Window* ElementWindow);			// Registers a new element to a window



	/* PUBLIC API FUNCTIONS */
	/* Windows */

	RAAXGUI_API bool BeginWindow(const char* Name, bool* Open, RaaxGUIWindowFlags Flags, SDK::FVector2D InitPosition = SDK::FVector2D(), SDK::FVector2D InitSize = SDK::FVector2D()); // Begins a new window
	RAAXGUI_API void EndWindow();									// Ends the current window

	RAAXGUI_API void SetNextWindowSize(SDK::FVector2D Size);		// Sets the next window size
	RAAXGUI_API void SetNextWindowPos(SDK::FVector2D Pos);			// Sets the next window position


	/* Rendering */

	RAAXGUI_API void NewFrame();									// Begins a new frame
	RAAXGUI_API void EndFrame();									// Ends the current frame


	/* Style */

	RAAXGUI_API class RaaxGUIStyle* GetStyle();						// Returns the current style


	/* Elements */

	RAAXGUI_API bool Checkbox(const char* Name, bool* Value);		// Draws a checkbox
	RAAXGUI_API void SliderFloat(const char* Name, float* Value, float MinValue, float MaxValue); // Draws a float slider
	RAAXGUI_API void SliderInt(const char* Name, int* Value, int MinValue, int MaxValue); // Draws an int slider




	/* STRUCTS AND ENUMS */
	// Forward declarations



	/* RaaxGUIContext*/
	/* The render que */
	struct RenderQue {
		std::vector<Window*> Windows;
	};



	/* Elements */
	/* Used to distinguish between different Element child classes */
	enum class ElementType {
		None = 0,
		Checkbox,
		Slider,
		Combo,
		Button,
		Seperator,
	};

	/* Checkbox Style */
	struct CheckboxStyle {
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

	struct SliderStyle {
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
	struct ElementStyle {
		CheckboxStyle CheckboxStyle;						// The style of the checkbox
		SliderStyle SliderStyle;							// The style of the slider
	};



	enum RaaxGUIWindowFlags_ {
		RaaxGUIWindowFlags_None = 0,
		RaaxGUIWindowFlags_NoTitleBar = 1 << 0,	// Disable title-bar
		RaaxGUIWindowFlags_NoResize = 1 << 1,	// Disable user resizing with the lower-right grip
		RaaxGUIWindowFlags_NoMove = 1 << 2,	// Disable user moving the window
		RaaxGUIWindowFlags_NoMainMove = 1 << 2,	// Disable user moving window by dragging on the main window
		RaaxGUIWindowFlags_NoTitleBarMove = 1 << 2,	// Disable user moving window by dragging on the title bar
	};

	enum class TextAlignment : uint8_t {
		Left,
		Center,
		Right,
	};

	enum class ResizeDirection {
		None,
		Top,
		TopRight,
		Right,
		BottomRight,
		Bottom,
		BottomLeft,
		Left,
		TopLeft,
	};

	struct WindowStyle {
		SDK::FLinearColor BackgroundColor;

		// Title bar
		SDK::FVector2D TitleBarSize;
		SDK::FLinearColor TitleBarColor;
		SDK::FLinearColor TitleBarTextColor;
		TextAlignment TitleBarTextAlignment;
		bool AutoTitleBarTextSize;
		float TitleBarTextSize;
	};

	struct RaaxGUIStyle {
		WindowStyle DefaultWindowStyle;
		ElementStyle DefaultElementStyle;

		void InitDarkStyle() {
			// Window
			DefaultWindowStyle.BackgroundColor = { 0.03f, 0.03f, 0.03f, 1.f }; // Darker grey background
			DefaultWindowStyle.TitleBarSize = { 0.f, 15.f };
			DefaultWindowStyle.TitleBarColor = { 0.1f, 0.1f, 0.1f, 1.f }; // Even darker grey for title bar
			DefaultWindowStyle.TitleBarTextColor = { 0.7f, 0.7f, 0.7f, 1.f }; // Dark grey for title text
			DefaultWindowStyle.TitleBarTextAlignment = TextAlignment::Center;
			DefaultWindowStyle.AutoTitleBarTextSize = true;
			DefaultWindowStyle.TitleBarTextSize = 8.f;

			// Checkbox
			DefaultElementStyle.CheckboxStyle.CheckboxButtonSize = SDK::FVector2D(15.f, 15.f);
			DefaultElementStyle.CheckboxStyle.CheckboxButtonColor = { 0.12f, 0.12f, 0.12f, 1.f }; // Darker grey
			DefaultElementStyle.CheckboxStyle.CheckboxButtonHoverColor = { 0.16f, 0.16f, 0.16f, 1.f }; // Dark grey hover
			DefaultElementStyle.CheckboxStyle.CheckboxButtonClickHeldColor = { 0.08f, 0.08f, 0.08f, 1.f }; // Darkest grey for click

			DefaultElementStyle.CheckboxStyle.CheckboxEnabledInnerBoxColor = { 0.0f, 0.6f, 0.7f, 1.f }; // Adjusted aqua for enabled state

			DefaultElementStyle.CheckboxStyle.CheckboxTextColor = { 0.7f, 0.7f, 0.7f, 1.f }; // Dark grey text
			DefaultElementStyle.CheckboxStyle.CheckboxTextSize = 12.f;
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
			DefaultElementStyle.SliderStyle.SliderValueTextSize = 8.f;
			DefaultElementStyle.SliderStyle.SliderValueTextOffset = 3.f;

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





	/* CLASSES */
	// Forward declarations
	class RaaxGUIContext;
	class Window;
	class Element;
	class CheckboxElement;



	/* Contains basic information about the current context(style, windows, etc) */
	class RaaxGUIContext {
	public:
		RAAXGUI_INTERNAL_API void MakeWindowTopMost(class Window* Window);	// Move a window to the top of the render que
		RAAXGUI_INTERNAL_API Window* GetCurrentWindow();					// Returns the current window being drawn
	public:
		/* Style */
		RaaxGUIStyle Style;					// The current style

		/* SetNext Info */
		bool UseNextWindowSize;				// Whether to use the next window size
		SDK::FVector2D NextWindowSize;		// The next window size
		bool UseNextWindowPos;				// Whether to use the next window position
		SDK::FVector2D NextWindowPos;		// The next window position

		/* Render Que */
		RenderQue RenderQue;				// The current render que (windows, elements, etc)

		/* Window Management */
		Window* CurrentWindow;				// The current window being drawn
	};

	/* Contains basic information about a window */
	class Window {
	public:
		bool Seen = false;											// Whether the BeginWindow function has been called yet this frame (used for garbage collection)

		WindowStyle Style;											// The window's style

		bool Open = false;											// Whether the window is open
		int ID = 0;													// The window's ID (used for differentiating between windows)

		std::string Name = "";										// The window's name

		SDK::FVector2D MinimumSize = SDK::FVector2D(50, 50);		// The window's minimum size

		SDK::FVector2D Position = SDK::FVector2D();					// The window's position
		SDK::FVector2D Size = SDK::FVector2D();						// The window's size

		SDK::FVector2D CurrentElementPosition = SDK::FVector2D();	// The position of the current element being drawn

		ResizeDirection CurrentResizeDirection = ResizeDirection::None;	// The current resize direction

		std::vector<class Element*> Elements;						// All the elements in the window
		std::vector<class Element*> ElementsLastFrame;				// All the elements rendered last frame
	public:
		Window() {
			Style = GetContext()->Style.DefaultWindowStyle;
		}

		RAAXGUI_INTERNAL_API void Draw();													// Draws the window

		RAAXGUI_INTERNAL_API bool ShouldDrag(SDK::FVector2D Position);						// Whether the window should be dragged

		RAAXGUI_INTERNAL_API void FixWindowPosition(SDK::FVector2D& Position);				// Fixes the window's position
		RAAXGUI_INTERNAL_API void FixWindowSize(SDK::FVector2D& Size);						// Fixes the window's size

		RAAXGUI_INTERNAL_API ResizeDirection GetResizeDirection(SDK::FVector2D Position);	// Get the current resize direction
		RAAXGUI_INTERNAL_API bool IsInResizeBounds(SDK::FVector2D Position);				// Whether the mouse is in the resize bounds
		RAAXGUI_INTERNAL_API bool IsInDragBounds(SDK::FVector2D Position);					// Whether the mouse is in the drag bounds
		RAAXGUI_INTERNAL_API bool IsInMenuBounds(SDK::FVector2D Position);					// Whether the mouse is in the menu bounds
		RAAXGUI_INTERNAL_API void OnClickTick(const SDK::FVector2D ClickPosition);			// Called every frame when the window is clicked
		RAAXGUI_INTERNAL_API void OnClickBegin(const SDK::FVector2D ClickPosition);			// Called when the window is clicked
		RAAXGUI_INTERNAL_API void OnClickEnd();												// Called when the window click is released
	};

	class Element {
	public:
		ElementType Type = ElementType::None;

		Window* ParentWindow = nullptr;

		SDK::FVector2D BoundsPosition = SDK::FVector2D();
		SDK::FVector2D BoundsSize = SDK::FVector2D();

		int ID = 0;
		int ElementIndex = 0;

		bool Seen = false;

		bool BeingClicked = false;
		bool BeingHovered = false;

		std::string Name = "";
	public:
		virtual void Draw() = 0;

		virtual void ClickTick(const SDK::FVector2D ClickPosition) = 0;
		virtual void OnClickBegin(const SDK::FVector2D ClickPosition) = 0;
		virtual void OnClickEnd() = 0;

		//virtual void OnHover(const SDK::FVector2D HoverPosition);

		virtual bool IsInElementBounds(const SDK::FVector2D Position) = 0;

		bool IsBeingClicked();
		bool IsBeingHovered();

		bool Text(const char* RenderText, SDK::FVector2D ScreenPosition, float FontSize, SDK::FLinearColor RenderColor, bool CenteredX, bool CenteredY, bool Outlined);
		bool FilledRect(SDK::FVector2D ScreenPosition, SDK::FVector2D ScreenSize, SDK::FLinearColor RenderColor, bool Outlined);
	};

	class CheckboxElement : public Element {
	public:
		CheckboxStyle Style;

		bool* Value = nullptr;

		SDK::FVector2D CheckboxRelativePosition;
	public:
		void Draw() override;

		void ClickTick(const SDK::FVector2D ClickPosition) override;
		void OnClickBegin(const SDK::FVector2D ClickPosition) override;
		void OnClickEnd() override;

		//void OnHoverCheckbox(const SDK::FVector2D HoverPosition);

		bool IsInElementBounds(const SDK::FVector2D Position) override;
	public:
		CheckboxElement() {
			Style = GetContext()->Style.DefaultElementStyle.CheckboxStyle;
			Type = ElementType::Checkbox;
		}
	};

	template<typename SliderValueType>
	class SliderElement : public Element {
	public:
		SliderStyle Style;

		SliderValueType* Value = nullptr;

		SliderValueType MinValue;
		SliderValueType MaxValue;

		SDK::FVector2D SliderRelativePosition;
	public:
		void Draw() override;

		void ClickTick(const SDK::FVector2D ClickPosition) override;
		void OnClickBegin(const SDK::FVector2D ClickPosition) override;
		void OnClickEnd() override;

		//void OnHoverSlider(const SDK::FVector2D HoverPosition);

		bool IsInElementBounds(const SDK::FVector2D Position) override;
	public:
		SliderElement() {
			Style = GetContext()->Style.DefaultElementStyle.SliderStyle;
			Type = ElementType::Slider;
		}
	};
}

inline RaaxGUI::RaaxGUIContext* GContext;