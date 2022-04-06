#pragma once
#include "Vector3.h"

class BaseBlock
{
public: //’è”
	static const int blockSize;
	static const int halfBlockSize;

public: //ƒƒ“ƒoŠÖ”
	BaseBlock();
	virtual ~BaseBlock();

	virtual void Init(const Vector3& pos);
	virtual void Update(const Vector3& pos);
	virtual void Draw(int offsetX, int offsetY);

public: //ƒƒ“ƒo•Ï”
	Vector3 pos;
private:
	Vector3 pos1, pos2;
};
