#include "StageClearedControler.h"
#include "InputManger.h"
#include "SChangeDir.h"

namespace
{
InputManger* inputManger = InputManger::Get();
}

void StageClearedControler::Init()
{
	//演出初期化
	goalEffect.reset(new GoalEffects);
	goalEffect->Init();

	//UI初期化
	_go_select_button.Init(60, TexManager::LoadTexture("Resources/back.png"), 360, 90);
	x1 = 1280.0f * (1.0f / 4.0f);
	y1 = 720 * (8.0f / 9.0f);
	_go_next_button.Init(60, TexManager::LoadTexture("Resources/next.png"), 200, 90);
	x2 = 1280.0f * (3.0f / 4.0f);
	y2 = 720 * (8.0f / 9.0f);

	clearedBackSprite.Create(TexManager::LoadTexture("Resources/stageClear.png"));

	//入力禁止
	ctrl_state = CONTROL_NOT_ALLOW;
	//最初にユーザーが選んでるやつ
	_user_selecting = USER_SELECT_NEXT;

	frameCount = 0;

	isAllowSwitching = false;

	gameMainSprite.CreateRtexSprite(0);
}

void StageClearedControler::Update()
{
	Update_CheckControlStates();

	Update_ControlMain();

	//選択が確認できたら、演出を切る

}

void StageClearedControler::Draw(int rtHandle)
{
	//背景描画しておく
	clearedBackSprite.DrawExtendSprite(0, 0, 1280, 720);
	clearedBackSprite.Draw();

	//ゲーム本編の画像を描画する
	gameMainSprite.DrawRTexSprite(rtHandle, LT.x, LT.y, RB.x, RB.y, 0);

	if (ctrl_state == CONTROL_NOT_ALLOW) { return; }

	//このクラス内の描画リソースは、操作が禁止されてないときは常に描画される

	//ゴール演出描画
	goalEffect->Draw();

	//UI描画
	Draw_UI();

}

void StageClearedControler::ControlActivate()
{
	if (ctrl_state != CONTROL_NOT_ALLOW) { return; }

	//演出状態に移行
	ctrl_state = CONTROL_DIRECING;

	goalEffect->Play();
}

void StageClearedControler::ControlDisable()
{
	//コントロール無効化
	ctrl_state = CONTROL_NOT_ALLOW;
}

void StageClearedControler::Update_CheckControlStates()
{
	//キャストしてこのタイミングでは整数で扱うようにする
	int user_selecting = static_cast<int>(_user_selecting);

	//レート
	float rate = static_cast<float>(frameCount) / static_cast<float>(DIRECTING_FRAME);
	//描画座標イージング実行
	LT = Rv3Ease::OutQuad(drawLT, targetLT, rate);
	RB = Rv3Ease::OutQuad(drawRB, targetRB, rate);

	//状態遷移の判定を行う
	switch (ctrl_state)
	{
	case StageClearedControler::CONTROL_NOT_ALLOW:
		//特になし（Active関数が呼ばれるまで）
		break;
	case StageClearedControler::CONTROL_DIRECING:
		//規定フレーム経過、または入力を検知
		frameCount++;

		if (frameCount > DIRECTING_FRAME)
		{
			//UI選択有効化
			ctrl_state = CONTROL_ACTIVE;
		}
		if (inputManger->DecisionTrigger())
		{
			frameCount = DIRECTING_FRAME;
		}

		break;
	case StageClearedControler::CONTROL_ACTIVE:
		//UI選択の入力（どっちを選択してるか？）
		if (inputManger->LeftTrigger()) { user_selecting = 0; }
		if (inputManger->RightTrigger()) { user_selecting = 1; }
		_user_selecting = static_cast<STAGE_CLEARED_USER_SELECTING>(user_selecting);

		//UI選択された
		if (inputManger->DecisionTrigger())
		{
			//ボタン押した
			if (user_selecting == 0) { _go_next_button.UI_Push(); }
			if (user_selecting == 1) { _go_select_button.UI_Push(); }

			ctrl_state = CONTROL_UI_SELECTED;
			SChangeDir::Get()->PlayChangingDirection();
		}

		break;

	case StageClearedControler::CONTROL_UI_SELECTED:

		break;

	default:
		break;
	}
}

void StageClearedControler::Update_ControlMain()
{
	switch (ctrl_state)
	{
	case StageClearedControler::CONTROL_NOT_ALLOW:
		//何もしない
		break;

	case StageClearedControler::CONTROL_DIRECING:
		//イージングするならここに



		break;

	case StageClearedControler::CONTROL_ACTIVE:
		//選択中、入力以外更新


		break;

	case StageClearedControler::CONTROL_UI_SELECTED:
		//UI選択後更新処理

		if (SChangeDir::Get()->isChangeActivate) {
			isAllowSwitching = true;
		}

		break;

	default:
		break;
	}

	//UI更新
	_go_next_button.Update();
	_go_select_button.Update();

}

void StageClearedControler::Draw_UI()
{
	switch (_user_selecting)
	{
	case StageClearedControler::USER_SELECT_NEXT:
		_go_next_button.Draw(x1, y1, SELECTING_SCALE, SELECTING_SCALE);
		_go_select_button.Draw(x2, y2);
		break;
	case StageClearedControler::USER_SELECT_BACK:
		_go_next_button.Draw(x1, y1);
		_go_select_button.Draw(x2, y2, SELECTING_SCALE, SELECTING_SCALE);
		break;
	default:
		break;
	}
}