#include "Drawing.h"

#include <string>

#include "../Game/SDK/Classes/Engine_Classes.h"

#include "../Utilities/Math.h"

#ifdef _IMGUI
#include "ImGui/imgui.h"
#include "../Hooks/Hooks.h"

std::vector<Drawing::LineCache> Drawing::RenderBufferLine, Drawing::UpdateBufferLine;
std::vector<Drawing::TextCache> Drawing::RenderBufferText, Drawing::UpdateBufferText;
std::vector<Drawing::CircleCache> Drawing::RenderBufferCircle, Drawing::UpdateBufferCircle;
std::vector<Drawing::FilledRectCache> Drawing::RenderBufferFilledRect, Drawing::UpdateBufferFilledRect;
std::vector<Drawing::RectCache> Drawing::RenderBufferRect, Drawing::UpdateBufferRect;
std::vector<Drawing::CorneredRectCache> Drawing::RenderBufferCorneredRect, Drawing::UpdateBufferCorneredRect;

std::mutex Drawing::SwapMutex;
#endif // _IMGUI

#ifdef _IMGUI
void Drawing::RenderQueuedDrawingInfo() {
	std::lock_guard<std::mutex> lock(SwapMutex);

	for (auto& Line : RenderBufferLine) {
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Line.ScreenPositionA.X, Line.ScreenPositionA.Y), ImVec2(Line.ScreenPositionB.X, Line.ScreenPositionB.Y), ImColor(Line.RenderColor.R, Line.RenderColor.G, Line.RenderColor.B, Line.RenderColor.A), Line.Thickness);
	}

	for (auto& Text : RenderBufferText) {
		if (Text.CenteredX || Text.CenteredY) {
			ImVec2 TextSize = ImGui::GetFont()->CalcTextSizeA(Text.FontSize, FLT_MAX, 0.0f, Text.RenderText.c_str());
			ImVec2 CenteredPos = ImVec2(Text.ScreenPosition.X - TextSize.x / 2, Text.ScreenPosition.Y - TextSize.y / 2);

			if (Text.CenteredX) Text.ScreenPosition.X = CenteredPos.x;
			if (Text.CenteredY) Text.ScreenPosition.Y = CenteredPos.y;
		}

		if (Text.Outlined) {
			ImU32 outlineColor = IM_COL32_BLACK;
			outlineColor = (outlineColor & 0x00FFFFFF) | (ImColor(Text.RenderColor.R, Text.RenderColor.G, Text.RenderColor.B, Text.RenderColor.A) & 0xFF000000);

			ImGui::GetBackgroundDrawList()->AddText(Hooks::Present::LargeFont, Text.FontSize, { Text.ScreenPosition.X + 1, Text.ScreenPosition.Y }, outlineColor, Text.RenderText.c_str());
			ImGui::GetBackgroundDrawList()->AddText(Hooks::Present::LargeFont, Text.FontSize, { Text.ScreenPosition.X - 1, Text.ScreenPosition.Y }, outlineColor, Text.RenderText.c_str());
			ImGui::GetBackgroundDrawList()->AddText(Hooks::Present::LargeFont, Text.FontSize, { Text.ScreenPosition.X, Text.ScreenPosition.Y + 1 }, outlineColor, Text.RenderText.c_str());
			ImGui::GetBackgroundDrawList()->AddText(Hooks::Present::LargeFont, Text.FontSize, { Text.ScreenPosition.X, Text.ScreenPosition.Y - 1 }, outlineColor, Text.RenderText.c_str());
		}

		ImGui::GetBackgroundDrawList()->AddText(Hooks::Present::LargeFont, Text.FontSize, ImVec2(Text.ScreenPosition.X, Text.ScreenPosition.Y), ImColor(Text.RenderColor.R, Text.RenderColor.G, Text.RenderColor.B, Text.RenderColor.A), Text.RenderText.c_str());
	}

	for (auto& Circle : RenderBufferCircle) {
		ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(Circle.ScreenPosition.X, Circle.ScreenPosition.Y), Circle.Radius, ImColor(Circle.RenderColor.R, Circle.RenderColor.G, Circle.RenderColor.B, Circle.RenderColor.A), Circle.Segments);
	}

	for (auto& FilledRect : RenderBufferFilledRect) {
		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(FilledRect.ScreenPosition.X, FilledRect.ScreenPosition.Y), ImVec2(FilledRect.ScreenPosition.X + FilledRect.ScreenSize.X, FilledRect.ScreenPosition.Y + FilledRect.ScreenSize.Y), ImColor(FilledRect.RenderColor.R, FilledRect.RenderColor.G, FilledRect.RenderColor.B, FilledRect.RenderColor.A));
	}

	for (auto& Rect : RenderBufferRect) {
		ImGui::GetBackgroundDrawList()->AddRect(ImVec2(Rect.ScreenPositionA.X, Rect.ScreenPositionA.Y), ImVec2(Rect.ScreenPositionA.X + Rect.ScreenSize.X, Rect.ScreenPositionA.Y + Rect.ScreenSize.Y), ImColor(Rect.RenderColor.R, Rect.RenderColor.G, Rect.RenderColor.B, Rect.RenderColor.A), 0, 0, Rect.Thickness);
	}

	for (auto& CorneredRect : RenderBufferCorneredRect) {
		float lineW = CorneredRect.ScreenSize.X / 4;
		float lineH = CorneredRect.ScreenSize.Y / 4;

		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(CorneredRect.ScreenPositionA.X, CorneredRect.ScreenPositionA.Y), ImVec2(CorneredRect.ScreenPositionA.X + lineW, CorneredRect.ScreenPositionA.Y), ImColor(CorneredRect.RenderColor.R, CorneredRect.RenderColor.G, CorneredRect.RenderColor.B, CorneredRect.RenderColor.A), CorneredRect.Thickness);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(CorneredRect.ScreenPositionA.X, CorneredRect.ScreenPositionA.Y), ImVec2(CorneredRect.ScreenPositionA.X, CorneredRect.ScreenPositionA.Y + lineH), ImColor(CorneredRect.RenderColor.R, CorneredRect.RenderColor.G, CorneredRect.RenderColor.B, CorneredRect.RenderColor.A), CorneredRect.Thickness);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(CorneredRect.ScreenPositionA.X + CorneredRect.ScreenSize.X, CorneredRect.ScreenPositionA.Y), ImVec2(CorneredRect.ScreenPositionA.X + CorneredRect.ScreenSize.X, CorneredRect.ScreenPositionA.Y + lineH), ImColor(CorneredRect.RenderColor.R, CorneredRect.RenderColor.G, CorneredRect.RenderColor.B, CorneredRect.RenderColor.A), CorneredRect.Thickness);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(CorneredRect.ScreenPositionA.X + CorneredRect.ScreenSize.X - lineW, CorneredRect.ScreenPositionA.Y), ImVec2(CorneredRect.ScreenPositionA.X + CorneredRect.ScreenSize.X, CorneredRect.ScreenPositionA.Y), ImColor(CorneredRect.RenderColor.R, CorneredRect.RenderColor.G, CorneredRect.RenderColor.B, CorneredRect.RenderColor.A), CorneredRect.Thickness);

		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(CorneredRect.ScreenPositionA.X, CorneredRect.ScreenPositionA.Y + CorneredRect.ScreenSize.Y), ImVec2(CorneredRect.ScreenPositionA.X + lineW, CorneredRect.ScreenPositionA.Y + CorneredRect.ScreenSize.Y), ImColor(CorneredRect.RenderColor.R, CorneredRect.RenderColor.G, CorneredRect.RenderColor.B, CorneredRect.RenderColor.A), CorneredRect.Thickness);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(CorneredRect.ScreenPositionA.X, CorneredRect.ScreenPositionA.Y + CorneredRect.ScreenSize.Y - lineH), ImVec2(CorneredRect.ScreenPositionA.X, CorneredRect.ScreenPositionA.Y + CorneredRect.ScreenSize.Y), ImColor(CorneredRect.RenderColor.R, CorneredRect.RenderColor.G, CorneredRect.RenderColor.B, CorneredRect.RenderColor.A), CorneredRect.Thickness);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(CorneredRect.ScreenPositionA.X + CorneredRect.ScreenSize.X, CorneredRect.ScreenPositionA.Y + CorneredRect.ScreenSize.Y - lineH), ImVec2(CorneredRect.ScreenPositionA.X + CorneredRect.ScreenSize.X, CorneredRect.ScreenPositionA.Y + CorneredRect.ScreenSize.Y), ImColor(CorneredRect.RenderColor.R, CorneredRect.RenderColor.G, CorneredRect.RenderColor.B, CorneredRect.RenderColor.A), CorneredRect.Thickness);
		ImGui::GetBackgroundDrawList()->AddLine(ImVec2(CorneredRect.ScreenPositionA.X + CorneredRect.ScreenSize.X - lineW, CorneredRect.ScreenPositionA.Y + CorneredRect.ScreenSize.Y), ImVec2(CorneredRect.ScreenPositionA.X + CorneredRect.ScreenSize.X, CorneredRect.ScreenPositionA.Y + CorneredRect.ScreenSize.Y), ImColor(CorneredRect.RenderColor.R, CorneredRect.RenderColor.G, CorneredRect.RenderColor.B, CorneredRect.RenderColor.A), CorneredRect.Thickness);
	}
}

