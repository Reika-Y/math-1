﻿#pragma once

//サイズを表す構造体
struct Size {
	int w;//幅
	int h;//高さ
};

//2D座標・ベクトルを表す構造体
struct Vector2 {
	Vector2():x(0),y(0){}
	Vector2(float inx,float iny):x(inx),y(iny){}
	float x, y;
	///ベクトルの大きさを返します
	float Magnitude()const;
	
	///正規化(大きさを１に)します
	void Normalize();

	///正規化ベクトルを返します
	Vector2 Normalized();

	void operator+=(const Vector2& v);
	void operator-=(const Vector2& v);
	void operator*=(float scale);
	Vector2 operator*(float scale);
	Vector2 operator-() {
		return Vector2(-x, -y);
	}
};

Vector2 operator+(const Vector2& va, const Vector2 vb);
Vector2 operator-(const Vector2& va, const Vector2 vb);
Vector2 operator*(const Vector2& v, float k);
///内積を返す
float Dot(const Vector2& va, const Vector2& vb);

///外積を返す
float Cross(const Vector2& va, const Vector2& vb);

///内積演算子
//float operator*(const Vector2& va, const Vector2& vb);

///外積演算子
//float operator%(const Vector2& va, const Vector2& vb);

//とりあえず「座標」って意味だとベクタより
//Positionのほうがよくね？って理由でこの名前
typedef Vector2 Position2;


//3D座標・ベクトルを表す構造体
struct Vector3 {
	Vector3() :x(0), y(0) ,z(0){}
	Vector3(float inx, float iny,float inz) :x(inx), y(iny) ,z(inz){}
	float x, y,z;
	///ベクトルの大きさを返します
	float Magnitude()const;

	///正規化(大きさを１に)します
	void Normalize();

	///正規化ベクトルを返します
	Vector3 Normalized();

	void operator+=(const Vector3& v);
	void operator-=(const Vector3& v);
	void operator*=(float scale);
	Vector3 operator*(float scale)const;
	Vector3 operator-() {
		return Vector3(-x, -y,-z);
	}
};
Vector3 operator+(const Vector3& va, const Vector3 vb);
Vector3 operator-(const Vector3& va, const Vector3 vb);

///内積を返す
float Dot(const Vector3& va, const Vector3& vb);

///外積を返す
Vector3 Cross(const Vector3& va, const Vector3& vb);

///乗算演算子
Vector3 operator*(const Vector3& va, const Vector3& vb);

///外積演算子
Vector3 operator%(const Vector3& va, const Vector3& vb);

typedef Vector3 Position3;
typedef Vector3 Color;

///円を表す構造体
struct Circle {
	float radius;//半径
	Position2 pos; //中心座標
	Circle() :radius(0), pos(0, 0) {}
	Circle(float r, Position2& p) :radius(r), pos(p) {}
};

///矩形を表す構造体
struct Rect {
	Position2 pos; //中心座標
	int w, h;//幅,高さ
	Rect() : pos(0, 0), w(0), h(0) {}
	Rect(float x, float y, int inw, int inh) :
		pos(x, y), w(inw), h(inh) {}
	Rect(Position2& inpos, int inw, int inh) :
		pos(inpos), w(inw), h(inh)
	{}
	void SetCenter(float x, float y) {
		pos.x = x;
		pos.y = y;
	}
	void SetCenter(const Position2& inpos) {
		pos.x = inpos.x;
		pos.y = inpos.y;
	}
	Vector2 Center() {
		return pos;
	}
	float Left() { return pos.x - w / 2; }
	float Top() { return pos.y - h / 2; }
	float Right() { return pos.x + w / 2; }
	float Bottom() { return pos.y + h / 2; }
	void Draw();//自分の矩形を描画する
	void Draw(Vector2& offset);//自分の矩形を描画する(オフセット付き)
};
