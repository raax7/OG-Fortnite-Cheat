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

	/* Element Style */
	struct ElementStyle {
		/* Checkbox Style */
		SDK::FVector2D CheckboxButtonSize;					// The size of the checkbox button

		SDK::FLinearColor CheckboxButtonColor;				// The color of the checkbox button
		SDK::FLinearColor CheckboxButtonHoverColor;			// The color of the checkbox button when hovered
		SDK::FLinearColor CheckboxButtonActiveColor;		// The color of the checkbox button when active
		SDK::FLinearColor CheckboxButtonClickHeldColor;		// The color of the checkbox button when the click is held

		SDK::FLinearColor CheckboxEnabledInnerBoxColor;		// The color of the inner box displayed when enabled
		SDK::FLinearColor CheckboxTextColor;				// The color of the text

		float CheckboxTextSize;								// The size of the text
		float CheckboxTextOffset;							// The offset of the text
		SDK::FVector2D CheckboxPadding;						// The padding of the checkbox
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
			DefaultWindowStyle.BackgroundColor = { 0.1f, 0.1f, 0.1f, 1.f };
			DefaultWindowStyle.TitleBarSize = { 0.f, 20.f };
			DefaultWindowStyle.TitleBarColor = { 0.2f, 0.2f, 0.2f, 1.f };
			DefaultWindowStyle.TitleBarTextColor = { 1.f, 1.f, 1.f, 1.f };
			DefaultWindowStyle.TitleBarTextAlignment = TextAlignment::Center;
			DefaultWindowStyle.AutoTitleBarTextSize = true;
			DefaultWindowStyle.TitleBarTextSize = 0.f;

			DefaultElementStyle.CheckboxButtonSize = SDK::FVector2D(20, 20);
			DefaultElementStyle.CheckboxButtonColor = { 0.2f, 0.2f, 0.2f, 1.f };
			DefaultElementStyle.CheckboxButtonHoverColor = { 0.3f, 0.3f, 0.3f, 1.f };
			DefaultElementStyle.CheckboxButtonActiveColor = { 0.4f, 0.4f, 0.4f, 1.f };
			DefaultElementStyle.CheckboxButtonClickHeldColor = { 0.5f, 0.5f, 0.5f, 1.f };
			DefaultElementStyle.CheckboxEnabledInnerBoxColor = { 1.0f, 0.2f, 0.2f, 1.f };
			DefaultElementStyle.CheckboxTextColor = { 1.f, 1.f, 1.f, 1.f };
			DefaultElementStyle.CheckboxTextSize = 12.f;
			DefaultElementStyle.CheckboxTextOffset = 5.f;
			DefaultElementStyle.CheckboxPadding = { 5.f, 5.f };
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

		WindowStyle Style = WindowStyle();							// The window's style

		bool Open = false;											// Whether the window is open
		int ID = 0;													// The window's ID (used for differentiating between windows)

		const char* Name = "";										// The window's name

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

			// TEMP FOR TESTING
			Style.BackgroundColor = { (float)((rand() % 255) / 255.f), (float)((rand() % 255) / 255.f), (float)((rand() % 255) / 255.f), 1.f };
		}

		RAAXGUI_INTERNAL_API void Draw();													// Draws the window

		RAAXGUI_INTERNAL_API bool ShouldDrag(SDK::FVector2D Position);						// Whether the window should be dragged

		RAAXGUI_INTERNAL_API void FixWindowPosition(SDK::FVector2D& Position);				// Fixes the window's position
		RAAXGUI_INTERNAL_API void FixWindowSize(SDK::FVector2D& Size);						// Fixes the window's size

		RAAXGUI_INTERNAL_API ResizeDirection GetResizeDirection(SDK::FVector2D Position);	// Get the current resize direction
		RAAXGUI_INTERNAL_API bool IsInResizeBounds(SDK::FVector2D Position);				// Whether the mouse is in the resize bounds
		RAAXGUI_INTERNAL_API bool IsInDragBounds(SDK::FVector2D Position);					// Whether the mouse is in the drag bounds
		RAAXGUI_INTERNAL_API bool IsInMenuBounds(SDK::FVector2D Position);					// Whether the mouse is in the menu bounds
		RAAXGUI_INTERNAL_API void OnClickBegin(const SDK::FVector2D ClickPosition);			// Called when the window is clicked
		RAAXGUI_INTERNAL_API void OnClickEnd();												// Called when the window click is released
	};

	class Element {
	public:
		ElementStyle Style;
		ElementType Type;

		Window* ParentWindow;

		int ID = 0;
		int ElementIndex = 0;

		bool Seen = false;


		bool BeingClicked = false;

		std::string Name = "";
	public:
		virtual void Draw();

		virtual void OnClickBegin(const SDK::FVector2D ClickPosition);
		virtual void OnClickEnd();

		virtual bool IsBeingClicked();
		//virtual void OnHover(const SDK::FVector2D HoverPosition);

		virtual bool IsInElementBounds(const SDK::FVector2D& Position);
	public:
		Element() {
			Style = GetContext()->Style.DefaultElementStyle;
		}
	};

	class CheckboxElement : public Element {
	public:
		void DrawCheckbox();

		void OnClickEndCheckbox();
		void OnClickBeginCheckbox();
		//void OnHoverCheckbox(const SDK::FVector2D HoverPosition);

		bool IsInElementBoundsCheckbox(const SDK::FVector2D& Position);
	public:
		bool* Value = nullptr;

		SDK::FVector2D CheckboxRelativePosition;
	public:
		CheckboxElement() {
			Type = ElementType::Checkbox;
		}
	};
}

inline RaaxGUI::RaaxGUIContext* GContext;