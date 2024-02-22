#pragma once
#include <vector>
#include "../../Game/SDK/Classes/Basic.h"
#include "../../Utilities/Logger.h"

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
	// Forward declare
	class Window* FindWindowById(int ID);
	class Window* RegisterNewWindow(int ID);

	bool BeginWindow(const char* Name, bool* Open, RaaxGUIWindowFlags Flags, SDK::FVector2D InitPosition = SDK::FVector2D(), SDK::FVector2D InitSize = SDK::FVector2D());
	void EndWindow();

	void SetNextWindowSize(SDK::FVector2D Size);
	void SetNextWindowPos(SDK::FVector2D Pos);

	void InitContext();
	class RaaxGUIContext* GetContext();

	void NewFrame();
	void Render();

	class RaaxGUIStyle* GetStyle();





#if 0
	enum class ElementType {
		None = 0,
		Checkbox,
		Slider,
		Combo,
		Button,
		Seperator,
	};

	struct ElementInfo {
		SDK::FVector2D Position;
		SDK::FVector2D Size;
		float TextSize;
		SDK::FLinearColor TextColor;
		SDK::FLinearColor BackgroundColor;
		SDK::FLinearColor BorderColor;
	};

	class Element {
	public:
		ElementType Type{};
	public:
		virtual void Draw();
	};
#endif

	enum RaaxGUIWindowFlags_ {
		RaaxGUIWindowFlags_None				= 0,
		RaaxGUIWindowFlags_NoTitleBar		= 1 << 0,	// Disable title-bar
		RaaxGUIWindowFlags_NoResize			= 1 << 1,	// Disable user resizing with the lower-right grip
		RaaxGUIWindowFlags_NoMove			= 1 << 2,	// Disable user moving the window
		RaaxGUIWindowFlags_NoMainMove		= 1 << 2,	// Disable user moving window by dragging on the main window
		RaaxGUIWindowFlags_NoTitleBarMove	= 1 << 2,	// Disable user moving window by dragging on the title bar
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
		SDK::FVector2D TitleBarSize = SDK::FVector2D();
		SDK::FLinearColor TitleBarColor = SDK::FLinearColor();
		SDK::FLinearColor TitleBarTextColor = SDK::FLinearColor();
		TextAlignment TitleBarTextAlignment = TextAlignment::Left;
		bool AutoTitleBarTextSize = true;
		float TitleBarTextSize = 0.f;
	};

	struct RaaxGUIStyle {
		WindowStyle DefaultWindowStyle;

		void InitDarkStyle() {
			DefaultWindowStyle.BackgroundColor = { 0.1f, 0.1f, 0.1f, 1.f };
			DefaultWindowStyle.TitleBarSize = { 0.f, 20.f };
			DefaultWindowStyle.TitleBarColor = { 0.2f, 0.2f, 0.2f, 1.f };
			DefaultWindowStyle.TitleBarTextColor = { 1.f, 1.f, 1.f, 1.f };
			DefaultWindowStyle.TitleBarTextAlignment = TextAlignment::Left;
			DefaultWindowStyle.AutoTitleBarTextSize = true;
			DefaultWindowStyle.TitleBarTextSize = 0.f;
		}
	};






	struct WindowRenderQue {
		class Window* Window;
		SDK::FVector2D Position;
		SDK::FVector2D Size;
	};

	struct RenderQue {
		std::vector<WindowRenderQue> Windows;
	};

	class RaaxGUIContext {
	public:
		virtual void MakeWindowTopMost(class Window* Window);
	public:
		class Window* CurrentWindow;
		RaaxGUIStyle Style;

		bool UseNextWindowSize;
		SDK::FVector2D NextWindowSize;
		bool UseNextWindowPos;
		SDK::FVector2D NextWindowPos;

		RenderQue RenderQue;

		std::vector<class Window*> Windows;
	};

	class Window {
	public:
		bool Seen = false;

		bool Open = false;
		int ID = -1;

		const char* Name = "";

		SDK::FVector2D MinimumSize = SDK::FVector2D(50, 50);

		SDK::FVector2D Position =  SDK::FVector2D();
		SDK::FVector2D Size = SDK::FVector2D();
		WindowStyle Style = WindowStyle();

		ResizeDirection CurrentResizeDirection = ResizeDirection::None;

		std::vector<class Element*> Elements;
	public:
		Window() {
			Style = GetContext()->Style.DefaultWindowStyle;

			// TEMP TESTING
			DEBUG_LOG("called");
			Style.BackgroundColor = { (float)((rand() % 255) / 255.f), (float)((rand() % 255) / 255.f), (float)((rand() % 255) / 255.f), 1.f };
		}


		virtual void FixWindowPosition(SDK::FVector2D& Position);
		virtual void FixWindowSize(SDK::FVector2D& Size);

		virtual ResizeDirection GetResizeDirection(SDK::FVector2D Position);
		virtual bool IsInResizeBounds(SDK::FVector2D Position);
		virtual bool IsInDragBounds(SDK::FVector2D Position);
		virtual bool IsInMenuBounds(SDK::FVector2D Position);
		virtual void OnClick();
	};
}

inline RaaxGUI::RaaxGUIContext* GContext;