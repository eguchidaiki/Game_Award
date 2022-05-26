#include "GameMainManager.h"
#include "InputManger.h"
#include "Raki_DX12B.h"
#include "Stage.h"
#include "Player.h"

namespace
{
	Stage* stage = Stage::Get();
	Player* player = Player::Get();
}

GameMainManager::GameMainManager()
{
}

GameMainManager::~GameMainManager()
{
}

void GameMainManager::Init()
{
	BackHandle = TexManager::LoadTexture("Resources/vvgkh4.png");
	//BackHandle = TexManager::LoadTexture("Resources/backSin.png");
	this->Back.Create(BackHandle);

	menuBGM = Audio::LoadSound_wav("Resources/sound/BGM/bgm01.wav");
	playBGM = Audio::LoadSound_wav("Resources/sound/BGM/bgm02.wav");

	ui.Init(&tutorial);

	tutorial.Create();

	stageClearCtrl.Init();
}

void GameMainManager::Update()
{
	//ゲーム内インスタンスの更新処理（ようは俺が作ってないクラスの更新処理。ややこしくなるからラップした）
	GameInstanceUpdate();

	//ゴールした判定？
	if (player->IsGoal)
	{
		Ischangecount = true;
		//UIコントロール有効化
		stageClearCtrl.ControlActivate();
		changecount = 0;
	}

	//クリア後コントロールが遷移を許可したら
	if (stageClearCtrl.isAllowSwitching) {
		switch (stageClearCtrl._user_selecting)
		{
		case StageClearedControler::STAGE_CLEARED_USER_SELECTING::USER_SELECT_NEXT:
			//次のステージ実行準備
			SetSelectToGame(NowScene + 1);
			break;

		case StageClearedControler::STAGE_CLEARED_USER_SELECTING::USER_SELECT_BACK:
			IsGoSelect = true;
			break;

		default:
			break;
		}
	}

	//クリアフラグが立ったら遷移演出、セレクトに移動する処理を書きたい所存
	stageClearCtrl.Update();
}

void GameMainManager::Draw()
{
	GameInstanceDraw();

	stageClearCtrl.Draw();
}

void GameMainManager::Finalize()
{
}

void GameMainManager::GameInstanceUpdate()
{
	if (!player->IsGoal)
	{
		ui.Update(player->playerTile, &Ischangecount, NowScene);

		if (IsStart == false)
		{
			stage->Reset(player->playerTile);
			player->Init();
			player->BodySetUp(player->playerTile);
			IsStart = true;
		}


		tutorial.Update();

		//各ステージの処理
#ifdef _DEBUG
		if (Input::isKeyTrigger(DIK_1))
		{
			stage->LoadStage("./Resources/stage/test.csv", player->playerTile);
			stage->drawOffsetX = 200.0f;
			stage->drawOffsetY = 0.0f;
			player->Init();
			player->BodySetUp(player->playerTile);
		}

#endif // _DEBUG

		player->Update(stage->drawOffsetX, stage->drawOffsetY);
		bool PlayerBodyStatus[4] = {};

		player->SetBodyStatus(PlayerBodyStatus);

		IsFolds[0] = player->IsLeftFold;
		IsFolds[1] = player->IsUpFold;
		IsFolds[2] = player->IsRightFold;
		IsFolds[3] = player->IsDownFold;

		IsOpens[0] = player->IsLeftOpen;
		IsOpens[1] = player->IsUpOpen;
		IsOpens[2] = player->IsRightOpen;
		IsOpens[3] = player->IsDownOpen;

		if (player->leg.FootIsAction == false && player->Body_Three.IsFold == true)
		{
			int test = 0;
		}

		stage->Updata();
		stage->FoldAndOpen(player->CenterPosition, PlayerBodyStatus, player->leg.FootIsAction, IsFolds, player->OpenCount, IsOpens);

		//ステージとの連動のため開く処理はこっちでやる
		if (player->OpenCount >= 2)
		{
			if (player->IsLeftOpen == true)
			{
				player->IsLeftOpen = false;
			}
			if (player->IsUpOpen == true)
			{
				player->IsUpOpen = false;
			}
			if (player->IsRightOpen == true)
			{
				player->IsRightOpen = false;
			}
			if (player->IsDownOpen == true)
			{
				player->IsDownOpen = false;
			}
			player->OpenCount = 0;
			player->IsOpenCountStart = false;
		}
	}
}

void GameMainManager::SetSelectToGame(int SelectStageNum)
{
	//ゲームシーンに移るときのセットアップ
	Ischangecount = false;
	IsGoSelect = false;
	changecount = 0;
	IsStart = false;
	NowScene = SelectStageNum;
	stageClearCtrl.Init();

	//ステージ番号から
	selecterPtr->LoadStage(SelectStageNum);

	if (NowScene == 0)
	{
		tutorial.StartTutorial();
	}
}

void GameMainManager::SetGameToSelect()
{
	//セレクトシーンに戻るときのセットアップ
	Ischangecount = false;
	IsGoSelect = false;
	changecount = 0;

	tutorial.Init();
}

void GameMainManager::GameInstanceDraw()
{
	//各ステージの処理
	SpriteManager::Get()->SetCommonBeginDraw();
	Back.DrawExtendSprite(0, 0, 1280, 720);
	Back.Draw();

	stage->Draw();
	player->Draw(stage->drawOffsetX, stage->drawOffsetY);
	tutorial.Draw(stage->drawOffsetX, stage->drawOffsetY);
	ui.Draw();
}
