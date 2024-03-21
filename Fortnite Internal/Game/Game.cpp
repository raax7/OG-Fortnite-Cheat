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
#include "../Utilities/Logger.h"

bool MenuOpen = true;

void Game::MenuCallback() {
	if (!SDK::GetLocalPlayer()) return;
	if (!SDK::GetLocalController()) return;
	if (!SDK::GetEngine()) return;

	if (Input::WasKeyJustReleased(Input::KeyName::Insert)) {
		MenuOpen = !MenuOpen;
	}

	static int Tab = 0;
	static int SubTab = 0;

	static bool WaitingForKeyInput = false;

#ifdef _ENGINE
	RaaxGUI::NewFrame();

	if (RaaxGUI::BeginWindow(skCrypt("Aimbot Settings"), &MenuOpen, RaaxGUI::RaaxGUIWindowFlags_None, SDK::FVector2D(rand() % 540, rand() % 540), SDK::FVector2D(300, 500))) {
		if (RaaxGUI::Checkbox(skCrypt("Aimbot"), &Config::Aimbot::Enabled)) {
			if (RaaxGUI::Checkbox(skCrypt("Silent Aim"), &Config::Aimbot::SilentAim)) {
				RaaxGUI::Checkbox(skCrypt("Use Aimkey For Silent"), &Config::Aimbot::UseAimKeyForSilent);
			}

			RaaxGUI::Checkbox(skCrypt("Show Aim Line"), &Config::Aimbot::ShowAimLine);
			RaaxGUI::Checkbox(skCrypt("Show FOV"), &Config::Aimbot::ShowFOV);

			RaaxGUI::Checkbox(skCrypt("Standard"), &Config::Aimbot::Standard::Enabled);
			if (Config::Aimbot::Standard::Enabled) {
				RaaxGUI::SliderInt(skCrypt("Standard FOV"), &Config::Aimbot::Standard::FOV, 0, 180);
				RaaxGUI::SliderFloat(skCrypt("Standard Smoothing"), &Config::Aimbot::Standard::Smoothing, 1.f, 20.f);
			}

			RaaxGUI::Checkbox(skCrypt("Close Aim"), &Config::Aimbot::CloseAim::Enabled);
			if (Config::Aimbot::CloseAim::Enabled) {
				RaaxGUI::SliderInt(skCrypt("Close FOV"), &Config::Aimbot::CloseAim::FOV, 0, 180);
				RaaxGUI::SliderFloat(skCrypt("Smoothing"), &Config::Aimbot::CloseAim::Smoothing, 1.f, 20.f);
			}

			RaaxGUI::Checkbox(skCrypt("Weakspot"), &Config::Aimbot::Weakspot::Enabled);
			if (Config::Aimbot::Weakspot::Enabled) {
				RaaxGUI::SliderInt(skCrypt("Weakspot FOV"), &Config::Aimbot::Weakspot::FOV, 0, 180);
				RaaxGUI::SliderFloat(skCrypt("Weakspot Smoothing"), &Config::Aimbot::Weakspot::Smoothing, 1.f, 20.f);
			}
		}
	}
	RaaxGUI::EndWindow();

	if (RaaxGUI::BeginWindow(skCrypt("Visual Settings"), &MenuOpen, RaaxGUI::RaaxGUIWindowFlags_None, SDK::FVector2D(rand() % 540, rand() % 540), SDK::FVector2D(250, 350))) {
		RaaxGUI::Checkbox(skCrypt("Weapon ESP"), &Config::Visuals::Weapons::Enabled);

		RaaxGUI::Checkbox(skCrypt("Player ESP"), &Config::Visuals::Players::Enabled);
		if (Config::Visuals::Players::Enabled) {
			RaaxGUI::Checkbox(skCrypt("Box"), &Config::Visuals::Players::Box);
			RaaxGUI::Checkbox(skCrypt("Skeleton"), &Config::Visuals::Players::Skeleton);
			RaaxGUI::Checkbox(skCrypt("Distance"), &Config::Visuals::Players::Distance);
			RaaxGUI::Checkbox(skCrypt("Name"), &Config::Visuals::Players::Name);
		}
	}
	RaaxGUI::EndWindow();

	if (RaaxGUI::BeginWindow(skCrypt("Exploit Settings"), &MenuOpen, RaaxGUI::RaaxGUIWindowFlags_None, SDK::FVector2D(rand() % 540, rand() % 540), SDK::FVector2D(250, 350))) {
		// Sub Tabs
		// the worlds GOOFIEST tab switcher
		RaaxGUI::SliderInt(skCrypt("Sub Tab"), &SubTab, 0, 4);

		switch (SubTab) {
		case 0:
		{
			RaaxGUI::Checkbox(skCrypt("Infinite Builds (client sided 99% of the time)"), &Config::Exploits::Player::InfiniteBuilds);
			RaaxGUI::Checkbox(skCrypt("Infinite Ammo (client sided 99% of the time)"), &Config::Exploits::Player::InfiniteAmmo);
			RaaxGUI::Checkbox(skCrypt("Edit Enemy Builds"), &Config::Exploits::Player::EditEnemyBuilds);
		}
		break;
		case 1:
		{
			RaaxGUI::Checkbox(skCrypt("No Spread"), &Config::Exploits::Weapon::NoSpread);
			RaaxGUI::Checkbox(skCrypt("No Recoil"), &Config::Exploits::Weapon::NoRecoil);
			RaaxGUI::Checkbox(skCrypt("No Reload"), &Config::Exploits::Weapon::NoReload);
			RaaxGUI::Checkbox(skCrypt("Rapid Fire"), &Config::Exploits::Weapon::RapidFire);
			RaaxGUI::Checkbox(skCrypt("Damage Multiplier"), &Config::Exploits::Weapon::UseDamageMultiplier);
			//ImGui::SliderFloat(skCrypt("Rapid Fire Amount"), &Config::Exploits::Weapon::RapidFireAmount, 1.f, 100.f);
			if (Config::Exploits::Weapon::UseDamageMultiplier) {
				RaaxGUI::SliderInt(skCrypt("Amount"), &Config::Exploits::Weapon::DamageMultiplier, 1, 100);
			}
		}
		break;
		case 2:
		{
			RaaxGUI::Checkbox(skCrypt("Fast Pickaxe"), &Config::Exploits::Pickaxe::FastPickaxe);
			if (Config::Exploits::Pickaxe::FastPickaxe) {
				//RaaxGUI::SameLine();
				RaaxGUI::SliderFloat(skCrypt("Speed"), &Config::Exploits::Pickaxe::SpeedMultiplier, 0.f, 25.f);
			}
		}
		break;
		case 3:
		{
			RaaxGUI::Checkbox(skCrypt("Infinite Boost"), &Config::Exploits::Vehicle::InfiniteBoost);
			RaaxGUI::Checkbox(skCrypt("Fly"), &Config::Exploits::Vehicle::Fly);
			if (Config::Exploits::Vehicle::Fly) {
				RaaxGUI::Checkbox(skCrypt("Fly Through Walls (also makes flying better)"), &Config::Exploits::Vehicle::FlyThroughWalls);
				RaaxGUI::Checkbox(skCrypt("Freeze In Air"), &Config::Exploits::Vehicle::FreezeInAir);
				RaaxGUI::Checkbox(skCrypt("No Tilting"), &Config::Exploits::Vehicle::NoTilting);
				RaaxGUI::SliderFloat(skCrypt("Fly Speed"), &Config::Exploits::Vehicle::FlySpeed, 35.f, 1000.f);
			}
		}
		break;
		case 4:
		{
			RaaxGUI::Checkbox(skCrypt("Prioritize Farthest Weapons"), &Config::Exploits::Pickup::PrioritizeFarthestWeapons);

			RaaxGUI::Checkbox(skCrypt("Auto Pickup"), &Config::Exploits::Pickup::AutoPickup);
			if (Config::Exploits::Pickup::AutoPickup) {
				RaaxGUI::SliderFloat(skCrypt("Auto Pickup Delay (MS)"), &Config::Exploits::Pickup::AutoPickupDelaySecs, 0.f, 10.f);
			}

			RaaxGUI::SliderInt(skCrypt("Max Pickup Distance"), &Config::Exploits::Pickup::MaxDistance, 1, 500);
			RaaxGUI::SliderInt(skCrypt("Max Pickup Amount"), &Config::Exploits::Pickup::MaxItems, 1, 500);
		}
		break;
		}
	}
	RaaxGUI::EndWindow();

	RaaxGUI::EndFrame();
#endif

#ifdef _IMGUI
	if (MenuOpen) {
		ImGui::Begin(skCrypt("Settings"));

		// Menu Tabs
		if (ImGui::Button(skCrypt("Aimbot"), ImVec2(150, 30))) {
			Tab = 0;
			SubTab = 0;
			WaitingForKeyInput = false;
		}
		ImGui::SameLine();
		if (ImGui::Button(skCrypt("Visuals"), ImVec2(150, 30))) {
			Tab = 1;
			SubTab = 0;
			WaitingForKeyInput = false;
		}
		ImGui::SameLine();
		if (ImGui::Button(skCrypt("Exploits"), ImVec2(150, 30))) {
			Tab = 2;
			SubTab = 0;
			WaitingForKeyInput = false;
		}

		switch (Tab) {
		case 0:
		{
			ImGui::Checkbox(skCrypt("Aimbot Enabled"), &Config::Aimbot::Enabled);
			if (Config::Aimbot::Enabled) {
				std::string ButtonName = std::string(skCrypt("None"));
				if ((uint8)Config::Aimbot::AimKey) {
					ButtonName = std::string(skCrypt("Aim Key: ")) + Input::GetKeyNameString((Input::KeyName)Config::Aimbot::AimKey);
				}

				if (WaitingForKeyInput) {
					ButtonName = std::string(skCrypt("..."));

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

				if (SDK::Cached::Functions::CalculateShot) {
					ImGui::Checkbox(skCrypt("Bullet TP"), &Config::Aimbot::BulletTP);
				}

				ImGui::Checkbox(skCrypt("Silent Aim"), &Config::Aimbot::SilentAim);
				if (Config::Aimbot::SilentAim) {
					ImGui::Checkbox(skCrypt("Use Aim-Key For Silent"), &Config::Aimbot::UseAimKeyForSilent);
				}
				ImGui::Checkbox(skCrypt("Show Aim Line"), &Config::Aimbot::ShowAimLine);
				ImGui::Checkbox(skCrypt("Show FOV"), &Config::Aimbot::ShowFOV);

				ImGui::Checkbox(skCrypt("Sticky Aim"), &Config::Aimbot::StickyAim);
				ImGui::Checkbox(skCrypt("Visible Check"), &Config::Aimbot::VisibleCheck);

				ImGui::Checkbox(skCrypt("Standard"), &Config::Aimbot::Standard::Enabled);
				if (Config::Aimbot::Standard::Enabled) {
					ImGui::SliderInt(skCrypt("Standard FOV"), &Config::Aimbot::Standard::FOV, 0, 180);
					ImGui::SliderFloat(skCrypt("Standard Smoothing"), &Config::Aimbot::Standard::Smoothing, 1.f, 20.f);
				}

				ImGui::Checkbox(skCrypt("Close Aim"), &Config::Aimbot::CloseAim::Enabled);
				if (Config::Aimbot::CloseAim::Enabled) {
					ImGui::SliderInt(skCrypt("Close FOV"), &Config::Aimbot::CloseAim::FOV, 0, 180);
					ImGui::SliderFloat(skCrypt("Smoothing"), &Config::Aimbot::CloseAim::Smoothing, 1.f, 20.f);
				}

				ImGui::Checkbox(skCrypt("Weakspot"), &Config::Aimbot::Weakspot::Enabled);
				if (Config::Aimbot::Weakspot::Enabled) {
					ImGui::SliderInt(skCrypt("Weakspot FOV"), &Config::Aimbot::Weakspot::FOV, 0, 180);
					ImGui::SliderFloat(skCrypt("Weakspot Smoothing"), &Config::Aimbot::Weakspot::Smoothing, 1.f, 20.f);
				}
			}
		}
		break;
		case 1:
		{
			ImGui::Checkbox(skCrypt("Weapon ESP"), &Config::Visuals::Weapons::Enabled);

			ImGui::Checkbox(skCrypt("Player ESP"), &Config::Visuals::Players::Enabled);
			if (Config::Visuals::Players::Enabled) {
				ImGui::Checkbox(skCrypt("Box"), &Config::Visuals::Players::Box);
				ImGui::Checkbox(skCrypt("Skeleton"), &Config::Visuals::Players::Skeleton);
				ImGui::Checkbox(skCrypt("Distance"), &Config::Visuals::Players::Distance);
				ImGui::Checkbox(skCrypt("Name"), &Config::Visuals::Players::Name);
			}
		}
		break;
		case 2:
		{
			// Sub Tabs
			if (ImGui::Button(skCrypt("Player"), ImVec2(80, 25))) {
				SubTab = 0;
			}
			ImGui::SameLine();
			if (ImGui::Button(skCrypt("Weapon"), ImVec2(80, 25))) {
				SubTab = 1;
			}
			ImGui::SameLine();
			if (ImGui::Button(skCrypt("Pickaxe"), ImVec2(80, 25))) {
				SubTab = 2;
			}
			ImGui::SameLine();
			if (ImGui::Button(skCrypt("Vehicle"), ImVec2(80, 25))) {
				SubTab = 3;
			}
			ImGui::SameLine();
			if (ImGui::Button(skCrypt("Pickup"), ImVec2(80, 25))) {
				SubTab = 4;
			}

			switch (SubTab) {
			case 0:
			{
				ImGui::Checkbox(skCrypt("Infinite Builds (client sided 99% of the time)"), &Config::Exploits::Player::InfiniteBuilds);
				ImGui::Checkbox(skCrypt("Infinite Ammo (client sided 99% of the time)"), &Config::Exploits::Player::InfiniteAmmo);
				ImGui::Checkbox(skCrypt("Edit Enemy Builds"), &Config::Exploits::Player::EditEnemyBuilds);
			}
			break;
			case 1:
			{
				ImGui::Checkbox(skCrypt("No Spread"), &Config::Exploits::Weapon::NoSpread);
				ImGui::Checkbox(skCrypt("No Recoil"), &Config::Exploits::Weapon::NoRecoil);
				ImGui::Checkbox(skCrypt("No Reload"), &Config::Exploits::Weapon::NoReload);
				ImGui::Checkbox(skCrypt("Rapid Fire"), &Config::Exploits::Weapon::RapidFire);
				ImGui::Checkbox(skCrypt("Damage Multiplier"), &Config::Exploits::Weapon::UseDamageMultiplier);
				//ImGui::SliderFloat(skCrypt("Rapid Fire Amount"), &Config::Exploits::Weapon::RapidFireAmount, 1.f, 100.f);
				if (Config::Exploits::Weapon::UseDamageMultiplier) {
					ImGui::SliderInt(skCrypt("Amount"), &Config::Exploits::Weapon::DamageMultiplier, 1, 100);
				}
			}
			break;
			case 2:
			{
				ImGui::Checkbox(skCrypt("Fast Pickaxe"), &Config::Exploits::Pickaxe::FastPickaxe);
				if (Config::Exploits::Pickaxe::FastPickaxe) {
					ImGui::SameLine();
					ImGui::SliderFloat(skCrypt("Speed"), &Config::Exploits::Pickaxe::SpeedMultiplier, 0.f, 25.f);
				}
			}
			break;
			case 3:
			{
				ImGui::Checkbox(skCrypt("Infinite Boost"), &Config::Exploits::Vehicle::InfiniteBoost);
				ImGui::Checkbox(skCrypt("Fly"), &Config::Exploits::Vehicle::Fly);
				if (Config::Exploits::Vehicle::Fly) {
					ImGui::Checkbox(skCrypt("Fly Through Walls (also makes flying better)"), &Config::Exploits::Vehicle::FlyThroughWalls);
					ImGui::Checkbox(skCrypt("Freeze In Air"), &Config::Exploits::Vehicle::FreezeInAir);
					ImGui::Checkbox(skCrypt("No Tilting"), &Config::Exploits::Vehicle::NoTilting);
					ImGui::SliderFloat(skCrypt("Fly Speed"), &Config::Exploits::Vehicle::FlySpeed, 35.f, 1000.f);
				}
			}
			break;
			case 4:
			{
				std::string ButtonName = std::string(skCrypt("None"));
				if ((uint8)Config::Exploits::Pickup::PickupAllKey) {
					ButtonName = std::string(skCrypt("Pickup Key: ")) + Input::GetKeyNameString((Input::KeyName)Config::Exploits::Pickup::PickupAllKey);
				}

				if (WaitingForKeyInput) {
					ButtonName = std::string(skCrypt("..."));

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

				ImGui::Checkbox(skCrypt("Prioritize Farthest Weapons"), &Config::Exploits::Pickup::PrioritizeFarthestWeapons);

				ImGui::Checkbox(skCrypt("Auto Pickup"), &Config::Exploits::Pickup::AutoPickup);
				if (Config::Exploits::Pickup::AutoPickup) {
					ImGui::SliderFloat(skCrypt("Auto Pickup Delay (MS)"), &Config::Exploits::Pickup::AutoPickupDelaySecs, 0.f, 10.f);
				}

				ImGui::SliderInt(skCrypt("Max Pickup Distance"), &Config::Exploits::Pickup::MaxDistance, 1, 500);
				ImGui::SliderInt(skCrypt("Max Pickup Amount"), &Config::Exploits::Pickup::MaxItems, 1, 500);
			}
			break;
			}
		}
		break;
		}

		ImGui::End();
	}
#endif

	if (MenuOpen) {
		// Draw cursor
		SDK::FVector2D MousePos = Input::GetMousePosition();
		Drawing::Circle(MousePos, 5.f, 16, SDK::FLinearColor(1.f, 1.f, 1.f, 1.f), true);
	}
}
void Game::DrawCallback() {
	if (!SDK::GetLocalPlayer()) return;
	if (!SDK::GetLocalController()) return;
	if (!SDK::GetEngine()) return;

	Actors::FortWeapon::Tick();
	Actors::BuildingWeakSpot::Tick();
	Actors::FortPawn::Tick();

	Actors::Draw();
}