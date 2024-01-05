#ifndef SDK_H
#define SDK_H

#include <Windows.h>
#include "Classes/CoreUObject_classes.h"

namespace SDK {
	namespace Cached {
		namespace VFT {
			inline uintptr_t PostRender;
			inline uintptr_t ProcessEvent;
		}

		namespace Offsets {
			inline uintptr_t GetBoneMatrix;



			inline uintptr_t PlayerState;
			inline uintptr_t Mesh;

			inline uintptr_t LegacyFontSize;

			namespace Engine {
				inline uintptr_t GameViewport;
			}

			namespace World {
				inline uintptr_t OwningGameInstance;
			}

			namespace GameViewportClient {
				inline uintptr_t World;
			}

			namespace GameInstance {
				inline uintptr_t LocalPlayers;
			}

			namespace LocalPlayer {
				inline uintptr_t ViewportClient;
				inline uintptr_t PlayerController;
			}
		}

		namespace Functions {
			inline void* K2_DrawLine;
			inline void* K2_DrawText;
			inline void* K2_Project;

			inline void* GetEngineVersion;

			inline void* GetAllActorsOfClass;
		}

		namespace Pointers {
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
		//return (uintptr_t)GetModuleHandleA(0);
		return *(uintptr_t*)(__readgsqword(0x60) + 0x10);
	}

	void Init();
}

#endif