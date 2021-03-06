#pragma once
#include "Vector3.h"

class BaseBlock
{
public: //定数
	static const int blockSize;
	static const int halfBlockSize;

public: //メンバ関数
	BaseBlock();
	virtual ~BaseBlock();

	virtual void Init(const Vector3& pos);
	virtual void Update(const Vector3& pos);
	virtual void Draw(int offsetX, int offsetY);

public: //メンバ変数
	Vector3 pos;
private:
	Vector3 pos1, pos2;
};
