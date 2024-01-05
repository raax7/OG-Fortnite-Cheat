#ifndef MENU_H
#define MENU_H

#include <Windows.h>
#include "../../SDK/SDK.h"
#include "../../SDK/Classes/Basic.h"
#include "../../SDK/Classes/CoreUObject_classes.h"
#include "../../../Configs/Config.h"
#include "../../SDK/Classes/Engine_classes.h"
#include "Drawing.h"

namespace Input {
	inline SDK::FVector2D MousePos;
	inline SDK::FVector2D LeftClickMousePos;
	inline bool LeftClickDown;
	inline bool LeftClickReleased;
	inline bool LeftClickRegistered;
	inline bool LeftClickReleasedRegistered;

	inline void updateInput() {
		// Mouse Position
		POINT MousePos_;
		GetCursorPos(&MousePos_);
		MousePos = SDK::FVector2D{ (float)MousePos_.x, (float)MousePos_.y };

		// LMB
		if (GetAsyncKeyState(VK_LBUTTON)) {
			// If LMB is being pressed and it wasn't pressed last tick, update the click position
			if (!LeftClickDown) {
				LeftClickMousePos = MousePos;
			}

			LeftClickDown = true;
			LeftClickReleased = false;  // Reset released flag
		}
		else {
			if (LeftClickReleased) {
				LeftClickReleased = false;
			}

			if (LeftClickDown) {
				LeftClickReleased = true;  // LMB is released
			}

			LeftClickDown = false;
			LeftClickRegistered = false;
			LeftClickReleasedRegistered = false;
		}
	}
}

namespace RaaxGUI {
	namespace Colors
	{
		inline SDK::FLinearColor Text{ 1.0f, 1.0f, 1.0f, 1.0f };
		inline SDK::FLinearColor Text_Shadow{ 0.0f, 0.0f, 0.0f, 0.0f };
		inline SDK::FLinearColor Text_Outline{ 0.0f, 0.0f, 0.0f, 0.30f };

		inline SDK::FLinearColor Window_Background{ 0.009f, 0.009f, 0.009f, 1.0f };
		inline SDK::FLinearColor Window_Header{ 0.10f, 0.15f, 0.84f, 1.0f };

		inline SDK::FLinearColor Button_Idle{ 0.10f, 0.15f, 0.84f, 1.0f };
		inline SDK::FLinearColor Button_Hovered{ 0.15f, 0.20f, 0.89f, 1.0f };
		inline SDK::FLinearColor Button_Active{ 0.20f, 0.25f, 0.94f, 1.0f };

		inline SDK::FLinearColor Checkbox_Idle{ 0.17f, 0.16f, 0.23f, 1.0f };
		inline SDK::FLinearColor Checkbox_Hovered{ 0.22f, 0.30f, 0.72f, 1.0f };
		inline SDK::FLinearColor Checkbox_Clicked{ 0.27f, 0.37f, 0.79f, 1.0f };
		inline SDK::FLinearColor Checkbox_Enabled{ 0.20f, 0.25f, 0.94f, 1.0f };

		inline SDK::FLinearColor Combobox_Idle{ 0.17f, 0.16f, 0.23f, 1.0f };
		inline SDK::FLinearColor Combobox_Hovered{ 0.17f, 0.16f, 0.23f, 1.0f };
		inline SDK::FLinearColor Combobox_Elements{ 0.239f, 0.42f, 0.82f, 1.0f };

		inline SDK::FLinearColor Slider_Idle{ 0.17f, 0.16f, 0.23f, 1.0f };
		inline SDK::FLinearColor Slider_Hovered{ 0.17f, 0.16f, 0.23f, 1.0f };
		inline SDK::FLinearColor Slider_Progress{ 0.22f, 0.30f, 0.72f, 1.0f };
		inline SDK::FLinearColor Slider_Button{ 0.10f, 0.15f, 0.84f, 1.0f };

		inline SDK::FLinearColor ColorPicker_Background{ 0.006f, 0.006f, 0.006f, 1.0f };
	}

	namespace Style {
		inline SDK::FVector2D GrabBarPadding(4, 3);
		inline float GrabBarSize = 20;

		inline SDK::FVector2D CheckBoxPadding(10, 10);
		inline float CheckBoxSize = 20.f;

		inline SDK::FVector2D SliderPadding(10, 15);
	}

	inline bool WasInWindowZoneOnClick = false;
	inline SDK::FVector2D WindowPositionOnClick;
	inline SDK::FVector2D WindowPosition;

	inline int ElementCount = 0;

