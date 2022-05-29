#include <RVector.h>
#include "PlayerBody.h"
#include "InputManger.h"
#include "Stage.h"
#include "Player.h"
#include <memory>
#include "ActFlag.h"

namespace
{
	static Stage* stage = Stage::Get();
	static Player* player = Player::Get();
	static ActFlag* isact = ActFlag::Get();
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
	Ease{},
	BodyLeg{}
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
	if (!IsActivate)
	{
		return;
	}
	Body_Open(player->CenterPosition);

	Body_Fold(player->CenterPosition);

	Body_Slide(player->CenterPosition);

	if (IsFold || this->Body_Type == BodyType::up || this->Body_Type == BodyType::down)
	{
		IsLegDraw = false;
	}
	else
	{
		IsLegDraw = true;
	}

	//体の四辺
	float BodyLeft;
	float BodyDown;

	//StartPosとEndPosの位置関係によって上下左右の設定を変える
	if (BodyStartPos.x < BodyEndPos.x)
	{
		BodyLeft = BodyStartPos.x - 5;
	}
	else
	{
		BodyLeft = BodyEndPos.x - 5;
	}

	if (BodyStartPos.y < BodyEndPos.y)
	{
		BodyDown = BodyEndPos.y;
	}
	else
	{
		BodyDown = BodyStartPos.y;
	}

	RVector3 FootUpPosition = { BodyLeft,BodyDown,0.0f };

	BodyLeg.Update(FootUpPosition, player->IsDownBody, 1);
}

void PlayerBody::Draw(int offsetX, int offsetY)
{
	if (!IsActivate)
	{
		return;
	}

	BodySprite.DrawExtendSprite(static_cast<int>(BodyStartPos.x) + offsetX, static_cast<int>(BodyStartPos.y) + offsetY,
		static_cast<int>(BodyEndPos.x) + offsetX, static_cast<int>(BodyEndPos.y) + offsetY);

	BodySprite.Draw();

	if (IsLegDraw)
	{
		BodyLeg.Draw(offsetX, offsetY, player->IsLeft, player->IsRight);
	}
}

