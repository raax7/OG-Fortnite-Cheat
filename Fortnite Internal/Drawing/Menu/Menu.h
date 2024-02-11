#pragma once
#include <Windows.h>

// NEW MENU CLASS

class Menu {
public:
	static inline bool Opened;
	static inline bool Resized;
};




#include <Windows.h>
#include "../../Game/SDK/SDK.h"
#include "../../Game/SDK/Classes/Basic.h"
#include "../../Game/SDK/Classes/CoreUObject_classes.h"
#include "../../Configs/Config.h"
#include "../../Game/SDK/Classes/Engine_classes.h"
#include "../Drawing.h"

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

		Drawing::FilledRect(SDK::FVector2D(WindowPosition_->X, WindowPosition_->Y), WindowSize, Colors::Window_Background, false);
		Drawing::FilledRect(SDK::FVector2D(WindowPosition_->X, WindowPosition_->Y), SDK::FVector2D(WindowSize.X, Style::GrabBarSize), Colors::Window_Header, false);

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
				Drawing::FilledRect(CheckBoxPosition, SDK::FVector2D(Style::CheckBoxSize, Style::CheckBoxSize), Colors::Checkbox_Clicked, false);
			}
			else {
				Drawing::FilledRect(CheckBoxPosition, SDK::FVector2D(Style::CheckBoxSize, Style::CheckBoxSize), Colors::Checkbox_Hovered, false);
			}

			if (Input::LeftClickReleased && !Input::LeftClickReleasedRegistered && ElementHoveredOnClick == CheckBoxName) {
				*CheckBoxValue = !*CheckBoxValue;
				ElementHoveredOnClick = nullptr;
				Input::LeftClickReleasedRegistered = true;
			}
		}
		else {
			Drawing::FilledRect(CheckBoxPosition, SDK::FVector2D(Style::CheckBoxSize, Style::CheckBoxSize), Colors::Checkbox_Idle, false);
		}

		if (*CheckBoxValue) {
			Drawing::FilledRect(SDK::FVector2D(CheckBoxPosition.X + 5, CheckBoxPosition.Y + 5), SDK::FVector2D(Style::CheckBoxSize - 10, Style::CheckBoxSize - 10), Colors::Checkbox_Enabled, false);
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
		Drawing::FilledRect(SliderPosition, SliderSize, Colors::Slider_Idle, false);

		// Draw the slider progress.
		Drawing::FilledRect(SliderPosition, SDK::FVector2D(SliderButtonPosition - SliderPosition.X, SliderSize.Y), Colors::Slider_Progress, false);

		Drawing::FilledRect(SDK::FVector2D(SliderButtonPosition - 2.5f, SliderPosition.Y - 2.5f), SDK::FVector2D(5.0f, SliderSize.Y + 5.0f), Colors::Slider_Button, false);

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
		Drawing::FilledRect(SliderPosition, SliderSize, Colors::Slider_Idle, false);

		// Draw the slider progress.
		Drawing::FilledRect(SliderPosition, SDK::FVector2D(SliderButtonPosition - SliderPosition.X, SliderSize.Y), Colors::Slider_Progress, false);

		Drawing::FilledRect(SDK::FVector2D(SliderButtonPosition - 2.5f, SliderPosition.Y - 2.5f), SDK::FVector2D(5.0f, SliderSize.Y + 5.0f), Colors::Slider_Button, false);

		// Add the text representation of the value
		wchar_t valueStr[64];
		swprintf(valueStr, sizeof(valueStr) / sizeof(valueStr[0]), L"%.2f", *SliderValue);  // convert float to wide string with 2 decimal places
		Drawing::Text(valueStr, SDK::FVector2D(SliderButtonPosition, SliderPosition.Y + SliderSize.Y + 5), 10.f, Colors::Text, false, false, true);  // adjust the position as required

		// Move the offset for the next control.
		Offset.Y += SliderSize.Y + Style::SliderPadding.Y + 25;
	}



	inline bool Open = true;
	inline SDK::FVector2D WindowPosition_ = SDK::FVector2D(100, 100);

	inline void Tick() {
		Input::updateInput();

		if (GetAsyncKeyState(VK_INSERT) & 1) {
			Open = !Open;
		}

		if (RaaxGUI::Window(L"​Charles FN​", &WindowPosition_, SDK::FVector2D(700, 650), Open)) {
			//RaaxGUI::SliderInt(L"test", &Config::test2, 0, 30);
			//RaaxGUI::SliderFloat(L"test return", &Config::testfloat, 0, 1000);
			//RaaxGUI::SliderInt(L"test ammo", &Config::test2, 0, 1000);

			//if (GetAsyncKeyState(VK_RIGHT) & 1) {
			//	Config::test2 += 1;
			//}
			//else if (GetAsyncKeyState(VK_LEFT) & 1) {
			//	Config::test2 -= 1;
			//}

			//if (GetAsyncKeyState(VK_DOWN) & 1) {
			//	Config::testfloat += 1.f;
			//}
			//else if (GetAsyncKeyState(VK_UP) & 1) {
			//	Config::testfloat -= 1.f;
			//}

			//RaaxGUI::SliderInt(L"​test​", &Config::test, 0, 3);

			//RaaxGUI::Combobox(L"Options", SDK::FVector2D(100, 20), &myComboValue, { L"Option1", L"Option2", L"Option3" });
			//RaaxGUI::Combobox(L"Options", std::vector<std::wstring>(3) = { L"Option1", L"Option2", L"Option3" }, &myComboValue, SDK::FVector2D(100, 20));

			RaaxGUI::CheckBox(L"​​​​R​​a​​​​p​​i​​​​​​​​d​ ​f​i​​​r​​​e​ ​s​e​​​x​​", &Config::rapidfiresex);

			RaaxGUI::CheckBox(L"​A​i​m​b​o​t", &Config::Aimbot::Enabled);
			if (Config::Aimbot::Enabled) {
				RaaxGUI::CheckBox(L"Silent Aim", &Config::Aimbot::SilentAim);
				if (Config::Aimbot::SilentAim) {
					RaaxGUI::CheckBox(L"Show Silent Rotations", &Config::Aimbot::ShowSilentRotations);
					RaaxGUI::CheckBox(L"Use AimKey for Silent", &Config::Aimbot::UseAimKeyForSilent);
				}

				RaaxGUI::SliderInt(L"​F​O​V​ ​S​i​z​e​", &Config::Aimbot::Standard::FOV, 0, 1000);
				RaaxGUI::SliderFloat(L"​S​m​o​o​t​h​i​n​g​", &Config::Aimbot::Standard::Smoothing, 1, 25);

				RaaxGUI::CheckBox(L"​C​l​o​s​e​ ​A​i​m​", &Config::Aimbot::CloseAim::Enabled);
				if (Config::Aimbot::CloseAim::Enabled) {
					RaaxGUI::SliderInt(L"​C​l​o​s​e​ ​F​O​V​ ​S​i​z​e​", &Config::Aimbot::CloseAim::FOV, 0, 1000);
					RaaxGUI::SliderFloat(L"​C​l​o​s​e​ ​S​m​o​​o​t​h​i​n​g​", &Config::Aimbot::CloseAim::Smoothing, 1, 25);
					RaaxGUI::SliderInt(L"​C​l​o​s​e​ ​R​a​n​g​e​ ​(​M​)​", &Config::Aimbot::CloseAim::Range, 1, 25);
				}

				RaaxGUI::CheckBox(L"​​W​e​​a​k​​s​​p​​o​t​ ​A​i​m​", &Config::Aimbot::Weakspot::Enabled);
				if (Config::Aimbot::Weakspot::Enabled) {
					RaaxGUI::SliderInt(L"​​W​e​a​​ks​p​o​t ​F​O​V​ ​S​i​z​e​", &Config::Aimbot::Weakspot::FOV, 0, 1000);
					RaaxGUI::SliderFloat(L"​​​W​​e​a​k​​s​po​t​​​ ​S​m​o​​o​t​h​i​n​g​", &Config::Aimbot::Weakspot::Smoothing, 1, 25);
				}
			}

			//RaaxGUI::SliderFloat(L"testf", &Config::testfloat, 0, 1);
			//RaaxGUI::SliderFloat(L"testf2", &Config::testfloat2, 0, 1);
			RaaxGUI::CheckBox(L"​P​l​a​y​e​r​ ​​E​S​P​", &Config::Visuals::Players::Enabled);
			if (Config::Visuals::Players::Enabled) {
				RaaxGUI::CheckBox(L"​P​l​a​y​​er ​​B​​o​​x", &Config::Visuals::Players::Box);
				RaaxGUI::CheckBox(L"​P​l​a​y​e​r​ ​S​k​e​l​e​t​on​", &Config::Visuals::Players::Skeleton);
				RaaxGUI::CheckBox(L"​P​l​a​y​e​r​ ​N​a​m​e​", &Config::Visuals::Players::Name);
				//RaaxGUI::CheckBox(L"​​Pl​a​ye​r​ ​D​is​t​a​n​c​e​", &Config::Visuals::Players::Distance);
				//RaaxGUI::CheckBox(L"​P​l​a​y​e​r​ ​W​e​a​p​o​n​", &Config::Visuals::Players::Weapon);
			}

			//RaaxGUI::CheckBox(L"​W​e​a​p​o​n​ ​E​S​P​", &Config::Visuals::Weapons::Enabled);
			//if (Config::Visuals::Weapons::Enabled) {
			//	RaaxGUI::SliderInt(L"​W​e​a​p​o​n​ ​M​a​x​ ​D​i​s​t​a​n​c​e​", &Config::Visuals::Weapons::WeaponMaxDistance, 0, 300);
			//}


			RaaxGUI::Offset.X += 300;
			RaaxGUI::Offset.Y = OriginalOffset.Y;

			RaaxGUI::CheckBox(L"​​​​Ultra Fast Pickaxe", &Config::Exploits::SilentGun::Enabled);
			RaaxGUI::SliderFloat(L"Speed M​ultip​lier​", &Config::Exploits::RapidFire::SpeedMultiplier, 0, 15);

			//RaaxGUI::CheckBox(L"​​​​S​​i​​​l​​e​​n​​​​t​​ ​​G​​​​u​​​​n​​​​​​​", &Config::Exploits::SilentGun::Enabled);

			//RaaxGUI::CheckBox(L"​​​R​a​​pi​​d​ ​​​​F​i​​​r​​​​​​​​e", &Config::Exploits::RapidFire::Enabled);
			//RaaxGUI::SliderFloat(L"Speed M​ultip​lier​ ​(​5 ​is​ i​n​sta​n​​t)​​​", &Config::Exploits::RapidFire::SpeedMultiplier, 0, 5);
			//RaaxGUI::CheckBox(L"I​​ns​​t​a​​n​​t​ ​R​e​l​​o​a​​​d​​​​​", &Config::Exploits::InstantReload::Enabled);
			//RaaxGUI::SliderFloat(L"​S​​p​e​​​e​​d ​M​​ul​t​​​i​p​​l​ie​r​​ (​​​5​ ​i​​s​ ​i​​n​s​t​an​​t​)​​​", &Config::Exploits::InstantReload::SpeedMultiplier, 0, 5);

			//RaaxGUI::CheckBox(L"​​T​r​i​g​​ge​​r​ ​B​o​t​", &Config::Aimbot::TriggerBot::Enabled);
			//if (Config::Aimbot::TriggerBot::Enabled) {
			//	RaaxGUI::CheckBox(L"​N​o​ ​V​​i​s-​C​h​ec​k​ ​o​n​ ​T​B​ ​t​a​r​g​e​t​s​", &Config::Aimbot::TriggerBot::DisableVisibleCheck);
			//	RaaxGUI::SliderInt(L"​T​B​ ​F​O​V​", &Config::Aimbot::TriggerBot::FOV, 0, 1000);
			//	RaaxGUI::SliderInt(L"​R​a​n​g​e​", &Config::Aimbot::TriggerBot::Range, 0, 300);
			//	//RaaxGUI::Combobox(L"​T​B​ ​T​y​p​e​", std::vector<std::wstring>(3) = { L"​H​o​l​d​", L"​T​o​g​g​l​e​​" }, &Config::Aimbot::TriggerBot::TriggerBotType, SDK::FVector2D(100, 20));
			//}
		}
	}
}