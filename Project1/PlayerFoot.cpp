#include "PlayerFoot.h"

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

void PlayerFoot::Update(RVector3& FaceCenterPos, bool IsDownBody, int BodyDis)
{
	if (FootIsAction == true)
	{
		ease.addTime += ease.maxTime / 40.0f;
		ease.timeRate = min(ease.addTime / ease.maxTime, 1.0f);

		FootLeftUpPosition = { FaceCenterPos.x - 30,ease.easeOut(FaceCenterPos.y + (IsDownBody * 50 * BodyDis) + 25,FaceCenterPos.y + (IsDownBody * 50 * BodyDis) + 15,ease.timeRate),0.0f };
		if (ease.timeRate >= 1.0f)
		{
			FootIsAction = false;
		}
	}

	if (IsFootUp == false)
	{
		FootLeftUpPosition = { FaceCenterPos.x - 30,FaceCenterPos.y + 25,0.0f };
	}
}

void PlayerFoot::Draw(int offsetX, int offsetY, bool isleft, bool isright)
{
	if (isleft)
	{
		FootSprite.DrawExtendSprite(
			static_cast<int>(FootLeftUpPosition.x) + offsetX, static_cast<int>(FootLeftUpPosition.y) + offsetY,
			static_cast<int>(FootLeftUpPosition.x) + 60 + offsetX, static_cast<int>(FootLeftUpPosition.y) + 8 + offsetY);
		/*DrawExtendGraph(
			static_cast<int>(FootLeftUpPosition.x) + offsetX, static_cast<int>(FootLeftUpPosition.y) + offsetY,
			static_cast<int>(FootLeftUpPosition.x) + 60 + offsetX, static_cast<int>(FootLeftUpPosition.y) + 8 + offsetY, FootHandle, true);*/
	}
	if (isright)
	{
		FootSprite.DrawExtendSprite(
			static_cast<int>(FootLeftUpPosition.x) + 60 + offsetX, static_cast<int>(FootLeftUpPosition.y) + offsetY,
			static_cast<int>(FootLeftUpPosition.x) + offsetX, static_cast<int>(FootLeftUpPosition.y) + 8 + offsetY);
		/*DrawExtendGraph(
			static_cast<int>(FootLeftUpPosition.x) + 60 + offsetX, static_cast<int>(FootLeftUpPosition.y) + offsetY,
			static_cast<int>(FootLeftUpPosition.x) + offsetX, static_cast<int>(FootLeftUpPosition.y) + 8 + offsetY, FootHandle, true);*/
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
}
