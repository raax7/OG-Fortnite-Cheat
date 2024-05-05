#include <Windows.h>

#include "Globals.h"

#ifdef _ENGINE
#include "Drawing/RaaxGUI/RaaxGUI.h"
#endif // _ENGINE

#include "Game/Features/Features.h"
#include "Game/Features/Visuals/Chams.h"
#include "Game/Input/Input.h"
#include "Game/SDK/SDK.h"
#include "Hooks/Hooks.h"

#include "External-Libs/LazyImporter.h"
#include "External-Libs/minhook/include/MinHook.h"
#if LOG_LEVEL > LOG_NONE
#include "Utilities/Logger.h"
#endif // LOG_LEVEL > LOG_NONE

/*
* NOTES
* 
* All specific offsets, VFT indexes, function addresses, visual explanations etc
* mentioned in comments are from Fortnite 7.40.
*/

// TO-DO:
// - Fix unloading crashing on ImGui on some versions of Fortnite
// - Improve GetPlayerViewpoint and GetViewpoint VFT index getting
// - Add WndProc hook for Engine
// - Add a season based feature system (allow/forbid features only on specific seasons)
// - Add more menu elements types to RaaxGUI
// - Add more features
// - Add a proper config system
// - Add a PCH
// - Add RaycastMulti offset finding for UE5
// - Use PropertyFlags instead of EClassCastFlags for HasTypeFlag
// - LOTS of improvements in Memory.h
// - Move bind setting to a seperate function (instead of the gay way I did it)

#if UNLOAD_THREAD
const Input::KeyName UnloadKey = Input::KeyName::F5;

VOID UnloadThread() {
    while (true) {
        if (Input::IsKeyDown(UnloadKey)) {
            // Beep to notify that the cheat has been unloaded
            LI_FN(Beep).safe()(500, 250);

            // Unhook WndProc
#ifdef _IMGUI
            LI_FN(SetWindowLongPtrA).safe()(RaaxDx::Window, GWLP_WNDPROC, (LONG_PTR)Hooks::WndProc::WndProcOriginal);
            RaaxDx::Unhook();
#endif // _IMGUI

            // Unhook all hooks
            if (Hooks::DrawTransition::Hook)                        delete Hooks::DrawTransition::Hook;
            if (Hooks::GetPlayerViewpoint::Hook)                    delete Hooks::GetPlayerViewpoint::Hook;
            if (Hooks::GetViewpoint::Hook)                          delete Hooks::GetViewpoint::Hook;

            MH_DisableHook(MH_ALL_HOOKS);
            MH_RemoveHook(MH_ALL_HOOKS);
            MH_Uninitialize();

            // Revert all features (chams, etc)
            Features::RevertAll();

            // Delete all feature managers
            if (Features::Visuals::ChamManagerFortPawn::Manager)    delete Features::Visuals::ChamManagerFortPawn::Manager;
            if (Features::Visuals::ChamManagerFortPickup::Manager)  delete Features::Visuals::ChamManagerFortPickup::Manager;

            // Free library
            LI_FN(FreeLibraryAndExitThread).safe()(CurrentModule, 0);
        }

        LI_FN(Sleep).safe()(50);
    }
}
#endif // UNLOAD_THREAD

VOID Main() {
#ifdef _IMGUI
#if LOAD_D3DCOMPILER_47
    // Load D3DCompiler_47.dll for ImGui
    LI_FN(LoadLibraryA).safe()(skCrypt("D3DCOMPILER_47.dll"));
#endif // LOAD_D3DCOMPILER_47
#endif // _IMGUI

    // Beep to notify that the cheat has been injected
    LI_FN(Beep).safe()(500, 500);

#if LOG_LEVEL > LOG_NONE
    static_assert(false, "Please set a custom path for your logger! i.e. \"C:\\Users\\YOUR_USER\\Desktop\\LOG_NAME.log\". DOUBLE CLICK ME AND REMOVE ME!");

    // Init logger
    Logger::InitLogger(std::string(skCrypt("C:\\Users\\YOUR_USER\\Desktop\\LOG_NAME.log")));
#endif // LOG_LEVEL > LOG_NONE

    // Init base address, GObjects, function addresses, offsets etc
    SDK::Init();

#ifdef _ENGINE
    // Init menu
    RaaxGUI::InitContext();
#endif // _ENGINE

    // Init hooks
    Hooks::Init();

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
    CurrentModule = hModule;
    
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
#if INIT_THREAD
        LI_FN(CreateThread).safe()(nullptr, 0, (LPTHREAD_START_ROUTINE)Main, nullptr, 0, nullptr);
#else
        Main();
#endif // INIT_THREAD
    }
    break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}