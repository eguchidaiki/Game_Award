﻿#include <RVector.h>

#include "PlayerBody.h"
#include "InputManger.h"
#include "Stage.h"

const float PlayerBody::BodySize = 50.0f;
const float PlayerBody::HalfBodySize = BodySize / 2.0f;

PlayerBody::PlayerBody() :
	IsActivate(false),
	Body_Type(),
	BodyStartPos{},
	BodyEndPos{},
	BodyCenterPos{},
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
	BodyIsFall(false),
	SlideDis(),
	Overlap(0),
	BodyDistance(1),
	Ease{}
{
	//BodyColor = WHITE;
	//Bodyhandle = LoadGraph("./Resources/playerSub.png");

	Bodyhandle = TexManager::LoadTexture("./Resources/playerSub.png");
	BodySprite.Create(Bodyhandle);
}

PlayerBody::~PlayerBody()
{
}

void PlayerBody::Init(RVector3 position, BodyType number)
{
	Body_Type = number;

	if (Body_Type == BodyType::left)
	{
		BodyStartPos = { position.x - 90.0f, position.y - 30.0f, 0.0f };
	}
	else if (Body_Type == BodyType::up)
	{
		BodyStartPos = { position.x - 30.0f, position.y - 90.0f, 0.0f };
	}
	else if (Body_Type == BodyType::right)
	{
		BodyStartPos = { position.x + 30.0f, position.y - 30.0f, 0.0f };
	}
	else if (Body_Type == BodyType::down)
	{
		BodyStartPos = { position.x - 30.0f, position.y + 30.0f, 0.0f };
	}

	BodyEndPos = { BodyStartPos.x + BodySize, BodyStartPos.y + BodySize, 0.0f };

	IsOpen = true;
	IsFold = false;
}

