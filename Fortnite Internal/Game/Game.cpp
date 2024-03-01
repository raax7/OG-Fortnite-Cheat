#include "Game.h"

#include "../Drawing/RaaxGUI/RaaxGUI.h"

#include "Actors/Actors.h"
#include "../Game/Input/Input.h"
#include "../Utilities/Logger.h"
#include "../Drawing/Drawing.h"
#include "SDK/Classes/Engine_Classes.h"
#include "../Configs/Config.h"

bool test = false;
bool test123 = false;
bool TOGGLEWINDOW = true;

void Game::DrawCallback() {
	Actors::FortWeapon::Tick();
	Actors::BuildingWeakSpot::Tick();
	Actors::FortPawn::Tick();

	if (Input::WasKeyJustReleased(Input::KeyName::Insert)) {
		test = !test;
	}

	RaaxGUI::NewFrame();

	if (RaaxGUI::BeginWindow(skCrypt("Aimbot Settings").decrypt(), &test, RaaxGUI::RaaxGUIWindowFlags_None, SDK::FVector2D(rand() % 540, rand() % 540), SDK::FVector2D(300, 500))) {
		if (RaaxGUI::Checkbox(skCrypt("Aimbot").decrypt(), &Config::Aimbot::Enabled)) {
			if (RaaxGUI::Checkbox(skCrypt("Silent Aim").decrypt(), &Config::Aimbot::SilentAim)) {
				RaaxGUI::Checkbox(skCrypt("Use Aimkey For Silent").decrypt(), &Config::Aimbot::UseAimKeyForSilent);
			}

			RaaxGUI::Checkbox(skCrypt("Show Aim Line").decrypt(), &Config::Aimbot::ShowAimLine);
			RaaxGUI::Checkbox(skCrypt("Show FOV").decrypt(), &Config::Aimbot::ShowFOV);

			if (RaaxGUI::Checkbox(skCrypt("Standard").decrypt(), &Config::Aimbot::Standard::Enabled)) {
				//RaaxGUI::SliderFloat(skCrypt("FOV").decrypt(), &Config::Aimbot::Standard::FOV, 0.f, 1000.f);
				//RaaxGUI::SliderFloat(skCrypt("Smoothing").decrypt(), &Config::Aimbot::Standard::Smoothing, 0.f, 100.f);
			}

			if (RaaxGUI::Checkbox(skCrypt("Close Aim").decrypt(), &Config::Aimbot::CloseAim::Enabled)) {
				//RaaxGUI::SliderFloat(skCrypt("FOV").decrypt(), &Config::Aimbot::CloseAim::FOV, 0.f, 1000.f);
				//RaaxGUI::SliderFloat(skCrypt("Smoothing").decrypt(), &Config::Aimbot::CloseAim::Smoothing, 0.f, 100.f);
			}

			if (RaaxGUI::Checkbox(skCrypt("Weakspot").decrypt(), &Config::Aimbot::Weakspot::Enabled)) {
				//RaaxGUI::SliderFloat(skCrypt("FOV").decrypt(), &Config::Aimbot::Weakspot::FOV, 0.f, 1000.f);
				//RaaxGUI::SliderFloat(skCrypt("Smoothing").decrypt(), &Config::Aimbot::Weakspot::Smoothing, 0.f, 100.f);
			}
		}
	}
	RaaxGUI::EndWindow();
	
	if (RaaxGUI::BeginWindow(skCrypt("Visual Settings").decrypt(), &test, RaaxGUI::RaaxGUIWindowFlags_None, SDK::FVector2D(rand() % 540, rand() % 540), SDK::FVector2D(250, 350))) {
		if (RaaxGUI::Checkbox(skCrypt("Player ESP").decrypt(), &Config::Visuals::Players::Enabled)) {
			RaaxGUI::Checkbox(skCrypt("Box").decrypt(), &Config::Visuals::Players::Box);
			RaaxGUI::Checkbox(skCrypt("Skeleton").decrypt(), &Config::Visuals::Players::Skeleton);
			RaaxGUI::Checkbox(skCrypt("Distance").decrypt(), &Config::Visuals::Players::Distance);
			RaaxGUI::Checkbox(skCrypt("Name").decrypt(), &Config::Visuals::Players::Name);
		}
	}
	RaaxGUI::EndWindow();

	RaaxGUI::BeginWindow(skCrypt("Dummy Window").decrypt(), &test, RaaxGUI::RaaxGUIWindowFlags_None, SDK::FVector2D(rand() % 540, rand() % 540), SDK::FVector2D(rand() % 540, rand() % 540));
	RaaxGUI::EndWindow();

	RaaxGUI::BeginWindow(skCrypt("Dummy Window 2").decrypt(), &test, RaaxGUI::RaaxGUIWindowFlags_None, SDK::FVector2D(rand() % 540, rand() % 540), SDK::FVector2D(rand() % 540, rand() % 540));
	RaaxGUI::EndWindow();

	RaaxGUI::EndFrame();

	if (test) {
		Drawing::Circle(Input::GetMousePosition(), 4.f, 16, SDK::FLinearColor(1.f, 1.f, 1.f, 1.f), true);
	}

	if (Input::WasKeyJustReleased(Input::KeyName::F2)) {
		TOGGLEWINDOW = !TOGGLEWINDOW;
	}




	if (Input::WasKeyJustReleased(Input::KeyName::F5)) {
		void* test = SDK::GetLocalPlayer();
	}



	if (Input::WasKeyJustReleased(Input::KeyName::Up)) {
		Config::Aimbot::Standard::FOV += 10;
	}
	if (Input::WasKeyJustReleased(Input::KeyName::Down)) {
		Config::Aimbot::Standard::FOV -= 10;
	}

	if (Input::WasKeyJustReleased(Input::KeyName::Right)) {
		Config::Aimbot::Standard::Smoothing += 0.1f;
	}
	if (Input::WasKeyJustReleased(Input::KeyName::Left)) {
		Config::Aimbot::Standard::Smoothing -= 0.1f;
	}

	Drawing::Text(std::to_string(Config::Aimbot::Standard::FOV).c_str(), SDK::FVector2D(100, 100), 16.f, SDK::FLinearColor(1.f, 1.f, 1.f, 1.f), true, true, true);
	Drawing::Text(std::to_string(Config::Aimbot::Standard::Smoothing).c_str(), SDK::FVector2D(100, 120), 16.f, SDK::FLinearColor(1.f, 1.f, 1.f, 1.f), true, true, true);
	Drawing::Text(std::to_string(Actors::MainTarget.LocalInfo.DistanceFromCrosshairDegrees).c_str(), SDK::FVector2D(100, 150), 18.f, SDK::FLinearColor(1.f, 0.f, 0.f, 1.f), true, true, true);
	Drawing::Text(std::to_string(Actors::MainTarget.LocalInfo.CurrentFOVSizeDegrees).c_str(), SDK::FVector2D(100, 170), 18.f, SDK::FLinearColor(1.f, 1.f, 0.f, 1.f), true, true, true);
}