void PlayerBody::Create()
{
	if ((BodySprite.spdata->size.x <= 0) || (BodySprite.spdata->size.y <= 0))
	{
		Bodyhandle = TexManager::LoadTexture("./Resources/playerSub.png");
		BodySprite.Create(Bodyhandle);
	}

	BodyLeg.Create();
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
				BodyEndPos = { player->CenterPosition.x - static_cast<float>(25 + BodySize * (BodyDistance - 1)),player->CenterPosition.y + 25 ,0.0f };
				BodyStartPos.x = Ease.easeOut(BodyEndPos.x - BodySize, BodyEndPos.x + BodySize, Ease.timeRate);
				BodyStartPos.y = BodyEndPos.y - BodySize;
			}
			else if (FoldCount == 1)
			{
				BodyStartPos = { player->CenterPosition.x - 25,player->CenterPosition.y - 25 ,0.0f };
				BodyEndPos.x = Ease.easeOut(BodyStartPos.x - BodySize, BodyStartPos.x + BodySize, Ease.timeRate);
				BodyEndPos.y = BodyStartPos.y + BodySize;
			}
		}
		if (Body_Type == BodyType::up)
		{
			if (FoldCount == 0)
			{
				BodyEndPos = { player->CenterPosition.x + 25.0f, player->CenterPosition.y - static_cast<float>(25 + BodySize * (BodyDistance - 1)), 0.0f };
				BodyStartPos.y = Ease.easeOut(BodyEndPos.y - BodySize, BodyEndPos.y + BodySize, Ease.timeRate);
				BodyStartPos.x = BodyEndPos.x - BodySize;
			}
			else if (FoldCount == 1)
			{
				BodyStartPos = { player->CenterPosition.x - 25.0f, player->CenterPosition.y - 25, 0.0f };
				BodyEndPos.y = Ease.easeOut(BodyStartPos.y - BodySize, BodyStartPos.y + BodySize, Ease.timeRate);
				BodyEndPos.x = BodyStartPos.x + BodySize;
			}
		}
		if (Body_Type == BodyType::right)
		{
			if (FoldCount == 0)
			{
				BodyStartPos = { player->CenterPosition.x + static_cast<float>(25 + BodySize * (BodyDistance - 1)),player->CenterPosition.y - 25 ,0.0f };
				BodyEndPos.x = Ease.easeOut(BodyStartPos.x + BodySize, BodyStartPos.x - BodySize, Ease.timeRate);
				BodyEndPos.y = BodyStartPos.y + BodySize;
			}
			else if (FoldCount == 1)
			{
				BodyEndPos = { player->CenterPosition.x + 25,player->CenterPosition.y + 25 ,0.0f };
				BodyStartPos.x = Ease.easeOut(BodyEndPos.x + BodySize, BodyEndPos.x - BodySize, Ease.timeRate);
				BodyStartPos.y = BodyEndPos.y - BodySize;
			}
		}
		if (Body_Type == BodyType::down)
		{
			if (FoldCount == 0)
			{
				BodyStartPos = { player->CenterPosition.x - 25.0f, player->CenterPosition.y + static_cast<float>(25 + BodySize * (BodyDistance - 1)), 0.0f };
				BodyEndPos.y = Ease.easeOut(BodyStartPos.y + BodySize, BodyStartPos.y - BodySize, Ease.timeRate);
				BodyEndPos.x = BodyStartPos.x + BodySize;
			}
			else if (FoldCount == 1)
			{
				BodyEndPos = { player->CenterPosition.x + 25.0f, player->CenterPosition.y + static_cast<float>(25 + BodySize * (BodyDistance - 1)), 0.0f };
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
				BodyEndPos = { player->CenterPosition.x - (25 + BodySize * (BodyDistance - 1)),player->CenterPosition.y + 25 ,0.0f };
				BodyStartPos = { BodyEndPos.x + 50, BodyEndPos.y - BodySize ,0.0f };
			}
			else if (FoldCount == 2)
			{
				BodyStartPos = { player->CenterPosition.x - 25,player->CenterPosition.y - 25,0.0f };
				BodyEndPos = { BodyStartPos.x + BodySize, BodyStartPos.y + BodySize ,0.0f };
			}
		}
		else if (Body_Type == BodyType::up)
		{
			if (FoldCount == 1)
			{
				BodyStartPos = { player->CenterPosition.x - 25,player->CenterPosition.y + (25 + BodySize * (BodyDistance - 1)) ,0.0f };
				BodyEndPos = { BodyStartPos.x + BodySize,BodyStartPos.y - BodySize ,0.0f };
			}
			else if (FoldCount == 2)
			{
				BodyStartPos = { player->CenterPosition.x - 25,player->CenterPosition.y - 25 ,0.0f };
				BodyEndPos = { BodyStartPos.x + BodySize,BodyStartPos.y + BodySize ,0.0f };
			}
		}
		else if (Body_Type == BodyType::right)
		{
			if (FoldCount == 1)
			{
				BodyStartPos = { player->CenterPosition.x + (25 + BodySize * (BodyDistance - 1)),player->CenterPosition.y - 25 ,0.0f };
				BodyEndPos = { BodyStartPos.x - BodySize,BodyStartPos.y + BodySize ,0.0f };
			}
			else if (FoldCount == 2)
			{
				BodyStartPos = { player->CenterPosition.x - 25,player->CenterPosition.y - 25 ,0.0f };
				BodyEndPos = { BodyStartPos.x + BodySize, BodyStartPos.y + BodySize ,0.0f };
			}
		}
		else if (Body_Type == BodyType::down)
		{
			if (FoldCount == 1)
			{
				BodyStartPos = { player->CenterPosition.x - 25,player->CenterPosition.y + (25 + BodySize * (BodyDistance - 1)) ,0.0f };
				BodyEndPos = { BodyStartPos.x + BodySize,BodyStartPos.y - BodySize ,0.0f };
			}
			else if (FoldCount == 2)
			{
				BodyStartPos = { player->CenterPosition.x - 25,player->CenterPosition.y - 25 ,0.0f };
				BodyEndPos = { BodyStartPos.x + BodySize,BodyStartPos.y + BodySize ,0.0f };
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
				BodyEndPos = { player->CenterPosition.x - static_cast<float>(25 + BodySize * (BodyDistance - 1)),player->CenterPosition.y + 25 ,0.0f };
				BodyStartPos.x = Ease.easeOut(BodyEndPos.x + BodySize, BodyEndPos.x - BodySize, Ease.timeRate);
				BodyStartPos.y = BodyEndPos.y - BodySize;
			}
			else if (FoldCount == 2)
			{
				BodyStartPos = { player->CenterPosition.x - 25,player->CenterPosition.y - 25 ,0.0f };
				BodyEndPos.x = Ease.easeOut(BodyStartPos.x + BodySize, BodyStartPos.x - BodySize, Ease.timeRate);
				BodyEndPos.y = BodyStartPos.y + BodySize;
			}
		}
		if (Body_Type == BodyType::up)
		{
			if (FoldCount == 1)
			{
				BodyEndPos = { player->CenterPosition.x + 25.0f, player->CenterPosition.y - static_cast<float>(25 + BodySize * (BodyDistance - 1)), 0.0f };
				BodyStartPos.y = Ease.easeOut(BodyEndPos.y + BodySize, BodyEndPos.y - BodySize, Ease.timeRate);
				BodyStartPos.x = BodyEndPos.x - BodySize;
			}
			else if (FoldCount == 2)
			{
				BodyStartPos = { center.x - 25.0f, center.y - 25.0f, 0.0f };
				BodyEndPos.y = Ease.easeOut(BodyStartPos.y + BodySize, BodyStartPos.y - BodySize, Ease.timeRate);
				BodyEndPos.x = BodyEndPos.x - BodySize;
			}
		}
		if (Body_Type == BodyType::right)
		{
			if (FoldCount == 1)
			{
				BodyStartPos = { player->CenterPosition.x + static_cast<float>(25 + BodySize * (BodyDistance - 1)),player->CenterPosition.y - 25 ,0.0f };
				BodyEndPos.x = Ease.easeOut(BodyStartPos.x - BodySize, BodyStartPos.x + BodySize, Ease.timeRate);
				BodyEndPos.y = BodyStartPos.y + BodySize;
			}
			else if (FoldCount == 2)
			{
				BodyEndPos = { player->CenterPosition.x + 25,player->CenterPosition.y + 25 ,0.0f };
				BodyStartPos.x = Ease.easeOut(BodyEndPos.x - BodySize, BodyEndPos.x + BodySize, Ease.timeRate);
				BodyStartPos.y = BodyEndPos.y - BodySize;
			}
		}
		if (Body_Type == BodyType::down)
		{
			if (FoldCount == 1)
			{
				BodyStartPos = { player->CenterPosition.x - 25.0f, player->CenterPosition.y + static_cast<float>(25 + BodySize * (BodyDistance - 1)), 0.0f };
				BodyEndPos.y = Ease.easeOut(BodyStartPos.y - BodySize, BodyStartPos.y + BodySize, Ease.timeRate);
				BodyEndPos.x = BodyStartPos.x + BodySize;
			}
			else if (FoldCount == 2)
			{
				BodyEndPos = { player->CenterPosition.x + 25.0f, player->CenterPosition.y + 25, 0.0f };
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
				BodyStartPos = { player->CenterPosition.x - (25 + BodyDistance * BodySize),player->CenterPosition.y - 25,0.0f };
				BodyEndPos = { BodyStartPos.x + BodySize,BodyStartPos.y + BodySize,0.0f };
			}
			else if (FoldCount == 1)
			{
				BodyStartPos = { player->CenterPosition.x - 25,player->CenterPosition.y - 25 ,0.0f };
				BodyEndPos = { BodyStartPos.x - BodySize,BodyStartPos.y + BodySize ,0.0f };
				IsFold = true;
				IsOpen = false;
			}
		}
		if (Body_Type == BodyType::up)
		{
			if (FoldCount == 0)
			{
				BodyStartPos = { player->CenterPosition.x - 25.0f,player->CenterPosition.y - (25 + BodyDistance * BodySize),0.0f };
				BodyEndPos = { BodyStartPos.x + 50,BodyStartPos.y + 50,0.0f };
			}
			else if (FoldCount == 1)
			{
				BodyStartPos = { player->CenterPosition.x - 25.0f,player->CenterPosition.y - 25.0f,0.0f };
				BodyEndPos = { BodyStartPos.x + 50,BodyStartPos.y + 50,0.0f };
				IsFold = true;
				IsOpen = false;
			}
		}
		if (Body_Type == BodyType::right)
		{
			if (FoldCount == 0)
			{
				BodyStartPos = { player->CenterPosition.x + (25 + (BodyDistance - 1) * BodySize),player->CenterPosition.y - 25,0.0f };
				BodyEndPos = { BodyStartPos.x + 50,BodyStartPos.y + 50 ,0.0f };
			}
			else if (FoldCount == 1)
			{
				BodyEndPos = { player->CenterPosition.x + 25,player->CenterPosition.y + 25,0.0f };
				BodyStartPos = { BodyEndPos.x + BodySize,BodyEndPos.y - BodySize,0.0f };
				IsFold = true;
				IsOpen = false;
			}
		}
		if (Body_Type == BodyType::down)
		{
			if (FoldCount == 0)
			{
				BodyStartPos = { player->CenterPosition.x - 25,player->CenterPosition.y + (25 + (BodyDistance - 1) * BodySize) ,0.0f };
				BodyEndPos = { BodyStartPos.x + 50,BodyStartPos.y + 50 ,0.0f };
			}
			else if (FoldCount == 1)
			{
				BodyEndPos = { player->CenterPosition.x + 25,player->CenterPosition.y + 25,0.0f };
				BodyStartPos = { BodyEndPos.x + BodySize,BodyEndPos.y - BodySize,0.0f };
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
			BodyStartPos = { Ease.easeOut(center.x - 75, center.x + 25, Ease.timeRate), center.y - 25.0f, 0.0f };
			BodyEndPos = { BodyStartPos.x + BodySize, center.y + 25.0f, 0.0f };
		}
		else if (Body_Type == BodyType::right)
		{
			BodyStartPos = { Ease.easeOut(center.x + 25, center.x - 75, Ease.timeRate), center.y - 25.0f, 0.0f };
			BodyEndPos = { BodyStartPos.x + BodySize, center.y + 25.0f, 0.0f };
		}
		else if (Body_Type == BodyType::up)
		{
			BodyStartPos = { center.x - 25.0f, Ease.easeOut(center.y - 75, center.y + 25, Ease.timeRate), 0.0f };
			BodyEndPos = { center.x + 25.0f, BodyStartPos.y + BodySize, 0.0f };
		}
		else if (Body_Type == BodyType::down)
		{
			BodyStartPos = { center.x - 25.0f, Ease.easeOut(center.y + 25, center.y - 75, Ease.timeRate), 0.0f };
			BodyEndPos = { center.x + 25.0f, BodyStartPos.y + BodySize, 0.0f };
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
			}
			else
			{
				BodyEndPos = { Ease.easeOut(center.x - 75, center.x - 25, Ease.timeRate), center.y - 25.0f, 0.0f };
			}
			BodyStartPos = { BodyEndPos.x + static_cast<float>(100 * IsFold - BodySize), center.y + 25.0f, 0.0f };
		}
		else if (Body_Type == BodyType::up)
		{
			if (SlidePat == -1)
			{
				BodyEndPos = { center.x - 25.0f, Ease.easeOut(center.y - 25, center.y - 75, Ease.timeRate), 0.0f };
			}
			else
			{
				BodyEndPos = { center.x - 25.0f, Ease.easeOut(center.y - 75, center.y - 25, Ease.timeRate), 0.0f };
			}
			BodyStartPos = { BodyEndPos.x + static_cast<float>(100 * IsFold - BodySize), center.y + 25.0f, 0.0f };
		}
		else if (Body_Type == BodyType::right)
		{
			if (SlidePat == -1)
			{
				BodyStartPos = { Ease.easeOut(center.x + 75, center.x + 25, Ease.timeRate), center.y - 25.0f, 0.0f };
			}
			else
			{
				BodyStartPos = { Ease.easeOut(center.x + 25, center.x + 75, Ease.timeRate), center.y - 30.0f, 0.0f };
			}
			BodyEndPos = { BodyStartPos.x + static_cast<float>(-100 * IsFold + BodySize), center.y + 25.0f, 0.0f };
		}
		else if (Body_Type == BodyType::down)
		{
			if (SlidePat == -1)
			{
				BodyStartPos = { center.x - 25.0f, Ease.easeOut(center.y + 75, center.y + 25, Ease.timeRate), 0.0f };
			}
			else
			{
				BodyStartPos = { center.x - 25.0f, Ease.easeOut(center.y + 25, center.y + 75, Ease.timeRate), 0.0f };
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

void PlayerBody::IsHitBody(RVector3* center, float& FallSpeed, bool& isfall, bool& isjump, bool& iscolide)
{
	Update(*center);

	if (IsFold || !IsActivate)
	{
		return;
	}

	//体の四辺
	float BodyLeft;
	float BodyRight;
	float BodyUp;
	float BodyDown;
	float BodyAndLegDown = -1;

	//StartPosとEndPosの位置関係によって上下左右の設定を変える
	if (BodyStartPos.x < BodyEndPos.x)
	{
		BodyLeft = BodyStartPos.x;
		BodyRight = BodyEndPos.x - 1;
	}
	else
	{
		BodyLeft = BodyEndPos.x;
		BodyRight = BodyStartPos.x - 1;
	}

	if (BodyStartPos.y < BodyEndPos.y)
	{
		BodyUp = BodyStartPos.y;
		BodyDown = BodyEndPos.y - 1;
		BodyAndLegDown = BodyDown + 8;
	}
	else
	{
		BodyUp = BodyEndPos.y;
		BodyDown = BodyStartPos.y - 1;
		BodyAndLegDown = BodyDown + 8;
	}

	//四辺をブロックサイズで割った数
	int BodyLeft_mapchip = (int)(BodyLeft - stage->offset.x) / 60;
	int BodyUp_mapchip = (int)(BodyUp - stage->offset.y) / 60;
	int BodyRight_mapchip = (int)(BodyRight - stage->offset.x) / 60;
	int BodyDown_mapchip = (int)(BodyDown - stage->offset.y) / 60;
	int BodyAndLegdown_mapchip = (int)(BodyAndLegDown - stage->offset.y) / 60;

	//タイル内のマップチップ座標
	int BodyLeft_mapchip_tile;
	int BodyUp_mapchip_tile;
	int BodyRight_mapchip_tile;
	int BodyDown_mapchip_tile;
	int BodyAndLegdown_mapchip_tile;

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

	BodyIsFall = false;

	//体の四隅とブロックとの判定
	for (i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			//左上
			if (stage->IsPositionTile({ BodyLeft,BodyUp,0.0f }, i, j))
			{
				BodyLeft_mapchip_tile = BodyLeft_mapchip % stage->GetStageTileWidth(i, j);
				BodyUp_mapchip_tile = BodyUp_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = BodyUp_mapchip_tile * stage->GetStageTileWidth(i, j) + BodyLeft_mapchip_tile;
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, mapchipPos)))
				{
					BuriedX = (BodyLeft_mapchip * 60) - BodyLeft;
					BuriedY = (BodyUp_mapchip * 60) - BodyUp;

					if (BuriedX > BuriedY)
					{
						if (IsHitUp == false)
						{
							player->CenterPosition.y = (BodyUp_mapchip + 1) * 60 + (player->CenterPosition.y - BodyUp);
							FallSpeed = 0.0f;
							IsHitUp = true;
						}
					}
					else if (BuriedX < BuriedY)
					{
						if (IsHitLeft == false)
						{
							player->CenterPosition.x = (BodyLeft_mapchip + 1) * 60 + (player->CenterPosition.x - BodyLeft);
							IsHitLeft = true;
						}
					}
				}
			}
			//左下
			if (stage->IsPositionTile({ BodyLeft,BodyAndLegDown,0.0f }, i, j))
			{
				BodyLeft_mapchip_tile = BodyLeft_mapchip % stage->GetStageTileWidth(i, j);
				BodyAndLegdown_mapchip_tile = BodyAndLegdown_mapchip % stage->GetStageTileHeight(i, j);

				mapchipPos = (BodyAndLegdown_mapchip_tile)*stage->GetStageTileWidth(i, j) + (BodyLeft_mapchip_tile);
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, mapchipPos)))
				{
					BuriedX = (BodyLeft_mapchip * 60) - BodyLeft;
					BuriedY = (BodyAndLegDown - 60) - (BodyAndLegdown_mapchip * 60);

					if (BuriedX > BuriedY)
					{
						if (IsHitDown == false)
						{
							player->CenterPosition.y = (BodyAndLegdown_mapchip * 60) - (BodyAndLegDown - player->CenterPosition.y);
							FallCount++;
							player->IsInitJump = false;
							IsHitDown = true;
						}
					}
					else if (BuriedX < BuriedY)
					{
						if (IsHitLeft == false)
						{
							player->CenterPosition.x = (BodyLeft_mapchip + 1) * 60 + (player->CenterPosition.x - BodyLeft);
							player->IsInitJump = false;
							IsHitLeft = true;
						}
					}
				}
			}
			//右上
			if (stage->IsPositionTile({ BodyRight,BodyUp,0.0f }, i, j))
			{
				BodyRight_mapchip_tile = BodyRight_mapchip % stage->GetStageTileWidth(i, j);
				BodyUp_mapchip_tile = BodyUp_mapchip % stage->GetStageTileHeight(i, j);

				mapchipPos = (BodyUp_mapchip_tile)*stage->GetStageTileWidth(i, j) + (BodyRight_mapchip_tile);
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, mapchipPos)))
				{
					BuriedX = (BodyRight - 60) - (BodyRight_mapchip * 60);
					BuriedY = (BodyUp_mapchip * 60) - BodyUp;

					if (BuriedX > BuriedY)
					{
						if (IsHitUp == false)
						{
							center->y = (BodyUp_mapchip + 1) * 60 + (center->y - BodyUp);
							FallSpeed = 0.0f;
							IsHitUp = true;
						}
					}
					else if (BuriedX < BuriedY)
					{
						if (IsHitRight == false)
						{
							player->CenterPosition.x = (BodyRight_mapchip * 60) - (BodyRight - player->CenterPosition.x);
							IsHitRight = true;
							jumpCountRight += player->IsRight;
						}
					}
				}
			}
			//右下
			if (stage->IsPositionTile({ BodyRight,BodyAndLegDown,0.0f }, i, j))
			{
				BodyRight_mapchip_tile = BodyRight_mapchip % stage->GetStageTileWidth(i, j);
				BodyAndLegdown_mapchip_tile = BodyAndLegdown_mapchip % stage->GetStageTileHeight(i, j);

				mapchipPos = (BodyAndLegdown_mapchip_tile)*stage->GetStageTileWidth(i, j) + (BodyRight_mapchip_tile);
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, mapchipPos)))
				{
					BuriedX = (BodyRight - 60) - (BodyRight_mapchip * 60);
					BuriedY = (BodyAndLegDown - 60) - (BodyAndLegdown_mapchip * 60);

					if (BuriedX > BuriedY)
					{
						if (IsHitDown == false)
						{
							player->CenterPosition.y = (BodyAndLegdown_mapchip * 60) - (BodyAndLegDown - player->CenterPosition.y);
							FallCount++;
							player->IsInitJump = false;
							IsHitDown = true;
						}
					}
					else if (BuriedX < BuriedY)
					{
						if (IsHitRight == false)
						{
							player->CenterPosition.x = (BodyRight_mapchip * 60) - (BodyRight - player->CenterPosition.x);
							IsHitRight = true;
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
			if (stage->IsPositionTile({ BodyLeft - 20,BodyUp - 20,0.0f }, i, j))
			{
				BodyLeft_mapchip_tile = (BodyLeft_mapchip - 1) % stage->GetStageTileWidth(i, j);
				BodyUp_mapchip_tile = (BodyUp_mapchip - 1) % stage->GetStageTileHeight(i, j);

				mapchipPos = BodyUp_mapchip_tile * stage->GetStageTileWidth(i, j) + BodyLeft_mapchip_tile;
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, mapchipPos)) &&
					player->IsLeft == true)
				{
					DiagonallyUpLeft = true;
				}
			}

			if (stage->IsPositionTile({ BodyRight + 20,BodyUp - 20,0.0f }, i, j))
			{
				BodyRight_mapchip_tile = (BodyRight_mapchip) % stage->GetStageTileWidth(i, j);
				BodyUp_mapchip_tile = (BodyUp_mapchip - 1) % stage->GetStageTileHeight(i, j);

				mapchipPos = (BodyUp_mapchip_tile)*stage->GetStageTileWidth(i, j) + (BodyRight_mapchip_tile);
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, mapchipPos)) &&
					player->IsRight == true)
				{
					DiagonallyUpRight = true;
				}
			}
		}
	}

	if (FallCount == 0)
	{
		BodyIsFall = true;
	}
}