void PlayerBody::Update(RVector3& center)
{
	//�J���Ă���r��
	if (IsFold == false && IsOpen == true && IsAction == true && IsSlide == false)
	{
		Ease.addTime += Ease.maxTime / 60.0f;
		Ease.timeRate = min(Ease.addTime / Ease.maxTime, 1.0f);

		if (Body_Type == BodyType::left)
		{
			if (FoldCount == 1)
			{
				BodyEndPos = { center.x - static_cast<float>(25 + BodySize * (BodyDistance - 1)),center.y + 25 ,0.0f };
				BodyStartPos.x = Ease.easeOut(BodyEndPos.x + BodySize, BodyEndPos.x - BodySize, Ease.timeRate);
				BodyStartPos.y = BodyEndPos.y - BodySize;
				if (Ease.timeRate < 0.5)
				{
					BodyCenterPos.x = BodyEndPos.x + (BodyStartPos.x - BodyEndPos.x) / 2;
				}
				else
				{
					BodyCenterPos.x = BodyStartPos.x + (BodyEndPos.x - BodyStartPos.x) / 2;
				}
			}
			else if (FoldCount == 2)
			{
				BodyStartPos = { center.x - 25,center.y - 25 ,0.0f };
				BodyEndPos.x = Ease.easeOut(BodyStartPos.x + BodySize, BodyStartPos.x - BodySize, Ease.timeRate);
				BodyEndPos.y = BodyStartPos.y + BodySize;
				if (Ease.timeRate < 0.5)
				{
					BodyCenterPos.x = BodyStartPos.x + (BodyEndPos.x - BodyStartPos.x) / 2;
				}
				else
				{
					BodyCenterPos.x = BodyEndPos.x + (BodyStartPos.x - BodyEndPos.x) / 2;
				}
			}
		}
		if (Body_Type == BodyType::up)
		{
			BodyEndPos = { center.x + 25.0f, center.y - 25.0f, 0.0f };
			BodyStartPos.y = Ease.easeOut(BodyEndPos.y + BodySize, BodyEndPos.y - BodySize, Ease.timeRate);
			BodyStartPos.x = BodyEndPos.x - BodySize;
			if (Ease.timeRate < 0.5)
			{
				BodyCenterPos.y = BodyEndPos.y + (BodyStartPos.y - BodyEndPos.y) / 2;
			}
			else
			{
				BodyCenterPos.y = BodyStartPos.y + (BodyEndPos.y - BodyStartPos.y) / 2;
			}
		}
		if (Body_Type == BodyType::right)
		{
			if (FoldCount == 1)
			{
				BodyStartPos = { center.x + static_cast<float>(25 + BodySize * (BodyDistance - 1)),center.y - 25 ,0.0f };
				BodyEndPos.x = Ease.easeOut(BodyStartPos.x - BodySize, BodyStartPos.x + BodySize, Ease.timeRate);
				BodyEndPos.y = BodyStartPos.y + BodySize;
				if (Ease.timeRate < 0.5)
				{
					BodyCenterPos.x = BodyEndPos.x + (BodyStartPos.x - BodyEndPos.x) / 2;
				}
				else
				{
					BodyCenterPos.x = BodyStartPos.x + (BodyEndPos.x - BodyStartPos.x) / 2;
				}
			}
			else if (FoldCount == 2)
			{
				BodyEndPos = { center.x + 25,center.y + 25 ,0.0f };
				BodyStartPos.x = Ease.easeOut(BodyEndPos.x - BodySize, BodyEndPos.x + BodySize, Ease.timeRate);
				BodyStartPos.y = BodyEndPos.y - BodySize;
				if (Ease.timeRate < 0.5)
				{
					BodyCenterPos.x = BodyStartPos.x + (BodyEndPos.x - BodyStartPos.x) / 2;
				}
				else
				{
					BodyCenterPos.x = BodyEndPos.x + (BodyStartPos.x - BodyEndPos.x) / 2;
				}
			}
		}
		if (Body_Type == BodyType::down)
		{
			BodyStartPos = { center.x - 25.0f, center.y + 25.0f, 0.0f };
			BodyEndPos.y = Ease.easeOut(BodyStartPos.y - BodySize, BodyStartPos.y + BodySize, Ease.timeRate);
			BodyEndPos.x = BodyStartPos.x + BodySize;
			BodyCenterPos.x = BodyStartPos.x + BodySize / 2;
			if (Ease.timeRate < 0.5)
			{
				BodyCenterPos.y = BodyEndPos.y + (BodyStartPos.y - BodyEndPos.y) / 2;
			}
			else
			{
				BodyCenterPos.y = BodyStartPos.y + (BodyEndPos.y - BodyStartPos.y) / 2;
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
				BodyStartPos = { center.x - (25 + BodyDistance * BodySize),center.y - 25,0.0f };
				BodyEndPos = { BodyStartPos.x + BodySize,BodyStartPos.y + BodySize,0.0f };
				BodyCenterPos = { BodyStartPos.x + 30,BodyStartPos.y + 30,0.0f };
			}
			else if (FoldCount == 1)
			{
				BodyStartPos = { center.x - 25,center.y - 25 ,0.0f };
				BodyEndPos = { BodyStartPos.x - BodySize,BodyStartPos.y + BodySize ,0.0f };
				BodyCenterPos = { BodyStartPos.x - 30,BodyStartPos.y + 30 ,0.0f };
				IsFold = true;
				IsOpen = false;
			}
		}
		if (Body_Type == BodyType::up)
		{
			BodyStartPos = { center.x - 25.0f,center.y - 75.0f,0.0f };
			BodyEndPos = { BodyStartPos.x + 50,BodyStartPos.y + 50,0.0f };
			BodyCenterPos = { BodyStartPos.x + 30,BodyStartPos.y + 30,0.0f };
		}
		if (Body_Type == BodyType::right)
		{
			if (FoldCount == 0)
			{
				BodyStartPos = { center.x + (25 + (BodyDistance - 1) * BodySize),center.y - 25,0.0f };
				BodyEndPos = { BodyStartPos.x + 50,BodyStartPos.y + 50 ,0.0f };
				BodyCenterPos = { BodyStartPos.x + 30,BodyStartPos.y + 30 ,0.0f };
			}
			else if (FoldCount == 1)
			{
				BodyEndPos = { center.x + 25,center.y + 25,0.0f };
				BodyStartPos = { BodyEndPos.x + BodySize,BodyEndPos.y - BodySize,0.0f };
				BodyCenterPos = { BodyEndPos.x + 30,BodyEndPos.y - 30,0.0f };
				IsFold = true;
				IsOpen = false;
			}
		}
		if (Body_Type == BodyType::down)
		{
			BodyStartPos = { center.x - 25,center.y + 25 ,0.0f };
			BodyEndPos = { BodyStartPos.x + 50,BodyStartPos.y + 50 ,0.0f };
			BodyCenterPos = { BodyStartPos.x + 30,BodyStartPos.y + 30 ,0.0f };
		}
	}
	//折っている途中
	if (IsFold == true && IsOpen == false && IsAction == true && IsSlide == false)
	{
		Ease.addTime += Ease.maxTime / 60.0f;
		Ease.timeRate = min(Ease.addTime / Ease.maxTime, 1.0f);

		if (Body_Type == BodyType::left)
		{
			if (FoldCount == 0)
			{
				BodyEndPos = { center.x - static_cast<float>(25 + BodySize * (BodyDistance - 1)),center.y + 25 ,0.0f };
				BodyStartPos.x = Ease.easeOut(BodyEndPos.x - BodySize, BodyEndPos.x + BodySize, Ease.timeRate);
				BodyStartPos.y = BodyEndPos.y - BodySize;
				if (Ease.timeRate < 0.5)
				{
					BodyCenterPos.x = BodyStartPos.x + (BodyEndPos.x - BodyStartPos.x) / 2;
				}
				else
				{
					BodyCenterPos.x = BodyEndPos.x + (BodyStartPos.x - BodyEndPos.x) / 2;
				}
			}
			else if (FoldCount == 1)
			{
				BodyStartPos = { center.x - 25,center.y - 25 ,0.0f };
				BodyEndPos.x = Ease.easeOut(BodyStartPos.x - BodySize, BodyStartPos.x + BodySize, Ease.timeRate);
				BodyEndPos.y = BodyStartPos.y + BodySize;
				if (Ease.timeRate < 0.5)
				{
					BodyCenterPos.x = BodyEndPos.x + (BodyStartPos.x - BodyEndPos.x) / 2;
				}
				else
				{
					BodyCenterPos.x = BodyStartPos.x + (BodyEndPos.x - BodyStartPos.x) / 2;
				}
			}
		}
		if (Body_Type == BodyType::up)
		{
			BodyEndPos = { center.x + 25.0f, center.y - 25.0f, 0.0f };
			BodyStartPos.y = Ease.easeOut(BodyEndPos.y - BodySize, BodyEndPos.y + BodySize, Ease.timeRate);
			BodyStartPos.x = BodyEndPos.x - BodySize;
			if (Ease.timeRate < 0.5)
			{
				BodyCenterPos.y = BodyStartPos.y + (BodyEndPos.y - BodyStartPos.y) / 2;
			}
			else
			{
				BodyCenterPos.y = BodyEndPos.y + (BodyStartPos.y - BodyEndPos.y) / 2;
			}
		}
		if (Body_Type == BodyType::right)
		{
			if (FoldCount == 0)
			{
				BodyStartPos = { center.x + static_cast<float>(25 + BodySize * (BodyDistance - 1)),center.y - 25 ,0.0f };
				BodyEndPos.x = Ease.easeOut(BodyStartPos.x + BodySize, BodyStartPos.x - BodySize, Ease.timeRate);
				BodyEndPos.y = BodyStartPos.y + BodySize;
				if (Ease.timeRate < 0.5)
				{
					BodyCenterPos.x = BodyStartPos.x + (BodyEndPos.x - BodyStartPos.x) / 2;
				}
				else
				{
					BodyCenterPos.x = BodyEndPos.x + (BodyStartPos.x - BodyEndPos.x) / 2;
				}
			}
			else if (FoldCount == 1)
			{
				BodyEndPos = { center.x + 25,center.y + 25 ,0.0f };
				BodyStartPos.x = Ease.easeOut(BodyEndPos.x + BodySize, BodyEndPos.x - BodySize, Ease.timeRate);
				BodyStartPos.y = BodyEndPos.y - BodySize;
				if (Ease.timeRate < 0.5)
				{
					BodyCenterPos.x = BodyEndPos.x + (BodyStartPos.x - BodyEndPos.x) / 2;
				}
				else
				{
					BodyCenterPos.x = BodyStartPos.x + (BodyEndPos.x - BodyStartPos.x) / 2;
				}
			}
		}
		if (Body_Type == BodyType::down)
		{
			BodyStartPos = { center.x - 25.0f, center.y + 25.0f, 0.0f };
			BodyEndPos.y = Ease.easeOut(BodyStartPos.y + BodySize, BodyStartPos.y - BodySize, Ease.timeRate);
			BodyEndPos.x = BodyStartPos.x + BodySize;
			if (Ease.timeRate < 0.5)
			{
				BodyCenterPos.y = BodyStartPos.y + (BodyEndPos.y - BodyStartPos.y) / 2;
			}
			else
			{
				BodyCenterPos.y = BodyEndPos.y + (BodyStartPos.y - BodyEndPos.y) / 2;
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
				BodyEndPos = { center.x - (25 + BodySize * (BodyDistance - 1)),center.y + 25 ,0.0f };
				BodyStartPos = { BodyEndPos.x + 50, BodyEndPos.y - BodySize ,0.0f };
				BodyCenterPos = { BodyEndPos.x + 30, BodyEndPos.y - 30 ,0.0f };
			}
			else if (FoldCount == 2)
			{
				BodyStartPos = { center.x - 25,center.y - 25,0.0f };
				BodyEndPos = { BodyStartPos.x + BodySize, BodyStartPos.y + BodySize ,0.0f };
				BodyCenterPos = { BodyStartPos.x + 30, BodyStartPos.y + 30 ,0.0f };
			}
		}
		else if (Body_Type == BodyType::up)
		{
			BodyStartPos = { center.x - 25,center.y + 25 ,0.0f };
			BodyEndPos = { BodyStartPos.x + BodySize,BodyStartPos.y - BodySize ,0.0f };
			BodyCenterPos = { BodyStartPos.x + 30,BodyStartPos.y - 30 ,0.0f };
		}
		else if (Body_Type == BodyType::right)
		{
			if (FoldCount == 1)
			{
				BodyStartPos = { center.x + (25 + BodySize * (BodyDistance - 1)),center.y - 25 ,0.0f };
				BodyEndPos = { BodyStartPos.x - BodySize,BodyStartPos.y + BodySize ,0.0f };
				BodyCenterPos = { BodyStartPos.x - 30,BodyStartPos.y + 30 ,0.0f };
			}
			else if (FoldCount == 2)
			{
				BodyStartPos = { center.x - 25,center.y - 25 ,0.0f };
				BodyEndPos = { BodyStartPos.x + BodySize, BodyStartPos.y + BodySize ,0.0f };
				BodyCenterPos = { BodyStartPos.x + 30, BodyStartPos.y + 30 ,0.0f };
			}
		}
		else if (Body_Type == BodyType::down)
		{
			BodyStartPos = { center.x - 25,center.y + 25 ,0.0f };
			BodyEndPos = { BodyStartPos.x + BodySize,BodyStartPos.y - BodySize ,0.0f };
			BodyCenterPos = { BodyStartPos.x + 30,BodyStartPos.y - 30 ,0.0f };
		}
	}

	//体のスライド
	//顔を挟むとなり移動
	if (IsSlide == true && Ease.isMove == true && SlideDis == 2)
	{
		Ease.addTime += Ease.maxTime / 25.0f;
		Ease.timeRate = min(Ease.addTime / Ease.maxTime, 1.0f);

		if (Body_Type == BodyType::left)
		{
			BodyStartPos = { Ease.easeOut(center.x - 75, center.x + 25, Ease.timeRate), center.y - 25.0f, 0.0f };
			BodyEndPos = { BodyStartPos.x + BodySize, center.y + 25.0f, 0.0f };
			BodyCenterPos = { BodyStartPos.x + 30.0f, BodyStartPos.y + 30.0f, 0.0f };
		}
		else if (Body_Type == BodyType::right)
		{
			BodyStartPos = { Ease.easeOut(center.x + 25, center.x - 75, Ease.timeRate), center.y - 25.0f, 0.0f };
			BodyEndPos = { BodyStartPos.x + BodySize, center.y + 25.0f, 0.0f };
			BodyCenterPos = { BodyStartPos.x + 30.0f, BodyStartPos.y + 30.0f, 0.0f };
		}
		else if (Body_Type == BodyType::up)
		{
			BodyStartPos = { center.x - 25.0f, Ease.easeOut(center.y - 75, center.y + 25, Ease.timeRate), 0.0f };
			BodyEndPos = { center.x + 25.0f, BodyStartPos.y + BodySize, 0.0f };
			BodyCenterPos = { BodyStartPos.x + 30.0f, BodyStartPos.y + 30.0f, 0.0f };
		}
		else if (Body_Type == BodyType::down)
		{
			BodyStartPos = { center.x - 25.0f, Ease.easeOut(center.y + 25, center.y - 75, Ease.timeRate), 0.0f };
			BodyEndPos = { center.x + 25.0f, BodyStartPos.y + BodySize, 0.0f };
			BodyCenterPos = { BodyStartPos.x + 30.0f, BodyStartPos.y + 30.0f, 0.0f };
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
				BodyEndPos = { Ease.easeOut(center.x - 25, center.x - 75, Ease.timeRate), center.y - 25.0f, 0.0f };
				BodyCenterPos = { BodyEndPos.x - 30.0f, BodyEndPos.y + 30, 0.0f };
			}
			else
			{
				BodyEndPos = { Ease.easeOut(center.x - 75, center.x - 25, Ease.timeRate), center.y - 25.0f, 0.0f };
				BodyCenterPos = { BodyEndPos.x - 30.0f, BodyEndPos.y + 30, 0.0f };
			}
			BodyStartPos = { BodyEndPos.x + static_cast<float>(100 * IsFold - BodySize), center.y + 25.0f, 0.0f };
		}
		else if (Body_Type == BodyType::right)
		{
			if (SlidePat == -1)
			{
				BodyStartPos = { Ease.easeOut(center.x + 75, center.x + 25, Ease.timeRate), center.y - 25.0f, 0.0f };
				BodyCenterPos = { BodyStartPos.x + 30.0f, BodyStartPos.y + 30, 0.0f };
			}
			else
			{
				BodyStartPos = { Ease.easeOut(center.x + 25, center.x + 75, Ease.timeRate), center.y - 30.0f, 0.0f };
				BodyCenterPos = { BodyStartPos.x + 30.0f, BodyStartPos.y + 30, 0.0f };
			}
			BodyEndPos = { BodyStartPos.x + static_cast<float>(-100 * IsFold + BodySize), center.y + 25.0f, 0.0f };
		}

		if (Ease.timeRate >= 1.0f)
		{
			Ease.isMove = false;
			IsAction = false;
			IsSlide = false;
		}
	}
}

