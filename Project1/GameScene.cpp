#include "GameScene.h"
#include "Raki_imguiMgr.h"
#include "TexManager.h"

#include "InputManger.h"
#include "NY_random.h"
#include "SChangeDir.h"

GameScene::GameScene(ISceneChanger* changer) : BaseScene(changer)
{
	camera->SetViewStatusEyeTargetUp(eye, target, up);

	stage->Create();
	stage->LoadStage("./Resources/stage/stage1.csv", player->playerTile);
	player->Create();
	player->Init();
	player->BodySetUp(player->playerTile);

	nowState = is_Select;

	selecter.reset(new StageSelecter);
	selecter->Init();
	gamemain.reset(new GameMainManager);
	gamemain->selecterPtr = selecter.get();
	gamemain->Init();

	SChangeDir::Get()->PlayChangedDirection();
}

//初期化
void GameScene::Initialize()
{

}

//更新
void GameScene::Update()
{
	camera->SetViewStatusEyeTargetUp(eye, target, up);

	switch (nowState)
	{
	case GameScene::is_Select:
		//ステージセレクト画面の処理
		selecter->Update();
		if (selecter->GetMoveGameMain() && !SChangeDir::Get()->isChangeingDirecting && !SChangeDir::Get()->isChangeActivate)
		{
			SChangeDir::Get()->PlayChangingDirection();
		}
		if (SChangeDir::Get()->isChangeActivate) {
			gamemain->SetSelectToGame(selecter->SelectStageNum);
			selecter->isChanging_GameMain = false;
			//セレクター終了
			selecter->Finalize();
			nowState = is_Game;
			InputManger::isMenu = true;
			SChangeDir::Get()->PlayChangedDirection();
		}

		break;


	case GameScene::is_Game:
		//ゲーム本編の処理
		gamemain->Update();
		if (gamemain->IsGoSelect == true && !SChangeDir::Get()->isChangeingDirecting && !SChangeDir::Get()->isChangeActivate)
		{
			SChangeDir::Get()->PlayChangingDirection();
		}
		if (SChangeDir::Get()->isChangeActivate) {
			gamemain->SetGameToSelect();
			selecter->isChanging_GameMain = false;
			selecter->state = selecter->is_selecting;
			nowState = is_Select;
			InputManger::isMenu = false;
			SChangeDir::Get()->PlayChangedDirection();
		}

		break;
	default:
		break;
	}
}

//描画
void GameScene::Draw()
{

	switch (nowState)
	{
	case GameScene::is_Select:
		//ステージセレクト画面の処理
		selecter->Draw();

		break;
	case GameScene::is_Game:
		gamemain->Draw();

		break;
	default:
		break;
	}

}