void Drawing::Line(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenPositionB, float Thickness, SDK::FLinearColor RenderColor, bool Outlined) {
	LineCache Cache;
	Cache.ScreenPositionA = ScreenPositionA;
	Cache.ScreenPositionB = ScreenPositionB;
	Cache.Thickness = Thickness;
	Cache.RenderColor = RenderColor;
	Cache.Outlined = Outlined;

	UpdateBufferLine.push_back(Cache);
}
void Drawing::Text(const char* RenderText, SDK::FVector2D ScreenPosition, float FontSize, SDK::FLinearColor RenderColor, bool CenteredX, bool CenteredY, bool Outlined) {
	TextCache Cache;
	Cache.RenderText = RenderText;
	Cache.ScreenPosition = ScreenPosition;
	Cache.FontSize = FontSize;
	Cache.RenderColor = RenderColor;
	Cache.CenteredX = CenteredX;
	Cache.CenteredY = CenteredY;
	Cache.Outlined = Outlined;

	UpdateBufferText.push_back(Cache);
}
void Drawing::Text(const wchar_t* RenderText, SDK::FVector2D ScreenPosition, float FontSize, SDK::FLinearColor RenderColor, bool CenteredX, bool CenteredY, bool Outlined) {
	std::string str = std::string(RenderText, RenderText + wcslen(RenderText));
	Text(str.c_str(), ScreenPosition, FontSize, RenderColor, CenteredX, CenteredY, Outlined);
}
SDK::FVector2D Drawing::TextSize(const wchar_t* RenderText, float FontSize) {
	if (Hooks::Present::LargeFont == nullptr) {
		return SDK::FVector2D();
	}

	std::string str = std::string(RenderText, RenderText + wcslen(RenderText));
	ImVec2 TextSize = Hooks::Present::LargeFont->CalcTextSizeA(FontSize, FLT_MAX, 0.0f, str.c_str());
	return SDK::FVector2D(TextSize.x, TextSize.y);
}
void Drawing::Circle(SDK::FVector2D ScreenPosition, float Radius, int32_t Segments, SDK::FLinearColor RenderColor, bool Outlined) {
	CircleCache Cache;
	Cache.ScreenPosition = ScreenPosition;
	Cache.Radius = Radius;
	Cache.Segments = Segments;
	Cache.RenderColor = RenderColor;
	Cache.Outlined = Outlined;

	UpdateBufferCircle.push_back(Cache);
}
void Drawing::FilledRect(SDK::FVector2D ScreenPosition, SDK::FVector2D ScreenSize, SDK::FLinearColor RenderColor, bool Outlined) {
	FilledRectCache Cache;
	Cache.ScreenPosition = ScreenPosition;
	Cache.ScreenSize = ScreenSize;
	Cache.RenderColor = RenderColor;
	Cache.Outlined = Outlined;

	UpdateBufferFilledRect.push_back(Cache);
}
void Drawing::Rect(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenSize, float Thickness, SDK::FLinearColor RenderColor, bool Outlined) {
	RectCache Cache;
	Cache.ScreenPositionA = ScreenPositionA;
	Cache.ScreenSize = ScreenSize;
	Cache.Thickness = Thickness;
	Cache.RenderColor = RenderColor;
	Cache.Outlined = Outlined;

	UpdateBufferRect.push_back(Cache);
}
void Drawing::CorneredRect(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenSize, float Thickness, SDK::FLinearColor RenderColor, bool Outlined) {
	CorneredRectCache Cache;
	Cache.ScreenPositionA = ScreenPositionA;
	Cache.ScreenSize = ScreenSize;
	Cache.Thickness = Thickness;
	Cache.RenderColor = RenderColor;
	Cache.Outlined = Outlined;

	UpdateBufferCorneredRect.push_back(Cache);
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