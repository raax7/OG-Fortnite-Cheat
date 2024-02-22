#include "Drawing.h"

#include "../Game/SDK/Classes/Engine_Classes.h"

#include <string>
#include <string>
#include "../Utilities/Math.h"

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

	Text(WideString.c_str(), ScreenPosition, FontSize, RenderColor, CenteredX, CenteredY, Outlined);
}
void Drawing::Text(const wchar_t* RenderText, SDK::FVector2D ScreenPosition, float FontSize, SDK::FLinearColor RenderColor, bool CenteredX, bool CenteredY, bool Outlined) {
	SDK::FString FString(RenderText);

	SDK::GetLocalCanvas()->K2_DrawText(FString, ScreenPosition, (int32)FontSize, RenderColor, CenteredX, CenteredY, Outlined);
}
SDK::FVector2D Drawing::TextSize(const wchar_t* RenderText, float FontSize) {
	SDK::FString FString(RenderText);

	return SDK::GetLocalCanvas()->K2_TextSize(FString, (int32)FontSize);
}
void Drawing::Circle(SDK::FVector2D ScreenPosition, float Radius, int32_t Segments, SDK::FLinearColor RenderColor, bool Outlined) {
	float Step = (float)(M_PI * 2.f / Segments);
	int Count = 0;
	SDK::FVector2D V[128];

	for (float a = 0; a < M_PI * 2.0; a += Step) {
		float X1 = Radius * cos(a) + ScreenPosition.X;
		float Y1 = Radius * sin(a) + ScreenPosition.Y;
		float X2 = Radius * cos(a + Step) + ScreenPosition.X;
		float Y2 = Radius * sin(a + Step) + ScreenPosition.Y;
		V[Count].X = X1;
		V[Count].Y = Y1;
		V[Count + 1].X = X2;
		V[Count + 1].Y = Y2;

		Line(SDK::FVector2D{ V[Count].X, V[Count].Y }, SDK::FVector2D{ X2, Y2 }, 1.0f, RenderColor, Outlined);
	}
}
void Drawing::FilledRect(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenSize, SDK::FLinearColor RenderColor, bool Outlined) {
	for (float i = 0.0f; i < ScreenSize.Y; i += 1.0f) {
		Line(SDK::FVector2D{ ScreenPositionA.X, ScreenPositionA.Y + i }, SDK::FVector2D{ ScreenPositionA.X + ScreenSize.X, ScreenPositionA.Y + i }, 1.0f, RenderColor, Outlined);
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