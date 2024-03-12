#include "../Hooks.h"

#include "../../Utilities/LazyImporter.h"
#include "../../Utilities/Logger.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall Hooks::WndProc::WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (Hooks::Present::ImGuiBeenSetup) {
		std::unique_lock<std::recursive_timed_mutex> lock(WndProcMutex, std::chrono::milliseconds(1000));
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
	}

	return LI_FN(CallWindowProcA).safe_cached()(WndProcOriginal, hWnd, uMsg, wParam, lParam);
}