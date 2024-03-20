#pragma once
#ifdef _IMGUI
#include <vector>
#endif

#include "../Game/SDK/Classes/Basic.h"
#include <mutex>

/* A wrapper for drawing functions */
namespace Drawing {
#ifdef _IMGUI
	/* Cache lines for ImGui processing */
	struct LineCache {
		SDK::FVector2D ScreenPositionA;
		SDK::FVector2D ScreenPositionB;
		float Thickness;
		SDK::FLinearColor RenderColor;
		bool Outlined;
	};

	/* Cache texts for ImGui processing */
	struct TextCache {
		std::string RenderText;
		SDK::FVector2D ScreenPosition;
		float FontSize;
		SDK::FLinearColor RenderColor;
		bool CenteredX;
		bool CenteredY;
		bool Outlined;
	};

	/* Cache circles for ImGui processing */
	struct CircleCache {
		SDK::FVector2D ScreenPosition;
		float Radius;
		int32_t Segments;
		SDK::FLinearColor RenderColor;
		bool Outlined;
	};

	/* Cache filled rectangles for ImGui processing */
	struct FilledRectCache {
		SDK::FVector2D ScreenPosition;
		SDK::FVector2D ScreenSize;
		SDK::FLinearColor RenderColor;
		bool Outlined;
	};

	/* Cache hollow rectangles for ImGui processing */
	struct RectCache {
		SDK::FVector2D ScreenPositionA;
		SDK::FVector2D ScreenSize;
		float Thickness;
		SDK::FLinearColor RenderColor;
		bool Outlined;
	};

	/* Cache cornered rectangles for ImGui processing */
	struct CorneredRectCache {
		SDK::FVector2D ScreenPositionA;
		SDK::FVector2D ScreenSize;
		float Thickness;
		SDK::FLinearColor RenderColor;
		bool Outlined;
	};



	inline std::vector<LineCache> RenderBufferLine, UpdateBufferLine;
	inline std::vector<TextCache> RenderBufferText, UpdateBufferText;
	inline std::vector<CircleCache> RenderBufferCircle, UpdateBufferCircle;
	inline std::vector<FilledRectCache> RenderBufferFilledRect, UpdateBufferFilledRect;
	inline std::vector<RectCache> RenderBufferRect, UpdateBufferRect;
	inline std::vector<CorneredRectCache> RenderBufferCorneredRect, UpdateBufferCorneredRect;
#endif

#ifdef _IMGUI
	inline void SwapBuffers() {
		std::swap(RenderBufferLine, UpdateBufferLine);
		std::swap(RenderBufferText, UpdateBufferText);
		std::swap(RenderBufferCircle, UpdateBufferCircle);
		std::swap(RenderBufferFilledRect, UpdateBufferFilledRect);
		std::swap(RenderBufferRect, UpdateBufferRect);
		std::swap(RenderBufferCorneredRect, UpdateBufferCorneredRect);

		UpdateBufferLine.clear();
		UpdateBufferText.clear();
		UpdateBufferCircle.clear();
		UpdateBufferFilledRect.clear();
		UpdateBufferRect.clear();
		UpdateBufferCorneredRect.clear();
	}

	/* Render the queued data for drawing */
	void RenderQueuedDrawingInfo();
#endif

	/*
	* @brief Draws a line on the screen
	*
	* @param ScreenPositionA - The starting position of the line
	* @param ScreenPositionB - The ending position of the line
	* @param Thickness - The thickness of the line
	* @param RenderColor - The color of the line
	* @param Outlined - Whether or not the line should be outlined
	*/
	void Line(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenPositionB, float Thickness, SDK::FLinearColor RenderColor, bool Outlined);

	/*
	* @brief Overloaded function for drawing text on the screen (char*)
	*
	* @param RenderText - The text to draw
	* @param ScreenPosition - The position of the text
	* @param FontSize - The size of the text
	* @param RenderColor - The color of the text
	* @param CenteredX - Whether or not the text should be centered on the X axis
	* @param CenteredY - Whether or not the text should be centered on the Y axis
	* @param Outlined - Whether or not the text should be outlined
	*/
	void Text(const char* RenderText, SDK::FVector2D ScreenPosition, float FontSize, SDK::FLinearColor RenderColor, bool CenteredX, bool CenteredY, bool Outlined);

	/*
	* @brief Overloaded function for drawing text on the screen (wchar_t*)
	* 
	* @param RenderText - The text to draw
	* @param ScreenPosition - The position of the text
	* @param FontSize - The size of the text
	* @param RenderColor - The color of the text
	* @param CenteredX - Whether or not the text should be centered on the X axis
	* @param CenteredY - Whether or not the text should be centered on the Y axis
	* @param Outlined - Whether or not the text should be outlined
	*/
	void Text(const wchar_t* RenderText, SDK::FVector2D ScreenPosition, float FontSize, SDK::FLinearColor RenderColor, bool CenteredX, bool CenteredY, bool Outlined);
	
	/*
	* @brief Gets the estimated size of text drawn on the screen (char*)
	*
	* @param RenderText - The text to get the size of
	* @param FontSize - The size of the text
	*/
	SDK::FVector2D TextSize(const char* RenderText, float FontSize);

	/*
	* @brief Gets the estimated size of text drawn on the screen (wchar_t*)
	* 
	* @param RenderText - The text to get the size of
	* @param FontSize - The size of the text
	*/
	SDK::FVector2D TextSize(const wchar_t* RenderText, float FontSize);

	/*
	* @brief Draws a circle on the screen (wchar_t*)
	* 
	* @param ScreenPosition - The position of the circle
	* @param Radius - The radius of the circle
	* @param Segments - The amount of segments the circle should have
	* @param RenderColor - The color of the circle
	*/
	void Circle(SDK::FVector2D ScreenPosition, float Radius, int32_t Segments, SDK::FLinearColor RenderColor, bool Outlined);

	/*
	* @brief Draws a filled rectangle on the screen
	* 
	* @param ScreenPosition - The starting position of the rectangle
	* @param ScreenSize - The size of the rectangle
	* @param RenderColor - The color of the rectangle
	*/
	void FilledRect(SDK::FVector2D ScreenPosition, SDK::FVector2D ScreenSize, SDK::FLinearColor RenderColor, bool Outlined);

	/*
	* @brief Draws a hollow rectangle on the screen
	* 
	* @param ScreenPositionA - The starting position of the rectangle
	* @param ScreenSize - The size of the rectangle
	* @param Thickness - The thickness of the rectangle
	* @param RenderColor - The color of the rectangle
	* @param Outlined - Whether or not the rectangle should be outlined
	*/
	void Rect(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenSize, float Thickness, SDK::FLinearColor RenderColor, bool Outlined);

	/*
	* @brief Draws a cornered rectangle on the screen
	* 
	* @param ScreenPositionA - The starting position of the rectangle
	* @param ScreenSize - The size of the rectangle
	* @param Thickness - The thickness of the rectangle
	* @param RenderColor - The color of the rectangle
	*/
	void CorneredRect(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenSize, float Thickness, SDK::FLinearColor RenderColor, bool Outlined);
};