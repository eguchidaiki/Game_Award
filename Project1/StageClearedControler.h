#pragma once

#include <memory>

//ゴール演出
#include "GoalEffects.h"
#include "Player.h"
#include "ButtonUI.h"
#include "StageSelecter.h"

//ステージクリア時の挙動を管理するクラス
class StageClearedControler
{
private:
	///プレイヤーの座標を参照（書き換え禁止）
	const RVector3* pCenterPos = &Player::Get()->CenterPosition;
	//現在のステージ番号取得

	Sprite gameMainSprite;

	StageSelecter* selecter;

public:
	StageClearedControler(){}
	~StageClearedControler(){}

	//初期化
	void Init(StageSelecter *ptr);

	void Update();

	void Draw(int rtHandle);

	//クリア時処理有効化
	void ControlActivate();
	//無効化
	void ControlDisable();

	//UIコントロールの状態定義
	enum STAGE_CLEARED_CONTROL_STATE
	{
		CONTROL_NOT_ALLOW,	//許可しない、描画もしない
		CONTROL_DIRECING,	//演出中なのでスキップのみ許可
		CONTROL_ACTIVE,		//UI操作を許可
		CONTROL_UI_SELECTED,
	};

	//ユーザー選択
	enum STAGE_CLEARED_USER_SELECTING
	{
		USER_SELECT_NEXT,
		USER_SELECT_BACK,
	};
	STAGE_CLEARED_USER_SELECTING _user_selecting = USER_SELECT_NEXT;

	//切り替え許可フラグ
	bool isAllowSwitching = false;

private:
	STAGE_CLEARED_CONTROL_STATE ctrl_state = CONTROL_NOT_ALLOW;
	
	//ゴール時演出するクラス
	std::shared_ptr<GoalEffects> goalEffect;

	//UIボタン
	UI_Button _go_select_button;
	float x1 = 0.0f, y1 = 0.0f;
	UI_Button _go_next_button;
	float x2 = 0.0f, y2 = 0.0f;
	const float SELECTING_SCALE = 1.2f;

	//ステージクリア背景
	Sprite clearedBackSprite;

	//UIコントロールの状態を取得、変更
	void Update_CheckControlStates();
	//フレームカウント
	int frameCount = 0;
	//演出総フレーム数
	const int DIRECTING_FRAME = 60;

	//描画座標
	RVector3 LT, RB;
	//レンダーテクスチャ初期座標
	RVector3 drawLT = { 0,0,0 };
	RVector3 drawRB = { 1280,720,0 };
	//ターゲット座標
	RVector3 targetLT = { 326.0f,165.0f,0 };
	RVector3 targetRB = { 933.0f,506.0f,0 };

	//状態に応じた実際の処理
	void Update_ControlMain();

	void Draw_UI();


};

