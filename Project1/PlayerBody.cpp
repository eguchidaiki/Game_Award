#include <RVector.h>

#include "PlayerBody.h"
#include "InputManger.h"
#include "Stage.h"
#include "Player.h"
#include <memory>

namespace
{
static Stage* stage = Stage::Get();
}

const float PlayerBody::BodySize = 50.0f;
const float PlayerBody::HalfBodySize = BodySize / 2.0f;

PlayerBody::PlayerBody() :
	IsActivate(false),
	Body_Type(),
	BodyStartPos{},
	BodyEndPos{},
	SlideStartPos{},
	IsFold(false),
	IsOpen(true),
	FoldCount(0),
	IsSlide(false),
	SlidePat(0),
	IsAction(false),
	IsHitLeft(false),
	IsHitUp(false),
	IsHitRight(false),
	IsHitDown(false),
	IsReverse{},
	IsCanFold{},
	BodyIsFall(false),
	SlideDis(),
	AfterBodyFoldCount(0),
	BodyDistance(1),
	Ease{}
{
}

PlayerBody::~PlayerBody()
{
}

void PlayerBody::Init(RVector3 position, BodyType number)
{
	Body_Type = number;

	if (Body_Type == BodyType::left)
	{
		BodyStartPos = {position.x - 90.0f, position.y - 30.0f, 0.0f};
	}
	else if (Body_Type == BodyType::up)
	{
		BodyStartPos = {position.x - 30.0f, position.y - 90.0f, 0.0f};
	}
	else if (Body_Type == BodyType::right)
	{
		BodyStartPos = {position.x + 30.0f, position.y - 30.0f, 0.0f};
	}
	else if (Body_Type == BodyType::down)
	{
		BodyStartPos = {position.x - 30.0f, position.y + 30.0f, 0.0f};
	}

	BodyEndPos = {BodyStartPos.x + BodySize, BodyStartPos.y + BodySize, 0.0f};

	IsOpen = true;
	IsFold = false;
}

void PlayerBody::Update(RVector3& center)
{
	Body_Open(center);

	Body_Fold(center);

	Body_Slide(center);
}

void PlayerBody::Draw(int offsetX, int offsetY)
{
	if (IsActivate == true)
	{
		BodySprite.DrawExtendSprite(static_cast<int>(BodyStartPos.x) + offsetX, static_cast<int>(BodyStartPos.y) + offsetY,
									static_cast<int>(BodyEndPos.x) + offsetX, static_cast<int>(BodyEndPos.y) + offsetY);

		BodySprite.Draw();
	}
	else
	{
		return;
	}
}

void PlayerBody::Create()
{
	if ((BodySprite.spdata->size.x <= 0) || (BodySprite.spdata->size.y <= 0))
	{
		Bodyhandle = TexManager::LoadTexture("./Resources/playerSub.png");
		BodySprite.Create(Bodyhandle);
	}
}

