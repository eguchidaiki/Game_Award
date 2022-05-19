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

}

void StageSelecter::Finalize()
{
	/*Audio::StopLoadedSound(menuBGM);*/
}

//void StageSelecter::PlayAudio()
//{
//	//再生
//	//Audio::PlayLoadedSound(menuBGM);
//}

void StageSelecter::LoadSprite()
{
	std::string fullImgPath = "Resources/selectAnime/";
	std::string filename = ".png";

	std::string page_1_4 = "1-4/";
	std::string page_5_8 = "5-8/";
	std::string page_9_12 = "9-12/";

	std::string imageName_1_4 = "select1-4 ";
	std::string imageName_5_8 = "select5-8 ";
	std::string imageName_9_12 = "select9-12 ";

	for (int i = 0; i < 20; i++)
	{
		std::string number = std::to_string(i + 1);
		std::string fullnumber = "(" + number + ")";

		std::string fullpath_1_4 = fullImgPath + page_1_4 + imageName_1_4 + fullnumber + filename;
		std::string fullpath_5_8 = fullImgPath + page_5_8 + imageName_5_8 + fullnumber + filename;
		std::string fullpath_9_12 = fullImgPath + page_9_12 + imageName_9_12 + fullnumber + filename;

		selectImg_1_4[i].Create(TexManager::LoadTexture(fullpath_1_4));
		selectImg_5_8[i].Create(TexManager::LoadTexture(fullpath_5_8));
		selectImg_9_12[i].Create(TexManager::LoadTexture(fullpath_9_12));
	}

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

	float mouse_x = Input::getMousePos().x;
	float mouse_y = Input::getMousePos().y;

	if (state == is_selecting)
	{
		//繝壹・繧ｸ驕ｷ遘ｻ縺吶ｋ縺ｮ縺具ｼ・
		bool isMove = false;
		if (mouse_x <= 1248 && mouse_x >= 1188 && mouse_y <= 686 && mouse_y >= 626
			&& Input::isMouseClickTrigger(MOUSE_L))
		{
			switch (nowpage)
			{
			case StageSelecter::page_1_4:
				nextpage = page_5_8;
				isMove = true;
				break;
			case StageSelecter::page_5_8:
				nextpage = page_9_12;
				isMove = true;
				break;
			case StageSelecter::page_9_12:
				break;
			default:
				break;
			}
			if (isMove) { pageMoveDir = is_front; }

		}
		else if (mouse_x <= 92 && mouse_x >= 32 && mouse_y <= 686 && mouse_y >= 626
			&& Input::isMouseClickTrigger(MOUSE_L))
		{
			switch (nowpage)
			{
			case StageSelecter::page_1_4:
				break;
			case StageSelecter::page_5_8:
				nextpage = page_1_4;
				isMove = true;
				break;
			case StageSelecter::page_9_12:
				nextpage = page_5_8;
				isMove = true;
				break;
			default:
				break;
			}
			if (isMove) { pageMoveDir = is_back; }
		}
	}

}

