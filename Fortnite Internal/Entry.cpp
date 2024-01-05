#include <Windows.h>

#include "Game/SDK/SDK.h"
#include "Game/Hooks/Hooks.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        LI_FN(Beep).safe()(500, 500);

        SDK::Init(); // Init base address, GObjects, function addresses, offsets etc
        Hooks::Init(); // Init base address, GObjects, function addresses, offsets etc
        //FreeLibraryAndExitThread(hModule, 0x0);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}