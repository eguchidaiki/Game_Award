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

	titleDirectState = TITLE_DIRECT_USER_SELECT;
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

	switch (titleDirectState)
	{
	case Title::TITLE_DIRECT_STARTING:
		//演出系

		break;
	case Title::TITLE_DIRECT_USER_SELECT:
		if ((inputManger->DownTrigger() || Input::isKeyTrigger(DIK_DOWN)) && user_selecting == is_start) { user_select_index++; }
		if ((inputManger->UpTrigger() || Input::isKeyTrigger(DIK_UP)) && user_selecting == is_end) { user_select_index--; }

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

	titleLogo.DrawExtendSprite(Raki_WinAPI::window_width / 2 - (380 / 2), Raki_WinAPI::window_height / 4 - (380 / 2),
		Raki_WinAPI::window_width / 2 + (380 / 2), Raki_WinAPI::window_height / 4 + (380 / 2));
	titleFont.DrawExtendSprite(Raki_WinAPI::window_width / 2 - (380 / 2), Raki_WinAPI::window_height / 4 - (230 / 2),
		Raki_WinAPI::window_width / 2 + (380 / 2), Raki_WinAPI::window_height / 4 + (230 / 2));

	titleLogo.Draw();
	titleFont.Draw();

	go_next_Button.Draw(MENUICON_START_X + MENUICON_SIZE_X / 2,
		MENUICON_START_Y + MENUICON_SIZE_Y / 2);
	go_end_Button.Draw(MENUICON_START_X + MENUICON_SIZE_X / 2,
		MENUICON_START_Y + (MENUICON_SIZE_Y / 2) * 3 + MENUICON_OFFSET);

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
	cursor.Draw();
}