void PlayerBody::IsOutsideBody(RVector3* center, float& FallSpeed, bool& isfall, bool& isjump, bool& iscolide)
{
	Update(*center);

	if (this->IsAction || !IsActivate || player->Player_IsAction)
	{
		return;
	}

	//体の中心
	RVector3 BodyCenterPos = { 0,0,0 };

	size_t NowStage = 0;
	size_t NowTile = 0;

	stage->GetPositionTile(player->CenterPosition, &NowStage, &NowTile);

	if (NowStage == static_cast<size_t>(-1) || NowTile == static_cast<size_t>(-1))
	{
		return;
	}

	//体の四辺
	float BodyLeft;
	float BodyRight;
	float BodyUp;
	float BodyDown;

	//StartPosとEndPosの位置関係によって上下左右の設定を変える
	if (BodyStartPos.x < BodyEndPos.x)
	{
		BodyCenterPos.x = BodyStartPos.x + HalfBodySize;
		BodyLeft = BodyStartPos.x;
		BodyRight = BodyEndPos.x - 1;
	}
	else
	{
		BodyCenterPos.x = BodyEndPos.x + HalfBodySize;
		BodyLeft = BodyEndPos.x;
		BodyRight = BodyStartPos.x - 1;
	}
	if (BodyStartPos.y < BodyEndPos.y)
	{
		BodyCenterPos.y = BodyStartPos.y + HalfBodySize;
		BodyUp = BodyStartPos.y;
		BodyDown = BodyEndPos.y - 1;
	}
	else
	{
		BodyCenterPos.y = BodyEndPos.y + HalfBodySize;
		BodyUp = BodyEndPos.y;
		BodyDown = BodyStartPos.y - 1;
	}

	size_t sub_stage = 0;
	size_t sub_tile = 0;

	stage->GetPositionTile({ BodyLeft,BodyUp,0 }, &sub_stage, &sub_tile);
	XMFLOAT2 LeftUpTile = { (float)sub_stage,(float)sub_tile };

	stage->GetPositionTile({ BodyLeft,BodyDown,0 }, &sub_stage, &sub_tile);
	XMFLOAT2 LeftDownTile = { (float)sub_stage,(float)sub_tile };

	stage->GetPositionTile({ BodyRight,BodyUp,0 }, &sub_stage, &sub_tile);
	XMFLOAT2 RightUpTile = { (float)sub_stage,(float)sub_tile };

	stage->GetPositionTile({ BodyRight,BodyDown,0 }, &sub_stage, &sub_tile);
	XMFLOAT2 RightDownTile = { (float)sub_stage,(float)sub_tile };

	//現時点での四隅のタイルをセット
	//左の壁(上下)
	XMFLOAT2 Leftwall =
	{
		(float)stage->GetStageTileOffsetX(NowStage, NowTile) * stage->blockSize,
		(float)stage->GetStageTileOffsetX(NowStage, NowTile) * stage->blockSize
	};
	//右の壁(上下)
	XMFLOAT2 Rightwall =
	{
		(float)(stage->GetStageTileOffsetX(NowStage, NowTile) + stage->GetStageTileWidth(NowStage, NowTile)) * stage->blockSize,
		(float)(stage->GetStageTileOffsetX(NowStage, NowTile) + stage->GetStageTileWidth(NowStage, NowTile)) * stage->blockSize
	};
	//上の壁(左右)
	XMFLOAT2 Upwall =
	{
		(float)stage->GetStageTileOffsetY(NowStage, NowTile) * stage->blockSize,
		(float)stage->GetStageTileOffsetY(NowStage, NowTile) * stage->blockSize
	};
	//下の壁(左右)
	XMFLOAT2 Downwall =
	{
		(float)(stage->GetStageTileOffsetY(NowStage, NowTile) + stage->GetStageTileHeight(NowStage, NowTile)) * stage->blockSize,
		(float)(stage->GetStageTileOffsetY(NowStage, NowTile) + stage->GetStageTileHeight(NowStage, NowTile)) * stage->blockSize
	};

	float NowLeft = Leftwall.x;
	float NowRight = Rightwall.x;
	float NowUp = Upwall.x;
	float NowDown = Downwall.x;

	//体の四隅との場外判定
	for (int i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (int j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			//左のoffset計算(上)
			if (BodyUp <= (stage->GetStageTileOffsetY(i, j) + stage->GetStageTileHeight(i, j)) * stage->blockSize &&
				BodyUp >= stage->GetStageTileOffsetY(i, j) * stage->blockSize)
			{
				if (Leftwall.x > stage->GetStageTileOffsetX(i, j) * stage->blockSize)
				{
					Leftwall.x = stage->GetStageTileOffsetX(i, j) * stage->blockSize;
				}
			}
			//左のoffset計算(下)
			if (BodyDown <= (stage->GetStageTileOffsetY(i, j) + stage->GetStageTileHeight(i, j)) * stage->blockSize &&
				BodyDown >= stage->GetStageTileOffsetY(i, j) * stage->blockSize)
			{
				if (Leftwall.y > (stage->GetStageTileOffsetX(i, j)) * stage->blockSize)
				{
					Leftwall.y = stage->GetStageTileOffsetX(i, j) * stage->blockSize;
				}
			}

			//右のoffset計算(上)
			if (BodyUp <= (stage->GetStageTileOffsetY(i, j) + stage->GetStageTileHeight(i, j)) * stage->blockSize &&
				BodyUp >= stage->GetStageTileOffsetY(i, j) * stage->blockSize)
			{
				if (Rightwall.x < (stage->GetStageTileOffsetX(i, j) + stage->GetStageTileWidth(i, j)) * stage->blockSize)
				{
					Rightwall.x = (stage->GetStageTileOffsetX(i, j) + stage->GetStageTileWidth(i, j)) * stage->blockSize;
				}
			}
			//右のoffset計算(下)
			if (BodyDown <= (stage->GetStageTileOffsetY(i, j) + stage->GetStageTileHeight(i, j)) * stage->blockSize &&
				BodyDown >= stage->GetStageTileOffsetY(i, j) * stage->blockSize)
			{
				if (Rightwall.y < (stage->GetStageTileOffsetX(i, j) + stage->GetStageTileWidth(i, j)) * stage->blockSize)
				{
					Rightwall.y = (stage->GetStageTileOffsetX(i, j) + stage->GetStageTileWidth(i, j)) * stage->blockSize;
				}
			}

			//上のoffset計算(左)
			if (BodyLeft <= (stage->GetStageTileOffsetX(i, j) + stage->GetStageTileWidth(i, j)) * stage->blockSize &&
				BodyLeft >= (stage->GetStageTileOffsetX(i, j)) * stage->blockSize)
			{
				if (Upwall.x > stage->GetStageTileOffsetY(i, j) * stage->blockSize)
				{
					Upwall.x = stage->GetStageTileOffsetY(i, j) * stage->blockSize;
				}
			}
			//上のoffset計算(右)
			if (BodyRight <= (stage->GetStageTileOffsetX(i, j) + stage->GetStageTileWidth(i, j)) * stage->blockSize &&
				BodyRight >= (stage->GetStageTileOffsetX(i, j)) * stage->blockSize)
			{
				if (Upwall.y > (stage->GetStageTileOffsetY(i, j)) * stage->blockSize)
				{
					Upwall.y = stage->GetStageTileOffsetY(i, j) * stage->blockSize;
				}
			}

			//下のoffset計算(左)
			if (BodyLeft <= (stage->GetStageTileOffsetX(i, j) + stage->GetStageTileWidth(i, j)) * stage->blockSize &&
				BodyLeft >= (stage->GetStageTileOffsetX(i, j)) * stage->blockSize)
			{
				if (Downwall.x < (stage->GetStageTileOffsetY(i, j) + stage->GetStageTileHeight(i, j)) * stage->blockSize)
				{
					Downwall.x = (stage->GetStageTileOffsetY(i, j) + stage->GetStageTileHeight(i, j)) * stage->blockSize;
				}
			}
			//下のoffset計算(右)
			if (BodyRight <= (stage->GetStageTileOffsetX(i, j) + stage->GetStageTileWidth(i, j)) * stage->blockSize &&
				BodyRight >= (stage->GetStageTileOffsetX(i, j)) * stage->blockSize)
			{
				if (Downwall.y < (stage->GetStageTileOffsetY(i, j) + stage->GetStageTileHeight(i, j)) * stage->blockSize)
				{
					Downwall.y = (stage->GetStageTileOffsetY(i, j) + stage->GetStageTileHeight(i, j)) * stage->blockSize;
				}
			}
		}
	}

	if (Leftwall.x < Leftwall.y)
	{
		NowLeft = Leftwall.y;
	}
	else
	{
		NowLeft = Leftwall.x;
	}

	if (Rightwall.x > Rightwall.y)
	{
		NowRight = Rightwall.y;
	}
	else
	{
		NowRight = Rightwall.x;
	}

	if (Upwall.x < Upwall.y)
	{
		NowUp = Upwall.y;
	}
	else
	{
		NowUp = Upwall.x;
	}

	if (Downwall.x > Downwall.y)
	{
		NowDown = Downwall.y;
	}
	else
	{
		NowDown = Downwall.x;
	}

	IsOutSideLeft = false;
	IsOutSideRight = false;
	IsOutSideUp = false;
	IsOutSideDown = false;

	if (NowLeft + 50 <= BodyLeft)
	{
		IsOutSideLeft = true;
	}
	if (NowRight + 50 >= BodyRight)
	{
		IsOutSideRight = true;
	}
	if (NowUp + 50 <= BodyUp)
	{
		IsOutSideUp = true;
	}
	if (NowDown + 50 >= BodyDown)
	{
		IsOutSideDown = true;
	}

	//四隅の壁との距離
	XMFLOAT2 Buried_LU =
	{
		fabs(Leftwall.x - BodyLeft),
		fabs(Upwall.x - BodyUp)
	};
	XMFLOAT2 Buried_LD =
	{
		fabs(Leftwall.y - BodyLeft),
		fabs(Downwall.x - BodyDown)
	};
	XMFLOAT2 Buried_RU =
	{
		fabs(Rightwall.x - BodyRight),
		fabs(Upwall.y - BodyUp)
	};
	XMFLOAT2 Buried_RD =
	{
		fabs(Rightwall.y - BodyRight),
		fabs(Downwall.y - BodyDown)
	};

	float push = 0;

	if ((Leftwall.x > BodyLeft && (Buried_LU.x < Buried_LU.y)) || (Leftwall.y > BodyLeft && (Buried_LD.x < Buried_LD.y)))
	{
		push = fabs(center->x - BodyLeft);
		center->x = NowLeft + push;
	}
	if ((Rightwall.x < BodyRight && (Buried_RU.x < Buried_RU.y)) || (Rightwall.y < BodyRight && (Buried_RD.x < Buried_RD.y)))
	{
		push = fabs(BodyRight - center->x);
		center->x = NowRight - push - 1;
	}
	if ((Upwall.x > BodyUp && (Buried_LU.x > Buried_LU.y)) || (Upwall.y > BodyUp && (Buried_RU.x > Buried_RU.y)))
	{
		push = fabs(center->y - BodyUp);
		center->y = NowUp + push + 1;
		FallSpeed = 0.0f;
	}
	if ((Downwall.x < BodyDown && (Buried_LD.x > Buried_LD.y)) || (Downwall.y < BodyDown && (Buried_RD.x > Buried_RD.y)))
	{
		push = fabs(BodyDown - center->y);
		center->y = NowDown - push;
	}
}