	inline SDK::FVector2D OriginalOffset;
	inline SDK::FVector2D Offset;
	inline SDK::FVector2D LastElementPosition;
	inline SDK::FVector2D LastElementSize;
	inline const wchar_t* ElementHoveredOnClick;
	inline bool SameLine = false;

	inline bool MouseInZone(SDK::FVector2D Pos, SDK::FVector2D Size)
	{
		if (Input::MousePos.X > Pos.X && Input::MousePos.Y > Pos.Y) {
			if (Input::MousePos.X < Pos.X + Size.X && Input::MousePos.Y < Pos.Y + Size.Y) {
				return true;
			}
		}

		return false;
	}

	inline bool MouseInZoneOnLeftClick(SDK::FVector2D Pos, SDK::FVector2D Size)
	{
		if (Input::LeftClickMousePos.X > Pos.X && Input::LeftClickMousePos.Y > Pos.Y) {
			if (Input::LeftClickMousePos.X < Pos.X + Size.X && Input::LeftClickMousePos.Y < Pos.Y + Size.Y) {
				return true;
			}
		}

		return false;
	}

	inline bool Window(const wchar_t* WindowName, SDK::FVector2D* WindowPosition_, SDK::FVector2D WindowSize, bool IsOpen) {
		if (!IsOpen) {
			return false;
		}

		ElementCount = 0;

		bool IsInZone = MouseInZoneOnLeftClick(SDK::FVector2D{ WindowPosition_->X, WindowPosition_->Y }, SDK::FVector2D(WindowSize.X, Style::GrabBarSize));

		if (IsInZone && Input::LeftClickDown || Input::LeftClickDown && WasInWindowZoneOnClick) {
			if (!WindowPositionOnClick.X && !WindowPositionOnClick.Y) {
				WindowPositionOnClick = SDK::FVector2D(WindowPosition_->X, WindowPosition_->Y);
				WasInWindowZoneOnClick = true;
			}

			WindowPosition_->X = WindowPositionOnClick.X + (Input::MousePos.X - Input::LeftClickMousePos.X);
			WindowPosition_->Y = WindowPositionOnClick.Y + (Input::MousePos.Y - Input::LeftClickMousePos.Y);
		}
		else {
			WindowPositionOnClick = SDK::FVector2D();
			WasInWindowZoneOnClick = false;
		}

		Offset = SDK::FVector2D(WindowPosition_->X, Style::GrabBarSize + WindowPosition_->Y);
		OriginalOffset = Offset;

		Drawing::FilledRect(SDK::FVector2D(WindowPosition_->X, WindowPosition_->Y), WindowSize, Colors::Window_Background);
		Drawing::FilledRect(SDK::FVector2D(WindowPosition_->X, WindowPosition_->Y), SDK::FVector2D(WindowSize.X, Style::GrabBarSize), Colors::Window_Header);

		SDK::FVector2D TextPos = SDK::FVector2D{ WindowPosition_->X + Style::GrabBarPadding.X, WindowPosition_->Y + (Style::GrabBarPadding.Y / 2) };
		float TextSize = (Style::GrabBarSize - Style::GrabBarPadding.Y - Style::GrabBarPadding.Y - Style::GrabBarPadding.Y);

		if (TextSize >= 1) {
			Drawing::Text(WindowName, TextPos, TextSize, Colors::Text, false, false, true);
		}

		return true;
	}

