#ifndef DRAWING_H
#define DRAWING_H

#include "../../SDK/SDK.h"
#include "../../SDK/Classes/Engine_classes.h"

namespace Drawing {
	inline SDK::UCanvas* TargetCanvas;

	inline void DrawOutlinedCornerBox(int X, int Y, int W, int H, SDK::FLinearColor Color, int Thickness) {
		float lineW = W / 4;
		float lineH = H / 4;

		TargetCanvas->K2_DrawLine(SDK::FVector2D(X, Y), SDK::FVector2D(X + lineW, Y), Thickness, Color);
		TargetCanvas->K2_DrawLine(SDK::FVector2D(X, Y), SDK::FVector2D(X, Y + lineH), Thickness, Color);
		TargetCanvas->K2_DrawLine(SDK::FVector2D(X + W, Y), SDK::FVector2D(X + W, Y + lineH), Thickness, Color);
		TargetCanvas->K2_DrawLine(SDK::FVector2D(X + W - lineW, Y), SDK::FVector2D(X + W, Y), Thickness, Color);

		TargetCanvas->K2_DrawLine(SDK::FVector2D(X, Y + H), SDK::FVector2D(X + lineW, Y + H), Thickness, Color);
		TargetCanvas->K2_DrawLine(SDK::FVector2D(X, Y + H - lineH), SDK::FVector2D(X, Y + H), Thickness, Color);
		TargetCanvas->K2_DrawLine(SDK::FVector2D(X + W, Y + H - lineH), SDK::FVector2D(X + W, Y + H), Thickness, Color);
		TargetCanvas->K2_DrawLine(SDK::FVector2D(X + W - lineW, Y + H), SDK::FVector2D(X + W, Y + H), Thickness, Color);
	}

	inline void FilledRect(SDK::FVector2D Position, SDK::FVector2D Size, SDK::FLinearColor Color)
	{
		if (!TargetCanvas) {
			return;
		}

		for (float i = 0.0f; i < Size.Y; i += 1.0f) {
			TargetCanvas->K2_DrawLine(SDK::FVector2D{ Position.X, Position.Y + i }, SDK::FVector2D{ Position.X + Size.X, Position.Y + i }, 1.0f, Color);
		}
	}

	inline void DrawCircle(SDK::FVector2D CirclePosition, int Radius, int NumSides, SDK::FLinearColor Color)
	{
		float PI = 3.1415927f;

		float Step = PI * 2.0 / NumSides;
		int Count = 0;
		SDK::FVector2D V[128];

		for (float a = 0; a < PI * 2.0; a += Step) {
			float X1 = Radius * cos(a) + CirclePosition.X;
			float Y1 = Radius * sin(a) + CirclePosition.Y;
			float X2 = Radius * cos(a + Step) + CirclePosition.X;
			float Y2 = Radius * sin(a + Step) + CirclePosition.Y;
			V[Count].X = X1;
			V[Count].Y = Y1;
			V[Count + 1].X = X2;
			V[Count + 1].Y = Y2;
			TargetCanvas->K2_DrawLine(SDK::FVector2D{ V[Count].X, V[Count].Y }, SDK::FVector2D{ X2, Y2 }, 1.0f, Color);
		}
	}

	inline void Text(const wchar_t* RenderText, SDK::FVector2D ScreenPosition, float FontSize, SDK::FLinearColor RenderColor, bool bCentreX, bool bCentreY, bool bOutlined)
	{
		if (!TargetCanvas) {
			return;
		}

		SDK::FString Text = RenderText;

		TargetCanvas->K2_DrawText(Text, ScreenPosition, FontSize, RenderColor, bCentreX, bCentreY, bOutlined);
	}
}

#endif