void PlayerBody::Body_Fold(RVector3& center)
{
	//折っている途中
	if (IsFold == true && IsOpen == false && IsAction == true && IsSlide == false)
	{
		Ease.addTime += Ease.maxTime / 60.0f;
		Ease.timeRate = min(Ease.addTime / Ease.maxTime, 1.0f);

		if (Body_Type == BodyType::left)
		{
			if (FoldCount == 0)
			{
				BodyEndPos = {center.x - static_cast<float>(25 + BodySize * (BodyDistance - 1)),center.y + 25 ,0.0f};
				BodyStartPos.x = Ease.easeOut(BodyEndPos.x - BodySize, BodyEndPos.x + BodySize, Ease.timeRate);
				BodyStartPos.y = BodyEndPos.y - BodySize;
			}
			else if (FoldCount == 1)
			{
				BodyStartPos = {center.x - 25,center.y - 25 ,0.0f};
				BodyEndPos.x = Ease.easeOut(BodyStartPos.x - BodySize, BodyStartPos.x + BodySize, Ease.timeRate);
				BodyEndPos.y = BodyStartPos.y + BodySize;
			}
		}
		if (Body_Type == BodyType::up)
		{
			if (FoldCount == 0)
			{
				BodyEndPos = {center.x + 25.0f, center.y - static_cast<float>(25 + BodySize * (BodyDistance - 1)), 0.0f};
				BodyStartPos.y = Ease.easeOut(BodyEndPos.y - BodySize, BodyEndPos.y + BodySize, Ease.timeRate);
				BodyStartPos.x = BodyEndPos.x - BodySize;
			}
			else if (FoldCount == 1)
			{
				BodyStartPos = {center.x - 25.0f, center.y - 25, 0.0f};
				BodyEndPos.y = Ease.easeOut(BodyStartPos.y - BodySize, BodyStartPos.y + BodySize, Ease.timeRate);
				BodyEndPos.x = BodyStartPos.x + BodySize;
			}
		}
		if (Body_Type == BodyType::right)
		{
			if (FoldCount == 0)
			{
				BodyStartPos = {center.x + static_cast<float>(25 + BodySize * (BodyDistance - 1)),center.y - 25 ,0.0f};
				BodyEndPos.x = Ease.easeOut(BodyStartPos.x + BodySize, BodyStartPos.x - BodySize, Ease.timeRate);
				BodyEndPos.y = BodyStartPos.y + BodySize;
			}
			else if (FoldCount == 1)
			{
				BodyEndPos = {center.x + 25,center.y + 25 ,0.0f};
				BodyStartPos.x = Ease.easeOut(BodyEndPos.x + BodySize, BodyEndPos.x - BodySize, Ease.timeRate);
				BodyStartPos.y = BodyEndPos.y - BodySize;
			}
		}
		if (Body_Type == BodyType::down)
		{
			if (FoldCount == 0)
			{
				BodyStartPos = {center.x - 25.0f, center.y + static_cast<float>(25 + BodySize * (BodyDistance - 1)), 0.0f};
				BodyEndPos.y = Ease.easeOut(BodyStartPos.y + BodySize, BodyStartPos.y - BodySize, Ease.timeRate);
				BodyEndPos.x = BodyStartPos.x + BodySize;
			}
			else if (FoldCount == 1)
			{
				BodyEndPos = {center.x + 25.0f, center.y + static_cast<float>(25 + BodySize * (BodyDistance - 1)), 0.0f};
				BodyStartPos.y = Ease.easeOut(BodyEndPos.y + BodySize, BodyEndPos.y - BodySize, Ease.timeRate);
				BodyStartPos.x = BodyStartPos.x + BodySize;
			}
		}

		if (Ease.timeRate >= 1.0f)
		{
			Ease.isMove = false;
			IsAction = false;
			FoldCount++;
		}
	}
	//折った後
	if (IsFold == true && IsOpen == false && IsAction == false)
	{
		if (Body_Type == BodyType::left)
		{
			if (FoldCount == 1)
			{
				BodyEndPos = {center.x - (25 + BodySize * (BodyDistance - 1)),center.y + 25 ,0.0f};
				BodyStartPos = {BodyEndPos.x + 50, BodyEndPos.y - BodySize ,0.0f};
			}
			else if (FoldCount == 2)
			{
				BodyStartPos = {center.x - 25,center.y - 25,0.0f};
				BodyEndPos = {BodyStartPos.x + BodySize, BodyStartPos.y + BodySize ,0.0f};
			}
		}
		else if (Body_Type == BodyType::up)
		{
			if (FoldCount == 1)
			{
				BodyStartPos = {center.x - 25,center.y + (25 + BodySize * (BodyDistance - 1)) ,0.0f};
				BodyEndPos = {BodyStartPos.x + BodySize,BodyStartPos.y - BodySize ,0.0f};
			}
			else if (FoldCount == 2)
			{
				BodyStartPos = {center.x - 25,center.y - 25 ,0.0f};
				BodyEndPos = {BodyStartPos.x + BodySize,BodyStartPos.y + BodySize ,0.0f};
			}
		}
		else if (Body_Type == BodyType::right)
		{
			if (FoldCount == 1)
			{
				BodyStartPos = {center.x + (25 + BodySize * (BodyDistance - 1)),center.y - 25 ,0.0f};
				BodyEndPos = {BodyStartPos.x - BodySize,BodyStartPos.y + BodySize ,0.0f};
			}
			else if (FoldCount == 2)
			{
				BodyStartPos = {center.x - 25,center.y - 25 ,0.0f};
				BodyEndPos = {BodyStartPos.x + BodySize, BodyStartPos.y + BodySize ,0.0f};
			}
		}
		else if (Body_Type == BodyType::down)
		{
			if (FoldCount == 1)
			{
				BodyStartPos = {center.x - 25,center.y + (25 + BodySize * (BodyDistance - 1)) ,0.0f};
				BodyEndPos = {BodyStartPos.x + BodySize,BodyStartPos.y - BodySize ,0.0f};
			}
			else if (FoldCount == 2)
			{
				BodyStartPos = {center.x - 25,center.y - 25 ,0.0f};
				BodyEndPos = {BodyStartPos.x + BodySize,BodyStartPos.y + BodySize ,0.0f};
			}
		}
	}
}

