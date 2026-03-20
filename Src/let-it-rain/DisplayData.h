#pragma once

#include <d2d1.h>
#include <vector>
#include <dcomp.h>
#include <wrl/client.h>
#include <memory>

class FastNoiseLite;

class DisplayData
{
public:
	explicit DisplayData(ID2D1DeviceContext* dc);
	~DisplayData();
	void SetRainColor(COLORREF color);
	void SetSceneBounds(RECT sceneRect, float scaleFactor);
	void InvalidateSpriteCache();

	int Width = 100;
	int Height = 100;
	float ScaleFactor = 1.0f; // FullHD is considered as 1. 4K will be 2(twice height and width change).

	RECT SceneRect = { 0, 0, 100, 100 };
	RECT SceneRectNorm = { 0, 0, 100, 100 }; // normalized to left top as 0,0

	// Cursor position in SceneRect coordinate space for particle repulsion
	float CursorX = -1000.0f;
	float CursorY = -1000.0f;
	bool CursorInteractionEnabled = true;

	// Visible window rects for particle collision (in SceneRect coordinate space)
	std::vector<RECT> WindowRects;

	ID2D1DeviceContext* DC;

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> DropColorBrush;
	// Single brush reused for all splatter draws — opacity is set dynamically at draw time
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> SplatterColorBrush;

	// Cache for pre-rendered snowflakes (one per shape type)
	std::vector<Microsoft::WRL::ComPtr<ID2D1Bitmap>> SpriteCache;

	int MaxSnowHeight = 0;
	int SettleColumnOffset = 0; // Current strip offset for amortized SettleSnow
	int FullSettleFramesRemaining = 0; // Burst mode: process all columns at high flow rate
	std::vector<uint8_t> ScenePixels;
	// Pre-computed mask: 1 if pixel is inside a visible window, 0 otherwise.
	// Rebuilt each time window rects are updated. Same dimensions as ScenePixels.
	std::vector<uint8_t> WindowMask;
	std::unique_ptr<FastNoiseLite> pNoiseGen;

	void RebuildWindowMask();

private:
	static bool IsSame(const RECT& l, const RECT& r);
};