	inline void CheckBox(const wchar_t* CheckBoxName, bool* CheckBoxValue) {
		ElementCount++;

		SDK::FVector2D CheckBoxPosition = SDK::FVector2D(WindowPosition.X + Style::CheckBoxPadding.X + Offset.X, WindowPosition.Y + Style::CheckBoxPadding.Y + Offset.Y);

		if (SameLine) {
			CheckBoxPosition = SDK::FVector2D((Style::CheckBoxPadding.X * 2) + LastElementSize.X + Offset.X, Style::CheckBoxPadding.Y + Offset.Y);

			SameLine = false;
		}

		bool IsInZone = MouseInZone(CheckBoxPosition, SDK::FVector2D(Style::CheckBoxSize, Style::CheckBoxSize));

		if (IsInZone && !ElementHoveredOnClick || ElementHoveredOnClick == CheckBoxName) {
			if (!ElementHoveredOnClick && Input::LeftClickDown && !Input::LeftClickRegistered) {
				ElementHoveredOnClick = CheckBoxName;
				Input::LeftClickRegistered = true;
			}

			if (Input::LeftClickDown && IsInZone) {
				Drawing::FilledRect(CheckBoxPosition, SDK::FVector2D(Style::CheckBoxSize, Style::CheckBoxSize), Colors::Checkbox_Clicked);
			}
			else {
				Drawing::FilledRect(CheckBoxPosition, SDK::FVector2D(Style::CheckBoxSize, Style::CheckBoxSize), Colors::Checkbox_Hovered);
			}

			if (Input::LeftClickReleased && !Input::LeftClickReleasedRegistered && ElementHoveredOnClick == CheckBoxName) {
				*CheckBoxValue = !*CheckBoxValue;
				ElementHoveredOnClick = nullptr;
				Input::LeftClickReleasedRegistered = true;
			}
		}
		else {
			Drawing::FilledRect(CheckBoxPosition, SDK::FVector2D(Style::CheckBoxSize, Style::CheckBoxSize), Colors::Checkbox_Idle);
		}

		if (*CheckBoxValue) {
			Drawing::FilledRect(SDK::FVector2D(CheckBoxPosition.X + 5, CheckBoxPosition.Y + 5), SDK::FVector2D(Style::CheckBoxSize - 10, Style::CheckBoxSize - 10), Colors::Checkbox_Enabled);
		}

		SDK::FVector2D TextPos = SDK::FVector2D{ CheckBoxPosition.X + Style::CheckBoxSize + 4, CheckBoxPosition.Y };
		Drawing::Text(CheckBoxName, TextPos, 10.f, Colors::Text, false, false, true);

		//if (!Offset.Y) {
		//	Offset.Y = CheckBoxPosition.Y;
		//}

		Offset.Y = Offset.Y + Style::CheckBoxSize + Style::CheckBoxPadding.Y;
	}

	inline void SliderInt(const wchar_t* SliderName, int* SliderValue, int Min, int Max, SDK::FVector2D SliderSize = SDK::FVector2D(240, 10)) {
		ElementCount++;

		SDK::FVector2D SliderPosition = SDK::FVector2D(WindowPosition.X + Style::SliderPadding.X + Offset.X, WindowPosition.Y + Style::SliderPadding.Y + Offset.Y);

		if (SameLine) {
			SliderPosition = SDK::FVector2D((Style::SliderPadding.X * 2) + LastElementSize.X + Offset.X, Style::SliderPadding.Y + Offset.Y);
			SameLine = false;
		}

		// Calculate the ratio of the current SliderValue within the range.
		float SliderRatio = static_cast<float>(*SliderValue - Min) / (Max - Min);

		// Calculate the position of the slider button based on the current value.
		float SliderButtonPosition = SliderPosition.X + SliderRatio * SliderSize.X;

		bool IsDraggingSlider = ElementHoveredOnClick == SliderName && Input::LeftClickDown;
		bool IsInZone = MouseInZone(SDK::FVector2D(SliderPosition.X, SliderPosition.Y), SliderSize) && Input::LeftClickDown;

		if (IsInZone && Input::LeftClickDown && !Input::LeftClickRegistered && !ElementHoveredOnClick) {
			ElementHoveredOnClick = SliderName;
			Input::LeftClickRegistered = true;
			IsDraggingSlider = true;
		}

		if (IsDraggingSlider) {
			int NewValue = Min + static_cast<int>((Input::MousePos.X - SliderPosition.X) * (Max - Min) / SliderSize.X);

			// Manually clamp the value between Min and Max without using std functions
			NewValue = (NewValue < Min) ? Min : (NewValue > Max) ? Max : NewValue;
			*SliderValue = NewValue;

			// Update based on the new value
			SliderRatio = static_cast<float>(*SliderValue - Min) / (Max - Min);
			SliderButtonPosition = SliderPosition.X + SliderRatio * SliderSize.X;
		}

		if (Input::LeftClickReleased && !Input::LeftClickReleasedRegistered && ElementHoveredOnClick == SliderName) {
			ElementHoveredOnClick = nullptr;
			Input::LeftClickReleasedRegistered = true;
		}

		// Draw the slider name
		Drawing::Text(SliderName, SDK::FVector2D(SliderPosition.X, SliderPosition.Y - 15), 10.f, Colors::Text, false, false, true);

		// Draw the slider background (idle state).
		Drawing::FilledRect(SliderPosition, SliderSize, Colors::Slider_Idle);

		// Draw the slider progress.
		Drawing::FilledRect(SliderPosition, SDK::FVector2D(SliderButtonPosition - SliderPosition.X, SliderSize.Y), Colors::Slider_Progress);

		Drawing::FilledRect(SDK::FVector2D(SliderButtonPosition - 2.5f, SliderPosition.Y - 2.5f), SDK::FVector2D(5.0f, SliderSize.Y + 5.0f), Colors::Slider_Button);

		// Add the text representation of the value
		wchar_t valueStr[64];
		swprintf(valueStr, sizeof(valueStr) / sizeof(valueStr[0]), L"%d", *SliderValue);  // convert integer to wide string
		Drawing::Text(valueStr, SDK::FVector2D(SliderButtonPosition, SliderPosition.Y + SliderSize.Y + 5), 10.f, Colors::Text, false, false, true);  // adjust the position as required

		// Move the offset for the next control.
		Offset.Y += SliderSize.Y + Style::SliderPadding.Y + 25;
	}