void PlayerBody::Body_Open(RVector3& center)
{
	//開いている途中
	if (IsFold == false && IsOpen == true && IsAction == true && IsSlide == false)
	{
		Ease.addTime += Ease.maxTime / 60.0f;
		Ease.timeRate = min(Ease.addTime / Ease.maxTime, 1.0f);

		if (Body_Type == BodyType::left)
		{
			if (FoldCount == 1)
			{
				BodyEndPos = {center.x - static_cast<float>(25 + BodySize * (BodyDistance - 1)),center.y + 25 ,0.0f};
				BodyStartPos.x = Ease.easeOut(BodyEndPos.x + BodySize, BodyEndPos.x - BodySize, Ease.timeRate);
				BodyStartPos.y = BodyEndPos.y - BodySize;
			}
			else if (FoldCount == 2)
			{
				BodyStartPos = {center.x - 25,center.y - 25 ,0.0f};
				BodyEndPos.x = Ease.easeOut(BodyStartPos.x + BodySize, BodyStartPos.x - BodySize, Ease.timeRate);
				BodyEndPos.y = BodyStartPos.y + BodySize;
			}
		}
		if (Body_Type == BodyType::up)
		{
			if (FoldCount == 1)
			{
				BodyEndPos = {center.x + 25.0f, center.y - static_cast<float>(25 + BodySize * (BodyDistance - 1)), 0.0f};
				BodyStartPos.y = Ease.easeOut(BodyEndPos.y + BodySize, BodyEndPos.y - BodySize, Ease.timeRate);
				BodyStartPos.x = BodyEndPos.x - BodySize;
			}
			else if (FoldCount == 2)
			{
				BodyStartPos = {center.x - 25.0f, center.y - 25.0f, 0.0f};
				BodyEndPos.y = Ease.easeOut(BodyStartPos.y + BodySize, BodyStartPos.y - BodySize, Ease.timeRate);
				BodyEndPos.x = BodyEndPos.x - BodySize;
			}
		}
		if (Body_Type == BodyType::right)
		{
			if (FoldCount == 1)
			{
				BodyStartPos = {center.x + static_cast<float>(25 + BodySize * (BodyDistance - 1)),center.y - 25 ,0.0f};
				BodyEndPos.x = Ease.easeOut(BodyStartPos.x - BodySize, BodyStartPos.x + BodySize, Ease.timeRate);
				BodyEndPos.y = BodyStartPos.y + BodySize;
			}
			else if (FoldCount == 2)
			{
				BodyEndPos = {center.x + 25,center.y + 25 ,0.0f};
				BodyStartPos.x = Ease.easeOut(BodyEndPos.x - BodySize, BodyEndPos.x + BodySize, Ease.timeRate);
				BodyStartPos.y = BodyEndPos.y - BodySize;
			}
		}
		if (Body_Type == BodyType::down)
		{
			if (FoldCount == 1)
			{
				BodyStartPos = {center.x - 25.0f, center.y + static_cast<float>(25 + BodySize * (BodyDistance - 1)), 0.0f};
				BodyEndPos.y = Ease.easeOut(BodyStartPos.y - BodySize, BodyStartPos.y + BodySize, Ease.timeRate);
				BodyEndPos.x = BodyStartPos.x + BodySize;
			}
			else if (FoldCount == 2)
			{
				BodyEndPos = {center.x + 25.0f, center.y + 25, 0.0f};
				BodyStartPos.y = Ease.easeOut(BodyEndPos.y - BodySize, BodyEndPos.y + BodySize, Ease.timeRate);
				BodyStartPos.x = BodyEndPos.x - BodySize;
			}
		}

		if (Ease.timeRate >= 1.0f)
		{
			Ease.isMove = false;
			IsAction = false;
			FoldCount--;
		}
	}
	//開いた後
	if (IsFold == false && IsOpen == true && IsAction == false)
	{
		if (Body_Type == BodyType::left)
		{
			if (FoldCount == 0)
			{
				BodyStartPos = {center.x - (25 + BodyDistance * BodySize),center.y - 25,0.0f};
				BodyEndPos = {BodyStartPos.x + BodySize,BodyStartPos.y + BodySize,0.0f};
			}
			else if (FoldCount == 1)
			{
				BodyStartPos = {center.x - 25,center.y - 25 ,0.0f};
				BodyEndPos = {BodyStartPos.x - BodySize,BodyStartPos.y + BodySize ,0.0f};
				IsFold = true;
				IsOpen = false;
			}
		}
		if (Body_Type == BodyType::up)
		{
			if (FoldCount == 0)
			{
				BodyStartPos = {center.x - 25.0f,center.y - (25 + BodyDistance * BodySize),0.0f};
				BodyEndPos = {BodyStartPos.x + 50,BodyStartPos.y + 50,0.0f};
			}
			else if (FoldCount == 1)
			{
				BodyStartPos = {center.x - 25.0f,center.y - 25.0f,0.0f};
				BodyEndPos = {BodyStartPos.x + 50,BodyStartPos.y + 50,0.0f};
				IsFold = true;
				IsOpen = false;
			}
		}
		if (Body_Type == BodyType::right)
		{
			if (FoldCount == 0)
			{
				BodyStartPos = {center.x + (25 + (BodyDistance - 1) * BodySize),center.y - 25,0.0f};
				BodyEndPos = {BodyStartPos.x + 50,BodyStartPos.y + 50 ,0.0f};
			}
			else if (FoldCount == 1)
			{
				BodyEndPos = {center.x + 25,center.y + 25,0.0f};
				BodyStartPos = {BodyEndPos.x + BodySize,BodyEndPos.y - BodySize,0.0f};
				IsFold = true;
				IsOpen = false;
			}
		}
		if (Body_Type == BodyType::down)
		{
			if (FoldCount == 0)
			{
				BodyStartPos = {center.x - 25,center.y + (25 + (BodyDistance - 1) * BodySize) ,0.0f};
				BodyEndPos = {BodyStartPos.x + 50,BodyStartPos.y + 50 ,0.0f};
			}
			else if (FoldCount == 1)
			{
				BodyEndPos = {center.x + 25,center.y + 25,0.0f};
				BodyStartPos = {BodyEndPos.x + BodySize,BodyEndPos.y - BodySize,0.0f};
				IsFold = true;
				IsOpen = false;
			}
		}
	}
}

