#include "Tutorial.h"
#include "ActFlag.h"

namespace
{
	ActFlag* actFlag = ActFlag::Get();
}

Tutorial::Tutorial() :
	isTutorial(false),
	isMoveTutorial(false),
	isFoldTutorial(false)
{
}

Tutorial::~Tutorial()
{
}

void Tutorial::Update()
{
	if (isTutorial == false)
	{
		return;
	}

	static bool isMove = false;
	static bool isFold = false;

	isMove = actFlag->MoveLeft() || actFlag->MoveRight();
	isFold = actFlag->FoldUp() || actFlag->FoldDown() ||
		actFlag->FoldLeft() || actFlag->FoldRight();

	if (isMoveTutorial == false)
	{
		if (isMove)
		{
			isMoveTutorial = true;
		}
	}
	else if (isFoldTutorial == false)
	{
		if (isFold)
		{
			isFoldTutorial = true;
		}
	}

	if (isMoveTutorial && isFoldTutorial)
	{
		isTutorial = false;
	}
}

void Tutorial::StartTutorial()
{
	isTutorial = true;
	isMoveTutorial = false;
	isFoldTutorial = false;
}
