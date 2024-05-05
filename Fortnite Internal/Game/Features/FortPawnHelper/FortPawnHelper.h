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

		/*
		* @brief Populate the bound corners of the pawn
		* 
		* @param FortPawnCache The FortPawn to populate the bound corners of
		* @param BottomLeft The bottom left corner of the pawn
		* @param TopRight The top right corner of the pawn
		*/
		void PopulateBoundCorners(Actors::Caches::FortPawnCache& FortPawnCache, SDK::FVector2D& BottomLeft, SDK::FVector2D& TopRight);
	};
}