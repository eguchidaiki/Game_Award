#include "BaseBlock.h"
#include <DxLib.h>
#include "DrawShape.h"
#include "Colors.h"

const int BaseBlock::blockSize = 60;
const int BaseBlock::halfBlockSize = BaseBlock::blockSize / 2;

BaseBlock::BaseBlock() :
	pos{}
{
}

BaseBlock::~BaseBlock()
{
}

void BaseBlock::Init(const Vector3& pos)
{
	this->pos = pos;
}

void BaseBlock::Update(const Vector3& pos)
{
	this->pos = pos;

	pos1.x = pos.x * blockSize;
	pos1.y = pos.y * blockSize;
	pos2.x = (pos.x + 1) * blockSize;
	pos2.y = (pos.y + 1) * blockSize;
}

void BaseBlock::Draw(int offsetX, int offsetY)
{
	static Vector3 startPos = {}, endPos = {};

	startPos.x = pos1.x + offsetX;
	startPos.y = pos1.y + offsetY;
	endPos.x = (pos.x + 1) + offsetX;
	endPos.y = (pos.y + 1) + offsetY;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0x80);
	DrawShape::DrawPlane(startPos, endPos, WHITE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0x80);
}
