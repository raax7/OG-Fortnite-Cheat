#include "Drawing.h"
#include <string>

#ifdef _IMGUI
#include "../External-Libs/ImGui/imgui.h"
#include "../Hooks/Hooks.h"

void Drawing::LineCache::DrawOutline() {
	ImU32 OutlineColor = ImColor(0.f, 0.f, 0.f, RenderColor.A);

	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(ScreenPositionA.X, ScreenPositionA.Y), ImVec2(ScreenPositionB.X, ScreenPositionB.Y), OutlineColor, Thickness + 1.f);
}
void Drawing::LineCache::DrawLine() {
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(ScreenPositionA.X, ScreenPositionA.Y), ImVec2(ScreenPositionB.X, ScreenPositionB.Y), ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A), Thickness);
}
void Drawing::LineCache::Draw() {
	if (Outlined) {
		DrawOutline();
	}

	DrawLine();
}
void Drawing::BatchLineCache::Draw() {
	for (auto& Line : Lines) {
		if (Line.Outlined) {
			Line.DrawOutline();
		}
	}

	for (auto& Line : Lines) {
		Line.DrawLine();
	}
}
void Drawing::TextCache::Draw() {
	ImVec2 TextPosition = ImVec2(ScreenPosition.X, ScreenPosition.Y);

	if (Outlined) {
		ImU32 OutlineColor = ImColor(0.f, 0.f, 0.f, RenderColor.A);

		ImVec2 Offsets[] = {
			ImVec2(-1.f, 0),
			ImVec2(1.f, 0),
			ImVec2(0, -1.f),
			ImVec2(0, 1.f),
		};

		for (ImVec2& Offset : Offsets) {
			ImVec2 OutlinePos = ImVec2(TextPosition.x + Offset.x, TextPosition.y + Offset.y);
			ImGui::GetBackgroundDrawList()->AddText(Hooks::Present::LargeFont, FontSize, OutlinePos, OutlineColor, RenderText.c_str());
		}
	}

	ImGui::GetBackgroundDrawList()->AddText(Hooks::Present::LargeFont, FontSize, TextPosition, ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A), RenderText.c_str());
}
void Drawing::CircleCache::Draw() {
	if (Outlined) {
		ImU32 OutlineColor = ImColor(0.f, 0.f, 0.f, RenderColor.A);

		ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(ScreenPosition.X, ScreenPosition.Y), Radius + 1.f, OutlineColor, Segments);
		ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(ScreenPosition.X, ScreenPosition.Y), Radius - 1.f, OutlineColor, Segments);
	}

	ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(ScreenPosition.X, ScreenPosition.Y), Radius, ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A), Segments);
}
void Drawing::FilledRectCache::Draw() {
	if (Outlined) {
		ImU32 OutlineColor = ImColor(0.f, 0.f, 0.f, RenderColor.A);

		ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(ScreenPosition.X, ScreenPosition.Y), ImVec2(ScreenPosition.X + ScreenSize.X, ScreenPosition.Y + ScreenSize.Y), OutlineColor);
	}
	
	ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(ScreenPosition.X, ScreenPosition.Y), ImVec2(ScreenPosition.X + ScreenSize.X, ScreenPosition.Y + ScreenSize.Y), ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A));
}
void Drawing::RectCache::Draw() {
	if (Outlined) {
		ImU32 OutlineColor = ImColor(0.f, 0.f, 0.f, RenderColor.A);

		ImGui::GetBackgroundDrawList()->AddRect(ImVec2(ScreenPosition.X, ScreenPosition.Y), ImVec2(ScreenPosition.X + ScreenSize.X, ScreenPosition.Y + ScreenSize.Y), OutlineColor, 0, 0, Thickness + 1.f);
	}
	
	ImGui::GetBackgroundDrawList()->AddRect(ImVec2(ScreenPosition.X, ScreenPosition.Y), ImVec2(ScreenPosition.X + ScreenSize.X, ScreenPosition.Y + ScreenSize.Y), ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A), 0, 0, Thickness);
}
void Drawing::TriangleCache::Draw() {
	ImVec2 Points[3] = {
		ImVec2(ScreenPositionA.X, ScreenPositionA.Y),
		ImVec2(ScreenPositionB.X, ScreenPositionB.Y),
		ImVec2(ScreenPositionC.X, ScreenPositionC.Y)
	};

	if (Outlined) {
		ImU32 OutlineColor = ImColor(0.f, 0.f, 0.f, RenderColor.A);

		ImGui::GetBackgroundDrawList()->AddTriangle(Points[0], Points[1], Points[2], OutlineColor, Thickness + 1.f);
	}

	if (Filled) {
		ImGui::GetBackgroundDrawList()->AddTriangleFilled(Points[0], Points[1], Points[2], ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A));
	}
	else {
		ImGui::GetBackgroundDrawList()->AddTriangle(Points[0], Points[1], Points[2], ImColor(RenderColor.R, RenderColor.G, RenderColor.B, RenderColor.A), Thickness);
	}
}

