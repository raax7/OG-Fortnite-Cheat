#include "Features.h"

#include "Visuals/Chams.h"

void Features::RevertAll()
{
    AutoRevertFeatures.clear();

    // Revert all features
    Features::Visuals::ChamManagerFortPawn::Manager->RevertAll();
    Features::Visuals::ChamManagerFortPickup::Manager->RevertAll();
}