void StageSelecter::PageChange()
{
	//繝壹・繧ｸ蛻・ｊ譖ｿ縺域ｼ泌・縺ｮ蜃ｦ逅・

	//繝壹・繧ｸ縺悟､峨ｏ繧九→縺・
	if (nowpage != nextpage)
	{
		animationFrame++;
		//谺｡縺ｮ繝壹・繧ｸ
		if (pageMoveDir == is_front)
		{
			//貍泌・荳ｭ縺ｯ迴ｾ蝨ｨ繝壹・繧ｸ繧定｡ｨ遉ｺ
			displayPage = nowpage;
			//貍泌・蠕・ｩ・
			state = is_pageChange_waiting;
			if (animationFrame % perFrame == 0)
			{
				nowDisplayNum++;
			}
			if (nowDisplayNum >= 20)
			{
				//謠冗判縺吶ｋ繧・▽繧貞・繧頑崛縺・
				displayPage = nextpage;
				//迴ｾ蝨ｨ繝壹・繧ｸ繧定ｨｭ螳・
				nowpage = nextpage;
				//蛻晄悄蛹・
				nowDisplayNum = 0;
				//蜈･蜉帙ｒ蜿嶺ｻ・
				state = is_selecting;

				animationFrame = 0;
			}
		}
		//蜑阪・繝壹・繧ｸ
		else
		{
			//繝壹・繧ｸ驕ｷ遘ｻ蛻､螳壹↓縺吶ｋ繧ｿ繧､繝溘Φ繧ｰ縺ｧ荳区ｺ門ｙ繧偵☆繧・
			if (state != is_pageChange_waiting)
			{
				state = is_pageChange_waiting;
				nowDisplayNum = 19;
				displayPage = nextpage;
			}
			//繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ騾ｲ陦・
			if (animationFrame % perFrame == 0)
			{
				nowDisplayNum--;
			}
			//貍泌・邨ゆｺ・
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
	//驕ｸ謚樔ｸｭ莉･螟悶・縺昴ｂ縺昴ｂ螳溯｡後＠縺ｪ縺・
	if (state != is_selecting)
	{
		return;
	}

	//蜷・・繝・け繧ｹ縺ｮ蟾ｦ蛛ｴ蠎ｧ讓吶ｒ險育ｮ・
	std::array<int, 4> boxLeft;
	for (int i = 0; i < 4; i++)
	{
		boxLeft[i] = NUMBOX_START_X + (NUMBOX_SIZE * i) + (NUMBOX_SPACE * i);
	}

	int selectingBoxNum = 0;
	bool isHit = false;
	//繧ｯ繝ｪ繝・け譎ゅ↓縺ｩ縺ｮ繝懊ャ繧ｯ繧ｹ繧偵け繝ｪ繝・け縺励※縺・ｋ縺区ｱゅａ繧・
	if (Input::isMouseClickTrigger(MOUSE_L) == true)
	{
		int mouse_x = static_cast<int>(Input::getMousePos().x);
		int mouse_y = static_cast<int>(Input::getMousePos().y);

		//繧ｹ繝・・繧ｸ縺ｫ繝偵ャ繝医＠縺ｦ縺・ｋ
		for (int i = 0; i < 4; i++)
		{
			if (mouse_x > boxLeft[i] && mouse_x < boxLeft[i] + NUMBOX_SIZE && mouse_y > NUMBOX_START_Y && mouse_y < NUMBOX_START_Y + NUMBOX_SIZE)
			{
				selectingBoxNum = i;
				//繧ｲ繝ｼ繝譛ｬ邱ｨ縺ｸ縺ｮ驕ｷ遘ｻ繧定ｨｱ蜿ｯ
				isChanging_GameMain = true;
				//繝偵ャ繝・
				isHit = true;
				//繧ｹ繝・・繧ｸ驕ｷ遘ｻ髢句ｧ・
				state = is_stageSelected_waiting;
				break;
			}
		}
		//荳蛟九ｂ繝偵ャ繝医＠縺ｦ縺・↑縺・ｴ蜷医・邨ゆｺ・
		if (!isHit) { return; }
	}
	else
	{//蜈･蜉帙′縺ｪ縺・↑繧臥ｵゆｺ・
		return;
	}

	//繧ｯ繝ｪ繝・け縺輔ｌ縺ｦ縺・ｋ繝懊ャ繧ｯ繧ｹ縺ｨ縲∫樟蝨ｨ縺ｮ繝壹・繧ｸ縺九ｉ縲√←縺ｮ繧ｹ繝・・繧ｸ縺ｫ遘ｻ蜍輔☆繧九・縺九ｒ豎ゅａ繧・
	if (isHit)
	{
		CheckLoadStage(selectingBoxNum);
		SelectStageNum = selectingBoxNum;
	}

}
