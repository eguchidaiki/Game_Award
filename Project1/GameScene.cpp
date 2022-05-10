#include "GameScene.h"
#include "Raki_imguiMgr.h"
#include "TexManager.h"

#include "InputManger.h"
#include "NY_random.h"

GameScene::GameScene(ISceneChanger *changer) : BaseScene(changer) {
	camera->SetViewStatusEyeTargetUp(eye, target, up);

	stage->Create();
	stage->LoadStage("./Resources/stage/test.csv", playerTile);
	player->Init();
	player->BodySetUp(playerTile);

	nowState = is_Select;

	selecter.Init();
	gamemain.Init();
}

//初期化
void GameScene::Initialize() {

}

//更新
void GameScene::Update() {
	camera->SetViewStatusEyeTargetUp(eye, target, up);


	switch (nowState)
	{
	case GameScene::is_Select:
		//ステージセレクト画面の処理
		selecter.Update();
		if (selecter.GetMoveGameMain()) {
			nowState = is_Game; 
		}

		break;


	case GameScene::is_Game:
		//ゲーム本編の処理
		gamemain.Update();

		break;
	default:
		break;
	}
}

//描画
void GameScene::Draw() {

	//背景に常にいる
	Raki_DX12B::Get()->StartDrawRenderTarget();

	Raki_DX12B::Get()->StartDrawBackbuffer();

	switch (nowState)
	{
	case GameScene::is_Select:
		//ステージセレクト画面の処理
		selecter.Draw();

		break;
	case GameScene::is_Game:
		gamemain.Draw();

		break;
	default:
		break;
	}

	//描画終了
	Raki_DX12B::Get()->CloseDraw();
}