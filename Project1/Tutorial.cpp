#include "Tutorial.h"
#include "Player.h"
#include "Stage.h"
#include "Raki_WinAPI.h"

namespace
{
Player* player = Player::Get();
}

const int Tutorial::backFrameWadth = 30;
const XMFLOAT2 Tutorial::moveSpriteSize = { 230.0f, 97.0f };
const XMFLOAT2 Tutorial::jumpSpriteSize = { 302.0f, 102.0f };
const XMFLOAT2 Tutorial::foldSpriteSize = { 230.0f, 97.0f };
const XMFLOAT2 Tutorial::selectSpriteSize = { 410.0f, 64.0f };

Tutorial::Tutorial() :
	moveSprite{},
	jumpSprite{},
	foldSprite{},
	isTutorial(false),
	tutorialState(TutorialState::NO_TUTORIAL),
	isFirst(true),
	isFirstOnly(false),
	isNormal(false),
	isSelect(false),
	isEase(false),
	easeState(TutorialState::NO_TUTORIAL),
	timeRate(0.0f),
	startPos{},
	endPos{},
	easePos{}
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
	isFirst = true;
	isFirstOnly = false;
}

void Tutorial::Update()
{
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

	if (isEase)
	{
		timeRate += 1.0f / 10.0f;
		easePos = Rv3Ease::lerp(startPos, endPos, timeRate);

		if (timeRate >= 1.0f)
		{
			isEase = false;
		}
	}

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
			isEase = true;
			easeState = tutorialState;
			timeRate = 0.0f;
			endPos = { backFrameWadth + 10.0f, 560.0f + 20.0f, 0.0f };
			easePos = startPos;

			tutorialState = TutorialState::JUMP;
		}
		break;
	}
	case TutorialState::JUMP:
	{
		if (isJump)
		{
			isEase = true;
			easeState = tutorialState;
			timeRate = 0.0f;
			endPos = { (backFrameWadth + 10.0f) + moveSpriteSize.x, 560.0f + 20.0f, 0.0f };
			easePos = startPos;

			tutorialState = TutorialState::FOLD;
		}
		break;
	}
	case TutorialState::FOLD:
	{
		if (isFold)
		{
			isEase = true;
			easeState = tutorialState;
			timeRate = 0.0f;
			endPos = { (backFrameWadth + 10.0f) + (moveSpriteSize.x + jumpSpriteSize.x),
				560.0f + 20.0f, 0.0f };
			easePos = startPos;

			tutorialState = TutorialState::NO_TUTORIAL;
		}
		break;
	}
	case TutorialState::SELECT:
	{
		if (isSelect)
		{
			isEase = true;
			easeState = tutorialState;
			timeRate = 0.0f;
			endPos = { (backFrameWadth + 10.0f) + (moveSpriteSize.x + jumpSpriteSize.x + foldSpriteSize.x),
				560.0f + 25.0f, 0.0f };
			easePos = startPos;

			tutorialState = TutorialState::NO_TUTORIAL;
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
	XMFLOAT2 offset = { player->CenterPosition.x + offsetX, player->CenterPosition.y + offsetY };
	bool isPlayerUpBody = player->Body_Two.IsActivate && player->Body_Two.IsOpen;

	Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 0.5f);
	frameSprite.DrawSprite(backFrameWadth, Raki_WinAPI::window_height - backFrameWadth - 130);
	frameSprite.Draw();

	if (tutorialState == TutorialState::NO_TUTORIAL)
	{
		MoveTutorial(offset, isPlayerUpBody);
		JumpTutorial(offset, isPlayerUpBody);
		FoldTutorial(offset, isPlayerUpBody);
		SelectTutorial(offset, isPlayerUpBody);
	}
	else
	{
		if (tutorialState >= TutorialState::MOVE)
		{
			MoveTutorial(offset, isPlayerUpBody);
		}
		if (tutorialState >= TutorialState::JUMP)
		{
			JumpTutorial(offset, isPlayerUpBody);
		}
		if (tutorialState >= TutorialState::FOLD)
		{
			FoldTutorial(offset, isPlayerUpBody);
		}
		if (tutorialState >= TutorialState::SELECT)
		{
			SelectTutorial(offset, isPlayerUpBody);
		}
	}

	Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
}

void Tutorial::Create()
{
	if ((frameSprite.spdata->size.x <= 0) || (frameSprite.spdata->size.y <= 0))
	{
		UINT frameHandle = TexManager::LoadTexture("./Resources/tutorial/wakuUI.png");
		frameSprite.Create(frameHandle);
	}
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
	if (isNormal)
	{
		tutorialState = TutorialState::MOVE;
	}
	else if (isSelect)
	{
		tutorialState = TutorialState::SELECT;
	}

	isTutorial = true;
	isFirst = true;
}

