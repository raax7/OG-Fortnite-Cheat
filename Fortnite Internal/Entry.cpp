#include <Windows.h>

#include "Globals.h"

#ifdef _ENGINE
#include "Drawing/RaaxGUI/RaaxGUI.h"
#endif // _ENGINE

#include "Game/Input/Input.h"
#include "Game/SDK/SDK.h"
#include "Hooks/Hooks.h"

#include "External-Libs/LazyImporter.h"
#if LOG_LEVEL > LOG_NONE
#include "Utilities/Logger.h"
#endif // LOG_LEVEL > LOG_NONE

/*
* NOTES
* 
* All specific offsets, VFT indexes, function addresses etc
* mentioned in comments are from Fortnite 7.40.
* 
* 
*/

// TO-DO:
// - Fix unloading crashing on ImGui on some versions of Fortnite
// - Improve input class even more (less reliance on tick based UE functions and force input events for WndProc input)
// - Improve GetPlayerViewpoint and GetViewpoint VFT index getting
// - Fix text size being innacurate on Engine rendering
// - Add WndProc hook for Engine
// - Add a season based feature system(allow / forbid features only on specific seasons)
// - Add more menu elements types to RaaxGUI
// - Add more features
// - Add a batch line processor for better line outlne handling
// - Add a proper config system
// - Add a PCH
// - Make everything in Memory.h my own code (no pasting)
// - Add WndProc as an option for Engine rendering
// - Add bitfield support for AutoRevertFeature

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
#ifdef _IMGUI
            LI_FN(SetWindowLongPtrA).safe()(RaaxDx::Window, GWLP_WNDPROC, (LONG_PTR)Hooks::WndProc::WndProcOriginal);
            RaaxDx::Unhook();
#endif // _IMGUI

            // Free library
            LI_FN(FreeLibraryAndExitThread).safe()(ThisModule, 0);
        }

        LI_FN(Sleep).safe()(50);
    }
}
#endif // UNLOAD_THREAD

VOID Main() {
    // Beep to notify that the cheat has been injected
    LI_FN(Beep).safe()(500, 500);

    // Seed random
    LI_FN(srand).safe()(time(NULL));

#if LOG_LEVEL > LOG_NONE
    // Init logger (REPLACE WITH YOUR OWN PATH)
    Logger::InitLogger(skCrypt("C:\\Users\\raax\\Desktop\\cheat.log").decrypt());
#endif // LOG_LEVEL > LOG_NONE

    SDK::Init();    // Init base address, GObjects, function addresses, offsets etc
    Hooks::Init();  // Init hooks

#ifdef _ENGINE
    RaaxGUI::InitContext(); // Init menu
#endif // _ENGINE

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
#if INIT_THREAD
        LI_FN(CreateThread).safe()(nullptr, 0, (LPTHREAD_START_ROUTINE)Main, nullptr, 0, nullptr);
#else
        Main();
#endif // INIT_THREAD
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}