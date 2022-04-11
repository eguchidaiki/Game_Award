#pragma once
#include "Vector3.h"

class DrawShape final
{
public: //シングルトン化
	static DrawShape* Get();
private:
	DrawShape();
	DrawShape(const DrawShape&) = delete;
	~DrawShape() {}
	DrawShape operator=(const DrawShape&) = delete;

private: //定数
	static const WORD rectIndex[6];

public: //静的メンバ関数
	static void Init();

	static void DrawPlane(const Vector3& pos1, const Vector3& pos2, const unsigned int& color);
	//static void DrawPlane(const Vector3& pos1, const Vector3& pos2, const COLOR_U8& color);

private: //静的メンバ変数
	//static VERTEX3D rectVertex[4];

};
