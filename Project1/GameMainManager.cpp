#include "GameMainManager.h"
#include "InputManger.h"
#include "Raki_DX12B.h"
#include "Stage.h"
#include "Player.h"
#include "SChangeDir.h"
#include "BackGroundGraphic.h"

namespace
{
	Stage* stage = Stage::Get();
	Player* player = Player::Get();
}

GameMainManager::GameMainManager() :
	BackHandle(0),
	IsFolds{},
	IsOpens{},
	IsStart(false),
	selecterPtr(nullptr)
{
}

GameMainManager::~GameMainManager()
{
}

void GameMainManager::Init()
{
	BackHandle = TexManager::LoadTexture("Resources/background03.png");
	this->Back.Create(BackHandle);


	playBGM = Audio::LoadSound_wav("Resources/sound/BGM/bgm02.wav");
	Audio::SetPlayRoopmode(playBGM, 255);

	ui.Init(&tutorial);

	tutorial.Create();

	stageClearCtrl.Init();

	gameMainGraphHandle = RenderTargetManager::GetInstance()->CreateRenderTexture(
		Raki_WinAPI::window_width, Raki_WinAPI::window_height);
}

void GameMainManager::Update()
{
	if (!player->IsGoal) {
		Audio::PlayLoadedSound(playBGM);
	}
	else {
		Audio::StopLoadedSound(playBGM);
	}

	//ゲーム内インスタンスの更新処理（ようは俺が作ってないクラスの更新処理。ややこしくなるからラップした）
	GameInstanceUpdate();

	//ゴールした判定？
	if (player->IsGoal)
	{
		//UIコントロール有効化
		stageClearCtrl.ControlActivate();
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

	if (Ischangecount)
	{
		changecount++;

		Audio::StopLoadedSound(playBGM);
	}

	//クリアフラグが立ったら遷移演出、セレクトに移動する処理を書きたい所存
	stageClearCtrl.Update();
}

void GameMainManager::Draw()
{
	//ハンドル設定
	RenderTargetManager::GetInstance()->SetRenderTarget(gameMainGraphHandle);
	RenderTargetManager::GetInstance()->ClearRenderTexture(gameMainGraphHandle);

	//ゲーム本編描画
	GameInstanceDraw();

	////ハンドル戻す
	RenderTargetManager::GetInstance()->SetDrawBackBuffer();

	//ステージクリア時の描画(ハンドル渡す)
	stageClearCtrl.Draw(gameMainGraphHandle);
}

void GameMainManager::Finalize()
{
}

void GameMainManager::GameInstanceUpdate()
{
	player->Update(stage->drawOffsetX, stage->drawOffsetY);

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

		//各ステージの処理
		bool PlayerBodyStatus[4] = {};

		if (player->isRespawn)
		{
			IsStart = false;
			player->isRespawn = false;
		}

		IsFolds[0] = player->IsLeftFold;
		IsFolds[1] = player->IsUpFold;
		IsFolds[2] = player->IsRightFold;
		IsFolds[3] = player->IsDownFold;

		IsOpens[0] = player->IsLeftOpen;
		IsOpens[1] = player->IsUpOpen;
		IsOpens[2] = player->IsRightOpen;
		IsOpens[3] = player->IsDownOpen;

		tutorial.Update();

		player->SetBodyStatus(PlayerBodyStatus);

		stage->Updata();
		stage->FoldAndOpen(player->CenterPosition, PlayerBodyStatus, player->FaceLeg.FootIsAction, IsFolds, player->OpenCount, IsOpens);

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


		//ゴールした判定？
		if (player->IsGoal && !Ischangecount)
		{
			//音停止
			Audio::StopLoadedSound(playBGM);
			//UIコントロール有効化
			stageClearCtrl.ControlActivate();
		}

		//ここにゴール演出、UI処理を入れる

		//セレクトに遷移する処理(仮)
		if (Ischangecount)
		{
			changecount++;

			if (changecount > 20)
			{
				IsGoSelect = true;
			}
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

	if (NowScene >= 3)
	{
		if (NowScene == 3)
		{
			tutorial.StartSelectTutorial();
		}
		else
		{
			tutorial.SkipTutorial(Tutorial::TutorialType::SELECT_TYPE);
		}
	}
	else if (NowScene >= 1)
	{
		if (NowScene == 1)
		{
			tutorial.StartNormalTutorial();
		}
		else
		{
			tutorial.SkipTutorial(Tutorial::TutorialType::NORMAL_TYPE);
		}
	}


	SChangeDir::Get()->PlayChangedDirection();
}

void GameMainManager::SetGameToSelect()
{
	//セレクトシーンに戻るときのセットアップ
	Ischangecount = false;
	IsGoSelect = false;
	changecount = 0;

	

	tutorial.Init();
}

StageClearedControler* GameMainManager::GetSCCtrl()
{
	return &stageClearCtrl;
}

void GameMainManager::GameInstanceDraw()
{
	//各ステージの処理
	SpriteManager::Get()->SetCommonBeginDraw();
	bg.Update();
	bg.Draw();

	stage->Draw();
	player->Draw(stage->drawOffsetX, stage->drawOffsetY);
	tutorial.Draw(stage->drawOffsetX, stage->drawOffsetY);
	ui.Draw();
}