void PlayerBody::Body_Slide(RVector3& center)
{
	//体のスライド
	//顔を挟むとなり移動
	if (IsSlide == true && Ease.isMove == true && SlideDis == 2)
	{
		Ease.addTime += Ease.maxTime / 25.0f;
		Ease.timeRate = min(Ease.addTime / Ease.maxTime, 1.0f);

		if (Body_Type == BodyType::left)
		{
			BodyStartPos = {Ease.easeOut(center.x - 75, center.x + 25, Ease.timeRate), center.y - 25.0f, 0.0f};
			BodyEndPos = {BodyStartPos.x + BodySize, center.y + 25.0f, 0.0f};
		}
		else if (Body_Type == BodyType::right)
		{
			BodyStartPos = {Ease.easeOut(center.x + 25, center.x - 75, Ease.timeRate), center.y - 25.0f, 0.0f};
			BodyEndPos = {BodyStartPos.x + BodySize, center.y + 25.0f, 0.0f};
		}
		else if (Body_Type == BodyType::up)
		{
			BodyStartPos = {center.x - 25.0f, Ease.easeOut(center.y - 75, center.y + 25, Ease.timeRate), 0.0f};
			BodyEndPos = {center.x + 25.0f, BodyStartPos.y + BodySize, 0.0f};
		}
		else if (Body_Type == BodyType::down)
		{
			BodyStartPos = {center.x - 25.0f, Ease.easeOut(center.y + 25, center.y - 75, Ease.timeRate), 0.0f};
			BodyEndPos = {center.x + 25.0f, BodyStartPos.y + BodySize, 0.0f};
		}

		if (Ease.timeRate >= 1.0f)
		{
			Ease.isMove = false;
			IsAction = false;
			IsSlide = false;

			if (Body_Type == BodyType::left)
			{
				Body_Type = BodyType::right;
			}
			else if (Body_Type == BodyType::right)
			{
				Body_Type = BodyType::left;
			}
			else if (Body_Type == BodyType::up)
			{
				Body_Type = BodyType::down;
			}
			else if (Body_Type == BodyType::down)
			{
				Body_Type = BodyType::up;
			}
		}
	}

	//顔を挟まないとなり移動(左右のみ)
	if (IsSlide == true && Ease.isMove == true && SlideDis == 1)
	{
		Ease.addTime += Ease.maxTime / 25.0f;
		Ease.timeRate = min(Ease.addTime / Ease.maxTime, 1.0f);

		if (Body_Type == BodyType::left)
		{
			if (SlidePat == -1)
			{
				BodyEndPos = {Ease.easeOut(center.x - 25, center.x - 75, Ease.timeRate), center.y - 25.0f, 0.0f};
			}
			else
			{
				BodyEndPos = {Ease.easeOut(center.x - 75, center.x - 25, Ease.timeRate), center.y - 25.0f, 0.0f};
			}
			BodyStartPos = {BodyEndPos.x + static_cast<float>(100 * IsFold - BodySize), center.y + 25.0f, 0.0f};
		}
		else if (Body_Type == BodyType::up)
		{
			if (SlidePat == -1)
			{
				BodyEndPos = {center.x - 25.0f, Ease.easeOut(center.y - 25, center.y - 75, Ease.timeRate), 0.0f};
			}
			else
			{
				BodyEndPos = {center.x - 25.0f, Ease.easeOut(center.y - 75, center.y - 25, Ease.timeRate), 0.0f};
			}
			BodyStartPos = {BodyEndPos.x + static_cast<float>(100 * IsFold - BodySize), center.y + 25.0f, 0.0f};
		}
		else if (Body_Type == BodyType::right)
		{
			if (SlidePat == -1)
			{
				BodyStartPos = {Ease.easeOut(center.x + 75, center.x + 25, Ease.timeRate), center.y - 25.0f, 0.0f};
			}
			else
			{
				BodyStartPos = {Ease.easeOut(center.x + 25, center.x + 75, Ease.timeRate), center.y - 30.0f, 0.0f};
			}
			BodyEndPos = {BodyStartPos.x + static_cast<float>(-100 * IsFold + BodySize), center.y + 25.0f, 0.0f};
		}
		else if (Body_Type == BodyType::down)
		{
			if (SlidePat == -1)
			{
				BodyStartPos = {center.x - 25.0f, Ease.easeOut(center.y + 75, center.y + 25, Ease.timeRate), 0.0f};
			}
			else
			{
				BodyStartPos = {center.x - 25.0f, Ease.easeOut(center.y + 25, center.y + 75, Ease.timeRate), 0.0f};
			}
			BodyEndPos = {BodyStartPos.x + static_cast<float>(-100 * IsFold + BodySize), center.y + 25.0f, 0.0f};
		}

		if (Ease.timeRate >= 1.0f)
		{
			Ease.isMove = false;
			IsAction = false;
			IsSlide = false;
		}
	}
}

