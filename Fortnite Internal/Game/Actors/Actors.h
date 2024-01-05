#pragma once
#include <Windows.h>
#include <chrono>
#include "../SDK/SDK.h"

namespace Actors {
	namespace FortPawn {
		void Tick(uintptr_t Canvas_);

		inline const float intervalSeconds = 0.25;
		inline std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();


		namespace Bones {
			inline int Head = 66;
			inline int Neck = 64;

			inline int Chest = -1;
			inline int ChestLeft = 8;
			inline int ChestRight = 36;

			inline int LeftShoulder = 9;
			inline int RightShoulder = 37;
			inline int LeftElbow = 10;
			inline int RightElbow = 38;
			inline int LeftHand = 11;
			inline int RightHand = 39;

			inline int LeftLeg = 67;
			inline int RightLeg = 74;
			inline int LeftKnee = 68;
			inline int RightKnee = 75;
			inline int LeftFoot = 71;
			inline int RightFoot = 78;

			inline int Pelvis = 2;
			inline int Bottom = 0;
		}

		inline std::vector<std::pair<int, int>> bonePairs = {
			{Bones::Head, Bones::Chest},
			{Bones::Chest, Bones::LeftShoulder},
			{Bones::Chest, Bones::RightShoulder},
			{Bones::LeftShoulder, Bones::LeftElbow},
			{Bones::RightShoulder, Bones::RightElbow},
			{Bones::LeftElbow, Bones::LeftHand},
			{Bones::RightElbow, Bones::RightHand},
			{Bones::Pelvis, Bones::LeftLeg},
			{Bones::Pelvis, Bones::RightLeg},
			{Bones::LeftLeg, Bones::LeftKnee},
			{Bones::RightLeg, Bones::RightKnee},
			{Bones::LeftKnee, Bones::LeftFoot},
			{Bones::RightKnee, Bones::RightFoot},
			{Bones::Chest, Bones::Pelvis},
		};
	}

	namespace FortWeapon {
		void Tick(uintptr_t Canvas_);
	}
}