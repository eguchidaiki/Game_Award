#include "Title.h"
#include "Raki_imguiMgr.h"
#include "TexManager.h"

#include "InputManger.h"
#include "NY_random.h"
#include "Raki_WinAPI.h" //無くても動く(どこでインクルードされてるか分からなかった)

namespace
{
static InputManger* inputManger = InputManger::Get();
}

using namespace myImgui;

Title::Title(ISceneChanger* changer) : BaseScene(changer)
{
	user_selecting = NOW_SELECTING::is_start;

	titleMainSprite.Create(TexManager::LoadTexture("Resources/title.png"));

	cursor.Create(TexManager::LoadTexture("Resources/wakuF01.png"));
}

//初期化
void Title::Initialize()
{
	SChangeDir::Get()->PlayChangedDirection();
}

void Title::Finalize()
{

}

//更新
void Title::Update()
{
	int user_select_index = static_cast<int>(user_selecting);
	if ((inputManger->DownTrigger() || Input::isKeyTrigger(DIK_DOWN)) && user_selecting == is_start) { user_select_index++; }
	if ((inputManger->UpTrigger() || Input::isKeyTrigger(DIK_UP)) && user_selecting == is_end) { user_select_index--; }

	user_selecting = static_cast<NOW_SELECTING>(user_select_index);

	switch (user_selecting)
	{
	case Title::is_start:
		if (inputManger->DecisionTrigger()) { 
			SChangeDir::Get()->PlayChangingDirection();
		}
		if (SChangeDir::Get()->isChangeActivate) {
			mSceneChanger->ChangeScene(eScene_Game);
		}

		break;
	case Title::is_end:
		if (inputManger->DecisionTrigger()) { mSceneChanger->EndAplication(); }

		break;
	default:
		break;
	}



}

//描画
void Title::Draw()
{
	SpriteManager::Get()->SetCommonBeginDraw();
	titleMainSprite.DrawSprite(0, 0);
	titleMainSprite.Draw();

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
