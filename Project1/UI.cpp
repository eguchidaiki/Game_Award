#include "UI.h"

void UI::Init()
{
	ResetHandle = TexManager::LoadTexture("Resources/UI/resetButton.png");
	this->ResetUISprite.CreateSprite({ 72, 77 }, { 0.5,0.5 }, ResetHandle, false);
	this->ResetUISprite.spdata.get()->position = { 1102.0f,650.5f,0.0f };

	PauseHandle = TexManager::LoadTexture("Resources/UI/pauseButton.png");
	this->PauseUISprite.CreateSprite({ 72, 77 }, { 0.5,0.5 }, PauseHandle, false);
	this->PauseUISprite.spdata.get()->position = { 1200.0f,650.5f,0.0f };

	ResetAddRot = 1;
	PauseAddRot = 1;
}

void UI::Update()
{
	mousePos = Input::getMousePos();

	IsInButton();

	if (IsResetPress)
	{
		ResetRot += ResetAddRot;

		if (ResetRot >= 30.0f && ResetAddRot > 0)
		{
			ResetAddRot = -1;
		}
		else if (ResetRot <= -30.0f && ResetAddRot < 0)
		{
			ResetAddRot = 1;
		}
	}
	else
	{
		ResetRot = 0;
	}

	if (IsPausePress)
	{
		PauseRot += PauseAddRot;

		if (PauseRot >= 30.0f && PauseAddRot > 0)
		{
			PauseAddRot = -1;
		}
		else if (PauseRot <= -30.0f && PauseAddRot < 0)
		{
			PauseAddRot = 1;
		}
	}
	else
	{
		PauseRot = 0;
	}

	ResetUISprite.spdata.get()->rotation = ResetRot;
	ResetUISprite.UpdateSprite();

	PauseUISprite.spdata.get()->rotation = PauseRot;
	PauseUISprite.UpdateSprite();
}

void UI::Draw()
{
	SpriteManager::Get()->SetCommonBeginDraw();

	//ResetUISprite.DrawExtendSprite(1066, 612, 1138, 689);
	
	ResetUISprite.Draw();

	//PauseUISprite.DrawExtendSprite(1164, 612, 1236, 689);
	
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
