#include "../Hooks.h"

#include "../../Drawing/Drawing.h"

#include "../../Game/Game.h"

#include "../../Utilities/Font.h"
#include "../../Utilities/Logger.h"

HRESULT __stdcall Hooks::Present::Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	if (Mutex.ShouldReturn() || Hooks::WndProc::Mutex.ShouldReturn()) {
		return PresentOriginal(pSwapChain, SyncInterval, Flags);
	}

	// Lock mutex (will unlock when function returns)
	ReturnLock Lock(&Mutex);
	ReturnLock Lock2(&Hooks::WndProc::Mutex);

	if (ImGuiBeenSetup == false) {
		RaaxDx::InitImGui(pSwapChain);

		Font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(&RawFontData, sizeof(RawFontData), 16.f);
		LargeFont = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(&RawFontData, sizeof(RawFontData), 48.0f);

		ImGuiBeenSetup = true;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Drawing::RenderDrawingData();
	Game::MenuCallback();

	ImGui::EndFrame();

	ImGui::Render();

	RaaxDx::DeviceContext->OMSetRenderTargets(1, &RaaxDx::RenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return PresentOriginal(pSwapChain, SyncInterval, Flags);
}