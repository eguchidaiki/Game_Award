#pragma once

#include <Sprite.h>
#include <Audio.h>

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
	//イージングスケーリング
	const float EASE_SCALE_X = 0.7f, EASE_SCALE_Y = 0.7f;
	const float EASE_SCALE_START_X = 1.0f;
	float easeScaleX = 1.0f, easeSceleY = 1.0f;

	static SoundData _ui_push_sound;

public:
	//このボタンはユーザーによって押されたか？
	bool isUserPushed = false;
	//このボタンが押されたことによって起こる処理を有効化させるか？
	bool isFunctionActivate = false;

	void Reset();
	//初期化
	void Init(int activateTimerag, UINT graphHandle, float baseWidth, float baseHeight);
	//更新
	void Update();
	//描画
	void Draw(float centerX, float centerY, float x_scale = 1.0f, float y_scale = 1.0f);

	//終了（いるか？）
	void Finalize() {}

	//押す
	void UI_Push();

};

