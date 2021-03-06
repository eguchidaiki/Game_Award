#include "UI.h"
#include "Player.h"
#include "Stage.h"
#include "InputManger.h"

namespace
{
	Stage* stage = Stage::Get();
	Player* player = Player::Get();
}

void UI::Init(Tutorial* tutorial)
{
	ResetHandle = TexManager::LoadTexture("Resources/UI/resetButton.png");
	this->ResetUISprite.Create(ResetHandle);

	PauseHandle = TexManager::LoadTexture("Resources/UI/pauseButton.png");
	this->PauseUISprite.Create(PauseHandle);

	ResetRot = 0;
	ResetAddRot = 1;
	PauseRot = 0;
	PauseAddRot = 1;

	this->tutorial = tutorial;
}

void UI::Update(unsigned char PlayerTile[4], bool* Isback, int StageNum)
{
	//mousePos = Input::getMousePos();

	IsInButton();

	if ((IsResetPress && Input::isMouseClickTrigger(0)) || InputManger::Get()->ResetTrigger())
	{
		player->DeathAnimation();

		stage->Reset(PlayerTile);
		player->Init();
		player->BodySetUp(PlayerTile);

		if (StageNum == 1 || StageNum == 3)
		{
			tutorial->Reset();
		}
	}

	int movestagecount = 0;

	for (int i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (int j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			if (stage->IsTileMove(i, j))
			{
				movestagecount++;
			}
		}
	}

	if ((IsPausePress && Input::isMouseClickTrigger(0)) || InputManger::Get()->MenuTrigger() && movestagecount == 0)
	{
		*Isback = true;
	}
}

void UI::Draw()
{
	SpriteManager::Get()->SetCommonBeginDraw();

	if (!IsResetPress)
	{
		ResetUISprite.DrawExtendSprite(1066.0f, 612.0f, 1138.0f, 689.0f);
	}
	else
	{
		ResetUISprite.DrawExtendSprite(1056.0f, 602.0f, 1148.0f, 699.0f);
	}

	if (!IsPausePress)
	{
		PauseUISprite.DrawExtendSprite(1164.0f, 612.0f, 1236.0f, 689.0f);
	}
	else
	{
		PauseUISprite.DrawExtendSprite(1154.0f, 602.0f, 1246.0f, 699.0f);
	}

	ResetUISprite.Draw();
	PauseUISprite.Draw();
}

void UI::IsInButton()
{
	if (mousePos.x >= 1066 && mousePos.y >= 612 && mousePos.x <= 1138 && mousePos.y <= 689)
	{
		IsResetPress = true;
	}
	else
	{
		IsResetPress = false;
	}

	if (mousePos.x >= 1164 && mousePos.y >= 612 && mousePos.x <= 1236 && mousePos.y <= 689)
	{
		IsPausePress = true;
	}
	else
	{
		IsPausePress = false;
	}
}
