#include "Tutorial.h"
#include "Player.h"
#include "Stage.h"

namespace
{
Player* player = Player::Get();
}

Tutorial::Tutorial() :
	moveSprite{},
	jumpSprite{},
	foldSprite{},
	isTutorial(false),
	tutorialState(TutorialState::NO_TUTORIAL),
	isMoveTutorial(false),
	isJumpTutorial(false),
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
	tutorialState = TutorialState::NO_TUTORIAL;
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
	static bool isJump = false;
	static bool isFold = false;
	static bool isSelect = false;

	isMove = player->IsWalk;
	isJump = player->IsJump;
	isFold = player->IsUpFold || player->IsDownFold ||
		player->IsLeftFold || player->IsRightFold;
	isSelect = Stage::isMoveSelectCursor;

	switch (tutorialState)
	{
	case TutorialState::NO_TUTORIAL:
	{
		isFirst = false;
		isTutorial = false;
		break;
	}
	case TutorialState::MOVE:
	{
		if (isMove)
		{
			tutorialState = TutorialState::JUMP;
		}
		break;
	}
	case TutorialState::JUMP:
	{
		if (isJump)
		{
			tutorialState = TutorialState::SELECT;
		}
		break;
	}
	case TutorialState::FOLD:
	{
		if (isFold)
		{
			tutorialState = TutorialState::NO_TUTORIAL;
		}
		break;
	}
	case TutorialState::SELECT:
	{
		if (isSelect)
		{
			tutorialState = TutorialState::FOLD;
		}
		break;
	}
	default:
	{
		break;
	}
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

	XMFLOAT2 offset = { player->CenterPosition.x + offsetX, player->CenterPosition.y + offsetY };
	bool isPlayerUpBody = player->Body_Two.IsActivate && player->Body_Two.IsOpen;

	switch (tutorialState)
	{
	case TutorialState::MOVE:
	{
		moveSprite.DrawSprite(offset.x - 50.0f, offset.y - (isPlayerUpBody * PlayerBody::BodySize + spriteSize.y + 10.0f));
		moveSprite.Draw();
		break;
	}
	case TutorialState::JUMP:
	{
		jumpSprite.DrawSprite(offset.x - 50.0f, offset.y - (isPlayerUpBody * PlayerBody::BodySize + spriteSize.y + 10.0f));
		jumpSprite.Draw();
		break;
	}
	case TutorialState::FOLD:
	{
		foldSprite.DrawSprite(offset.x - 50.0f, offset.y - (isPlayerUpBody * PlayerBody::BodySize + spriteSize.y + 10.0f));
		foldSprite.Draw();
		break;
	}
	case TutorialState::SELECT:
	{
		selectSprite.DrawSprite(offset.x - 50.0f, offset.y - (isPlayerUpBody * PlayerBody::BodySize + spriteSize.y + 10.0f));
		selectSprite.Draw();
		break;
	}
	default:
	{
		break;
	}
	}
}

void Tutorial::Create()
{
	if ((moveSprite.spdata->size.x <= 0) || (moveSprite.spdata->size.y <= 0))
	{
		UINT moveHandle = TexManager::LoadTexture("./Resources/tutorial/idouUI.png");
		moveSprite.Create(moveHandle);
	}
	if ((jumpSprite.spdata->size.x <= 0) || (jumpSprite.spdata->size.y <= 0))
	{
		UINT jumpHandle = TexManager::LoadTexture("./Resources/tutorial/jump/AJump1.png");
		jumpSprite.Create(jumpHandle);
	}
	if ((foldSprite.spdata->size.x <= 0) || (foldSprite.spdata->size.y <= 0))
	{
		UINT foldHandle = TexManager::LoadTexture("./Resources/tutorial/oruUI.png");
		foldSprite.Create(foldHandle);
	}
	if ((selectSprite.spdata->size.x <= 0) || (selectSprite.spdata->size.y <= 0))
	{
		UINT selectHandle = TexManager::LoadTexture("./Resources/tutorial/select.png");
		selectSprite.Create(selectHandle);
	}
}

void Tutorial::Reset()
{
	isTutorial = true;
	tutorialState = TutorialState::MOVE;
	isFirst = true;
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
	tutorialState = TutorialState::MOVE;
	isMoveTutorial = true;
	isFoldTutorial = false;
}
