#pragma once
#include "SpriteManager.h"
#include "../RakiEngine_Library/SpriteManager.h"
#include "Sprite.h"
#include <Raki_Input.h>
#include <DirectXMath.h>
#include "Player.h"
#include "Stage.h"

class UI
{
public:
	void Init();

	void Update(Stage* stage, Player* player, unsigned char PlayerTile[4], bool* Isback);

	void Draw();

	void IsInButton();

	//private:
	UINT ResetHandle;
	Sprite ResetUISprite;

	UINT PauseHandle;
	Sprite PauseUISprite;

	bool IsResetPress;
	bool IsPausePress;

	float ResetRot;
	float ResetAddRot;
	float PauseRot;
	float PauseAddRot;

	XMFLOAT2 mousePos;
};