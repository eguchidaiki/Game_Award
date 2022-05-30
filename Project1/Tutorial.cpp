#include "Tutorial.h"
#include "Player.h"
#include "Stage.h"
#include "Raki_WinAPI.h"

namespace
{
Player* player = Player::Get();
}

const int Tutorial::backFrameWadth = 30;
const int Tutorial::frameWadth = 6;
const XMFLOAT2 Tutorial::frameSize = { 150.0f - Tutorial::frameWadth * 2, 220.0f - Tutorial::frameWadth * 2 };
const XMFLOAT2 Tutorial::moveSpriteSize = { 230.0f, 97.0f };
const XMFLOAT2 Tutorial::jumpSpriteSize = { 302.0f, 102.0f };
const XMFLOAT2 Tutorial::foldSpriteSize = { 230.0f, 97.0f };
const XMFLOAT2 Tutorial::selectSpriteSize = { 215.0f, 56.0f };

Tutorial::Tutorial() :
	moveSprite{},
	jumpSprite{},
	foldSprite{},
	isTutorial(false),
	tutorialState(TutorialState::NO_TUTORIAL),
	isFirst(true),
	isFirstOnly(false),
	tutorialType(TutorialType::NORMAL_TYPE),
	frameOffset{ 0.0f, 440.0f },
	drawScale(0.5f),
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
			endPos = {
				backFrameWadth + frameWadth + 10,
				(backFrameWadth + frameWadth + frameOffset.y) + 10,
				0.0f
			};
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
			endPos = {
				backFrameWadth + frameWadth - 10,
				(backFrameWadth + frameWadth + frameOffset.y) + 10 + (moveSpriteSize.y) * drawScale,
				0.0f
			};
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
			endPos = {
				backFrameWadth + frameWadth + 10,
				(backFrameWadth + frameWadth + frameOffset.y) + 10 + (moveSpriteSize.y + jumpSpriteSize.y) * drawScale,
				0.0f
			};
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
			endPos = {
				backFrameWadth + frameWadth + 10,
				(backFrameWadth + frameWadth + frameOffset.y) + 10 + (moveSpriteSize.y + jumpSpriteSize.y + foldSpriteSize.y + 20) * drawScale,
				0.0f
			};
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

	float frameUpPos = backFrameWadth + frameWadth + frameOffset.y;
	float frameDownPos = backFrameWadth + frameSize.y + frameWadth + frameOffset.y;
	float frameLeftPos = backFrameWadth + frameWadth;
	float frameRightPos = backFrameWadth + frameSize.x + frameWadth;

	Sprite::SetSpriteColorParam(206.0f / 255.0f, 206.0f / 255.0f, 206.0f / 255.0f, 0.5f);
	frameSprite.DrawExtendSprite(frameLeftPos, frameUpPos,
								 frameRightPos, frameDownPos);
	Sprite::SetSpriteColorParam(0.0f, 0.0f, 0.0f, 0.5f);
	frameSprite.DrawExtendSprite(frameLeftPos - frameWadth, frameUpPos - frameWadth,
								 frameRightPos, frameUpPos);
	frameSprite.DrawExtendSprite(frameLeftPos, frameDownPos,
								 frameRightPos + frameWadth, frameDownPos + frameWadth);
	frameSprite.DrawExtendSprite(frameLeftPos - frameWadth, frameUpPos,
								 frameLeftPos, frameDownPos + frameWadth);
	frameSprite.DrawExtendSprite(frameRightPos, frameUpPos - frameWadth,
								 frameRightPos + frameWadth, frameDownPos);
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
		UINT frameHandle = TexManager::LoadTexture("./Resources/WhitePixle.png");
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
	switch (tutorialType)
	{
	case TutorialType::NORMAL_TYPE:
		tutorialState = TutorialState::MOVE;
		break;
	case TutorialType::SELECT_TYPE:
		tutorialState = TutorialState::SELECT;
		break;
	default:
		break;
	}

	isTutorial = true;
	isFirst = true;
}

void Tutorial::StartNormalTutorial()
{
	if (isTutorial && tutorialType == TutorialType::NORMAL_TYPE)
	{
		return;
	}

	if (isFirstOnly && isFirst == false)
	{
		isTutorial = false;
		return;
	}

	isTutorial = true;
	tutorialType = TutorialType::NORMAL_TYPE;
	tutorialState = TutorialState::MOVE;
}

void Tutorial::StartSelectTutorial()
{
	if (isTutorial && tutorialType == TutorialType::SELECT_TYPE)
	{
		return;
	}

	if (isFirstOnly && isFirst == false)
	{
		isTutorial = false;
		return;
	}

	isTutorial = true;
	tutorialType = TutorialType::SELECT_TYPE;
	tutorialState = TutorialState::SELECT;
}

void Tutorial::SkipTutorial(const TutorialType& tutorialType)
{
	isTutorial = false;
	this->tutorialType = tutorialType;
	tutorialState = TutorialState::NO_TUTORIAL;
}

void Tutorial::MoveTutorial(const XMFLOAT2& offset, bool flag)
{
	if (tutorialType < TutorialType::NORMAL_TYPE)
	{
		return;
	}

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
			x = backFrameWadth + frameWadth + 10;
			y = (backFrameWadth + frameWadth + frameOffset.y) + 10;
		}

		Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 0.5f);
		moveSprite.DrawExtendSprite(x, y, x + moveSpriteSize.x * drawScale, y + moveSpriteSize.y * drawScale);
	}

	moveSprite.Draw();
}

void Tutorial::JumpTutorial(const XMFLOAT2& offset, bool flag)
{
	if (tutorialType < TutorialType::NORMAL_TYPE)
	{
		return;
	}

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
			x = backFrameWadth + frameWadth - 10;
			y = (backFrameWadth + frameWadth + frameOffset.y) + 10 + (moveSpriteSize.y) * drawScale;
		}

		Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 0.5f);
		jumpSprite.DrawExtendSprite(x, y, x + jumpSpriteSize.x * drawScale, y + jumpSpriteSize.y * drawScale);
	}

	jumpSprite.Draw();
}

void Tutorial::FoldTutorial(const XMFLOAT2& offset, bool flag)
{
	if (tutorialType < TutorialType::NORMAL_TYPE)
	{
		return;
	}

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
			x = backFrameWadth + frameWadth + 10;
			y = (backFrameWadth + frameWadth + frameOffset.y) + 10 + (moveSpriteSize.y + jumpSpriteSize.y) * drawScale;
		}

		Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 0.5f);
		foldSprite.DrawExtendSprite(x, y, x + foldSpriteSize.x * drawScale, y + foldSpriteSize.y * drawScale);
	}

	foldSprite.Draw();
}

void Tutorial::SelectTutorial(const XMFLOAT2& offset, bool flag)
{
	if (tutorialType < TutorialType::SELECT_TYPE)
	{
		return;
	}

	static float x = 0.0f, y = 0.0f;

	if (tutorialState == TutorialState::SELECT)
	{
		x = offset.x - 50.0f;
		y = offset.y - (flag * PlayerBody::BodySize + selectSpriteSize.y + 10.0f);
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
			x = backFrameWadth + frameWadth + 10;
			y = (backFrameWadth + frameWadth + frameOffset.y) + 10 + (moveSpriteSize.y + jumpSpriteSize.y + foldSpriteSize.y + 20) * drawScale;
		}

		Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 0.5f);
		selectSprite.DrawExtendSprite(x, y, x + selectSpriteSize.x * drawScale, y + selectSpriteSize.y * drawScale);
	}

	selectSprite.Draw();
}
