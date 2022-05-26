#pragma once
#include "SpriteManager.h"
#include "../RakiEngine_Library/SpriteManager.h"
#include "Sprite.h"
#include <Raki_Input.h>
#include <DirectXMath.h>
#include "Tutorial.h"

class UI
{
public:
	void Init(Tutorial* tutorial);

	void Update(unsigned char PlayerTile[4], bool* Isback, int StageNum);

	void Draw();

	void IsInButton();

	//private:
	UINT ResetHandle = 0;
	Sprite ResetUISprite;

	UINT PauseHandle = 0;
	Sprite PauseUISprite;

	bool IsResetPress = false;
	bool IsPausePress = false;

	float ResetRot = 0.0f;
	float ResetAddRot = 0.0f;
	float PauseRot = 0.0f;
	float PauseAddRot = 0.0f;

	XMFLOAT2 mousePos = { 0.0f, 0.0f };

	//チュートリアルクラスポインター
	Tutorial* tutorial = nullptr;
};