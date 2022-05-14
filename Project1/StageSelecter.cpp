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
	//リソース読み込み
	LoadSprite();
	//menuBGM = Audio::LoadSound_wav("Resources/sound/BGM/bgm01.wav");

	//最初のページ
	nowpage = page_1_4;
	nextpage = nowpage;

	nowDisplayNum = 0;
}

void StageSelecter::Update()
{
	//ステージ選択の入力を検知
	CheckToStageChangeInput();
	//ステージ選択のページを切り替える入力を検知
	CheckToPageChangeInput();
	//ページに応じた演出処理
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
//	//�Đ�
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
	//ステージ選択中でないなら処理しない
	if (state != is_selecting)
	{
		return;
	}

	float mouse_x = Input::getMousePos().x;
	float mouse_y = Input::getMousePos().y;

	if (state == is_selecting)
	{
		//ページ遷移するのか？
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
	//ページ切り替え演出の処理

	//ページが変わるとき
	if (nowpage != nextpage)
	{
		animationFrame++;
		//次のページ
		if (pageMoveDir == is_front)
		{
			//演出中は現在ページを表示
			displayPage = nowpage;
			//演出待機
			state = is_pageChange_waiting;
			if (animationFrame % perFrame == 0)
			{
				nowDisplayNum++;
			}
			if (nowDisplayNum >= 20)
			{
				//描画するやつを切り替え
				displayPage = nextpage;
				//現在ページを設定
				nowpage = nextpage;
				//初期化
				nowDisplayNum = 0;
				//入力を受付
				state = is_selecting;

				animationFrame = 0;
			}
		}
		//前のページ
		else
		{
			//ページ遷移判定にするタイミングで下準備をする
			if (state != is_pageChange_waiting)
			{
				state = is_pageChange_waiting;
				nowDisplayNum = 19;
				displayPage = nextpage;
			}
			//アニメーション進行
			if (animationFrame % perFrame == 0)
			{
				nowDisplayNum--;
			}
			//演出終了
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
			stagePtr->LoadStage("./Resources/stage/stage1.csv", playerPtr->playerTile);
			stagePtr->drawOffsetX = 0.0f;
			stagePtr->drawOffsetY = 0.0f;
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
	//選択中以外はそもそも実行しない
	if (state != is_selecting)
	{
		return;
	}

	//各ボックスの左側座標を計算
	std::array<int, 4> boxLeft;
	for (int i = 0; i < 4; i++)
	{
		boxLeft[i] = NUMBOX_START_X + (NUMBOX_SIZE * i) + (NUMBOX_SPACE * i);
	}

	int selectingBoxNum = 0;
	bool isHit = false;
	//クリック時にどのボックスをクリックしているか求める
	if (Input::isMouseClickTrigger(MOUSE_L) == true)
	{
		int mouse_x = static_cast<int>(Input::getMousePos().x);
		int mouse_y = static_cast<int>(Input::getMousePos().y);

		//ステージにヒットしている
		for (int i = 0; i < 4; i++)
		{
			if (mouse_x > boxLeft[i] && mouse_x < boxLeft[i] + NUMBOX_SIZE && mouse_y > NUMBOX_START_Y && mouse_y < NUMBOX_START_Y + NUMBOX_SIZE)
			{
				selectingBoxNum = i;
				//ゲーム本編への遷移を許可
				isChanging_GameMain = true;
				//ヒット
				isHit = true;
				//ステージ遷移開始
				state = is_stageSelected_waiting;
				break;
			}
		}
		//一個もヒットしていない場合は終了
		if (!isHit) { return; }
	}
	else
	{//入力がないなら終了
		return;
	}

	//クリックされているボックスと、現在のページから、どのステージに移動するのかを求める
	if (isHit)
	{
		CheckLoadStage(selectingBoxNum);
		SelectStageNum = selectingBoxNum;
	}

}
