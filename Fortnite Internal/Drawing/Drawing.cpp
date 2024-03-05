#include "Drawing.h"

#include <string>

#include "../Game/SDK/Classes/Engine_Classes.h"

#include "../Utilities/Math.h"

#ifdef _IMGUI
#include "ImGui/imgui.h"
#include "../Hooks/Hooks.h"
#endif // _IMGUI

#ifdef _IMGUI
void Drawing::Line(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenPositionB, float Thickness, SDK::FLinearColor RenderColor, bool Outlined) {
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(ScreenPositionA.X, ScreenPositionA.Y), ImVec2(ScreenPositionB.X, ScreenPositionB.Y), ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A), Thickness);
}
void Drawing::Text(const char* RenderText, SDK::FVector2D ScreenPosition, float FontSize, SDK::FLinearColor RenderColor, bool CenteredX, bool CenteredY, bool Outlined) {
	if (CenteredX || CenteredY) {
		ImVec2 TextSize = ImGui::GetFont()->CalcTextSizeA(FontSize, FLT_MAX, 0.0f, RenderText);
		ImVec2 CenteredPos = ImVec2(ScreenPosition.X - TextSize.x / 2, ScreenPosition.Y - TextSize.y / 2);

		if (CenteredX) ScreenPosition.X = CenteredPos.x;
		if (CenteredY) ScreenPosition.Y = CenteredPos.y;
	}

	if (Outlined) {
		ImU32 outlineColor = IM_COL32_BLACK;
		outlineColor = (outlineColor & 0x00FFFFFF) | (ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A) & 0xFF000000);

		ImGui::GetBackgroundDrawList()->AddText(Hooks::Present::LargeFont, FontSize, { ScreenPosition.X + 1, ScreenPosition.Y }, outlineColor, RenderText);
		ImGui::GetBackgroundDrawList()->AddText(Hooks::Present::LargeFont, FontSize, { ScreenPosition.X - 1, ScreenPosition.Y }, outlineColor, RenderText);
		ImGui::GetBackgroundDrawList()->AddText(Hooks::Present::LargeFont, FontSize, { ScreenPosition.X, ScreenPosition.Y + 1 }, outlineColor, RenderText);
		ImGui::GetBackgroundDrawList()->AddText(Hooks::Present::LargeFont, FontSize, { ScreenPosition.X, ScreenPosition.Y - 1 }, outlineColor, RenderText);
	}

	ImGui::GetBackgroundDrawList()->AddText(Hooks::Present::LargeFont, FontSize, ImVec2(ScreenPosition.X, ScreenPosition.Y), ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A), RenderText);
}
void Drawing::Text(const wchar_t* RenderText, SDK::FVector2D ScreenPosition, float FontSize, SDK::FLinearColor RenderColor, bool CenteredX, bool CenteredY, bool Outlined) {
	int BufferSize = WideCharToMultiByte(CP_UTF8, 0, RenderText, -1, nullptr, 0, nullptr, nullptr);
	std::string RenderText_(BufferSize, '\0');
	WideCharToMultiByte(CP_UTF8, 0, RenderText, -1, &RenderText_[0], BufferSize, nullptr, nullptr);

	if (CenteredX || CenteredY) {
		ImVec2 TextSize = ImGui::GetFont()->CalcTextSizeA(FontSize, FLT_MAX, 0.0f, RenderText_.c_str());
		ImVec2 CenteredPos = ImVec2(ScreenPosition.X - TextSize.x / 2, ScreenPosition.Y - TextSize.y / 2);

		if (CenteredX) ScreenPosition.X = CenteredPos.x;
		if (CenteredY) ScreenPosition.Y = CenteredPos.y;
	}

	if (Outlined) {
		ImU32 outlineColor = IM_COL32_BLACK;
		outlineColor = (outlineColor & 0x00FFFFFF) | (ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A) & 0xFF000000);

		ImGui::GetBackgroundDrawList()->AddText(Hooks::Present::LargeFont, FontSize, { ScreenPosition.X + 1, ScreenPosition.Y }, outlineColor, RenderText_.c_str());
		ImGui::GetBackgroundDrawList()->AddText(Hooks::Present::LargeFont, FontSize, { ScreenPosition.X - 1, ScreenPosition.Y }, outlineColor, RenderText_.c_str());
		ImGui::GetBackgroundDrawList()->AddText(Hooks::Present::LargeFont, FontSize, { ScreenPosition.X, ScreenPosition.Y + 1 }, outlineColor, RenderText_.c_str());
		ImGui::GetBackgroundDrawList()->AddText(Hooks::Present::LargeFont, FontSize, { ScreenPosition.X, ScreenPosition.Y - 1 }, outlineColor, RenderText_.c_str());
	}

	ImGui::GetBackgroundDrawList()->AddText(Hooks::Present::LargeFont, FontSize, ImVec2(ScreenPosition.X, ScreenPosition.Y), ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A), RenderText_.c_str());
}
SDK::FVector2D Drawing::TextSize(const wchar_t* RenderText, float FontSize) {
	int BufferSize = WideCharToMultiByte(CP_UTF8, 0, RenderText, -1, nullptr, 0, nullptr, nullptr);
	std::string RenderText_(BufferSize, '\0');
	WideCharToMultiByte(CP_UTF8, 0, RenderText, -1, &RenderText_[0], BufferSize, nullptr, nullptr);

	ImVec2 TextSize = ImGui::GetFont()->CalcTextSizeA(FontSize, FLT_MAX, 0.0f, RenderText_.c_str());

	return SDK::FVector2D(TextSize.x, TextSize.y);
}
void Drawing::Circle(SDK::FVector2D ScreenPosition, float Radius, int32_t Segments, SDK::FLinearColor RenderColor, bool Outlined) {
	ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(ScreenPosition.X, ScreenPosition.Y), Radius, ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A), Segments);
}
void Drawing::FilledRect(SDK::FVector2D ScreenPosition, SDK::FVector2D ScreenSize, SDK::FLinearColor RenderColor, bool Outlined) {
	ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(ScreenPosition.X, ScreenPosition.Y), ImVec2(ScreenSize.X, ScreenSize.Y), ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A));
}
void Drawing::Rect(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenSize, float Thickness, SDK::FLinearColor RenderColor, bool Outlined) {
	ImGui::GetBackgroundDrawList()->AddRect(ImVec2(ScreenPositionA.X, ScreenPositionA.Y), ImVec2(ScreenPositionA.X + ScreenSize.X, ScreenPositionA.Y + ScreenSize.Y), ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A), 0, 0, Thickness);
}
void Drawing::CorneredRect(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenSize, float Thickness, SDK::FLinearColor RenderColor, bool Outlined) {
	float lineW = ScreenSize.X / 4;
	float lineH = ScreenSize.Y / 4;

	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(ScreenPositionA.X, ScreenPositionA.Y), ImVec2(ScreenPositionA.X + lineW, ScreenPositionA.Y), ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A), Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(ScreenPositionA.X, ScreenPositionA.Y), ImVec2(ScreenPositionA.X, ScreenPositionA.Y + lineH), ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A), Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(ScreenPositionA.X + ScreenSize.X, ScreenPositionA.Y), ImVec2(ScreenPositionA.X + ScreenSize.X, ScreenPositionA.Y + lineH), ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A), Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(ScreenPositionA.X + ScreenSize.X - lineW, ScreenPositionA.Y), ImVec2(ScreenPositionA.X + ScreenSize.X, ScreenPositionA.Y), ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A), Thickness);

	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(ScreenPositionA.X, ScreenPositionA.Y + ScreenSize.Y), ImVec2(ScreenPositionA.X + lineW, ScreenPositionA.Y + ScreenSize.Y), ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A), Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(ScreenPositionA.X, ScreenPositionA.Y + ScreenSize.Y - lineH), ImVec2(ScreenPositionA.X, ScreenPositionA.Y + ScreenSize.Y), ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A), Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(ScreenPositionA.X + ScreenSize.X, ScreenPositionA.Y + ScreenSize.Y - lineH), ImVec2(ScreenPositionA.X + ScreenSize.X, ScreenPositionA.Y + ScreenSize.Y), ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A), Thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(ScreenPositionA.X + ScreenSize.X - lineW, ScreenPositionA.Y + ScreenSize.Y), ImVec2(ScreenPositionA.X + ScreenSize.X, ScreenPositionA.Y + ScreenSize.Y), ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A), Thickness);
}
#endif // _IMGUI
#ifdef _ENGINE
void Drawing::Line(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenPositionB, float Thickness, SDK::FLinearColor RenderColor, bool Outlined) {
	if (Outlined) {
		SDK::GetLocalCanvas()->K2_DrawLine(ScreenPositionA, ScreenPositionB, Thickness + 1.f, SDK::FLinearColor(0.f, 0.f, 0.f, 1.f));
		SDK::GetLocalCanvas()->K2_DrawLine(ScreenPositionA, ScreenPositionB, Thickness - 1.f, SDK::FLinearColor(0.f, 0.f, 0.f, 1.f));
	}

	SDK::GetLocalCanvas()->K2_DrawLine(ScreenPositionA, ScreenPositionB, Thickness, RenderColor);
}
void Drawing::Text(const char* RenderText, SDK::FVector2D ScreenPosition, float FontSize, SDK::FLinearColor RenderColor, bool CenteredX, bool CenteredY, bool Outlined) {
	int WideStrLength = MultiByteToWideChar(CP_UTF8, 0, RenderText, -1, nullptr, 0);
	std::wstring WideString(WideStrLength, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, RenderText, -1, &WideString[0], WideStrLength);

	// Fix text size being innacurate on engine rendering. (Maybe UFont::ScalingFactor?)

	Text(WideString.c_str(), ScreenPosition, FontSize, RenderColor, CenteredX, CenteredY, Outlined);
}
void Drawing::Text(const wchar_t* RenderText, SDK::FVector2D ScreenPosition, float FontSize, SDK::FLinearColor RenderColor, bool CenteredX, bool CenteredY, bool Outlined) {
	SDK::FString FString(RenderText);

	// Fix text size being innacurate on engine rendering. (Maybe UFont::ScalingFactor?)

	SDK::GetLocalCanvas()->K2_DrawText(FString, ScreenPosition, (int32)FontSize, RenderColor, CenteredX, CenteredY, Outlined);
}
SDK::FVector2D Drawing::TextSize(const wchar_t* RenderText, float FontSize) {
	SDK::FString FString(RenderText);

	// Only use 3/4ths of the font size to make the size accurate. Unreal Engine is weird
	FontSize *= 0.75f;

	return SDK::GetLocalCanvas()->K2_TextSize(FString, (int32)FontSize);
}
void Drawing::Circle(SDK::FVector2D ScreenPosition, float Radius, int32_t Segments, SDK::FLinearColor RenderColor, bool Outlined) {
	if (Segments < 3) {
		Segments = 3;
	}

	float AngleStep = (2.f * M_PI) / static_cast<float>(Segments);
	SDK::FVector2D PreviousPoint = SDK::FVector2D(
		Radius * cos(0) + ScreenPosition.X,
		Radius * sin(0) + ScreenPosition.Y
	);

	for (int SegmentCount = 1; SegmentCount <= Segments; SegmentCount++) {
		SDK::FVector2D CurrentPoint = SDK::FVector2D(
			Radius * cos(AngleStep * SegmentCount) + ScreenPosition.X,
			Radius * sin(AngleStep * SegmentCount) + ScreenPosition.Y
		);

		Line(PreviousPoint, CurrentPoint, 1.0f, RenderColor, Outlined);

		PreviousPoint = CurrentPoint;
	}
}
void Drawing::FilledRect(SDK::FVector2D ScreenPosition, SDK::FVector2D ScreenSize, SDK::FLinearColor RenderColor, bool Outlined) {
	for (float i = 0.0f; i < ScreenSize.Y; i += 1.0f) {
		Line(SDK::FVector2D{ ScreenPosition.X, ScreenPosition.Y + i }, SDK::FVector2D{ ScreenPosition.X + ScreenSize.X, ScreenPosition.Y + i }, 1.0f, RenderColor, Outlined);
	}
}
void Drawing::Rect(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenSize, float Thickness, SDK::FLinearColor RenderColor, bool Outlined) {
	SDK::FVector2D ScreenPositionB = SDK::FVector2D(ScreenPositionA.X + ScreenSize.X, ScreenPositionA.Y + ScreenSize.Y);

	Line(ScreenPositionA, SDK::FVector2D(ScreenPositionB.X, ScreenPositionA.Y), Thickness, RenderColor, Outlined);
	Line(ScreenPositionA, SDK::FVector2D(ScreenPositionA.X, ScreenPositionB.Y), Thickness, RenderColor, Outlined);
	Line(SDK::FVector2D(ScreenPositionA.X, ScreenPositionB.Y), ScreenPositionB, Thickness, RenderColor, Outlined);
	Line(SDK::FVector2D(ScreenPositionB.X, ScreenPositionA.Y), ScreenPositionB, Thickness, RenderColor, Outlined);
}
void Drawing::CorneredRect(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenSize, float Thickness, SDK::FLinearColor RenderColor, bool Outlined) {
	float lineW = ScreenSize.X / 4;
	float lineH = ScreenSize.Y / 4;

	Line(SDK::FVector2D(ScreenPositionA.X, ScreenPositionA.Y), SDK::FVector2D(ScreenPositionA.X + lineW, ScreenPositionA.Y), Thickness, RenderColor, Outlined);
	Line(SDK::FVector2D(ScreenPositionA.X, ScreenPositionA.Y), SDK::FVector2D(ScreenPositionA.X, ScreenPositionA.Y + lineH), Thickness, RenderColor, Outlined);
	Line(SDK::FVector2D(ScreenPositionA.X + ScreenSize.X, ScreenPositionA.Y), SDK::FVector2D(ScreenPositionA.X + ScreenSize.X, ScreenPositionA.Y + lineH), Thickness, RenderColor, Outlined);
	Line(SDK::FVector2D(ScreenPositionA.X + ScreenSize.X - lineW, ScreenPositionA.Y), SDK::FVector2D(ScreenPositionA.X + ScreenSize.X, ScreenPositionA.Y), Thickness, RenderColor, Outlined);

	Line(SDK::FVector2D(ScreenPositionA.X, ScreenPositionA.Y + ScreenSize.Y), SDK::FVector2D(ScreenPositionA.X + lineW, ScreenPositionA.Y + ScreenSize.Y), Thickness, RenderColor, Outlined);
	Line(SDK::FVector2D(ScreenPositionA.X, ScreenPositionA.Y + ScreenSize.Y - lineH), SDK::FVector2D(ScreenPositionA.X, ScreenPositionA.Y + ScreenSize.Y), Thickness, RenderColor, Outlined);
	Line(SDK::FVector2D(ScreenPositionA.X + ScreenSize.X, ScreenPositionA.Y + ScreenSize.Y - lineH), SDK::FVector2D(ScreenPositionA.X + ScreenSize.X, ScreenPositionA.Y + ScreenSize.Y), Thickness, RenderColor, Outlined);
	Line(SDK::FVector2D(ScreenPositionA.X + ScreenSize.X - lineW, ScreenPositionA.Y + ScreenSize.Y), SDK::FVector2D(ScreenPositionA.X + ScreenSize.X, ScreenPositionA.Y + ScreenSize.Y), Thickness, RenderColor, Outlined);
}
#endif // _ENGINE