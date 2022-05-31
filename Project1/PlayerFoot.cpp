#include "PlayerFoot.h"
#include "Player.h"

namespace
{
	static Player* player = Player::Get();
}

PlayerFoot::PlayerFoot() :
	FootLeftUpPosition{},
	ease{},
	FootIsAction(false),
	IsFootUp(false),
	FootHandle(-1)
{
	Init();
}

PlayerFoot::~PlayerFoot()
{
}

void PlayerFoot::Init()
{
}

void PlayerFoot::Set()
{
	ease.addTime = 0.1f;
	ease.maxTime = 1.2f;
	ease.timeRate = 0.0f;
	FootIsAction = true;
	IsFootUp = true;
}

void PlayerFoot::Update(RVector3& DownPos, bool IsDownBody, int BodyDis)
{
	if (!FootIsAction && player->IsWalk && (!player->IsJump || !player->Player_IsAction || !player->IsAllFall))
	{
		IsWalkAnimation = true;
	}
	else
	{
		IsWalkAnimation = false;
		WalkAnimationCount = 0;
	}

	static size_t animationCount = 0;

	animationCount++;
	animationCount %= 0xFFFFFFFF;

	if (IsWalkAnimation)
	{
		if (animationCount % 10 == 0)
		{
			WalkAnimationCount++;
		}
		if (WalkAnimationCount > 11)
		{
			WalkAnimationCount = 0;
		}
	}

	if (FootIsAction == true)
	{
		ease.addTime += ease.maxTime / 40.0f;
		ease.timeRate = min(ease.addTime / ease.maxTime, 1.0f);

		FootLeftUpPosition = { DownPos.x,ease.easeOut(DownPos.y ,DownPos.y - 15,ease.timeRate),0.0f };
		if (ease.timeRate >= 1.0f)
		{
			FootIsAction = false;
		}
	}
	else
	{
		FootLeftUpPosition = DownPos;
	}
}

void PlayerFoot::Draw(int offsetX, int offsetY, bool isleft, bool isright)
{
	if (player->IsGoal)
	{
		return;
	}

	if (IsWalkAnimation)
	{
		FootAnimation.uvOffsetHandle = WalkAnimationCount;
		if (isleft)
		{
			FootAnimation.DrawExtendSprite(
				static_cast<int>(FootLeftUpPosition.x) + offsetX, static_cast<int>(FootLeftUpPosition.y - 6) + offsetY,
				static_cast<int>(FootLeftUpPosition.x) + 60 + offsetX, static_cast<int>(FootLeftUpPosition.y) + 8 + offsetY);
		}
		if (isright)
		{
			FootAnimation.DrawExtendSprite(
				static_cast<int>(FootLeftUpPosition.x) + 60 + offsetX, static_cast<int>(FootLeftUpPosition.y - 6) + offsetY,
				static_cast<int>(FootLeftUpPosition.x) + offsetX, static_cast<int>(FootLeftUpPosition.y) + 8 + offsetY);
		}
		FootAnimation.Draw();
	}

	if (isleft && !IsWalkAnimation)
	{
		FootSprite.DrawExtendSprite(
			static_cast<int>(FootLeftUpPosition.x) + offsetX, static_cast<int>(FootLeftUpPosition.y) + offsetY,
			static_cast<int>(FootLeftUpPosition.x) + 60 + offsetX, static_cast<int>(FootLeftUpPosition.y) + 8 + offsetY);
	}
	if (isright && !IsWalkAnimation)
	{
		FootSprite.DrawExtendSprite(
			static_cast<int>(FootLeftUpPosition.x) + 60 + offsetX, static_cast<int>(FootLeftUpPosition.y) + offsetY,
			static_cast<int>(FootLeftUpPosition.x) + offsetX, static_cast<int>(FootLeftUpPosition.y) + 8 + offsetY);
	}

	FootSprite.Draw();
}

void PlayerFoot::Create()
{
	if ((FootSprite.spdata->size.x <= 0) || (FootSprite.spdata->size.y <= 0))
	{
		FootHandle = TexManager::LoadTexture("./Resources/playerLegs.png");
		FootSprite.Create(FootHandle);
	}

	if ((FootAnimation.spdata->size.x <= 0) || (FootAnimation.spdata->size.y <= 0))
	{
		FootAnimation.CreateAndSetDivisionUVOffsets(12, 12, 1, 57, 13, TexManager::LoadTexture("Resources/playerRunLeg.png"));
	}
}