void PlayerBody::Draw(int offsetX, int offsetY)
{
	if (IsActivate == true)
	{
		/*DrawExtendGraph(static_cast<int>(BodyStartPos.x) + offsetX, static_cast<int>(BodyStartPos.y) + offsetY,
			static_cast<int>(BodyEndPos.x) + offsetX, static_cast<int>(BodyEndPos.y) + offsetY, Bodyhandle, true);*/

		if (IsFold == true)
		{
			BodySprite.DrawExtendSprite(static_cast<int>(BodyStartPos.x) + offsetX, static_cast<int>(BodyStartPos.y) + offsetY,
				static_cast<int>(BodyEndPos.x) + offsetX, static_cast<int>(BodyEndPos.y) + offsetY);
		}
		else
		{
			BodySprite.DrawExtendSprite(static_cast<int>(BodyStartPos.x) + offsetX, static_cast<int>(BodyStartPos.y) + offsetY,
				static_cast<int>(BodyEndPos.x) + offsetX, static_cast<int>(BodyEndPos.y) + offsetY);
		}
	}

	BodySprite.Draw();
}

void PlayerBody::setactivate(RVector3 center)
{
	if (IsActivate == true)
	{
		IsFold = false;
		IsOpen = true;
		IsSlide = false;
		BodyDistance = 1;
		Overlap = 0;
		FoldCount = 0;

		if (Body_Type == BodyType::left)
		{
			BodyStartPos = { center.x - 90.0f, center.y - 30.0f, 0.0f };
		}
		else if (Body_Type == BodyType::right)
		{
			BodyStartPos = { center.x + 30.0f, center.y - 30.0f, 0.0f };
		}
		else if (Body_Type == BodyType::up)
		{
			BodyStartPos = { center.x - 30.0f, center.y - 90.0f, 0.0f };
		}
		else if (Body_Type == BodyType::down)
		{
			BodyStartPos = { center.x - 30.0f, center.y + 30.0f, 0.0f };
		}

		BodyEndPos = { BodyStartPos.x + 60.0f, BodyStartPos.y + 60.0f, 0.0f };
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

void PlayerBody::IsHitBody(Stage& stage, RVector3* center, float& FallSpeed, bool& isfall, bool& isjump, bool& iscolide)
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
	int BodyLeft_mapchip = (int)(BodyLeft - stage.offset.x) / 60;
	int BodyUp_mapchip = (int)(BodyUp - stage.offset.y) / 60;
	int BodyRight_mapchip = (int)(BodyRight - stage.offset.x) / 60;
	int BodyDown_mapchip = (int)(BodyDown - stage.offset.y) / 60;

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

	if (BodyLeft < stage.offset.x)
	{
		center->x = stage.offset.x + (center->x - BodyLeft);
		iscolide = true;
	}
	if (BodyUp < stage.offset.y)
	{
		center->y = stage.offset.y + (center->y - BodyUp);
		iscolide = true;
	}

	for (i = 0; i < stage.GetStageDataSize(); i++)
	{
		for (j = 0; j < stage.GetStageTileDataSize(i); j++)
		{
			//左上
			if (stage.GetPositionTile({ BodyLeft,BodyUp,0.0f }, i, j))
			{
				BodyLeft_mapchip_tile = BodyLeft_mapchip % stage.GetStageTileWidth(i, j);
				BodyUp_mapchip_tile = BodyUp_mapchip % stage.GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = BodyUp_mapchip_tile * stage.GetStageTileWidth(i, j) + BodyLeft_mapchip_tile;
				if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
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
						}
					}
				}
			}
			//左下
			if (stage.GetPositionTile({ BodyLeft,BodyDown,0.0f }, i, j))
			{
				BodyLeft_mapchip_tile = BodyLeft_mapchip % stage.GetStageTileWidth(i, j);
				BodyDown_mapchip_tile = BodyDown_mapchip % stage.GetStageTileHeight(i, j);

				mapchipPos = (BodyDown_mapchip_tile)*stage.GetStageTileWidth(i, j) + (BodyLeft_mapchip_tile);
				if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
				{
					BuriedX = (BodyLeft_mapchip * 60) - BodyLeft;
					BuriedY = (BodyDown - 60) - (BodyDown_mapchip * 60);

					if (BuriedX > BuriedY)
					{
						center->y = (BodyDown_mapchip * 60) - (BodyDown - center->y);
						IsHitDown = true;
						FallCount++;
					}
					else if (BuriedX < BuriedY)
					{
						if (IsHitLeft == false)
						{
							center->x = (BodyLeft_mapchip + 1) * 60 + (center->x - BodyLeft);
							IsHitLeft = true;
						}
					}
				}
			}
			//右上
			if (stage.GetPositionTile({ BodyRight,BodyUp,0.0f }, i, j))
			{
				BodyRight_mapchip_tile = BodyRight_mapchip % stage.GetStageTileWidth(i, j);
				BodyUp_mapchip_tile = BodyUp_mapchip % stage.GetStageTileHeight(i, j);

				mapchipPos = (BodyUp_mapchip_tile)*stage.GetStageTileWidth(i, j) + (BodyRight_mapchip_tile);
				if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
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
						}
					}
				}
			}
			//右下
			if (stage.GetPositionTile({ BodyRight,BodyDown,0.0f }, i, j))
			{
				BodyRight_mapchip_tile = BodyRight_mapchip % stage.GetStageTileWidth(i, j);
				BodyDown_mapchip_tile = BodyDown_mapchip % stage.GetStageTileHeight(i, j);

				mapchipPos = (BodyDown_mapchip_tile)*stage.GetStageTileWidth(i, j) + (BodyRight_mapchip_tile);
				if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
				{
					BuriedX = (BodyRight - 60) - (BodyRight_mapchip * 60);
					BuriedY = (BodyDown - 60) - (BodyDown_mapchip * 60);

					if (BuriedX > BuriedY)
					{
						center->y = (BodyDown_mapchip * 60) - (BodyDown - center->y);
						IsHitDown = true;
						FallCount++;
					}
					else if (BuriedX < BuriedY)
					{
						if (IsHitRight == false)
						{
							center->x = (BodyRight_mapchip * 60) - (BodyRight - center->x);
							IsHitRight = true;
						}
					}
				}
			}
		}
	}

	if (FallCount > 0)
	{
		BodyIsFall = false;
		//FallSpeed = 0.0f;
	}
	else
	{
		BodyIsFall = true;
	}
}

void PlayerBody::Extrude(RVector3* center, RVector3 extrudepos, float extrudedis, BodyType extrudetype, bool& isfall, bool& isjump, bool& iscolide)
{
	switch (extrudetype)
	{
	case BodyType::left:
		if (center->x - extrudepos.x < extrudedis)
		{
			center->x = extrudepos.x + extrudedis;
			iscolide = true;
		}
		else
		{
			iscolide = false;
		}
		break;
	case BodyType::right:
		if (extrudepos.x - center->x < extrudedis)
		{
			center->x = extrudepos.x - extrudedis;
			iscolide = true;
		}
		else
		{
			iscolide = false;
		}
		break;
	case BodyType::up:
		if (center->y - extrudepos.y < extrudedis)
		{
			center->y = extrudepos.y + extrudedis;
			iscolide = true;
		}
		else
		{
			iscolide = false;
		}
		break;
	case BodyType::down:
		if (extrudepos.y - center->y < extrudedis)
		{
			center->y = extrudepos.y - extrudedis;
			iscolide = true;
		}
		else
		{
			iscolide = false;
		}
		break;
	default:
		break;
	}
}