#include <Windows.h>

#include "Game/SDK/SDK.h"
#include "Hooks/Hooks.h"

#include "Globals.h"

// TO-DO:
// - Convert all input to be got through Unreal Engine functions
// - Convert menu to use indexes instead of strings for items
// - Convert menu to a class
// - Add a runtime string encryption and decryption system
// - Add a check for if a player is inside the FOV (even not visible) and if so, don't aimbot weakspots
// - Convert FOV size to camera degrees instead of pixels
// - Add a batch line processor for betting line outlne handling

// NOTES:
// GetWeaponStats VFT: UFortItemDefinition[0xD0]

VOID Main() {
    // Beep to notify that the cheat has been injected
    LI_FN(Beep).safe()(500, 500);

    // Init logger (REPLACE WITH YOUR OWN PATH)
    Logger::InitLogger(skCrypt("C:\\Users\\raax\\Desktop\\cheat.log").decrypt());

    // Init base address, GObjects, function addresses, offsets etc
    SDK::Init();

    // Init hooks
    Hooks::Init();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
#if defined(SHOULD_CREATE_THREAD)
        LI_FN(CreateThread).safe()(nullptr, 0, (LPTHREAD_START_ROUTINE)Main, nullptr, 0, nullptr);
#else // SHOULD_CREATE_THREAD
        Main();
#endif // !SHOULD_CREATE_THREAD
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}