void Drawing::RenderDrawingData() {
	std::lock_guard<std::mutex> Lock(DrawingMutex);

	for (auto& RenderObject : RenderBuffer) {
		RenderObject->Draw();
	}
}

void Drawing::Line(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenPositionB, float Thickness, SDK::FLinearColor RenderColor, bool Outlined) {
	std::lock_guard<std::mutex> Lock(DrawingMutex);
	auto Cache = std::make_unique<LineCache>(ScreenPositionA, ScreenPositionB, Thickness, RenderColor, Outlined);
	DrawingQueue.push_back(std::move(Cache));
}
void Drawing::Text(const char* RenderText, SDK::FVector2D ScreenPosition, float FontSize, SDK::FLinearColor RenderColor, bool CenteredX, bool CenteredY, bool Outlined) {
	std::lock_guard<std::mutex> Lock(DrawingMutex);
	if (Hooks::Present::LargeFont == nullptr) {
		return;
	}
	
	auto Cache = std::make_unique<TextCache>(RenderText, ScreenPosition, FontSize, RenderColor, CenteredX, CenteredY, Outlined);
	
	// Handle centred text here so it doesn't go crazy
	if (CenteredX || CenteredY) {
		ImVec2 TextSize = Hooks::Present::LargeFont->CalcTextSizeA(FontSize, FLT_MAX, FLT_MAX, RenderText);
		ImVec2 CentredPos = ImVec2(ScreenPosition.X - TextSize.x / 2.f, ScreenPosition.Y - TextSize.y / 2.f);

		if (CenteredX) {
			Cache->ScreenPosition.X = CentredPos.x;
		}

		if (CenteredY) {
			Cache->ScreenPosition.Y = CentredPos.y;
		}
	}
	
	DrawingQueue.push_back(std::move(Cache));
}
void Drawing::Text(const wchar_t* RenderText, SDK::FVector2D ScreenPosition, float FontSize, SDK::FLinearColor RenderColor, bool CenteredX, bool CenteredY, bool Outlined) {
	std::string String = std::string(RenderText, RenderText + wcslen(RenderText));
	Text(String.c_str(), ScreenPosition, FontSize, RenderColor, CenteredX, CenteredY, Outlined);
}
SDK::FVector2D Drawing::TextSize(const char* RenderText, float FontSize) {
	if (Hooks::Present::LargeFont == nullptr) {
		return SDK::FVector2D();
	}

	ImVec2 TextSize = Hooks::Present::LargeFont->CalcTextSizeA(FontSize, FLT_MAX, 0.0f, RenderText);
	return SDK::FVector2D(TextSize.x, TextSize.y);
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
	std::lock_guard<std::mutex> Lock(DrawingMutex);
	auto Cache = std::make_unique<CircleCache>(ScreenPosition, Radius, Segments, RenderColor, Outlined);
	DrawingQueue.push_back(std::move(Cache));
}
void Drawing::FilledRect(SDK::FVector2D ScreenPosition, SDK::FVector2D ScreenSize, SDK::FLinearColor RenderColor, bool Outlined) {
	std::lock_guard<std::mutex> Lock(DrawingMutex);
	auto Cache = std::make_unique<FilledRectCache>(ScreenPosition, ScreenSize, RenderColor, Outlined);
	DrawingQueue.push_back(std::move(Cache));
}
void Drawing::Rect(SDK::FVector2D ScreenPosition, SDK::FVector2D ScreenSize, float Thickness, SDK::FLinearColor RenderColor, bool Outlined) {
	std::lock_guard<std::mutex> Lock(DrawingMutex);
	auto Cache = std::make_unique<RectCache>(ScreenPosition, ScreenSize, Thickness, RenderColor, Outlined);
	DrawingQueue.push_back(std::move(Cache));
}
void Drawing::CorneredRect(SDK::FVector2D ScreenPosition, SDK::FVector2D ScreenSize, float Thickness, SDK::FLinearColor RenderColor, bool Outlined) {
	std::lock_guard<std::mutex> Lock(DrawingMutex);
	float LineW = ScreenSize.X / 4.f;
	float LineH = ScreenSize.Y / 4.f;

	float Correction = Thickness / 2.f;
	Correction *= -1.f;

	auto Cache = std::make_unique<BatchLineCache>();

	// Top-left corner horizontal and vertical
	Cache->Lines.push_back(Drawing::LineCache(SDK::FVector2D(ScreenPosition.X + Correction, ScreenPosition.Y), SDK::FVector2D(ScreenPosition.X + LineW, ScreenPosition.Y), Thickness, RenderColor, Outlined));
	Cache->Lines.push_back(Drawing::LineCache(SDK::FVector2D(ScreenPosition.X, ScreenPosition.Y + Correction), SDK::FVector2D(ScreenPosition.X, ScreenPosition.Y + LineH), Thickness, RenderColor, Outlined));

	// Top-right corner horizontal and vertical
	Cache->Lines.push_back(Drawing::LineCache(SDK::FVector2D(ScreenPosition.X + ScreenSize.X - LineW, ScreenPosition.Y), SDK::FVector2D(ScreenPosition.X + ScreenSize.X - Correction, ScreenPosition.Y), Thickness, RenderColor, Outlined));
	Cache->Lines.push_back(Drawing::LineCache(SDK::FVector2D(ScreenPosition.X + ScreenSize.X, ScreenPosition.Y + Correction), SDK::FVector2D(ScreenPosition.X + ScreenSize.X, ScreenPosition.Y + LineH), Thickness, RenderColor, Outlined));

	// Bottom-left corner horizontal and vertical
	Cache->Lines.push_back(Drawing::LineCache(SDK::FVector2D(ScreenPosition.X + Correction, ScreenPosition.Y + ScreenSize.Y), SDK::FVector2D(ScreenPosition.X + LineW, ScreenPosition.Y + ScreenSize.Y), Thickness, RenderColor, Outlined));
	Cache->Lines.push_back(Drawing::LineCache(SDK::FVector2D(ScreenPosition.X, ScreenPosition.Y + ScreenSize.Y - LineH), SDK::FVector2D(ScreenPosition.X, ScreenPosition.Y + ScreenSize.Y - Correction), Thickness, RenderColor, Outlined));

	// Bottom-right corner horizontal and vertical
	Cache->Lines.push_back(Drawing::LineCache(SDK::FVector2D(ScreenPosition.X + ScreenSize.X - LineW, ScreenPosition.Y + ScreenSize.Y), SDK::FVector2D(ScreenPosition.X + ScreenSize.X - Correction, ScreenPosition.Y + ScreenSize.Y), Thickness, RenderColor, Outlined));
	Cache->Lines.push_back(Drawing::LineCache(SDK::FVector2D(ScreenPosition.X + ScreenSize.X, ScreenPosition.Y + ScreenSize.Y - LineH), SDK::FVector2D(ScreenPosition.X + ScreenSize.X, ScreenPosition.Y + ScreenSize.Y - Correction), Thickness, RenderColor, Outlined));

	DrawingQueue.push_back(std::move(Cache));
}
void Drawing::Triangle(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenPositionB, SDK::FVector2D ScreenPositionC, float Thickness, SDK::FLinearColor RenderColor, bool Filled, bool Outlined) {
	std::lock_guard<std::mutex> Lock(DrawingMutex);
	auto Cache = std::make_unique<TriangleCache>(ScreenPositionA, ScreenPositionB, ScreenPositionC, Thickness, RenderColor, Filled, Outlined);
	DrawingQueue.push_back(std::move(Cache));
}
#endif // _IMGUI
#ifdef _ENGINE
#include "../Game/SDK/Classes/Engine_Classes.h"
#include "../Utilities/Math.h"

