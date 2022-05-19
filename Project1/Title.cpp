#include "Title.h"
#include "Raki_imguiMgr.h"
#include "TexManager.h"

#include "InputManger.h"
#include "NY_random.h"

using namespace myImgui;

Title::Title(ISceneChanger* changer) : BaseScene(changer)
{
	titleMainSprite.Create(TexManager::LoadTexture("Resources/titr1.png"));

	cursor.Create(TexManager::LoadTexture("Resources/cursor02.png"));
}

//初期化
void Title::Initialize()
{

}

void Title::Finalize()
{

}

//更新
void Title::Update()
{
	int user_select_index = static_cast<int>(user_selecting);
	if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_CROSS_DOWN) && user_selecting == is_start) { user_select_index++; }
	if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_CROSS_UP) && user_selecting == is_end) { user_select_index--; }
	if (Input::isKeyTrigger(DIK_DOWN) && user_selecting == is_start) { user_select_index++; }
	if (Input::isKeyTrigger(DIK_UP) && user_selecting == is_end) { user_select_index--; }

	user_selecting = static_cast<NOW_SELECTING>(user_select_index);

	switch (user_selecting)
	{
	case Title::is_start:
		if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_A) || Input::isKeyReleased(DIK_RETURN)) { mSceneChanger->ChangeScene(eScene_Game); }

		break;
	case Title::is_end:
		if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_A) || Input::isKeyReleased(DIK_RETURN)) { mSceneChanger->EndAplication(); }

		break;
	default:
		break;
	}



}

//描画
void Title::Draw()
{
	Raki_DX12B::Get()->StartDrawRenderTarget();
	Raki_DX12B::Get()->StartDrawBackbuffer();



	SpriteManager::Get()->SetCommonBeginDraw();
	titleMainSprite.DrawSprite(0, 0);
	titleMainSprite.Draw();

	switch (user_selecting)
	{
	case Title::is_start:
		cursor.DrawSprite(MENUICON_START_X + MENUICON_SIZE_X, MENUICON_START_Y + MENUICON_SIZE_Y);
		break;
	case Title::is_end:
		cursor.DrawSprite(MENUICON_START_X + MENUICON_SIZE_X , MENUICON_START_Y + MENUICON_SIZE_Y * 2 + MENUICON_OFFSET);
		break;
	default:
		break;
	}
	cursor.Draw();

	//描画終了
	Raki_DX12B::Get()->CloseDraw();
}
