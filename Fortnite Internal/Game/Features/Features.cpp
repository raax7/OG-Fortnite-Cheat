#include "Features.h"

#include "FortPawnHelper/Chams.h"

void Features::RevertAll() {
    AutoRevertFeatures.clear();

    // Revert all features
    Features::FortPawnHelper::Chams::RevertAll();
}