bool PlayerBody::IsReverseHitBody(const unsigned char& direction)
{
	if (Player::Get()->IsPressInStage() == false || this->IsFold)
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

	char* mapchip = { 0 };

	//設定用の値
	int SettingMapchip = stage->FoldSimulation(RVector3(BodyLeft, BodyUp, 0.0f), direction, &mapchip);

	if (SettingMapchip == -1)
	{
		//return false;
	}

	if (this->Body_Type == direction)
	{
		//return false;
	}

	int BlockCount = 0;

	for (size_t i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (size_t j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			//左上
			if (stage->IsPositionTile({ BodyLeft,BodyUp,0.0f }, i, j))
			{
				BodyLeft_mapchip_tile = BodyLeft_mapchip % stage->GetStageTileWidth(i, j);
				BodyUp_mapchip_tile = BodyUp_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = BodyUp_mapchip_tile * stage->GetStageTileWidth(i, j) + BodyLeft_mapchip_tile;

				if (mapchip != nullptr && stage->IsMapchipBlocks(mapchip[mapchipPos]))
				{
					return true;
				}
			}
			//左下
			if (stage->IsPositionTile({ BodyLeft,BodyDown,0.0f }, i, j))
			{
				BodyLeft_mapchip_tile = BodyLeft_mapchip % stage->GetStageTileWidth(i, j);
				BodyDown_mapchip_tile = BodyDown_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = BodyDown_mapchip_tile * stage->GetStageTileWidth(i, j) + BodyLeft_mapchip_tile;

				if (mapchip != nullptr && stage->IsMapchipBlocks(mapchip[mapchipPos]))
				{
					return true;
				}
			}
			//右上
			if (stage->IsPositionTile({ BodyRight,BodyUp,0.0f }, i, j))
			{
				BodyRight_mapchip_tile = BodyRight_mapchip % stage->GetStageTileWidth(i, j);
				BodyUp_mapchip_tile = BodyUp_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = BodyUp_mapchip_tile * stage->GetStageTileWidth(i, j) + BodyRight_mapchip_tile;

				if (mapchip != nullptr && stage->IsMapchipBlocks(mapchip[mapchipPos]))
				{
					return true;
				}
			}
			//右下
			if (stage->IsPositionTile({ BodyRight,BodyDown,0.0f }, i, j))
			{
				BodyRight_mapchip_tile = BodyRight_mapchip % stage->GetStageTileWidth(i, j);
				BodyDown_mapchip_tile = BodyDown_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = BodyDown_mapchip_tile * stage->GetStageTileWidth(i, j) + BodyRight_mapchip_tile;

				if (mapchip != nullptr && stage->IsMapchipBlocks(mapchip[mapchipPos]))
				{
					return true;
				}
			}
		}
	}

	return false;
}

