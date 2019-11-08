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
const float scale_max = 5.0f;

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
	BitMapDraw(60.0f, 3, true);
	return 0;
}

// 日食のようなグラデーションの実装
void Rendering::Corona(void)
{
	// 一番内側の円の半径
	int _rad1 = 120;
	// 中間の円の半径
	int _rad2 = 130;
	// 一番外側の円の半径
	int _rad3 = 230;

	for (int y = 0; y < size.h; y++)
	{
		for (int x = 0; x < size.w; x++)
		{
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
	}
}

// 球形のレンダリング
void Rendering::SphereRendering(void)
{
	// 模様の分割数
	int div = 16;
	// 半径
	int _rad = 180;
	// 光源のベクトル
	Vector3 light = { -1.0f / 1.732f,-1.0f / 1.732f ,-1.0f / 1.732f };
	// アンビエント光
	auto ambient = 128.0f;
	// ディレクショナル光
	auto direct = 128.0f;

	for (int y = 0; y < size.h; y++)
	{
		for (int x = 0; x < size.w; x++)
		{
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
				auto fangle_xz = atan2f(dz, dx);
				if (fangle_xz < 0.0f)
				{
					// 角度を+にする
					fangle_xz += PI;
				}

				// 模様付け
				if (static_cast<int>(fangle_xz / (2.0f * PI) * div) & 1)
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
	}
}

void Rendering::BitMapDraw(float angle, float scale, bool isFilter)
{
	// ラジアン変換
	float rad = (2 * PI) / (360.0f / angle);
	// 拡大率
	auto fScale = 1.0f / scale;

	// 回転後x方向の基底ベクトル
	Vector2 vec_x = { cosf(rad),sinf(rad) };
	// 回転後y方向の基底ベクトル
	Vector2 vec_y = { cosf(rad + PI / 2.0f),sinf(rad + PI / 2.0f) };

	// 拡縮
	// 拡大率調整
	if (scale > scale_max)
	{
		scale = scale_max;
	}
	if (scale < scale_min)
	{
		scale = scale_min;
	}
	auto fscale = 1.0f / scale;
	vec_x *= fscale;
	vec_y *= fscale;

	// 回転後の左上座標
	Vector2 lt = { (-size.w / 2.0f * vec_x.x + -size.h / 2.0f * vec_y.x),(-size.w / 2.0f * vec_x.y + -size.h / 2.0f * vec_y.y) };

	for (int y = 0; y < size.h; y++)
	{
		auto pos = lt;
		for (int x = 0; x < size.w; x++)
		{
			auto draw_x = pos.x + size.w / 2.0f;
			auto draw_y = pos.y + size.h / 2.0f;
			// 色格納用
			int r, g, b = 0;

			if (isFilter)
			{
				GetPixelFiltered(draw_x, draw_y, &r, &g, &b);
			}
			else
			{
				GetPixcel(static_cast<int>(draw_x), static_cast<int>(draw_y), r, g, b);
			}
			// 描画
			DxLib::DrawPixel(x, y, DxLib::GetColor(r, g, b));

			// x方向に1ピクセルずらす
			pos += vec_x;
		}
		// y方向に1ピクセルずらす
		lt += vec_y;
	}
}

// バイリニアフィルタをかける
int Rendering::FilterBiLinear(int lt, int rt, int ld, int rd, float lu, float lv)
{
	// 真上
	auto lv0 = lt * (1.0f - lu) + rt * lu;
	// 真下
	auto lv1 = ld * (1.0f - lu) + rd * lu;
	// 求めるドットを返す
	return static_cast<int>(lv0 * (1.0f - lv) + lv1 * lv);
}

// バイリニアフィルタ付きピクセルの取得
int Rendering::GetPixelFiltered(float x, float y, int* r, int* g, int* b)
{
	// 色情報格納用
	int color[4][3];

	// 点1取得
	GetPixcel(static_cast<int>(x), static_cast<int>(y), color[0][0], color[0][1], color[0][2]);
	// 点2取得
	GetPixcel(static_cast<int>(x+1), static_cast<int>(y), color[1][0], color[1][1], color[1][2]);
	// 点3取得
	GetPixcel(static_cast<int>(x), static_cast<int>(y+1), color[2][0], color[2][1], color[2][2]);
	// 点4取得
	GetPixcel(static_cast<int>(x+1), static_cast<int>(y+1), color[3][0], color[3][1], color[3][2]);

	auto lu = x - static_cast<int>(x);  
	auto lv = y - static_cast<int>(y);

	*r = FilterBiLinear(color[0][0], color[1][0], color[2][0], color[3][0], lu, lv);
	*g = FilterBiLinear(color[0][1], color[1][1], color[2][1], color[3][1], lu, lv);
	*b = FilterBiLinear(color[0][2], color[1][2], color[2][2], color[3][2], lu, lv);

	return 0;
}

int Rendering::GetPixcel(int x, int y, int& r, int& g, int& b)
{
	if (DxLib::GetPixelSoftImage(_hundle, x, y, &r, &g, &b, nullptr) == -1)
	{
		// エラーだったら黒にする
		r = g = b = 0;
	}
	return 0;
}


