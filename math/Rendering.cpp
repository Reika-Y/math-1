#include "Rendering.h"
#include <math.h>
#include <DxLib.h>
#include "Application.h"

// 画面サイズ
auto size = Application::Instance().screenSize();
// 円周率
const float PI = 3.1415927f;
// 最小拡大率
const float scale_min = 0.1f;
// 最大拡大率
const float scale_max = 2.0f;

Rendering::Rendering()
{
	// 読み込み
	_hundle = DxLib::LoadSoftImage("image/image.bmp");
	// サイズ取得
	DxLib::GetSoftImageSize(_hundle, &_imageSize.w, &_imageSize.h);
}

Rendering::~Rendering()
{
}

// スキャンラインアルゴリズムによるレンダリング
int Rendering::RenderScanLine(void)
{
	for (int y = 0; y < size.h; y++)
	{
		for (int x = 0; x < size.w; x++)
		{
			ScaleImage(x, y, 2.0f);
		}
	}
	return 0;
}

// 日食のようなグラデーションの実装
void Rendering::Corona(int x, int y)
{
	// 一番内側の円の半径
	int _rad1 = 120;
	// 中間の円の半径
	int _rad2 = 130;
	// 一番外側の円の半径
	int _rad3 = 230;

	// 相対座標の計算
	auto dx = x - size.w / 2;
	auto dy = y - size.h / 2;

	// 画面の中央からどれだけ離れているか
	auto rc = sqrtf(dx * dx + dy * dy);
	// 境界をぼかしてジャギーを目立たなくする
	if ((rc > _rad1) && (rc < _rad3))
	{
		int nBright = 0;
		// 明るさが画面中心からの距離に連動
		if (rc < _rad2)
		{
			// _rad1の部分で明るさ0, _rad2の部分で明るさ255
			nBright = static_cast<int>((rc - _rad1) / (_rad2 - _rad1) * 255.0f);
		}
		else
		{
			// _rad2の部分で明るさ255、_rad3の部分で明るさ0
			nBright = static_cast<int>(255.0f - (rc - _rad2) / (_rad3 - _rad2) * 255.0f);
		}
		DxLib::DrawPixel(x, y, DxLib::GetColor(0, nBright, nBright));
	}
}

// 球形のレンダリング
void Rendering::SphereRendering(int x, int y)
{
	// 半径
	int _rad = 180;
	// 光源のベクトル
	Vector3 light = { -1.0f / 1.732f,-1.0f / 1.732f ,-1.0f / 1.732f };
	// アンビエント光
	auto ambient = 128.0f;
	// ディレクショナル光
	auto direct = 128.0f;

	// 相対座標の計算
	auto dx = x - size.w / 2.0f;
	auto dy = y - size.h / 2.0f;

	// zの2乗計算
	auto dzsq = _rad * _rad - dx * dx - dy * dy;
	if (dzsq > 0.0f)
	{
		// z座標の+方向を取得
		auto dz = sqrtf(dzsq);
		// 法線
		auto normal = Vector3{ dx / _rad, dy / _rad, dz / _rad };
		// 内積
		auto dot = normal.x * light.x + normal.y * light.y + normal.z * light.z;
		int nBright = 0;
		if (dot >= 0.0f)
		{
			// 光源からの光が当たる
			// やってることはLambert反射(明るさ= Ambient + direct * cosθ)
			nBright = static_cast<int>(powf(dot, 2.0f) * direct + ambient);
		}
		else
		{
			// 光源からの光が当たらない
			nBright = static_cast<int>(ambient);
		}
		// xz平面上での座標
		auto fangle = atan2f(dz, dx);
		if (fangle < 0.0f)
		{
			// 角度を+にする
			fangle += PI;
		}
		// 模様付け
		if (static_cast<int>(fangle / (2.0f * PI) * 16) & 1)
		{
			// 白
			DxLib::DrawPixel(x, y, DxLib::GetColor(nBright, nBright, nBright));
		}
		else
		{
			// マゼンタ
			DxLib::DrawPixel(x, y, DxLib::GetColor(nBright, 0, nBright));
		}
	}
}

// 画像の任意倍率スケーリング
void Rendering::ScaleImage(int x, int y, float scale)
{
	// サイズの範囲外なら即リターン
	if (x / scale >= _imageSize.w || y / scale >= _imageSize.h)
	{
		return;
	}
	// 拡大率調整
	if (scale > scale_max)
	{
		scale = scale_max;
	}
	if (scale < scale_min)
	{
		scale = scale_min;
	}

	// 色格納用
	int r, g, b = 0;
	// ピクセルの色を取得
	DxLib::GetPixelSoftImage(_hundle, x / scale, y / scale, &r, &g, &b,nullptr);
	// 描画
	DxLib::DrawPixel(x, y, DxLib::GetColor(r, g, b));
}

// 画像の回転
void Rendering::RotateImage(int x, int y)
{

}