void PlayerBody::IsAroundBlock()
{
	//体の中心
	RVector3 BodyCenterPos = { 0,0,0 };

	//体の四辺
	float BodyLeft;
	float BodyRight;
	float BodyUp;
	float BodyDown;

	//StartPosとEndPosの位置関係によって上下左右の設定を変える
	if (BodyStartPos.x < BodyEndPos.x)
	{
		BodyCenterPos.x = BodyStartPos.x + HalfBodySize;
		BodyLeft = BodyStartPos.x;
		BodyRight = BodyEndPos.x;
	}
	else
	{
		BodyCenterPos.x = BodyEndPos.x + HalfBodySize;
		BodyLeft = BodyEndPos.x;
		BodyRight = BodyStartPos.x;
	}

	if (BodyStartPos.y < BodyEndPos.y)
	{
		BodyCenterPos.y = BodyStartPos.y + HalfBodySize;
		BodyUp = BodyStartPos.y;
		BodyDown = BodyEndPos.y;
	}
	else
	{
		BodyCenterPos.y = BodyEndPos.y + HalfBodySize;
		BodyUp = BodyEndPos.y;
		BodyDown = BodyStartPos.y;
	}

	//体の四辺をブロックサイズで割った数
	int BodyLeft_mapchip = (int)(BodyLeft - stage->offset.x) / 60;
	int BodyUp_mapchip = (int)(BodyUp - stage->offset.y) / 60;
	int BodyRight_mapchip = (int)(BodyRight - stage->offset.x) / 60;
	int BodyDown_mapchip = (int)(BodyDown - stage->offset.y) / 60;

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

	IsLeftBlock = false;
	IsRightBlock = false;
	IsUpBlock = false;
	IsDownBlock = false;

	if (IsFold || !IsActivate)
	{
		return;
	}

	for (int i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (int j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			//左隣(上)
			if (stage->IsPositionTile({ NextLeft,BodyUp,0.0 }, i, j))
			{
				X_mapchip_tile = NextLeft_mapchip % stage->GetStageTileWidth(i, j);
				Y_mapchip_tile = BodyUp_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = Y_mapchip_tile * stage->GetStageTileWidth(i, j) + X_mapchip_tile;
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, mapchipPos)))
				{
					IsLeftBlock = true;
				}
			}

			//左隣(下)
			if (stage->IsPositionTile({ NextLeft,BodyDown,0.0 }, i, j))
			{
				X_mapchip_tile = NextLeft_mapchip % stage->GetStageTileWidth(i, j);
				Y_mapchip_tile = BodyDown_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = Y_mapchip_tile * stage->GetStageTileWidth(i, j) + X_mapchip_tile;
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, mapchipPos)))
				{
					IsLeftBlock = true;
				}
			}

			//右隣(上)
			if (stage->IsPositionTile({ NextRight,BodyUp,0.0 }, i, j))
			{
				X_mapchip_tile = NextRight_mapchip % stage->GetStageTileWidth(i, j);
				Y_mapchip_tile = BodyUp_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = Y_mapchip_tile * stage->GetStageTileWidth(i, j) + X_mapchip_tile;
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, mapchipPos)))
				{
					IsRightBlock = true;
				}
			}

			//右隣(下)
			if (stage->IsPositionTile({ NextRight,BodyDown,0.0 }, i, j))
			{
				X_mapchip_tile = NextRight_mapchip % stage->GetStageTileWidth(i, j);
				Y_mapchip_tile = BodyDown_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = Y_mapchip_tile * stage->GetStageTileWidth(i, j) + X_mapchip_tile;
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, mapchipPos)))
				{
					IsRightBlock = true;
				}
			}

			//上隣(左)
			if (stage->IsPositionTile({ BodyLeft,NextUp,0.0 }, i, j))
			{
				X_mapchip_tile = BodyLeft_mapchip % stage->GetStageTileWidth(i, j);
				Y_mapchip_tile = NextUp_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = Y_mapchip_tile * stage->GetStageTileWidth(i, j) + X_mapchip_tile;
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, mapchipPos)))
				{
					IsUpBlock = true;
				}
			}

			//上隣(右)
			if (stage->IsPositionTile({ BodyRight,NextUp,0.0 }, i, j))
			{
				X_mapchip_tile = BodyRight_mapchip % stage->GetStageTileWidth(i, j);
				Y_mapchip_tile = NextUp_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = Y_mapchip_tile * stage->GetStageTileWidth(i, j) + X_mapchip_tile;
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, mapchipPos)))
				{
					IsUpBlock = true;
				}
			}

			//下隣(左)
			if (stage->IsPositionTile({ BodyLeft,NextDown,0.0 }, i, j))
			{
				X_mapchip_tile = BodyLeft_mapchip % stage->GetStageTileWidth(i, j);
				Y_mapchip_tile = NextDown_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = Y_mapchip_tile * stage->GetStageTileWidth(i, j) + X_mapchip_tile;
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, mapchipPos)))
				{
					IsDownBlock = true;
				}
			}

			//下隣(右)
			if (stage->IsPositionTile({ BodyRight,NextDown,0.0 }, i, j))
			{
				X_mapchip_tile = BodyRight_mapchip % stage->GetStageTileWidth(i, j);
				Y_mapchip_tile = NextDown_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = Y_mapchip_tile * stage->GetStageTileWidth(i, j) + X_mapchip_tile;
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, mapchipPos)))
				{
					IsDownBlock = true;
				}
			}
		}
	}
}
