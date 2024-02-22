#include "Game.h"

#include "../Drawing/RaaxGUI/RaaxGUI.h"

#include "Actors/Actors.h"
#include "../Game/Input/Input.h"
#include "../Utilities/Logger.h"
#include "../Drawing/Drawing.h"

bool test = false;
bool TOGGLEWINDOW = true;

void Game::DrawCallback() {
	Actors::FortWeapon::Tick();
	Actors::BuildingWeakSpot::Tick();
	Actors::FortPawn::Tick();

	if (Input::WasKeyJustReleased(Input::KeyName::Insert)) {
		test = !test;
	}

	if (Input::WasKeyJustReleased(Input::KeyName::F1)) {
		test = !test;

		DEBUG_LOG("START");

		DEBUG_LOG(std::to_string(*reinterpret_cast<uintptr_t*>(SDK::GetLocalController())));
		DEBUG_LOG(std::to_string(**reinterpret_cast<uintptr_t**>(SDK::GetLocalController())));
		DEBUG_LOG(std::to_string(reinterpret_cast<uintptr_t>(SDK::GetLocalController())));

		DEBUG_LOG("------");

		DEBUG_LOG(std::to_string(*reinterpret_cast<uintptr_t*>(SDK::GetLocalPlayer())));
		DEBUG_LOG(std::to_string(**reinterpret_cast<uintptr_t**>(SDK::GetLocalPlayer())));
		DEBUG_LOG(std::to_string(reinterpret_cast<uintptr_t>(SDK::GetLocalPlayer())));

		DEBUG_LOG("END");
	}

	RaaxGUI::NewFrame();

	RaaxGUI::BeginWindow(skCrypt("Raax").decrypt(), &test, RaaxGUI::RaaxGUIWindowFlags_None, SDK::FVector2D(rand() % 540, rand() % 540), SDK::FVector2D(rand() % 540, rand() % 540));
	RaaxGUI::EndWindow();

	RaaxGUI::BeginWindow(skCrypt("Rgggaax").decrypt(), &test, RaaxGUI::RaaxGUIWindowFlags_None, SDK::FVector2D(rand() % 540, rand() % 540), SDK::FVector2D(rand() % 540, rand() % 540));
	RaaxGUI::EndWindow();

	RaaxGUI::BeginWindow(skCrypt("Raax33").decrypt(), &test, RaaxGUI::RaaxGUIWindowFlags_None, SDK::FVector2D(rand() % 540, rand() % 540), SDK::FVector2D(rand() % 540, rand() % 540));
	RaaxGUI::EndWindow();

	if (TOGGLEWINDOW) {
		RaaxGUI::BeginWindow(skCrypt("Raax3gdfgfg3").decrypt(), &test, RaaxGUI::RaaxGUIWindowFlags_None, SDK::FVector2D(rand() % 540, rand() % 540), SDK::FVector2D(rand() % 540, rand() % 540));
		RaaxGUI::EndWindow();
	}

	RaaxGUI::Render();

	if (test) {
		Drawing::Circle(Input::GetMousePosition(), 4.f, 16, SDK::FLinearColor(1.f, 1.f, 1.f, 1.f), true);
	}

	if (Input::WasKeyJustReleased(Input::KeyName::F2)) {
		TOGGLEWINDOW = !TOGGLEWINDOW;
	}




	if (Input::WasKeyJustReleased(Input::KeyName::F5)) {
		void* test = SDK::GetLocalPlayer();
	}
}