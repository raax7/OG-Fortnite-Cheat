#include <Windows.h>

#include "Game/SDK/SDK.h"
#include "Game/Hooks/Hooks.h"

#include "Globals.h"

// TO-DO:
// - Convert all input to be got through Unreal Engine functions
// - Convert menu to use indexes instead of strings for items
// - Convert menu to a class

VOID Main() {
    // Beep to notify that the cheat has been injected
    LI_FN(Beep).safe()(500, 500);

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
#if CREATE_THREAD
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Main, nullptr, 0, nullptr);
#endif // CREATE_THREAD
#if !CREATE_THREAD
        Main();
#endif // !CREATE_THREAD
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}