void Drawing::Line(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenPositionB, float Thickness, SDK::FLinearColor RenderColor, bool Outlined) {
	if (Outlined) {
		SDK::GetLocalCanvas()->K2_DrawLine(ScreenPositionA, ScreenPositionB, Thickness + 1.f, SDK::FLinearColor(0.f, 0.f, 0.f, 1.f));
		SDK::GetLocalCanvas()->K2_DrawLine(ScreenPositionA, ScreenPositionB, Thickness - 1.f, SDK::FLinearColor(0.f, 0.f, 0.f, 1.f));
	}

	SDK::GetLocalCanvas()->K2_DrawLine(ScreenPositionA, ScreenPositionB, Thickness, RenderColor);
}
void Drawing::Text(const char* RenderText, SDK::FVector2D ScreenPosition, float FontSize, SDK::FLinearColor RenderColor, bool CentredX, bool CentredY, bool Outlined) {
	std::string String(RenderText);
	std::wstring WideString = std::wstring(String.begin(), String.end());

	Text(WideString.c_str(), ScreenPosition, FontSize, RenderColor, CentredX, CentredY, Outlined);
}
void Drawing::Text(const wchar_t* RenderText, SDK::FVector2D ScreenPosition, float FontSize, SDK::FLinearColor RenderColor, bool CentredX, bool CentredY, bool Outlined) {
	SDK::FString FString(RenderText);

	SDK::GetLocalCanvas()->K2_DrawText(FString, ScreenPosition, Outlined ? (int32)FontSize - 2 : (int32)FontSize, RenderColor, CentredX, CentredY, Outlined);
}
SDK::FVector2D Drawing::TextSize(const char* RenderText, float FontSize) {
	std::string String(RenderText);
	std::wstring WideString = std::wstring(String.begin(), String.end());

	return TextSize(WideString.c_str(), FontSize);
}
SDK::FVector2D Drawing::TextSize(const wchar_t* RenderText, float FontSize) {
	SDK::FString FString(RenderText);

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
	for (int i = 0; i < ScreenSize.X; i++) {
		Line(SDK::FVector2D(ScreenPosition.X + i, ScreenPosition.Y), SDK::FVector2D(ScreenPosition.X + i, ScreenPosition.Y + ScreenSize.Y), 1.f, RenderColor, false);
	}

	if (Outlined) {
		SDK::GetLocalCanvas()->K2_DrawBox(ScreenPosition, ScreenSize, 1.f, SDK::FLinearColor(0.f, 0.f, 0.f, 1.f));
	}
}
void Drawing::Rect(SDK::FVector2D ScreenPosition, SDK::FVector2D ScreenSize, float Thickness, SDK::FLinearColor RenderColor, bool Outlined) {
	if (Outlined) {
		SDK::GetLocalCanvas()->K2_DrawBox(ScreenPosition, ScreenSize, Thickness + 1.f, SDK::FLinearColor(0.f, 0.f, 0.f, 1.f));
		SDK::GetLocalCanvas()->K2_DrawBox(ScreenPosition, ScreenSize, Thickness - 1.f, SDK::FLinearColor(0.f, 0.f, 0.f, 1.f));
	}

	SDK::GetLocalCanvas()->K2_DrawBox(ScreenPosition, ScreenSize, Thickness, RenderColor);
}
void Drawing::CorneredRect(SDK::FVector2D ScreenPosition, SDK::FVector2D ScreenSize, float Thickness, SDK::FLinearColor RenderColor, bool Outlined) {
	float lineW = ScreenSize.X / 4;
	float lineH = ScreenSize.Y / 4;

	Line(SDK::FVector2D(ScreenPosition.X, ScreenPosition.Y), SDK::FVector2D(ScreenPosition.X + lineW, ScreenPosition.Y), Thickness, RenderColor, Outlined);
	Line(SDK::FVector2D(ScreenPosition.X, ScreenPosition.Y), SDK::FVector2D(ScreenPosition.X, ScreenPosition.Y + lineH), Thickness, RenderColor, Outlined);
	Line(SDK::FVector2D(ScreenPosition.X + ScreenSize.X, ScreenPosition.Y), SDK::FVector2D(ScreenPosition.X + ScreenSize.X, ScreenPosition.Y + lineH), Thickness, RenderColor, Outlined);
	Line(SDK::FVector2D(ScreenPosition.X + ScreenSize.X - lineW, ScreenPosition.Y), SDK::FVector2D(ScreenPosition.X + ScreenSize.X, ScreenPosition.Y), Thickness, RenderColor, Outlined);

	Line(SDK::FVector2D(ScreenPosition.X, ScreenPosition.Y + ScreenSize.Y), SDK::FVector2D(ScreenPosition.X + lineW, ScreenPosition.Y + ScreenSize.Y), Thickness, RenderColor, Outlined);
	Line(SDK::FVector2D(ScreenPosition.X, ScreenPosition.Y + ScreenSize.Y - lineH), SDK::FVector2D(ScreenPosition.X, ScreenPosition.Y + ScreenSize.Y), Thickness, RenderColor, Outlined);
	Line(SDK::FVector2D(ScreenPosition.X + ScreenSize.X, ScreenPosition.Y + ScreenSize.Y - lineH), SDK::FVector2D(ScreenPosition.X + ScreenSize.X, ScreenPosition.Y + ScreenSize.Y), Thickness, RenderColor, Outlined);
	Line(SDK::FVector2D(ScreenPosition.X + ScreenSize.X - lineW, ScreenPosition.Y + ScreenSize.Y), SDK::FVector2D(ScreenPosition.X + ScreenSize.X, ScreenPosition.Y + ScreenSize.Y), Thickness, RenderColor, Outlined);
}
void Drawing::Triangle(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenPositionB, SDK::FVector2D ScreenPositionC, float Thickness, SDK::FLinearColor RenderColor, bool Filled, bool Outlined) {
	// ADD LATER
}

#endif // _ENGINE