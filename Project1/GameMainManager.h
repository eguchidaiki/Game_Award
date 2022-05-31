#pragma once

#include "UI.h"
#include "Raki_imguiMgr.h"
#include "Tutorial.h"

#include "StageSelecter.h"
#include "StageClearedControler.h"
#include "BackGroundGraphic.h"

#include <RenderTargetManager.h>
#include <array>
#include <Audio.h>

class GameMainManager
{
private:
	// ゲームループで使う変数の宣言
	UINT BackHandle;
	Sprite Back;
	const int tutorialOffsetX = 320, tutorialOffsetY = 45;

	SoundData playBGM;

	UI ui;

	Tutorial tutorial;

	BackGroundGraphic bg;

	//ゲーム本編の描画ハンドル
	int gameMainGraphHandle;

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

	bool IsFolds[4];
	bool IsOpens[4];

	StageSelecter* selecterPtr;

	GameMainManager();
	~GameMainManager();

	void Init();
	void Update();
	void Draw();
	void Finalize();
	void SetSelectToGame(int SelectStageNum);
	void SetGameToSelect();

	StageClearedControler* GetSCCtrl();

private:
	void GameInstanceUpdate();
	void GameInstanceDraw();

	StageClearedControler stageClearCtrl;

};

