#pragma once
#include <chrono>
#include <memory>
#include <unordered_map>
#include <vector>

#include "../../SDK/Classes/Engine_Classes.h"

#include "../../../Configs/ConfigTypes.h"

namespace Features
{
    namespace Visuals
    {
        /** @brief Manager class for chams (applying, updating and removing) */
        class ChamManager
        {
        protected:
            /** @brief Struct to hold actor and last update time */
            struct ChamActor
            {
                SDK::AActor* Actor;
                std::chrono::time_point<std::chrono::steady_clock> LastApplyTime;

                ChamActor(SDK::AActor* Actor, std::chrono::time_point<std::chrono::steady_clock> LastApplyTime) : Actor(Actor), LastApplyTime(LastApplyTime) {}
            };

        protected:
            ConfigTypes::BaseChamsSettings& ChamSettings;                       // Reference to base chams settings

            SDK::UMaterial* ChamsMaterial = nullptr;                            // Pointer to the material
            SDK::UMaterialInstanceDynamic* ChamsMaterialInstance = nullptr;	    // Pointer to the dynamic material

            std::vector<SDK::FName> MaterialColorParameters;                    // Vector of color parameters for the material
            std::vector<SDK::FName> MaterialEmissiveParameters;			        // Vector of emissive intensity parameters for the material

            std::string MaterialName;										    // Name of the material

            std::unordered_map<SDK::AActor*, std::unique_ptr<ChamActor>> ChamActorsCache; // Cache of actors with chams

            int ReapplyChamsDelay = 1;										    // Delay in seconds to re-apply chams

            bool Initiated;													    // Flag indicating if Init() has been called

        public:
            /**
             * @brief Constructor for ChamManager
             *
             * @param ChamSettings Reference to base chams settings
             */
            ChamManager(ConfigTypes::BaseChamsSettings& ChamSettings) : Initiated(false), ChamSettings(ChamSettings) {}

            /** @brief Revert all chams */
            void RevertAll();

            /** @brief Update the dynamic material settings */
            virtual void UpdateDynamicMaterialSettings();

            /** @brief Update the material settings */
            virtual void UpdateMaterialSettings();

            /**
             * @brief Tick function for processing each actor
             *
             * @param Actor Pointer to the actor
             */
            virtual void Tick(SDK::AActor* Actor);

            /**
             * @brief Initialize the chams
             *
             * @param ColorParameters Vector of color parameters
             * @param EmissiveParameters Vector of emissive parameters
             * @param MaterialName Name of the material
             */
            virtual void Init(const std::vector<SDK::FName> MaterialColorParameters, const std::vector<SDK::FName> MaterialEmissiveParameters, const std::string MaterialName);

        protected:
            /**
             * @brief Check if chams should be applied to the actor
             *
             * @param Actor Pointer to the actor
             *
             * @return True if chams should be applied, false otherwise
             */
            bool ShouldApplyChams(SDK::AActor* Actor);

            /**
             * @brief Apply chams to the actor
             *
             * @param Actor Pointer to the actor
             * @param Material Pointer to the material
             * @param SendingNullptrMaterial Flag indicating if sending nullptr material
             *
             * @return True if chams applied successfully, false otherwise
             */
            bool ApplyChams(SDK::AActor* Actor, SDK::UMaterialInstanceDynamic* Material, const bool SendingNullptrMaterial = false);

            /**
             * @brief Remove chams from the actor
             *
             * @param Actor Pointer to the actor
             */
            void RemoveChams(SDK::AActor* Actor);

            /**
             * @brief Check if dynamic material is valid
             *
             * @param DynamicMaterial Pointer to the dynamic material
             *
             * @return True if dynamic material is valid, false otherwise
             */
            bool IsDynamicMaterialValid(SDK::UMaterialInstanceDynamic* DynamicMaterial);

            /**
             * @brief Get the material
             *
             * @return Pointer to the material
             */
            SDK::UMaterial* GetMaterial();

            /**
             * @brief Get the dynamic material
             *
             * @return Pointer to the dynamic material
             */
            SDK::UMaterialInstanceDynamic* GetDynamicMaterial();

        private:
            /**
             * @brief Custom function to determine if chams should be applied to the actor
             *
             * @param Actor Pointer to the actor
             *
             * @return True if chams should be applied, false otherwise
             */
            virtual bool ShouldApplyChamsCustom(SDK::AActor* Actor) = 0;

            /**
             * @brief Get the mesh components of the actor
             *
             * @param Actor Pointer to the actor
             *
             * @return Vector of mesh components
             */
            virtual std::vector<SDK::UMeshComponent*> GetActorMeshes(SDK::AActor* Actor) = 0;
        };

        /** @brief Manager class for chams (applying, updating and removing) for AFortPawns */
        class ChamManagerFortPawn : public ChamManager
        {
        public:
            /** @brief Pointer to the manager class for AFortPawns */
            static ChamManagerFortPawn* Manager;

        public:
            /**
             * @brief Constructor for ChamManagerFortPawn
             *
             * @param ChamSettings Reference to base chams settings
             */
            ChamManagerFortPawn(ConfigTypes::BaseChamsSettings& ChamSettings) : ChamManager(ChamSettings) {}

        private:
            /**
             * @brief Get the mesh components of the actor
             *
             * @param Actor Pointer to the actor
             *
             * @return Vector of mesh components
             */
            std::vector<SDK::UMeshComponent*> GetActorMeshes(SDK::AActor* Actor) override;

            /**
             * @brief Custom function to determine if chams should be applied to the actor
             *
             * @param Actor Pointer to the actor
             *
             * @return True if chams should be applied, false otherwise
             */
            bool ShouldApplyChamsCustom(SDK::AActor* Actor) override;
        };

        /** @brief Manager class for chams (applying, updating and removing) for AFortPickups */
        class ChamManagerFortPickup : public ChamManager
        {
        public:
            /** @brief Pointer to the manager class for AFortPawns */
            static ChamManagerFortPickup* Manager;

        public:
            /**
             * @brief Constructor for ChamManagerFortPickup
             *
             * @param ChamSettings Reference to pickup chams settings
             */
            ChamManagerFortPickup(ConfigTypes::PickupChamsSettings& ChamSettings) : ChamManager(ChamSettings) {}

        private:
            /**
             * @brief Get the mesh components of the actor
             *
             * @param Actor Pointer to the actor
             *
             * @return Vector of mesh components
             */
            std::vector<SDK::UMeshComponent*> GetActorMeshes(SDK::AActor* Actor) override;

            /**
             * @brief Custom function to determine if chams should be applied to the actor
             *
             * @param Actor Pointer to the actor
             *
             * @return True if chams should be applied, false otherwise
             */
            bool ShouldApplyChamsCustom(SDK::AActor* Actor) override;
        };
    }
}