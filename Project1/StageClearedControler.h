#pragma once

#include <memory>

//ゴール演出
#include "GoalEffects.h"
#include "Player.h"


//ボタンクラス（動作自体は同じで、グラフィックやサイズなどを差し替えることで使い回す）
class UI_Button
{
private:
	//ボタンの描画サイズ
	float width = 0.0f, height = 0.0f;
	//スプライト
	Sprite uiSprite;
	//ユーザーが押してから処理を有効化するまでのラグ(frame数)
	int activateTimerag = 0;
	//フレームカウント
	int count = 0;

public:
	//このボタンはユーザーによって押されたか？
	bool isUserPushed = false;
	//このボタンが押されたことによって起こる処理を有効化させるか？
	bool isFunctionActivate = false;


	//初期化
	void Init(int activateTimerag, UINT graphHandle, float baseWidth, float baseHeight);
	//更新
	void Update();
	//描画
	void Draw(float centerX, float centerY, float x_scale = 1.0f, float y_scale = 1.0f);

	//終了（いるか？）
	void Finalize(){}

	//押す
	void UI_Push();

};


//ステージクリア時の挙動を管理するクラス
class StageClearedControler
{
private:
	///プレイヤーの座標を参照（書き換え禁止）
	const RVector3* pCenterPos = &Player::Get()->CenterPosition;

public:
	StageClearedControler(){}
	~StageClearedControler(){}

	//初期化
	void Init();

	void Update();

	void Draw();

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

	//UIコントロールの状態を取得、変更
	void Update_CheckControlStates();
	int frameCount = 0;
	const int DIRECTING_FRAME = 60;

	//状態に応じた実際の処理
	void Update_ControlMain();

	void Draw_UI();


};

