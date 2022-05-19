#include "StageSelecter.h"
#include <string>

#include <Raki_WinAPI.h>
#include <Raki_imguiMgr.h>

#include "Stage.h"
#include "Player.h"
#include "InputManger.h"

namespace
{
	static InputManger* inputManager = InputManger::Get();
	Stage* stagePtr = Stage::Get();
	Player* playerPtr = Player::Get();
}

StageSelecter::StageSelecter()
{

}

StageSelecter::~StageSelecter()
{
}

void StageSelecter::Init()
{
	//繝ｪ繧ｽ繝ｼ繧ｹ隱ｭ縺ｿ霎ｼ縺ｿ
	LoadSprite();
	//menuBGM = Audio::LoadSound_wav("Resources/sound/BGM/bgm01.wav");

	//譛蛻昴・繝壹・繧ｸ
	nowpage = page_1_4;
	nextpage = nowpage;

	//ステージ1を選択中
	user_selecting = UI_STAGEBOX_1;

	nowDisplayNum = 0;
}

void StageSelecter::Update()
{
	//繧ｹ繝・・繧ｸ驕ｸ謚槭・蜈･蜉帙ｒ讀懃衍
	CheckToStageChangeInput();
	//繧ｹ繝・・繧ｸ驕ｸ謚槭・繝壹・繧ｸ繧貞・繧頑崛縺医ｋ蜈･蜉帙ｒ讀懃衍
	CheckToPageChangeInput();
	//繝壹・繧ｸ縺ｫ蠢懊§縺滓ｼ泌・蜃ｦ逅・
	PageChange();


}

void StageSelecter::Draw()
{
	switch (displayPage)
	{
	case StageSelecter::page_1_4:
		selectImg_1_4[nowDisplayNum].DrawSprite(0, 0);
		selectImg_1_4[nowDisplayNum].Draw();
		break;
	case StageSelecter::page_5_8:
		selectImg_5_8[nowDisplayNum].DrawSprite(0, 0);
		selectImg_5_8[nowDisplayNum].Draw();
		break;
	case StageSelecter::page_9_12:
		selectImg_9_12[nowDisplayNum].DrawSprite(0, 0);
		selectImg_9_12[nowDisplayNum].Draw();
		break;
	case StageSelecter::page_13_16:
		selectImg_13_16[nowDisplayNum].DrawSprite(0, 0);
		selectImg_13_16[nowDisplayNum].Draw();
		break;

	case StageSelecter::page_17_20:
		selectImg_13_16[nowDisplayNum].DrawSprite(0, 0);
		selectImg_13_16[nowDisplayNum].Draw();
	default:
		break;
	}

	/*ImguiMgr::Get()->StartDrawImgui("mouse", 150, 100);
	ImGui::Text("pos x : %f    y : %f", Input::getMousePos().x, Input::getMousePos().y);
	ImguiMgr::Get()->EndDrawImgui();*/

	SelectLeft.DrawSprite(29, 623);
	SelectLeft.Draw();
	SelectRight.DrawSprite(1184, 623);
	SelectRight.Draw();

	float mouse_x = Input::getMousePos().x;
	float mouse_y = Input::getMousePos().y;

	if (mouse_x <= 92 && mouse_x >= 32 && mouse_y <= 686 && mouse_y >= 626)
	{
		SelectLeft.DrawExtendSprite(19, 613, 29 + 77, 623 + 77);
	}
	else
	{
		SelectLeft.DrawSprite(29, 623);
	}
	SelectLeft.Draw();

	if (mouse_x <= 1248 && mouse_x >= 1188 && mouse_y <= 686 && mouse_y >= 626)
	{
		SelectRight.DrawExtendSprite(1174, 613, 1184 + 77, 623 + 77);
	}
	else
	{
		SelectRight.DrawSprite(1184, 623);
	}
	SelectRight.Draw();

	//カーソル描画
	DrawCursor();

}

void StageSelecter::Finalize()
{

}

