#include "../Hooks.h"

#include "../../Utilities/Font.h"

#include "../../Game/Game.h"

HRESULT __stdcall Hooks::Present::Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
	if (ImGuiBeenSetup == false) {
		RaaxDx::InitImGui(pSwapChain);

		Font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(&rawData, sizeof(rawData), 16.f);
		LargeFont = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(&rawData, sizeof(rawData), 48.0f);

		ImGuiBeenSetup = true;
	}

	Game::CurrentFrame++;

	if (!ImGuiBeenSetup)
		RaaxDx::InitImGui(pSwapChain);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Game::DrawCallback();
	Game::MenuCallback();

	ImGui::EndFrame();
	ImGui::Render();
	RaaxDx::DeviceContext->OMSetRenderTargets(1, &RaaxDx::RenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return PresentOriginal(pSwapChain, SyncInterval, Flags);
}