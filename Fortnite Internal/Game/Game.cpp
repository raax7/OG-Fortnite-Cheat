#include "Game.h"
#include "Actors/Actors.h"
#include "../Drawing/Menu/Menu.h"

void Game::DrawCallback() {
	Actors::FortWeapon::Tick();
	Actors::BuildingWeakSpot::Tick();
	Actors::FortPawn::Tick();

	RaaxGUI::Tick();
}