void StageSelecter::Changing_UI_Number()
{
	//入力によってインクリメント、デクリメント
	int select_number = static_cast<int>(user_selecting);
	if (Input::isXpadStickTilt(XPAD_LSTICK_DIR_LEFT) || 
		Input::isXpadButtonPushTrigger(XPAD_BUTTON_CROSS_LEFT) || 
		Input::isKeyTrigger(DIK_LEFT)) {
		if (user_selecting != UI_BACK) { select_number--; }
	}

	if (Input::isXpadStickTilt(XPAD_LSTICK_DIR_RIGHT) || 
		Input::isXpadButtonPushTrigger(XPAD_BUTTON_CROSS_RIGHT) ||
		Input::isKeyTrigger(DIK_RIGHT)) {
		if (user_selecting != UI_FRONT) { select_number++; }
	}
	user_selecting = static_cast<NOW_SELECTING>(select_number);
}

void StageSelecter::LoadSprite()
{
	std::string fullImgPath = "Resources/selectAnime/";
	std::string filename = ".png";

	std::string page_1_4 = "1-4/";
	std::string page_5_8 = "5-8/";
	std::string page_9_12 = "9-12/";
	std::string page_13_16 = "13-16/";

	std::string imageName_1_4 = "select1-4 ";
	std::string imageName_5_8 = "select5-8 ";
	std::string imageName_9_12 = "select9-12 ";
	std::string imageName_13_16 = "select13-16 ";

	for (int i = 0; i < 20; i++)
	{
		std::string number = std::to_string(i + 1);
		std::string fullnumber = "(" + number + ")";

		std::string fullpath_1_4 = fullImgPath + page_1_4 + imageName_1_4 + fullnumber + filename;
		std::string fullpath_5_8 = fullImgPath + page_5_8 + imageName_5_8 + fullnumber + filename;
		std::string fullpath_9_12 = fullImgPath + page_9_12 + imageName_9_12 + fullnumber + filename;
		std::string fullpath_13_16 = fullImgPath + page_13_16 + imageName_13_16 + fullnumber + filename;

		selectImg_1_4[i].Create(TexManager::LoadTexture(fullpath_1_4));
		selectImg_5_8[i].Create(TexManager::LoadTexture(fullpath_5_8));
		selectImg_9_12[i].Create(TexManager::LoadTexture(fullpath_9_12));
		selectImg_13_16[i].Create(TexManager::LoadTexture(fullpath_13_16));
	}

	selectCursor.Create(TexManager::LoadTexture("Resources/cursor02.png"));
	SelectLeft.Create(TexManager::LoadTexture(fullImgPath + "SelectLeft" + filename));
	SelectRight.Create(TexManager::LoadTexture(fullImgPath + "SelectRight" + filename));
}

void StageSelecter::CheckToPageChangeInput()
{
	//繧ｹ繝・・繧ｸ驕ｸ謚樔ｸｭ縺ｧ縺ｪ縺・↑繧牙・逅・＠縺ｪ縺・
	if (state != is_selecting)
	{
		return;
	}

	//入力に応じてUI番号変更
	Changing_UI_Number();

	switch (user_selecting)
	{
	case StageSelecter::UI_BACK:
		//最初のページでないときにBACK
		if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_A) && nowpage != page_1_4) {
			//移動方向設定
			pageMoveDir = is_back;
			//次ページ設定
			int pageNum = static_cast<int>(nextpage);
			pageNum--;
			nextpage = static_cast<STAGE_PAGE>(pageNum);
		}
		else if (Input::isKeyTrigger(DIK_RETURN) && nowpage != page_1_4) {
			//移動方向設定
			pageMoveDir = is_back;
			//次ページ設定
			int pageNum = static_cast<int>(nextpage);
			pageNum--;
			nextpage = static_cast<STAGE_PAGE>(pageNum);
		}

		break;
	case StageSelecter::UI_FRONT:
		//最後のページでないときにFRONT
		if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_A) && nowpage != page_17_20) {
			//移動方向設定
			pageMoveDir = is_front;
			//次ページ設定
			int pageNum = static_cast<int>(nextpage);
			pageNum++;
			nextpage = static_cast<STAGE_PAGE>(pageNum);
		}
		else if (Input::isKeyTrigger(DIK_RETURN) && nowpage != page_17_20) {
			//移動方向設定
			pageMoveDir = is_front;
			//次ページ設定
			int pageNum = static_cast<int>(nextpage);
			pageNum++;
			nextpage = static_cast<STAGE_PAGE>(pageNum);
		}

		break;
	default:
		break;
	}

}

