#include "../Hooks.h"

#include "../../External-Libs/LazyImporter.h"
#include "../../Game/Game.h"
#include "../../Utilities/Logger.h"

#include "../../Drawing/Drawing.h"
#include "../../Utilities/RaaxAssert.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall Hooks::WndProc::WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (Hooks::Present::ImGuiBeenSetup && Hooks::WndProc::Mutex.ShouldReturn() == false)
    {
        ReturnLock Lock(&Hooks::WndProc::Mutex);

        ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

        // If the menu is open, we don't want to send input to the game
        if (Game::MenuOpen)
        {
            return true;
        }
    }

    return LI_FN(CallWindowProcA).safe_cached()(WndProcOriginal, hWnd, uMsg, wParam, lParam);
}