void PlayerBody::setactivate(RVector3 center)
{
	IsFold = false;
	IsOpen = true;
	IsSlide = false;
	BodyDistance = 1;
	AfterBodyFoldCount = 0;
	FoldCount = 0;

	if (IsActivate == true)
	{
		if (Body_Type == BodyType::left)
		{
			BodyStartPos = {center.x - 90.0f, center.y - 30.0f, 0.0f};
		}
		else if (Body_Type == BodyType::right)
		{
			BodyStartPos = {center.x + 30.0f, center.y - 30.0f, 0.0f};
		}
		else if (Body_Type == BodyType::up)
		{
			BodyStartPos = {center.x - 30.0f, center.y - 90.0f, 0.0f};
		}
		else if (Body_Type == BodyType::down)
		{
			BodyStartPos = {center.x - 30.0f, center.y + 30.0f, 0.0f};
		}

		BodyEndPos = {BodyStartPos.x + 60.0f, BodyStartPos.y + 60.0f, 0.0f};
	}
}

void PlayerBody::setslide(int slidepat, int move_dis)
{
	IsAction = true;
	IsSlide = true;
	Ease.isMove = true;

	Ease.addTime = 0.1f;
	Ease.maxTime = 1.2f;
	Ease.timeRate = 0.0f;

	this->SlidePat = slidepat;

	SlideDis = move_dis;
}