void StageSelecter::PageChange()
{

	if (nowpage != nextpage)
	{
		animationFrame++;

		if (pageMoveDir == is_front)
		{

			displayPage = nowpage;

			state = is_pageChange_waiting;
			if (animationFrame % perFrame == 0)
			{
				nowDisplayNum++;
			}
			if (nowDisplayNum >= 20)
			{

				displayPage = nextpage;

				nowpage = nextpage;

				nowDisplayNum = 0;
				//最後のページのときは最終フレームで固定
				if (displayPage == STAGE_PAGE::page_17_20) {
					nowDisplayNum = 19;
				}

				state = is_selecting;

				animationFrame = 0;
			}
		}
		else
		{
			if (state != is_pageChange_waiting)
			{
				state = is_pageChange_waiting;
				nowDisplayNum = 19;
				displayPage = nextpage;
			}

			if (animationFrame % perFrame == 0)
			{
				nowDisplayNum--;
			}

			if (nowDisplayNum <= 0)
			{
				nowDisplayNum = 0;
				state = is_selecting;
				nowpage = nextpage;
				animationFrame = 0;
			}
		}
	}
}

void StageSelecter::CheckLoadStage(int boxnum)
{
	switch (nowpage)
	{
	case StageSelecter::page_1_4:
		if (boxnum == 0)
		{
			//stagePtr->LoadStage("./Resources/stage/stage1_test.csv", playerPtr->playerTile);
			stagePtr->LoadStage("./Resources/stage/stage1.csv", playerPtr->playerTile);
			stagePtr->drawOffsetX = Raki_WinAPI::window_width / 2;
			stagePtr->drawOffsetY = Raki_WinAPI::window_height / 2;
			playerPtr->Init();
			playerPtr->BodySetUp(playerPtr->playerTile);
		}
		else if (boxnum == 1)
		{
			stagePtr->LoadStage("./Resources/stage/stage2.csv", playerPtr->playerTile);
			stagePtr->drawOffsetX = 500.0f;
			stagePtr->drawOffsetY = 75.0f;
			playerPtr->Init();
			playerPtr->BodySetUp(playerPtr->playerTile);
		}
		else if (boxnum == 2)
		{
			stagePtr->LoadStage("./Resources/stage/stage3.csv", playerPtr->playerTile);
			stagePtr->drawOffsetX = 0.0f;
			stagePtr->drawOffsetY = 0.0f;
			playerPtr->Init();
			playerPtr->BodySetUp(playerPtr->playerTile);
		}
		else
		{
			stagePtr->LoadStage("./Resources/stage/stage4.csv", playerPtr->playerTile);
			stagePtr->drawOffsetX = 0.0f;
			stagePtr->drawOffsetY = 0.0f;
			playerPtr->Init();
			playerPtr->BodySetUp(playerPtr->playerTile);
		}
		break;


	case StageSelecter::page_5_8:
		if (boxnum == 0)
		{
			stagePtr->LoadStage("./Resources/stage/stage5.csv", playerPtr->playerTile);
			stagePtr->drawOffsetX = 0.0f;
			stagePtr->drawOffsetY = 0.0f;
			playerPtr->Init();
			playerPtr->BodySetUp(playerPtr->playerTile);
		}
		else if (boxnum == 1)
		{
			stagePtr->LoadStage("./Resources/stage/stage6.csv", playerPtr->playerTile);
			stagePtr->drawOffsetX = 0.0f;
			stagePtr->drawOffsetY = 0.0f;
			playerPtr->Init();
			playerPtr->BodySetUp(playerPtr->playerTile);
		}
		else if (boxnum == 2)
		{
			stagePtr->LoadStage("./Resources/stage/stage7.csv", playerPtr->playerTile);
			stagePtr->drawOffsetX = 0.0f;
			stagePtr->drawOffsetY = 0.0f;
			playerPtr->Init();
			playerPtr->BodySetUp(playerPtr->playerTile);
		}
		else
		{
			stagePtr->LoadStage("./Resources/stage/stage8.csv", playerPtr->playerTile);
			stagePtr->drawOffsetX = 0.0f;
			stagePtr->drawOffsetY = 0.0f;
			playerPtr->Init();
			playerPtr->BodySetUp(playerPtr->playerTile);
		}
		break;



	case StageSelecter::page_9_12:
		if (boxnum == 0)
		{
			stagePtr->LoadStage("./Resources/stage/stage9.csv", playerPtr->playerTile);
			stagePtr->drawOffsetX = 0.0f;
			stagePtr->drawOffsetY = 0.0f;
			playerPtr->Init();
			playerPtr->BodySetUp(playerPtr->playerTile);
		}
		else if (boxnum == 1)
		{
			stagePtr->LoadStage("./Resources/stage/stage10.csv", playerPtr->playerTile);
			stagePtr->drawOffsetX = 0.0f;
			stagePtr->drawOffsetY = 0.0f;
			playerPtr->Init();
			playerPtr->BodySetUp(playerPtr->playerTile);
		}
		else if (boxnum == 2)
		{
			stagePtr->LoadStage("./Resources/stage/stage11.csv", playerPtr->playerTile);
			stagePtr->drawOffsetX = 0.0f;
			stagePtr->drawOffsetY = 0.0f;
			playerPtr->Init();
			playerPtr->BodySetUp(playerPtr->playerTile);
		}
		else
		{
			stagePtr->LoadStage("./Resources/stage/stage12.csv", playerPtr->playerTile);
			stagePtr->drawOffsetX = 0.0f;
			stagePtr->drawOffsetY = 0.0f;
			playerPtr->Init();
			playerPtr->BodySetUp(playerPtr->playerTile);
		}
		break;


	default:
		break;
	}

}

