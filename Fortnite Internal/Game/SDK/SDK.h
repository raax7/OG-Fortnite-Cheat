#ifndef SDK_H
#define SDK_H

#include <Windows.h>
#include "Classes/CoreUObject_classes.h"

namespace SDK {
	namespace Cached {
		namespace VFT {
			inline uintptr_t PostRender;
			inline uintptr_t ProcessEvent;
			inline uintptr_t GetPlayerViewpoint;
			inline uintptr_t GetViewpoint;
		}

		namespace Offsets {
			namespace Canvas {
				inline uintptr_t SizeX;
				inline uintptr_t SizeY;
			}

			namespace Character {
				inline uintptr_t Mesh;
			}

			namespace Pawn {
				inline uintptr_t PlayerState;
			}

			namespace Font {
				inline uintptr_t LegacyFontSize;
			}

			namespace Engine {
				inline uintptr_t GameViewport;
			}

			namespace GameViewportClient {
				inline uintptr_t World;
				inline uintptr_t GameInstance;
			}

			namespace GameInstance {
				inline uintptr_t LocalPlayers;
			}

			namespace Player {
				inline uintptr_t PlayerController;
			}

			namespace PlayerController {
				inline uintptr_t AcknowledgedPawn;
				inline uintptr_t PlayerCameraManager;
			}

			namespace FortItemEntry {
				inline uintptr_t ItemDefinition;
			}

			namespace FortPickup {
				inline uintptr_t PrimaryPickupItemEntry;
			}

			namespace FortItemDefinition {
				inline uintptr_t DisplayName;
				inline uintptr_t Tier;
			}

			namespace Actor {
				inline uintptr_t RootComponent;
			}

			namespace SceneComponent {
				inline uintptr_t RelativeLocation;
			}

			namespace FortPlayerStateAthena {
				inline uintptr_t TeamIndex;
			}
		}

		namespace Functions {
			namespace Canvas {
				inline void* K2_DrawLine;
				inline void* K2_DrawText;
				inline void* K2_TextSize;
				inline void* K2_Project;
			}

			namespace KismetSystemLibrary {
				inline void* GetEngineVersion;
				inline void* LineTraceSingle;
			}

			namespace GameplayStatics {
				inline void* GetAllActorsOfClass;
			}

			namespace PlayerCameraManager {
				inline void* GetCameraLocation;
				inline void* GetCameraRotation;
			}

			namespace PlayerController {
				inline void* ClientSetRotation;
			}

			namespace FortPlayerState {
				inline void* GetPlayerName;
			}

			namespace SkinnedMeshComponent {
				inline void* GetBoneName;
				inline void* GetSocketTransform;
			}

			inline uintptr_t GetBoneMatrix;
		}
	}

	inline bool IsValidPointer(uintptr_t Address) {
		if (!Address) {
			return false;
		}

		if (LI_FN(IsBadWritePtr).safe_cached()(&Address, 8)) {
			return false;
		}

		return true;
	}

	inline uintptr_t GetBaseAddress() {
		return *(uintptr_t*)(__readgsqword(0x60) + 0x10);
	}

	void Init();
}

#endif