void Tutorial::StartNormalTutorial()
{
	if (isTutorial && isNormal)
	{
		return;
	}

	if (isFirstOnly && isFirst == false)
	{
		isTutorial = false;
		return;
	}

	isTutorial = true;
	isNormal = true;
	isSelect = false;
	tutorialState = TutorialState::MOVE;
}

void Tutorial::StartSelectTutorial()
{
	if (isTutorial && isSelect)
	{
		return;
	}

	if (isFirstOnly && isFirst == false)
	{
		isTutorial = false;
		return;
	}

	isTutorial = true;
	isNormal = false;
	isSelect = true;
	tutorialState = TutorialState::SELECT;
}

void Tutorial::MoveTutorial(const XMFLOAT2& offset, bool flag)
{
	static float x = 0.0f, y = 0.0f;

	if (tutorialState == TutorialState::MOVE)
	{
		x = offset.x - 50.0f;
		y = offset.y - (flag * PlayerBody::BodySize + moveSpriteSize.y + 10.0f);
		startPos = { x, y, 0.0f };

		Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
		moveSprite.DrawSprite(x, y);
	}
	else
	{
		if (isEase && easeState == TutorialState::MOVE)
		{
			x = easePos.x;
			y = easePos.y;
		}
		else
		{
			x = backFrameWadth + 10.0f;
			y = 560.0f + 25.0f;
		}

		Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 0.5f);
		moveSprite.DrawSprite(x, y);
	}

	moveSprite.Draw();
}

void Tutorial::JumpTutorial(const XMFLOAT2& offset, bool flag)
{
	static float x = 0.0f, y = 0.0f;

	if (tutorialState == TutorialState::JUMP)
	{
		x = offset.x - 50.0f;
		y = offset.y - (flag * PlayerBody::BodySize + jumpSpriteSize.y + 10.0f);
		startPos = { x, y, 0.0f };

		Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
		jumpSprite.DrawSprite(x, y);
	}
	else
	{
		if (isEase && easeState == TutorialState::JUMP)
		{
			x = easePos.x;
			y = easePos.y;
		}
		else
		{
			x = (backFrameWadth + 10.0f) + moveSpriteSize.x;
			y = 560.0f + 20.0f;
		}

		Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 0.5f);
		jumpSprite.DrawSprite(x, y);
	}

	jumpSprite.Draw();
}

void Tutorial::FoldTutorial(const XMFLOAT2& offset, bool flag)
{
	static float x = 0.0f, y = 0.0f;

	if (tutorialState == TutorialState::FOLD)
	{
		x = offset.x - 50.0f;
		y = offset.y - (flag * PlayerBody::BodySize + foldSpriteSize.y + 10.0f);
		startPos = { x, y, 0.0f };

		Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
		foldSprite.DrawSprite(x, y);
	}
	else
	{
		if (isEase && easeState == TutorialState::FOLD)
		{
			x = easePos.x;
			y = easePos.y;
		}
		else
		{
			x = (backFrameWadth + 10.0f) + (moveSpriteSize.x + jumpSpriteSize.x);
			y = 560.0f + 20.0f;
		}

		Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 0.5f);
		foldSprite.DrawSprite(x, y);
	}

	foldSprite.Draw();
}

void Tutorial::SelectTutorial(const XMFLOAT2& offset, bool flag)
{
	if (isSelect == false)
	{
		return;
	}

	static float x = 0.0f, y = 0.0f;

	if (tutorialState == TutorialState::SELECT)
	{
		x = offset.x - 50.0f;
		y = offset.y - (flag * PlayerBody::BodySize + jumpSpriteSize.y + 10.0f);
		startPos = { x, y, 0.0f };

		Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
		selectSprite.DrawSprite(offset.x - 50.0f, offset.y - (flag * PlayerBody::BodySize + selectSpriteSize.y + 10.0f));
	}
	else
	{
		if (isEase && easeState == TutorialState::SELECT)
		{
			x = easePos.x;
			y = easePos.y;
		}
		else
		{
			x = (backFrameWadth + 10.0f) + (moveSpriteSize.x + jumpSpriteSize.x + foldSpriteSize.x);
			y = 560.0f + 25.0f;
		}

		Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 0.5f);
		selectSprite.DrawSprite(x, y);
	}

	selectSprite.Draw();
}
