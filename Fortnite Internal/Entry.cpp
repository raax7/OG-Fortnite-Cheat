#include <Windows.h>

#include "Game/SDK/SDK.h"
#include "Hooks/Hooks.h"
#include "Drawing/RaaxGUI/RaaxGUI.h"

#include "Globals.h"

// TO-DO:
// - Convert menu to a class (ONGOING)
// - Add a pickaxe check for weakspot aimbot
// - Convert FOV size to camera degrees instead of pixels
// - Add a batch line processor for better line outlne handling
// - Fix struct offset grabbing not working on new UProperty handling

// NOTES:
// GetWeaponStats VFT: UFortItemDefinition[0xD0]

VOID Main() {
    // TEMPORARY, REMOVE LATER!!!
    srand(time(0));
    // TEMPORARY, REMOVE LATER!!!

    // Beep to notify that the cheat has been injected
    LI_FN(Beep).safe()(500, 500);

#ifdef _DEBUG
    // Init logger (REPLACE WITH YOUR OWN PATH)
    Logger::InitLogger(skCrypt("C:\\Users\\raax\\Desktop\\cheat.log").decrypt());
#endif // _DEBUG

    SDK::Init();    // Init base address, GObjects, function addresses, offsets etc
    Hooks::Init();  // Init hooks

    RaaxGUI::InitContext(); // Init menu
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