#include "Tutorial.h"
#include "Player.h"
#include "InputManger.h"

namespace
{
Player* player = Player::Get();
}

Tutorial::Tutorial() :
	moveHandle(0),
	foldHandle(0),
	moveSprite{},
	foldSprite{},
	isTutorial(false),
	isMoveTutorial(false),
	isFoldTutorial(false),
	isFirst(true),
	isFirstOnly(false),
	spriteSize{ 230.0f, 97.0f }
{
	Init();
}

Tutorial::~Tutorial()
{
}

void Tutorial::Init()
{
	isTutorial = false;
	isMoveTutorial = true;
	isFoldTutorial = false;
	isFirst = true;
	isFirstOnly = false;
}

void Tutorial::Update()
{
	if (isTutorial == false)
	{
		return;
	}

	if (isFirstOnly && isFirst == false)
	{
		isTutorial = false;
		return;
	}

	static bool isMove = false;
	static bool isFold = false;

	isMove = player->IsWalk;
	isFold = player->IsUpFold || player->IsDownFold ||
		player->IsLeftFold || player->IsRightFold;

	if (isMoveTutorial == true)
	{
		if (isMove)
		{
			isMoveTutorial = false;
			isFoldTutorial = true;
		}
	}
	else if (isFoldTutorial == true)
	{
		if (isFold)
		{
			isFoldTutorial = false;
		}
	}

	if (isMoveTutorial == false && isFoldTutorial == false)
	{
		isFirst = false;
		isTutorial = false;
	}
}

void Tutorial::Draw(int offsetX, int offsetY)
{
	if (isTutorial == false)
	{
		return;
	}

	if (isFirstOnly && isFirst == false)
	{
		return;
	}

	bool isPlayerUpBody = player->Body_Two.IsActivate && player->Body_Two.IsOpen;

	if (isMoveTutorial)
	{
		moveSprite.DrawSprite(static_cast<float>(offsetX) - 50.0f, static_cast<float>(offsetY) - (isPlayerUpBody * PlayerBody::BodySize + spriteSize.y + 10.0f));
		moveSprite.Draw();
	}
	if (isFoldTutorial)
	{
		foldSprite.DrawSprite(static_cast<float>(offsetX) - 50.0f, static_cast<float>(offsetY) - (isPlayerUpBody * PlayerBody::BodySize + spriteSize.y + 10.0f));
		foldSprite.Draw();
	}
}

void Tutorial::Create()
{
	if ((moveSprite.spdata->size.x <= 0) || (moveSprite.spdata->size.y <= 0))
	{
		moveHandle = TexManager::LoadTexture("./Resources/idouUI.png");
		moveSprite.Create(moveHandle);
	}
	if ((foldSprite.spdata->size.x <= 0) || (foldSprite.spdata->size.y <= 0))
	{
		foldHandle = TexManager::LoadTexture("./Resources/oruUI.png");
		foldSprite.Create(foldHandle);
	}
}

void Tutorial::StartTutorial()
{
	if (isTutorial)
	{
		return;
	}

	if (isFirstOnly && isFirst == false)
	{
		isTutorial = false;
		return;
	}

	isTutorial = true;
	isMoveTutorial = true;
	isFoldTutorial = false;
}
