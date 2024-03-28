#pragma once
#include <vector>
#include <chrono>
#include <memory>

#include "../../SDK/Classes/FortniteGame_Classes.h"

namespace Features {
    namespace FortPawnHelper {
        /* Manager for chams (enabling, disabling, initializing etc) */
        namespace Chams {
            struct ChammedPawn {
                SDK::AFortPawn* FortPawn;
                std::chrono::time_point<std::chrono::steady_clock> LastUpdateTime;

                ChammedPawn(SDK::AFortPawn* FortPawn, std::chrono::time_point<std::chrono::steady_clock> LastUpdateTime) : FortPawn(FortPawn), LastUpdateTime(LastUpdateTime) {}

                const bool operator==(const SDK::AFortPawn* FortPawn) const {
                    return this->FortPawn == FortPawn;
                }
            };

            inline SDK::FName SColor1;
            inline SDK::FName SColor2;
            inline SDK::FName DissolvePatternEmissiveBrightness;
            inline SDK::FName GradientPassEmissiveA;
            inline SDK::FName GradientPassEmissiveB;

            /* Array of ChammedPawns used for auto reverting materials */
            inline std::vector<std::unique_ptr<ChammedPawn>> ChammedPawns;



            /*
            * @brief Check if the chams should be applied to the pawn
            * 
            * @param FortPawn The FortPawn to check if chams should be applied
            * 
            * @return If the chams should be applied
            */
            bool ShouldApplyChams(SDK::AFortPawn* FortPawn);

            /*
            * @brief Apply chams to the pawn
            * 
            * @param FortPawn The FortPawn to apply chams to
            * @param Material The material to apply to the pawn
            * 
            * @return If the chams were applied
            */
            bool ApplyChams(SDK::AFortPawn* FortPawn, SDK::UMaterialInstanceDynamic* Material, bool SendingNullptrMaterial = false);

            /*
            * @brief Remove chams from the pawn
            * 
            * @param FortPawn The FortPawn to remove chams from
            */
            void RemoveChams(SDK::AFortPawn* FortPawn);

            /**
            * @brief Tick the chams
            * This function will apply chams if they should be applied, update chams if they are already applied, and remove chams if they should not be applied
            * 
            * @param FortPawn The FortPawn to tick
            */
            void Tick(SDK::AFortPawn* FortPawn);

            /* Update the UMaterialInstanceDynamic settings */
            void UpdateDynamicMaterialSettings();

            /* Update the UMaterial settings */
            void UpdateMaterialSettings();

            /* Revert all chams */
            void RevertAll();

            /* Initiate the chams */
            void Init();
        };
    }
}