	inline void SliderFloat(const wchar_t* SliderName, float* SliderValue, float Min, float Max, SDK::FVector2D SliderSize = SDK::FVector2D(240, 10)) {
		ElementCount++;

		SDK::FVector2D SliderPosition = SDK::FVector2D(WindowPosition.X + Style::SliderPadding.X + Offset.X, WindowPosition.Y + Style::SliderPadding.Y + Offset.Y);

		if (SameLine) {
			SliderPosition = SDK::FVector2D((Style::SliderPadding.X * 2) + LastElementSize.X + Offset.X, Style::SliderPadding.Y + Offset.Y);
			SameLine = false;
		}

		// Calculate the ratio of the current SliderValue within the range.
		float SliderRatio = (*SliderValue - Min) / (Max - Min);

		// Calculate the position of the slider button based on the current value.
		float SliderButtonPosition = SliderPosition.X + SliderRatio * SliderSize.X;

		bool IsDraggingSlider = ElementHoveredOnClick == SliderName && Input::LeftClickDown;
		bool IsInZone = MouseInZone(SDK::FVector2D(SliderPosition.X, SliderPosition.Y), SliderSize) && Input::LeftClickDown;

		if (IsInZone && Input::LeftClickDown && !Input::LeftClickRegistered && !ElementHoveredOnClick) {
			ElementHoveredOnClick = SliderName;
			Input::LeftClickRegistered = true;
			IsDraggingSlider = true;
		}

		if (IsDraggingSlider) {
			float NewValue = Min + ((Input::MousePos.X - SliderPosition.X) * (Max - Min) / SliderSize.X);

			// Manually clamp the value between Min and Max without using std functions
			NewValue = (NewValue < Min) ? Min : (NewValue > Max) ? Max : NewValue;
			*SliderValue = NewValue;

			// Update ratio and button position
			SliderRatio = (*SliderValue - Min) / (Max - Min);
			SliderButtonPosition = SliderPosition.X + SliderRatio * SliderSize.X;
		}

		if (Input::LeftClickReleased && !Input::LeftClickReleasedRegistered && ElementHoveredOnClick == SliderName) {
			ElementHoveredOnClick = nullptr;
			Input::LeftClickReleasedRegistered = true;
		}

		// Draw the slider name
		Drawing::Text(SliderName, SDK::FVector2D(SliderPosition.X, SliderPosition.Y - 15), 10.f, Colors::Text, false, false, true);

		// Draw the slider background (idle state).
		Drawing::FilledRect(SliderPosition, SliderSize, Colors::Slider_Idle);

		// Draw the slider progress.
		Drawing::FilledRect(SliderPosition, SDK::FVector2D(SliderButtonPosition - SliderPosition.X, SliderSize.Y), Colors::Slider_Progress);

		Drawing::FilledRect(SDK::FVector2D(SliderButtonPosition - 2.5f, SliderPosition.Y - 2.5f), SDK::FVector2D(5.0f, SliderSize.Y + 5.0f), Colors::Slider_Button);

		// Add the text representation of the value
		wchar_t valueStr[64];
		swprintf(valueStr, sizeof(valueStr) / sizeof(valueStr[0]), L"%.2f", *SliderValue);  // convert float to wide string with 2 decimal places
		Drawing::Text(valueStr, SDK::FVector2D(SliderButtonPosition, SliderPosition.Y + SliderSize.Y + 5), 10.f, Colors::Text, false, false, true);  // adjust the position as required

		// Move the offset for the next control.
		Offset.Y += SliderSize.Y + Style::SliderPadding.Y + 25;
	}



	inline bool Open = true;
	inline SDK::FVector2D WindowPosition_ = SDK::FVector2D(100, 100);

	inline void Tick(uintptr_t Canvas) {
		Drawing::TargetCanvas = reinterpret_cast<SDK::UCanvas*>(Canvas);
		Input::updateInput();

		if (GetAsyncKeyState(VK_INSERT) & 1) {
			Open = !Open;
		}

		if (RaaxGUI::Window(L"ERA", &WindowPosition_, SDK::FVector2D(700, 450), Open)) {
			RaaxGUI::SliderInt(L"test", &Config::test, 0, 1000);
		}
	}
}

#endif