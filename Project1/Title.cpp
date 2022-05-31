#include "Title.h"
#include "Raki_imguiMgr.h"
#include "TexManager.h"

#include "InputManger.h"
#include "NY_random.h"
#include "Raki_WinAPI.h" //無くても動く(どこでインクルードされてるか分からなかった)
#include "TitleAudio.h"

namespace
{
static InputManger* inputManger = InputManger::Get();
}

using namespace myImgui;

Title::Title(ISceneChanger* changer) : BaseScene(changer)
{
	user_selecting = NOW_SELECTING::is_start;

	//titleMainSprite.Create(TexManager::LoadTexture("Resources/titleResource/title.png"));

	cursor.Create(TexManager::LoadTexture("Resources/wakuF01.png"));

	titleBackScreen.Create(TexManager::LoadTexture("Resources/titleResource/titleBack.png"));
	titleLogo.Create(TexManager::LoadTexture("Resources/titleResource/titleUI03.png"));
	titleFont.Create(TexManager::LoadTexture("Resources/titleResource/titleUI02.png"));

	go_next_Button.Init(60, TexManager::LoadTexture("Resources/titleResource/titleUI04.png"),
		MENUICON_SIZE_X, MENUICON_SIZE_Y);
	go_end_Button.Init(60, TexManager::LoadTexture("Resources/titleResource/titleUI05.png"),
		MENUICON_SIZE_X, MENUICON_SIZE_Y);

	titleDirectState = TITLE_DIRECT_STARTING;

	uiMoveSound = Audio::LoadSound_wav("Resources/sound/SE/mouse02.wav");
}

//初期化
void Title::Initialize()
{
	SChangeDir::Get()->PlayChangedDirection();
	TitleAudio::Get()->Init();
	TitleAudio::Get()->Play();
}

void Title::Finalize()
{

}

//更新
void Title::Update()
{
	int user_select_index = static_cast<int>(user_selecting);

	float title_rate = static_cast<float>(frame) / static_cast<float>(TITLE_ALPHA_EASE_TIME);
	float button_rate = static_cast<float>(frame - TITLE_ALPHA_EASE_TIME) / static_cast<float>(BUTTON_EASE_TIME);

	switch (titleDirectState)
	{
	case Title::TITLE_DIRECT_STARTING:
		frame++;
		//演出系
		if (frame < TITLE_ALPHA_EASE_TIME) {
			//タイトルロゴ座標、カラーを補完
			titleAlpha = S_ALPHA + (E_ALPHA - S_ALPHA) * title_rate;
			title_lt = Rv3Ease::lerp(title_s_lt, title_e_lt, title_rate);
		}
		//ボタンの補完
		else {
			//補完
			buttonAlpha = S_ALPHA + (E_ALPHA - S_ALPHA) * button_rate;
			ui_lt = Rv3Ease::lerp(s_lt, e_lt, button_rate);
		}

		//ユーザー選択モード移行
		if (frame > TITLE_ALPHA_EASE_TIME + BUTTON_EASE_TIME) {
			titleDirectState = TITLE_DIRECT_USER_SELECT;
		}
		if (InputManger::Get()->DecisionTrigger()) {
			frame = TITLE_ALPHA_EASE_TIME + BUTTON_EASE_TIME;
		}

		break;
	case Title::TITLE_DIRECT_USER_SELECT:
		if ((inputManger->DownTrigger() || Input::isKeyTrigger(DIK_DOWN)) && user_selecting == is_start) { 
			user_select_index++;
			Audio::PlayLoadedSound(uiMoveSound, true);
		}
		if ((inputManger->UpTrigger() || Input::isKeyTrigger(DIK_UP)) && user_selecting == is_end) { 
			user_select_index--; 
			Audio::PlayLoadedSound(uiMoveSound, true);
		}

		user_selecting = static_cast<NOW_SELECTING>(user_select_index);

		switch (user_selecting)
		{
		case Title::is_start:
			if (inputManger->DecisionTrigger()) {
				titleDirectState = TITLE_DIRECT_ENDING;
				go_next_Button.UI_Push();
				SChangeDir::Get()->PlayChangingDirection();
			}

			break;
		case Title::is_end:
			if (inputManger->DecisionTrigger()) { 
				titleDirectState = TITLE_DIRECT_ENDING;
				go_end_Button.UI_Push();
			}

			break;
		default:
			break;
		}

		break;
	case Title::TITLE_DIRECT_ENDING:
		//演出終了まで待機、終了後該当シーンへ移動
		if (SChangeDir::Get()->isChangeActivate && go_next_Button.isFunctionActivate) {
			mSceneChanger->ChangeScene(eScene_Game);
		}
		if (go_end_Button.isFunctionActivate) {
			mSceneChanger->EndAplication();
		}

		break;
	default:
		break;
	}

	go_next_Button.Update();
	go_end_Button.Update();




}

//描画
void Title::Draw()
{
	SpriteManager::Get()->SetCommonBeginDraw();

	titleBackScreen.DrawSprite(0, 0);
	titleBackScreen.Draw();

	//ロゴ表示
	Sprite::SetSpriteColorParam(1, 1, 1, titleAlpha);
	titleLogo.DrawExtendSprite(title_lt.x, title_lt.y,
		title_lt.x + title_width, title_lt.y + title_height);
	titleFont.DrawExtendSprite(title_lt.x, title_lt.y + title_height * 0.2,
		title_lt.x + title_width, title_lt.y + title_height * 0.8);
	titleLogo.Draw();
	titleFont.Draw();

	Sprite::SetSpriteColorParam(1, 1, 1, buttonAlpha);
	go_next_Button.Draw(ui_lt.x + MENUICON_SIZE_X / 2,
		ui_lt.y + MENUICON_SIZE_Y / 2);
	go_end_Button.Draw(ui_lt.x + MENUICON_SIZE_X / 2,
		ui_lt.y + (MENUICON_SIZE_Y / 2) * 3 + MENUICON_OFFSET);

	switch (user_selecting)
	{
	case Title::is_start:
		cursor.DrawSprite(MENUICON_START_X - 18, MENUICON_START_Y - 15);
		break;
	case Title::is_end:
		cursor.DrawSprite(MENUICON_START_X - 18, MENUICON_START_Y - 15 + MENUICON_SIZE_Y * 1 + MENUICON_OFFSET);
		break;
	default:
		break;
	}

	Sprite::SetSpriteColorParam(1, 1, 1, 1);

	if (titleDirectState == TITLE_DIRECT_USER_SELECT) {
		cursor.Draw();
	}

}
