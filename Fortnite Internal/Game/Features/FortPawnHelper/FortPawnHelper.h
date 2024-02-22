#pragma once
#include "../../Actors/ActorCache.h"

namespace Features {
	namespace FortPawnHelper {
		/*
		* @brief Poplate all bone positions used in the skeleton
		*
		* @param FortPawn The FortPawn to populate bones
		*/
		bool PopulateBones(Actors::Caches::FortPawnCache& FortPawnCache);

		/*
		* @brief Poplate all bone visibilities used in the skeleton
		*
		* @param FortPawn The FortPawn to populate visibilities
		*/
		void PopulateVisibilities(Actors::Caches::FortPawnCache& FortPawnCache);
	};
}