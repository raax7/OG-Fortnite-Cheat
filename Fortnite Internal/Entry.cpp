#include <Windows.h>

#include "Game/SDK/SDK.h"
#include "Hooks/Hooks.h"
#include "Drawing/RaaxGUI/RaaxGUI.h"
#include "Game/Input/Input.h"

#include "Globals.h"

// TO-DO:
// - Convert menu to a class (ONGOING)
// - Add a pickaxe check for weakspot aimbot
// - Convert FOV size to camera degrees instead of pixels
// - Add a batch line processor for better line outlne handling
// - Fix struct offset grabbing not working on new UProperty handling
// - Add a proper config system
// - Move all class functions to a BLANK_function.cpp file
// - Test font size with float UFont::ScalingFactor
// - Convert GetBoneLocation to use GetSocketLocation instead of GetBoneMatrix (incase InitArticulate is thunked out)
// - Add a PCH
// - Make everything in Memory.h my own code (no pasting)


// NOTES:
// GetWeaponStats VFT: UFortItemDefinition[0xD0]

#if UNLOAD_THREAD
const Input::KeyName UnloadKey = Input::KeyName::F5;

VOID UnloadThread() {
    while (true) {
        if (Input::IsKeyDown(UnloadKey)) {
            // Beep to notify that the cheat has been unloaded
            LI_FN(Beep).safe()(500, 250);

            // Unhook all hooks
            if (Hooks::PostRender::Hook)            delete Hooks::PostRender::Hook;
            if (Hooks::GetPlayerViewpoint::Hook)    delete Hooks::GetPlayerViewpoint::Hook;
            if (Hooks::GetViewpoint::Hook)          delete Hooks::GetViewpoint::Hook;

            // Free library
            LI_FN(FreeLibraryAndExitThread).safe()(ThisModule, 0);
        }

        LI_FN(Sleep).safe()(50);
    }
}
#endif // UNLOAD_THREAD

VOID Main() {
    // Seed random
    LI_FN(srand).safe()(time(NULL));

    // Beep to notify that the cheat has been injected
    LI_FN(Beep).safe()(500, 500);

#ifdef _DEBUG
    // Init logger (REPLACE WITH YOUR OWN PATH)
    Logger::InitLogger(skCrypt("C:\\Users\\raax\\Desktop\\cheat.log").decrypt());
#endif // _DEBUG

    SDK::Init();    // Init base address, GObjects, function addresses, offsets etc
    Hooks::Init();  // Init hooks

    RaaxGUI::InitContext(); // Init menu

#if UNLOAD_THREAD
    // Create a thread to handle unloading
    LI_FN(CreateThread).safe()(nullptr, 0, (LPTHREAD_START_ROUTINE)UnloadThread, nullptr, 0, nullptr);
#endif // UNLOAD_THREAD
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    ThisModule = hModule;

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
#if CREATE_THREAD
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