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


	//BGM
	SoundData menuBGM;
	SoundData playBGM;

	UI ui;

	Tutorial tutorial;

public:
	//今プレイしているステージ番号
	int NowScene = 0;
	//ステージリセットを一回だけ通すフラグ
	bool IsStart;
	//セレクトシーン遷移フラグ
	bool IsGoSelect = false;
	//セレクトシーンに戻るまでのカウントとその開始フラグ
	int changecount = 0;
	bool Ischangecount = false;

	GameMainManager();
	~GameMainManager();

	void Init();
	void Update();
	void Draw();
	void Finalize();
	void SetSelectToGame(int SelectStageNum);
	void SetGameToSelect();

private:
	void GameInstanceUpdate();
	void GameInstanceDraw();

};