void PlayerBody::IsHitBody(RVector3* center, float& FallSpeed, bool& isfall, bool& isjump, bool& iscolide)
{
	Update(*center);

	//体の四辺
	float BodyLeft;
	float BodyRight;
	float BodyUp;
	float BodyDown;

	//StartPosとEndPosの位置関係によって上下左右の設定を変える
	if (BodyStartPos.x < BodyEndPos.x)
	{
		BodyLeft = BodyStartPos.x;
		BodyRight = BodyStartPos.x + (BodySize - 1.0f);
	}
	else
	{
		BodyLeft = BodyEndPos.x;
		BodyRight = BodyEndPos.x + (BodySize - 1.0f);
	}

	if (BodyStartPos.y < BodyEndPos.y)
	{
		BodyUp = BodyStartPos.y;
		BodyDown = BodyStartPos.y + ((BodySize + 8) - 1.0f);
	}
	else
	{
		BodyUp = BodyEndPos.y;
		BodyDown = BodyEndPos.y + ((BodySize + 8) - 1.0f);
	}

	//四辺をブロックサイズで割った数
	int BodyLeft_mapchip = (int)(BodyLeft - stage->offset.x) / 60;
	int BodyUp_mapchip = (int)(BodyUp - stage->offset.y) / 60;
	int BodyRight_mapchip = (int)(BodyRight - stage->offset.x) / 60;
	int BodyDown_mapchip = (int)(BodyDown - stage->offset.y) / 60;

	//タイル内のマップチップ座標
	int BodyLeft_mapchip_tile;
	int BodyUp_mapchip_tile;
	int BodyRight_mapchip_tile;
	int BodyDown_mapchip_tile;

	//押し出す方向を決めるための距離
	float BuriedX = 0;
	float BuriedY = 0;

	//ステージの数
	size_t i = 0;
	//タイルの数
	size_t j = 0;

	//マップチップの座標
	int mapchipPos = 0;

	IsHitLeft = false;
	IsHitUp = false;
	IsHitRight = false;
	IsHitDown = false;

	int FallCount = 0;

	int JumpCountLeft = 0;
	int jumpCountRight = 0;

	if (BodyLeft < stage->offset.x)
	{
		center->x = stage->offset.x + (center->x - BodyLeft);
		iscolide = true;
	}
	if (BodyUp < stage->offset.y)
	{
		center->y = stage->offset.y + (center->y - BodyUp);
		iscolide = true;
	}

	//体の四隅とブロックとの判定
	for (i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			//左上
			if (stage->IsPositionTile({BodyLeft,BodyUp,0.0f}, i, j))
			{
				BodyLeft_mapchip_tile = BodyLeft_mapchip % stage->GetStageTileWidth(i, j);
				BodyUp_mapchip_tile = BodyUp_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = BodyUp_mapchip_tile * stage->GetStageTileWidth(i, j) + BodyLeft_mapchip_tile;
				if (stage->GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
				{
					BuriedX = (BodyLeft_mapchip * 60) - BodyLeft;
					BuriedY = (BodyUp_mapchip * 60) - BodyUp;

					if (BuriedX > BuriedY)
					{
						if (IsHitUp == false)
						{
							center->y = (BodyUp_mapchip + 1) * 60 + (center->y - BodyUp);
							IsHitUp = true;
						}
						FallSpeed = 0.0f;
					}
					else if (BuriedX < BuriedY)
					{
						if (IsHitLeft == false && Body_Type == BodyType::left || Body_Type == BodyType::up)
						{
							center->x = (BodyLeft_mapchip + 1) * 60 + (center->x - BodyLeft);
							IsHitLeft = true;
							JumpCountLeft += Player::Get()->IsLeft;
						}
					}
				}
			}
			//左下
			if (stage->IsPositionTile({BodyLeft,BodyDown,0.0f}, i, j))
			{
				BodyLeft_mapchip_tile = BodyLeft_mapchip % stage->GetStageTileWidth(i, j);
				BodyDown_mapchip_tile = BodyDown_mapchip % stage->GetStageTileHeight(i, j);

				mapchipPos = (BodyDown_mapchip_tile)*stage->GetStageTileWidth(i, j) + (BodyLeft_mapchip_tile);
				if (stage->GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
				{
					BuriedX = (BodyLeft_mapchip * 60) - BodyLeft;
					BuriedY = (BodyDown - 60) - (BodyDown_mapchip * 60);

					if (BuriedX > BuriedY)
					{
						center->y = (BodyDown_mapchip * 60) - (BodyDown - center->y);
						IsHitDown = true;
						FallCount++;
						Player::Get()->IsInitJump = false;
					}
					else if (BuriedX < BuriedY)
					{
						if (IsHitLeft == false)
						{
							center->x = (BodyLeft_mapchip + 1) * 60 + (center->x - BodyLeft);
							IsHitLeft = true;
							JumpCountLeft += Player::Get()->IsLeft;
							Player::Get()->IsInitJump = false;
						}
					}
				}
			}
			//右上
			if (stage->IsPositionTile({BodyRight,BodyUp,0.0f}, i, j))
			{
				BodyRight_mapchip_tile = BodyRight_mapchip % stage->GetStageTileWidth(i, j);
				BodyUp_mapchip_tile = BodyUp_mapchip % stage->GetStageTileHeight(i, j);

				mapchipPos = (BodyUp_mapchip_tile)*stage->GetStageTileWidth(i, j) + (BodyRight_mapchip_tile);
				if (stage->GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
				{
					BuriedX = (BodyRight - 60) - (BodyRight_mapchip * 60);
					BuriedY = (BodyUp_mapchip * 60) - BodyUp;

					if (BuriedX > BuriedY)
					{
						if (IsHitUp == false)
						{
							center->y = (BodyUp_mapchip + 1) * 60 + (center->y - BodyUp);
							IsHitUp = true;
						}
						FallSpeed = 0.0f;
					}
					else if (BuriedX < BuriedY)
					{
						if (IsHitRight == false && Body_Type == BodyType::right || Body_Type == BodyType::up)
						{
							center->x = (BodyRight_mapchip * 60) - (BodyRight - center->x);
							IsHitRight = true;
							jumpCountRight += Player::Get()->IsRight;
						}
					}
				}
			}
			//右下
			if (stage->IsPositionTile({BodyRight,BodyDown,0.0f}, i, j))
			{
				BodyRight_mapchip_tile = BodyRight_mapchip % stage->GetStageTileWidth(i, j);
				BodyDown_mapchip_tile = BodyDown_mapchip % stage->GetStageTileHeight(i, j);

				mapchipPos = (BodyDown_mapchip_tile)*stage->GetStageTileWidth(i, j) + (BodyRight_mapchip_tile);
				if (stage->GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
				{
					BuriedX = (BodyRight - 60) - (BodyRight_mapchip * 60);
					BuriedY = (BodyDown - 60) - (BodyDown_mapchip * 60);

					if (BuriedX > BuriedY)
					{
						center->y = (BodyDown_mapchip * 60) - (BodyDown - center->y);
						IsHitDown = true;
						FallCount++;
						Player::Get()->IsInitJump = false;
					}
					else if (BuriedX < BuriedY)
					{
						if (IsHitRight == false)
						{
							center->x = (BodyRight_mapchip * 60) - (BodyRight - center->x);
							IsHitRight = true;
							jumpCountRight += Player::Get()->IsRight;
						}
					}
				}
			}
		}
	}

	bool DiagonallyUpLeft = false;
	bool DiagonallyUpRight = false;

	//進む方向の斜め上にブロックがあるかどうか
	for (i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			if (stage->IsPositionTile({BodyLeft - 20,BodyUp - 20,0.0f}, i, j))
			{
				BodyLeft_mapchip_tile = (BodyLeft_mapchip - 1) % stage->GetStageTileWidth(i, j);
				BodyUp_mapchip_tile = (BodyUp_mapchip - 1) % stage->GetStageTileHeight(i, j);

				mapchipPos = BodyUp_mapchip_tile * stage->GetStageTileWidth(i, j) + BodyLeft_mapchip_tile;
				if (stage->GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK &&
					Player::Get()->IsLeft == true)
				{
					DiagonallyUpLeft = true;
				}
			}

			if (stage->IsPositionTile({BodyRight + 20,BodyUp - 20,0.0f}, i, j))
			{
				BodyRight_mapchip_tile = (BodyRight_mapchip) % stage->GetStageTileWidth(i, j);
				BodyUp_mapchip_tile = (BodyUp_mapchip - 1) % stage->GetStageTileHeight(i, j);

				mapchipPos = (BodyUp_mapchip_tile)*stage->GetStageTileWidth(i, j) + (BodyRight_mapchip_tile);
				if (stage->GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK &&
					Player::Get()->IsRight == true)
				{
					DiagonallyUpRight = true;
				}
			}
		}
	}

	if (((JumpCountLeft > 0 && DiagonallyUpLeft == false) || (jumpCountRight > 0 && DiagonallyUpRight == false)) && Player::Get()->IsInitJump == false)
	{
		Player::Get()->IsJump = true;
		Player::Get()->FallSpeed = -5.6f;
		Player::Get()->IsWalk = true;
		Player::Get()->IsInitJump = true;
	}

	if (FallCount > 0)
	{
		BodyIsFall = false;
	}
	else
	{
		BodyIsFall = true;
	}
}

bool PlayerBody::IsReverseHitBody(const unsigned char& direction)
{
	if (Player::Get()->IsPressInStage() == false)
	{
		return false;
	}

	//体の四辺
	float BodyLeft;
	float BodyRight;
	float BodyUp;
	float BodyDown;

	int ReverseLeftCount = 0;
	int ReverseUpCount = 0;
	int ReverseRightCount = 0;
	int ReverseDownCount = 0;

	//StartPosとEndPosの位置関係によって上下左右の設定を変える
	if (BodyStartPos.x < BodyEndPos.x)
	{
		BodyLeft = BodyStartPos.x;
		BodyRight = BodyStartPos.x + (BodySize - 1.0f);
	}
	else
	{
		BodyLeft = BodyEndPos.x;
		BodyRight = BodyEndPos.x + (BodySize - 1.0f);
	}

	if (BodyStartPos.y < BodyEndPos.y)
	{
		BodyUp = BodyStartPos.y;
		BodyDown = BodyStartPos.y + ((BodySize + 8) - 1.0f);
	}
	else
	{
		BodyUp = BodyEndPos.y;
		BodyDown = BodyEndPos.y + ((BodySize + 8) - 1.0f);
	}

	//四辺をブロックサイズで割った数
	int BodyLeft_mapchip = (int)(BodyLeft - stage->offset.x) / 60;
	int BodyUp_mapchip = (int)(BodyUp - stage->offset.y) / 60;
	int BodyRight_mapchip = (int)(BodyRight - stage->offset.x) / 60;
	int BodyDown_mapchip = (int)(BodyDown - stage->offset.y) / 60;

	//タイル内のマップチップ座標
	int BodyLeft_mapchip_tile;
	int BodyUp_mapchip_tile;
	int BodyRight_mapchip_tile;
	int BodyDown_mapchip_tile;

	//マップチップの座標
	int mapchipPos = 0;

	char* mapchip = {0};

	//設定用の値
	int SettingMapchip = stage->FoldSimulation(RVector3(BodyLeft, BodyUp, 0.0f), direction, &mapchip);

	if (SettingMapchip == -1)
	{
		//return false;
	}

	if (this->Body_Type == direction)
	{
		return true;
	}

	int BlockCount = 0;

	for (size_t i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (size_t j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			//左上
			if (stage->IsPositionTile({BodyLeft,BodyUp,0.0f}, i, j))
			{
				BodyLeft_mapchip_tile = BodyLeft_mapchip % stage->GetStageTileWidth(i, j);
				BodyUp_mapchip_tile = BodyUp_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = BodyUp_mapchip_tile * stage->GetStageTileWidth(i, j) + BodyLeft_mapchip_tile;

				if (mapchip != nullptr && mapchip[mapchipPos] == MapchipData::BLOCK)
				{
					BlockCount++;
				}
			}
			//左下
			if (stage->IsPositionTile({BodyLeft,BodyDown,0.0f}, i, j))
			{
				BodyLeft_mapchip_tile = BodyLeft_mapchip % stage->GetStageTileWidth(i, j);
				BodyDown_mapchip_tile = BodyDown_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = BodyDown_mapchip_tile * stage->GetStageTileWidth(i, j) + BodyLeft_mapchip_tile;

				if (mapchip != nullptr && mapchip[mapchipPos] == MapchipData::BLOCK)
				{
					BlockCount++;
				}
			}
			//右上
			if (stage->IsPositionTile({BodyRight,BodyUp,0.0f}, i, j))
			{
				BodyRight_mapchip_tile = BodyRight_mapchip % stage->GetStageTileWidth(i, j);
				BodyUp_mapchip_tile = BodyUp_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = BodyUp_mapchip_tile * stage->GetStageTileWidth(i, j) + BodyRight_mapchip_tile;

				if (mapchip != nullptr && mapchip[mapchipPos] == MapchipData::BLOCK)
				{
					BlockCount++;
				}
			}
			//右下
			if (stage->IsPositionTile({BodyRight,BodyDown,0.0f}, i, j))
			{
				BodyRight_mapchip_tile = BodyRight_mapchip % stage->GetStageTileWidth(i, j);
				BodyDown_mapchip_tile = BodyDown_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = BodyDown_mapchip_tile * stage->GetStageTileWidth(i, j) + BodyRight_mapchip_tile;

				if (mapchip != nullptr && mapchip[mapchipPos] == MapchipData::BLOCK)
				{
					BlockCount++;
				}
			}
		}
	}

	if (this->Body_Type == direction)
	{
		return true;
	}

	if (BlockCount > 0)
	{
		return false;
	}

	return true;
}

void PlayerBody::IsAroundBlock()
{
	//体の中心
	RVector3 BodyCenterPos = {0,0,0};

	//StartPosとEndPosの位置関係によって上下左右の設定を変える
	if (BodyStartPos.x < BodyEndPos.x)
	{
		BodyCenterPos.x = BodyStartPos.x + HalfBodySize;
	}
	else
	{
		BodyCenterPos.x = BodyEndPos.x + HalfBodySize;
	}

	if (BodyStartPos.y < BodyEndPos.y)
	{
		BodyCenterPos.y = BodyStartPos.y + HalfBodySize;
	}
	else
	{
		BodyCenterPos.y = BodyEndPos.y + HalfBodySize;
	}

	//中心座標をブロックサイズで割った数
	int Center_X_mapchip = (int)(BodyCenterPos.x - stage->offset.x) / 60;
	int Center_Y_mapchip = (int)(BodyCenterPos.y - stage->offset.x) / 60;

	//体の四辺
	float NextLeft = BodyCenterPos.x - BodySize;
	float NextRight = BodyCenterPos.x + BodySize;
	float NextUp = BodyCenterPos.y - BodySize;
	float NextDown = BodyCenterPos.y + BodySize;

	//四辺をブロックサイズで割った数
	int NextLeft_mapchip = (int)(NextLeft - stage->offset.x) / 60;
	int NextUp_mapchip = (int)(NextUp - stage->offset.y) / 60;
	int NextRight_mapchip = (int)(NextRight - stage->offset.x) / 60;
	int NextDown_mapchip = (int)(NextDown - stage->offset.y) / 60;

	//タイル内のマップチップ座標
	int X_mapchip_tile;
	int Y_mapchip_tile;

	//マップチップの座標
	int mapchipPos = 0;

	for (int i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (int j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			//左隣
			if (stage->IsPositionTile({NextLeft,BodyCenterPos.y,0.0}, i, j))
			{
				X_mapchip_tile = NextLeft_mapchip % stage->GetStageTileWidth(i, j);
				Y_mapchip_tile = Center_Y_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = Y_mapchip_tile * stage->GetStageTileWidth(i, j) + X_mapchip_tile;
				if (stage->GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
				{
					IsLeftBlock = true;
				}
				else
				{
					IsLeftBlock = false;
				}
			}

			//右隣
			if (stage->IsPositionTile({NextRight,BodyCenterPos.y,0.0}, i, j))
			{
				X_mapchip_tile = NextRight_mapchip % stage->GetStageTileWidth(i, j);
				Y_mapchip_tile = Center_Y_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = Y_mapchip_tile * stage->GetStageTileWidth(i, j) + X_mapchip_tile;
				if (stage->GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
				{
					IsRightBlock = true;
				}
				else
				{
					IsRightBlock = false;
				}
			}

			//上隣
			if (stage->IsPositionTile({BodyCenterPos.x,NextUp,0.0}, i, j))
			{
				X_mapchip_tile = Center_X_mapchip % stage->GetStageTileWidth(i, j);
				Y_mapchip_tile = NextUp_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = Y_mapchip_tile * stage->GetStageTileWidth(i, j) + X_mapchip_tile;
				if (stage->GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
				{
					IsUpBlock = true;
				}
				else
				{
					IsUpBlock = false;
				}
			}

			//下隣
			if (stage->IsPositionTile({BodyCenterPos.x,NextDown,0.0}, i, j))
			{
				X_mapchip_tile = Center_X_mapchip % stage->GetStageTileWidth(i, j);
				Y_mapchip_tile = NextDown_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = Y_mapchip_tile * stage->GetStageTileWidth(i, j) + X_mapchip_tile;
				if (stage->GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
				{
					IsDownBlock = true;
				}
				else
				{
					IsDownBlock = false;
				}
			}
		}
	}
}
