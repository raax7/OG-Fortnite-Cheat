#pragma once
#ifdef _IMGUI
#include <vector>
#include <memory>
#include <mutex>
#endif

#include "../Game/SDK/Classes/Basic.h"

/* A wrapper for drawing functions */
namespace Drawing {
#ifdef _IMGUI
	class IDrawingCache {
	public:
		virtual void Draw() = 0;
	};

	/* Cache lines for ImGui processing */
	class LineCache : public IDrawingCache {
	public:
		LineCache(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenPositionB, float Thickness, SDK::FLinearColor RenderColor, bool Outlined) : ScreenPositionA(ScreenPositionA), ScreenPositionB(ScreenPositionB), Thickness(Thickness), RenderColor(RenderColor), Outlined(Outlined) { }

		void DrawOutline();
		void DrawLine();

		void Draw() override;
	public:
		SDK::FVector2D ScreenPositionA;
		SDK::FVector2D ScreenPositionB;
		float Thickness;
		SDK::FLinearColor RenderColor;
		bool Outlined;
	};

	/* Cache for multiple lines in batch for ImGui processing */
	class BatchLineCache : public IDrawingCache {
	public:
		void Draw() override;
	public:
		std::vector<LineCache> Lines;
	};

	/* Cache texts for ImGui processing */
	class TextCache : public IDrawingCache {
	public:
		TextCache(std::string RenderText, SDK::FVector2D ScreenPosition, float FontSize, SDK::FLinearColor RenderColor, bool CenteredX, bool CenteredY, bool Outlined) : RenderText(RenderText), ScreenPosition(ScreenPosition), FontSize(FontSize), RenderColor(RenderColor), CenteredX(CenteredX), CenteredY(CenteredY), Outlined(Outlined) { }

		void Draw() override;
	public:
		std::string RenderText;
		SDK::FVector2D ScreenPosition;
		float FontSize;
		SDK::FLinearColor RenderColor;
		bool CenteredX;
		bool CenteredY;
		bool Outlined;
	};

	/* Cache circles for ImGui processing */
	class CircleCache : public IDrawingCache {
	public:
		CircleCache(SDK::FVector2D ScreenPosition, float Radius, int32_t Segments, SDK::FLinearColor RenderColor, bool Outlined) : ScreenPosition(ScreenPosition), Radius(Radius), Segments(Segments), RenderColor(RenderColor), Outlined(Outlined) { }

		void Draw() override;
	public:
		SDK::FVector2D ScreenPosition;
		float Radius;
		int32_t Segments;
		SDK::FLinearColor RenderColor;
		bool Outlined;
	};

	/* Cache filled rectangles for ImGui processing */
	class FilledRectCache : public IDrawingCache {
	public:
		FilledRectCache(SDK::FVector2D ScreenPosition, SDK::FVector2D ScreenSize, SDK::FLinearColor RenderColor, bool Outlined) : ScreenPosition(ScreenPosition), ScreenSize(ScreenSize), RenderColor(RenderColor), Outlined(Outlined) { }

		void Draw() override;
	public:
		SDK::FVector2D ScreenPosition;
		SDK::FVector2D ScreenSize;
		SDK::FLinearColor RenderColor;
		bool Outlined;
	};

	/* Cache hollow rectangles for ImGui processing */
	class RectCache : public IDrawingCache {
	public:
		RectCache(SDK::FVector2D ScreenPosition, SDK::FVector2D ScreenSize, float Thickness, SDK::FLinearColor RenderColor, bool Outlined) : ScreenPosition(ScreenPosition), ScreenSize(ScreenSize), Thickness(Thickness), RenderColor(RenderColor), Outlined(Outlined) { }

		void Draw() override;
	public:
		SDK::FVector2D ScreenPosition;
		SDK::FVector2D ScreenSize;
		float Thickness;
		SDK::FLinearColor RenderColor;
		bool Outlined;
	};

	/* Cache triangles for ImGui processing */
	class TriangleCache : public IDrawingCache {
	public:
		TriangleCache(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenPositionB, SDK::FVector2D ScreenPositionC, float Thickness, SDK::FLinearColor RenderColor, bool Filled, bool Outlined) : ScreenPositionA(ScreenPositionA), ScreenPositionB(ScreenPositionB), ScreenPositionC(ScreenPositionC), Thickness(Thickness), RenderColor(RenderColor), Filled(Filled), Outlined(Outlined) { }

		void Draw() override;
	public:
		SDK::FVector2D ScreenPositionA;
		SDK::FVector2D ScreenPositionB;
		SDK::FVector2D ScreenPositionC;
		float Thickness;
		SDK::FLinearColor RenderColor;
		bool Filled;
		bool Outlined;
	};



	inline std::mutex DrawingMutex;
	inline std::vector<std::unique_ptr<IDrawingCache>> RenderBuffer, DrawingQueue;



	inline void SwapBuffers() {
		std::lock_guard<std::mutex> Lock(DrawingMutex);

		std::swap(RenderBuffer, DrawingQueue);

		DrawingQueue.clear();
	}

	/* Render the queued data for drawing */
	void RenderDrawingData();
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
	* @param ScreenPosition - The starting position of the rectangle
	* @param ScreenSize - The size of the rectangle
	* @param Thickness - The thickness of the rectangle
	* @param RenderColor - The color of the rectangle
	* @param Outlined - Whether or not the rectangle should be outlined
	*/
	void Rect(SDK::FVector2D ScreenPosition, SDK::FVector2D ScreenSize, float Thickness, SDK::FLinearColor RenderColor, bool Outlined);

	/*
	* @brief Draws a cornered rectangle on the screen
	* 
	* @param ScreenPosition - The starting position of the rectangle
	* @param ScreenSize - The size of the rectangle
	* @param Thickness - The thickness of the rectangle
	* @param RenderColor - The color of the rectangle
	*/
	void CorneredRect(SDK::FVector2D ScreenPosition, SDK::FVector2D ScreenSize, float Thickness, SDK::FLinearColor RenderColor, bool Outlined);

	/*
	* @brief Draws a triangle on the screen
	* 
	* @param ScreenPositionA - The first position of the triangle
	* @param ScreenPositionB - The second position of the triangle
	* @param ScreenPositionC - The third position of the triangle
	* @param Thickness - The thickness of the triangle
	* @param RenderColor - The color of the triangle
	* @param Filled - Whether or not the triangle should be filled
	* @param Outlined - Whether or not the triangle should be outlined
	*/
	void Triangle(SDK::FVector2D ScreenPositionA, SDK::FVector2D ScreenPositionB, SDK::FVector2D ScreenPositionC, float Thickness, SDK::FLinearColor RenderColor, bool Filled, bool Outlined);
};