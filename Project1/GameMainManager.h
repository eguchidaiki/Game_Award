#pragma once

#include "Stage.h"
#include "Player.h"
#include "UI.h"
#include "Raki_imguiMgr.h"

class GameMainManager
{
private:
	Stage* stage;
	Player* player;

	// ゲームループで使う変数の宣言
	UINT BackHandle;
	Sprite Back;
	unsigned char playerTile[4] = { 0 };
	const int drawOffsetX = 320, drawOffsetY = 45;

	UI ui;



public:
	bool IsGoSelect = false;
	bool Ischangecount = false;
	int changecount = 0;
	GameMainManager();
	~GameMainManager();

	void Init(Stage* stageptr, Player* playerptr);
	void Update();
	void Draw();
	void Finalize();

private:
	void GameInstanceUpdate();
	void GameInstanceDraw();

};