void StageSelecter::CheckToStageChangeInput()
{
	if (state != is_selecting)
	{
		return;
	}

	bool selected = false;
	int select_Stage_num = 0;
	switch (user_selecting)
	{
	case StageSelecter::UI_STAGEBOX_1:
		if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_A) || Input::isKeyReleased(DIK_RETURN)) {
			select_Stage_num = 0;
			selected = true;
		}
		break;
	case StageSelecter::UI_STAGEBOX_2:
		if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_A) || Input::isKeyReleased(DIK_RETURN)) {
			select_Stage_num = 1;
			selected = true;
		}
		break;
	case StageSelecter::UI_STAGEBOX_3:
		if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_A) || Input::isKeyReleased(DIK_RETURN)) {
			select_Stage_num = 2;
			selected = true;
		}
		break;
	case StageSelecter::UI_STAGEBOX_4:
		if (Input::isXpadButtonPushTrigger(XPAD_BUTTON_A) || Input::isKeyReleased(DIK_RETURN)) {
			select_Stage_num = 3;
			selected = true;
		}
		break;
	default:
		break;
	}
	if (selected) { 
		CheckLoadStage(select_Stage_num);
		state = is_stageSelected_waiting; 

		//これでステージ開始
		isChanging_GameMain = true;
	}

}

void StageSelecter::DrawCursor()
{
	std::array<int, 4> boxLeft;
	for (int i = 0; i < 4; i++)
	{
		boxLeft[i] = NUMBOX_START_X + (NUMBOX_SIZE * i) + (NUMBOX_SPACE * i);
	}

	switch (user_selecting)
	{
	case StageSelecter::UI_BACK:
		selectCursor.DrawSprite(29 + 67, 623 + 67);
		break;
	case StageSelecter::UI_STAGEBOX_1:
		selectCursor.DrawSprite(boxLeft[0] + NUMBOX_SIZE, NUMBOX_START_Y + NUMBOX_SIZE);
		break;
	case StageSelecter::UI_STAGEBOX_2:
		selectCursor.DrawSprite(boxLeft[1] + NUMBOX_SIZE, NUMBOX_START_Y + NUMBOX_SIZE);
		break;
	case StageSelecter::UI_STAGEBOX_3:
		selectCursor.DrawSprite(boxLeft[2] + NUMBOX_SIZE, NUMBOX_START_Y + NUMBOX_SIZE);
		break;
	case StageSelecter::UI_STAGEBOX_4:
		selectCursor.DrawSprite(boxLeft[3] + NUMBOX_SIZE, NUMBOX_START_Y + NUMBOX_SIZE);
		break;
	case StageSelecter::UI_FRONT:
		selectCursor.DrawSprite(1184 + 67, 623 + 67);
		break;
	default:
		break;
	}

	selectCursor.Draw();

}
