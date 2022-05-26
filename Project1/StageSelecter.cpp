#include "StageSelecter.h"
#include <string>

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

	if (nowpage != StageSelecter::page_1_4)
	{
		SelectLeft.DrawSprite(29, 623);
		SelectLeft.Draw();
	}
	if (nowpage != StageSelecter::page_17_20)
	{
		SelectRight.DrawSprite(1184, 623);
		SelectRight.Draw();
	}

	//float mouse_x = Input::getMousePos().x;
	//float mouse_y = Input::getMousePos().y;

	/*if (mouse_x <= 92 && mouse_x >= 32 && mouse_y <= 686 && mouse_y >= 626)
	{
		SelectLeft.DrawExtendSprite(19, 613, 29 + 77, 623 + 77);
	}
	else
	{
		SelectLeft.DrawSprite(29, 623);
	}
	SelectLeft.Draw();*/

	/*if (mouse_x <= 1248 && mouse_x >= 1188 && mouse_y <= 686 && mouse_y >= 626)
	{
		SelectRight.DrawExtendSprite(1174, 613, 1184 + 77, 623 + 77);
	}
	else
	{
		SelectRight.DrawSprite(1184, 623);
	}
	SelectRight.Draw();*/

	//カーソル描画
	DrawCursor();

}

void StageSelecter::Finalize()
{

}

void StageSelecter::GoNextStage()
{
	if (_nowStageNumber == 20) { return; }

	_nowStageNumber++;

	LoadStage(_nowStageNumber);
}

void StageSelecter::Changing_UI_Number()
{
	//入力によってインクリメント、デクリメント
	int select_number = static_cast<int>(user_selecting);
	if (inputManager->LeftTrigger() || Input::isKeyTrigger(DIK_LEFT)) {
		if (user_selecting != UI_BACK) { select_number--; }
	}

	if (inputManager->RightTrigger() || Input::isKeyTrigger(DIK_RIGHT)) {
		if (user_selecting != UI_FRONT && (nowpage != StageSelecter::page_17_20 || user_selecting != UI_STAGEBOX_4))
		{
			select_number++;
		}
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
	selectCursor.CreateAndSetDivisionUVOffsets(cursorSpriteCount, 4, 1, 50, 50, TexManager::LoadTexture("Resources/UI/Cursor/stageSelect.png"));
	//selectCursor.Create(TexManager::LoadTexture("Resources/UI/Cursor.png"));
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
		if (inputManager->DecisionTrigger() && nowpage != page_1_4) {
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
		if (inputManager->DecisionTrigger() && nowpage != page_17_20) {
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

				user_selecting = StageSelecter::UI_STAGEBOX_1;
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
				user_selecting = StageSelecter::UI_STAGEBOX_4;
			}
		}
	}
}

void StageSelecter::CheckLoadStage(int boxnum)
{
	int pageNumber = static_cast<int>(nowpage);

	int LoadStageNumber = (pageNumber * 4) + (boxnum) + 1;

	SelectStageNum = LoadStageNumber;
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
		if (inputManager->DecisionTrigger()) {
			select_Stage_num = 0;
			selected = true;
		}
		break;
	case StageSelecter::UI_STAGEBOX_2:
		if (inputManager->DecisionTrigger()) {
			select_Stage_num = 1;
			selected = true;
		}
		break;
	case StageSelecter::UI_STAGEBOX_3:
		if (inputManager->DecisionTrigger()) {
			select_Stage_num = 2;
			selected = true;
		}
		break;
	case StageSelecter::UI_STAGEBOX_4:
		if (inputManager->DecisionTrigger()) {
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

	for (int i = 0; i < cursorSpriteCount; i++)
	{
		static float x = 0.0f, y = 0.0f;
		selectCursor.uvOffsetHandle = i;

		switch (user_selecting)
		{
		case StageSelecter::UI_BACK:
			x = PAGEMOVE_LEFT_X + PAGEMOVE_SIZE * (i / 2);
			y = PAGEMOVE_Y + PAGEMOVE_SIZE * (i % 2);
			selectCursor.DrawExtendSprite(x - PAGEMOVE_SIZE / 4.0f, y - PAGEMOVE_SIZE / 4.0f,
										  x + PAGEMOVE_SIZE / 4.0f, y + PAGEMOVE_SIZE / 4.0f);
			break;
		case StageSelecter::UI_STAGEBOX_1:
			x = boxLeft[0] + NUMBOX_SIZE * (i / 2);
			y = NUMBOX_START_Y + NUMBOX_SIZE * (i % 2);
			selectCursor.DrawExtendSprite(x - NUMBOX_SIZE / 4.0f, y - NUMBOX_SIZE / 4.0f,
										  x + NUMBOX_SIZE / 4.0f, y + NUMBOX_SIZE / 4.0f);
			break;
		case StageSelecter::UI_STAGEBOX_2:
			x = boxLeft[1] + NUMBOX_SIZE * (i / 2);
			y = NUMBOX_START_Y + NUMBOX_SIZE * (i % 2);
			selectCursor.DrawExtendSprite(x - NUMBOX_SIZE / 4.0f, y - NUMBOX_SIZE / 4.0f,
										  x + NUMBOX_SIZE / 4.0f, y + NUMBOX_SIZE / 4.0f);
			break;
		case StageSelecter::UI_STAGEBOX_3:
			x = boxLeft[2] + NUMBOX_SIZE * (i / 2);
			y = NUMBOX_START_Y + NUMBOX_SIZE * (i % 2);
			selectCursor.DrawExtendSprite(x - NUMBOX_SIZE / 4.0f, y - NUMBOX_SIZE / 4.0f,
										  x + NUMBOX_SIZE / 4.0f, y + NUMBOX_SIZE / 4.0f);
			break;
		case StageSelecter::UI_STAGEBOX_4:
			x = boxLeft[3] + NUMBOX_SIZE * (i / 2);
			y = NUMBOX_START_Y + NUMBOX_SIZE * (i % 2);
			selectCursor.DrawExtendSprite(x - NUMBOX_SIZE / 4.0f, y - NUMBOX_SIZE / 4.0f,
										  x + NUMBOX_SIZE / 4.0f, y + NUMBOX_SIZE / 4.0f);
			break;
		case StageSelecter::UI_FRONT:
			x = PAGEMOVE_RIGHT_X + PAGEMOVE_SIZE * (i / 2);
			y = PAGEMOVE_Y + PAGEMOVE_SIZE * (i % 2);
			selectCursor.DrawExtendSprite(x - PAGEMOVE_SIZE / 4.0f, y - PAGEMOVE_SIZE / 4.0f,
										  x + PAGEMOVE_SIZE / 4.0f, y + PAGEMOVE_SIZE / 4.0f);
			break;
		default:
			break;
		}
	}

	selectCursor.Draw();
}

void StageSelecter::LoadStage(int stagenum)
{
	string stageNumber = std::to_string(stagenum);

	string stageFilePath = "./Resources/stage/stage";
	string filename = ".csv";

	string stageFullPath = stageFilePath + stageNumber + filename;

	stagePtr->LoadStage(stageFullPath.c_str(), playerPtr->playerTile);
	playerPtr->Init();
	playerPtr->BodySetUp(playerPtr->playerTile);
}
