#include "Game.h"

#include "SDK/Classes/Engine_Classes.h"

#include "../Drawing/Drawing.h"

#include "Actors/Actors.h"
#include "../Game/Input/Input.h"
#include "../Configs/Config.h"

#ifdef _IMGUI
#include "../External-Libs/ImGui/imgui.h"
#endif
#ifdef _ENGINE
#include "../Drawing/RaaxGUI/RaaxGUI.h"
#endif

bool test = true;
bool test123 = false;
bool TOGGLEWINDOW = true;

bool WaitingForKeyInput = false;

int Tab = 0;

void Game::MenuCallback() {
	if (!SDK::GetLocalPlayer()) return;
	if (!SDK::GetLocalController()) return;
	if (!SDK::GetEngine()) return;

#ifdef _ENGINE
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

	RaaxGUI::EndFrame();
#endif

#ifdef _IMGUI
	if (test) {
		ImGui::Begin(skCrypt("Settings").decrypt());

		// Menu Tabs
		if (ImGui::Button(skCrypt("Aimbot").decrypt(), ImVec2(150, 30))) {
			Tab = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button(skCrypt("Visuals").decrypt(), ImVec2(150, 30))) {
			Tab = 1;
		}
		ImGui::SameLine();
		if (ImGui::Button(skCrypt("Exploits").decrypt(), ImVec2(150, 30))) {
			Tab = 2;
		}

		switch (Tab) {
		case 0:
		{
			ImGui::Checkbox(skCrypt("Aimbot Enabled").decrypt(), &Config::Aimbot::Enabled);
			if (Config::Aimbot::Enabled) {
				std::string ButtonName = skCrypt("None").decrypt();
				if ((uint8)Config::Aimbot::AimKey) {
					ButtonName = skCrypt("Aim Key: ").decrypt() + Input::GetKeyNameString((Input::KeyName)Config::Aimbot::AimKey);
				}

				if (WaitingForKeyInput) {
					ButtonName = skCrypt("...").decrypt();

					std::vector<Input::KeyName> Keys = Input::GetAllDownKeys();

					if (Keys.size() > 0) {
						for (int i = 0; i < Keys.size(); i++) {
							if (Keys[i] != Input::KeyName::AnyKey
								&& Keys[i] != Input::KeyName::MouseX
								&& Keys[i] != Input::KeyName::MouseY
								&& Keys[i] != Input::KeyName::LeftMouseButton) {
								Config::Aimbot::AimKey = (KeyName)Keys[i];
								WaitingForKeyInput = false;
							}
						}
					}
				}

				if (ImGui::Button(ButtonName.c_str(), ImVec2(250, 40))) {
					WaitingForKeyInput = true;
				}

				ImGui::Checkbox(skCrypt("Silent Aim").decrypt(), &Config::Aimbot::SilentAim);
				if (Config::Aimbot::SilentAim) ImGui::Checkbox(skCrypt("Use Aim-Key For Silent").decrypt(), &Config::Aimbot::UseAimKeyForSilent);
				ImGui::Checkbox(skCrypt("Show Aim Line").decrypt(), &Config::Aimbot::ShowAimLine);
				ImGui::Checkbox(skCrypt("Show FOV").decrypt(), &Config::Aimbot::ShowFOV);

				ImGui::Checkbox(skCrypt("Standard").decrypt(), &Config::Aimbot::Standard::Enabled);
				if (Config::Aimbot::Standard::Enabled) {
					ImGui::SliderInt(skCrypt("Standard FOV").decrypt(), &Config::Aimbot::Standard::FOV, 0, 180);
					ImGui::SliderFloat(skCrypt("Standard Smoothing").decrypt(), &Config::Aimbot::Standard::Smoothing, 1.f, 20.f);
				}

				ImGui::Checkbox(skCrypt("Close Aim").decrypt(), &Config::Aimbot::CloseAim::Enabled);
				if (Config::Aimbot::CloseAim::Enabled) {
					ImGui::SliderInt(skCrypt("Close FOV").decrypt(), &Config::Aimbot::CloseAim::FOV, 0, 180);
					ImGui::SliderFloat(skCrypt("Smoothing").decrypt(), &Config::Aimbot::CloseAim::Smoothing, 1.f, 20.f);
				}

				ImGui::Checkbox(skCrypt("Weakspot").decrypt(), &Config::Aimbot::Weakspot::Enabled);
				if (Config::Aimbot::Weakspot::Enabled) {
					ImGui::SliderInt(skCrypt("Weakspot FOV").decrypt(), &Config::Aimbot::Weakspot::FOV, 0, 180);
					ImGui::SliderFloat(skCrypt("Weakspot Smoothing").decrypt(), &Config::Aimbot::Weakspot::Smoothing, 1.f, 20.f);
				}
			}
		}
		break;
		case 1:
		{
			ImGui::Checkbox(skCrypt("Weapon ESP").decrypt(), &Config::Visuals::Weapons::Enabled);

			ImGui::Checkbox(skCrypt("Player ESP").decrypt(), &Config::Visuals::Players::Enabled);
			if (Config::Visuals::Players::Enabled) {
				ImGui::Checkbox(skCrypt("Box").decrypt(), &Config::Visuals::Players::Box);
				ImGui::Checkbox(skCrypt("Skeleton").decrypt(), &Config::Visuals::Players::Skeleton);
				ImGui::Checkbox(skCrypt("Distance").decrypt(), &Config::Visuals::Players::Distance);
				ImGui::Checkbox(skCrypt("Name").decrypt(), &Config::Visuals::Players::Name);
			}
		}
		break;
		case 2:
		{
			static int SubTab = 0;

			// Sub Tabs
			if (ImGui::Button(skCrypt("Player").decrypt(), ImVec2(80, 25))) {
				SubTab = 0;
			}
			ImGui::SameLine();
			if (ImGui::Button(skCrypt("Weapon").decrypt(), ImVec2(80, 25))) {
				SubTab = 1;
			}
			ImGui::SameLine();
			if (ImGui::Button(skCrypt("Pickaxe").decrypt(), ImVec2(80, 25))) {
				SubTab = 2;
			}
			ImGui::SameLine();
			if (ImGui::Button(skCrypt("Vehicle").decrypt(), ImVec2(80, 25))) {
				SubTab = 3;
			}
			ImGui::SameLine();
			if (ImGui::Button(skCrypt("Pickup").decrypt(), ImVec2(80, 25))) {
				SubTab = 4;
			}

			switch (SubTab) {
			case 0:
			{
				ImGui::Checkbox(skCrypt("Infinite Builds").decrypt(), &Config::Exploits::Player::InfiniteBuilds);
				ImGui::Checkbox(skCrypt("Infinite Ammo").decrypt(), &Config::Exploits::Player::InfiniteAmmo);
				ImGui::Checkbox(skCrypt("Edit Enemy Builds").decrypt(), &Config::Exploits::Player::EditEnemyBuilds);
			}
			break;
			case 1:
			{
				ImGui::Checkbox(skCrypt("No Spread").decrypt(), &Config::Exploits::Weapon::NoSpread);
				ImGui::Checkbox(skCrypt("No Recoil").decrypt(), &Config::Exploits::Weapon::NoRecoil);
				ImGui::Checkbox(skCrypt("No Reload").decrypt(), &Config::Exploits::Weapon::NoReload);
				ImGui::Checkbox(skCrypt("Rapid Fire").decrypt(), &Config::Exploits::Weapon::RapidFire);
				ImGui::Checkbox(skCrypt("Damage Multiplier").decrypt(), &Config::Exploits::Weapon::UseDamageMultiplier);
				//ImGui::SliderFloat(skCrypt("Rapid Fire Amount").decrypt(), &Config::Exploits::Weapon::RapidFireAmount, 1.f, 100.f);
				if (Config::Exploits::Weapon::UseDamageMultiplier) {
					ImGui::SameLine();
					ImGui::SliderInt(skCrypt("Multiplier Amount").decrypt(), &Config::Exploits::Weapon::DamageMultiplier, 1, 100);
				}
			}
			break;
			case 2:
			{
				ImGui::Checkbox(skCrypt("Fast Pickaxe").decrypt(), &Config::Exploits::Pickaxe::FastPickaxe);
				if (Config::Exploits::Pickaxe::FastPickaxe) {
					ImGui::SameLine();
					ImGui::SliderFloat(skCrypt("Speed").decrypt(), &Config::Exploits::Pickaxe::SpeedMultiplier, 0.f, 25.f);
				}
			}
			break;
			case 3:
			{
				ImGui::Checkbox(skCrypt("Enabled").decrypt(), &Config::Exploits::Vehicle::Enabled);

				ImGui::Checkbox(skCrypt("Infinite Boost").decrypt(), &Config::Exploits::Vehicle::InfiniteBoost);
				ImGui::Checkbox(skCrypt("Fly").decrypt(), &Config::Exploits::Vehicle::Fly);
				ImGui::Checkbox(skCrypt("Fly Through Walls").decrypt(), &Config::Exploits::Vehicle::FlyThroughWalls);
				ImGui::Checkbox(skCrypt("Freeze In Air").decrypt(), &Config::Exploits::Vehicle::FreezeInAir);
				ImGui::SliderFloat(skCrypt("Fly Speed").decrypt(), &Config::Exploits::Vehicle::FlySpeed, 35.f, 1000.f);
			}
			break;
			case 4:
			{
				std::string ButtonName = skCrypt("None").decrypt();
				if ((uint8)Config::Exploits::Pickup::PickupAllKey) {
					ButtonName = skCrypt("Pickup Key: ").decrypt() + Input::GetKeyNameString((Input::KeyName)Config::Exploits::Pickup::PickupAllKey);
				}

				if (WaitingForKeyInput) {
					ButtonName = skCrypt("...").decrypt();

					std::vector<Input::KeyName> Keys = Input::GetAllDownKeys();

					if (Keys.size() > 0) {
						for (int i = 0; i < Keys.size(); i++) {
							if (Keys[i] != Input::KeyName::AnyKey
								&& Keys[i] != Input::KeyName::MouseX
								&& Keys[i] != Input::KeyName::MouseY
								&& Keys[i] != Input::KeyName::LeftMouseButton) {
								Config::Exploits::Pickup::PickupAllKey = (KeyName)Keys[i];
								WaitingForKeyInput = false;
							}
						}
					}
				}

				if (ImGui::Button(ButtonName.c_str(), ImVec2(250, 40))) {
					WaitingForKeyInput = true;
				}

				ImGui::Checkbox(skCrypt("Prioritize Farthest Weapons").decrypt(), &Config::Exploits::Pickup::PrioritizeFarthestWeapons);

				ImGui::Checkbox(skCrypt("Auto Pickup").decrypt(), &Config::Exploits::Pickup::AutoPickup);
				if (Config::Exploits::Pickup::AutoPickup) {
					ImGui::SliderFloat(skCrypt("Auto Pickup Delay (MS)").decrypt(), &Config::Exploits::Pickup::AutoPickupDelaySecs, 0.f, 10.f);
				}

				ImGui::SliderInt(skCrypt("Max Pickup Distance").decrypt(), &Config::Exploits::Pickup::MaxDistance, 1, 500);
				ImGui::SliderInt(skCrypt("Max Pickup Amount").decrypt(), &Config::Exploits::Pickup::MaxItems, 1, 500);
			}
			break;
			}
		}
		break;
		}

		ImGui::End();
	}
#endif

	if (test) {
		Drawing::Circle(Input::GetMousePosition(), 4.f, 16, SDK::FLinearColor(1.f, 1.f, 1.f, 1.f), true);
	}
}
void Game::DrawCallback() {
	if (!SDK::GetLocalPlayer()) return;
	if (!SDK::GetLocalController()) return;
	if (!SDK::GetEngine()) return;

	Actors::FortWeapon::Tick();
	Actors::BuildingWeakSpot::Tick();
	Actors::FortPawn::Tick();

	if (Input::WasKeyJustReleased(Input::KeyName::Insert)) {
		test = !test;
	}

	Actors::Draw();
}