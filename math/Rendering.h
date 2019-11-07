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
	void Corona(int x, int y);
	void SphereRendering(int x, int y);
	void ScaleImage(int x, int y, float scale);

	int _hundle;
	Size _imageSize;
};

