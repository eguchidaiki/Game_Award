#pragma once
#include "BaseScene.h"
#include "ISceneChanger.h"
#include "NY_Object3DMgr.h"
#include "SpriteManager.h"
#include "Audio.h"
#include "../RakiEngine_Library/SpriteManager.h"

#include "testParticle.h"
#include "Player.h"
#include "Stage.h"
#include "Tutorial.h"

#include "ButtonUI.h"
#include "SChangeDir.h"

enum NowState
{
	title,
	game,
	over,
};


class Title : public BaseScene
{

public:
	Title(ISceneChanger* changer);
	~Title() override {};
	void Initialize() override;    //初期化処理をオーバーライド。
	void Finalize() override;   //終了処理をオーバーライド。
	void Update() override;        //更新処理をオーバーライド。
	void Draw() override;          //描画処理をオーバーライド。

	//シーンで使うインスタンス
	XMFLOAT3 eye = {0,0,-150};
	XMFLOAT3 target = {0,0,0};
	XMFLOAT3 up = {0,1,0};

	// ゲームループで使う変数の宣言

	enum NOW_DIRECT_STATE
	{
		TITLE_DIRECT_STARTING,
		TITLE_DIRECT_USER_SELECT,
		TITLE_DIRECT_ENDING,
	};
	NOW_DIRECT_STATE titleDirectState;

	int frame = 0;
	const int TITLE_ALPHA_EASE_TIME = 90;
	float titleAlpha = 0.0f;
	const float S_ALPHA = 0.0f;
	const float E_ALPHA = 1.0f;
	RVector3 title_s_lt = { Raki_WinAPI::window_width / 2 - (380 / 2) ,
		Raki_WinAPI::window_height / 4 - (380 / 2) + 60 + 60 ,0 };
	RVector3 title_e_lt = { Raki_WinAPI::window_width / 2 - (380 / 2) ,
		Raki_WinAPI::window_height / 4 - (380 / 2) + 60 ,0};
	RVector3 title_lt;

	float title_width = 380;
	float title_height = 380;

	const int BUTTON_EASE_TIME		= 90;
	RVector3 s_lt = { 500,595,0 };
	RVector3 e_lt = { 500,495,0 };
	RVector3 ui_lt;
	float buttonAlpha = 0.0f;

	enum NOW_SELECTING
	{
		is_start,
		is_end,
	};
	NOW_SELECTING user_selecting;

	const int drawOffsetX = 320, drawOffsetY = 45;
	//タイトルのメインスプライト
	Sprite titleMainSprite;

	//スプライト
	Sprite titleBackScreen;
	Sprite titleLogo;
	Sprite titleFont;
	
	//ボタン
	UI_Button go_next_Button;
	UI_Button go_end_Button;

	//カーソル
	Sprite cursor;
	const int cursorSpriteCount = 4;

	const int MENUICON_START_X = 500;
	const int MENUICON_START_Y = 495;
	const int MENUICON_SIZE_Y = 60;
	const int MENUICON_SIZE_X = 280;
	const int MENUICON_OFFSET = 30;

	SoundData uiMoveSound;

};

