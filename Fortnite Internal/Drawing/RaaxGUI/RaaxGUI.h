#pragma once
#include <Windows.h>
#include <vector>
#include "../../Utilities/Error.h"
#include "../../Game/SDK/Classes/Basic.h"
#include "../Drawing.h"

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

namespace RaaxGUI {
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

	struct WindowStyle {
		SDK::FLinearColor BackgroundColor;
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

		SDK::FVector2D Position =  SDK::FVector2D();
		SDK::FVector2D Size = SDK::FVector2D();
		WindowStyle Style = WindowStyle();

		std::vector<class Element*> Elements;
	public:
		Window() {
			DEBUG_LOG("called");
			Style.BackgroundColor = { (float)((rand() % 255) / 255.f), (float)((rand() % 255) / 255.f), (float)((rand() % 255) / 255.f), 1.f };
		}

		virtual bool IsInDragBounds(SDK::FVector2D Position);
		virtual bool IsInMenuBounds(SDK::FVector2D Position);
		virtual void OnClick();
	};

	Window* FindWindowById(int ID);
	Window* RegisterNewWindow(int ID);

	bool BeginWindow(const char* Name, bool* Open, SDK::FVector2D InitPosition = SDK::FVector2D(), SDK::FVector2D InitSize = SDK::FVector2D());
	void EndWindow();

	void SetNextWindowSize(SDK::FVector2D Size);
	void SetNextWindowPos(SDK::FVector2D Pos);

	void InitContext();
	RaaxGUIContext* GetContext();

	void NewFrame();
	void Render();
}

inline RaaxGUI::RaaxGUIContext* GContext;