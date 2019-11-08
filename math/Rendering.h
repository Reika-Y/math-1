#pragma once
#include "Geometry.h"

class Rendering
{
public:
	Rendering();
	~Rendering();
	// スキャンラインアルゴリズムによるレンダリング
	int RenderScanLine(void);
private:
	void Corona(void);
	void SphereRendering(void);
	void BitMapDraw(float angle, float scale, bool isFilter);

	int FilterBiLinear(int lt, int rt, int ld, int rd, float lu, float lv);
	int GetPixelFiltered(float x, float y, int* r, int* g, int* b);
	int GetPixcel(int x, int y, int& r, int& g, int& b);
	int _hundle;
	Size _imageSize;
};

