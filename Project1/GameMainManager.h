#pragma once

#include "UI.h"
#include "Raki_imguiMgr.h"
#include "Tutorial.h"

#include <Audio.h>

class GameMainManager
{
private:
	// ゲームループで使う変数の宣言
	UINT BackHandle;
	Sprite Back;
	unsigned char playerTile[4] = { 0 };
	const int drawOffsetX = 320, drawOffsetY = 45;


	//髻ｳ
	SoundData menuBGM;
	SoundData playBGM;

	UI ui;

	Tutorial tutorial;

public:
	bool IsGoSelect = false;
	bool Ischangecount = false;
	int changecount = 0;
	GameMainManager();
	~GameMainManager();

	void Init();
	void Update();
	void Draw();
	void Finalize();

private:
	void GameInstanceUpdate();
	void GameInstanceDraw();

};

