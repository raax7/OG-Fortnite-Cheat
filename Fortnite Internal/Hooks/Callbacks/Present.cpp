#include "../Hooks.h"

#include "../../Drawing/Drawing.h"

#include "../../Game/Game.h"

#include "../../Utilities/Font.h"
#include "../../Utilities/Logger.h"

HRESULT __stdcall Hooks::Present::Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	std::lock_guard<std::mutex> lock(PresentMutex);
	std::unique_lock<std::recursive_timed_mutex> lock2(Hooks::WndProc::WndProcMutex, std::chrono::milliseconds(1000));

	if (ImGuiBeenSetup == false) {
		RaaxDx::InitImGui(pSwapChain);

		Font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(&RawFontData, sizeof(RawFontData), 16.f);
		LargeFont = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(&RawFontData, sizeof(RawFontData), 48.0f);

		ImGuiBeenSetup = true;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Drawing::RenderQueuedDrawingInfo();
	Game::MenuCallback();

	ImGui::EndFrame();
	ImGui::Render();
	RaaxDx::DeviceContext->OMSetRenderTargets(1, &RaaxDx::RenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return PresentOriginal(pSwapChain, SyncInterval, Flags);
}