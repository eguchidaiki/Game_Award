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
	isFirstOnly(false)
{
	Init();
}

Tutorial::~Tutorial()
{
}

void Tutorial::Init()
{
	isTutorial = false;
	isMoveTutorial = false;
	isFoldTutorial = false;
	isFirst = true;
	isFirstOnly = false;
}

void Tutorial::Update()
{
	static bool isMove = false;
	static bool isFold = false;

	isMove = player->IsWalk;
	isFold = player->IsUpFold || player->IsDownFold ||
		player->IsLeftFold || player->IsRightFold;

	if (isMoveTutorial && player->IsWalk)
	{
		isMoveTutorial = false;
		isFoldTutorial = true;
	}
}

void Tutorial::Draw(int offsetX, int offsetY)
{
	if (isMoveTutorial == true)
	{
		moveSprite.DrawSprite(offsetX, offsetY);
		moveSprite.Draw();
	}
	if(isFoldTutorial == true)
	{
		foldSprite.DrawSprite(offsetX, offsetY);
		foldSprite.Draw();
	}
}

void Tutorial::Create()
{
	if ((moveSprite.spdata->size.x <= 0) || (moveSprite.spdata->size.y <= 0))
	{
		moveHandle = TexManager::LoadTexture("./Resources/tutorialMove.png");
		moveSprite.Create(moveHandle);
	}
	if ((foldSprite.spdata->size.x <= 0) || (foldSprite.spdata->size.y <= 0))
	{
		foldHandle = TexManager::LoadTexture("./Resources/tutorialFold.png");
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

void Tutorial::ResetTutorial()
{
	isTutorial = false;
	isMoveTutorial = true;
	isFoldTutorial = false;
	isFirst = true;
	isFirstOnly = false;
}
