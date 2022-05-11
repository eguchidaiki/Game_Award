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
	BackHandle = TexManager::LoadTexture("Resources/background03.png");
	//BackHandle = TexManager::LoadTexture("Resources/backSin.png");
	this->Back.Create(BackHandle);

	menuBGM = Audio::LoadSound_wav("Resources/sound/BGM/bgm01.wav");
	playBGM = Audio::LoadSound_wav("Resource/sound/BGM/bgm02.wav");

	ui.Init();

	tutorial.Create();
}

void GameMainManager::Update()
{
	//ゲーム内インスタンスの更新処理（ようは俺が作ってないクラスの更新処理。ややこしくなるからラップした）
	GameInstanceUpdate();

	//クリアフラグが立ったら遷移演出、セレクトに移動する処理を書きたい所存

}

void GameMainManager::Draw()
{
	GameInstanceDraw();
}

void GameMainManager::Finalize()
{
}

void GameMainManager::GameInstanceUpdate()
{
	//playerTile[0] = player->playerTile[0];
	//playerTile[1] = player->playerTile[1];
	//playerTile[2] = player->playerTile[2];
	//playerTile[3] = player->playerTile[3];

	ui.Update(playerTile, &Ischangecount);



	//各ステージの処理
#ifdef _DEBUG
	if (Input::isKeyTrigger(DIK_1))
	{
		stage->LoadStage("./Resources/stage/stage1.csv", playerTile);
		player->Init();
		player->BodySetUp(playerTile);
	}

	if (InputManger::Get()->ResetTrigger())
	{
		stage->Reset(playerTile);
		player->Init();
		player->BodySetUp(playerTile);
	}
#endif // _DEBUG

	player->Update(drawOffsetX, drawOffsetY);
	player->Update(drawOffsetX, drawOffsetY);
	bool PlayerBodyStatus[4] = {};

	player->SetBodyStatus(PlayerBodyStatus);

	bool IsFolds[4] = {
		player->IsLeftFold,
		player->IsUpFold,
		player->IsRightFold,
		player->IsDownFold,
	};

	bool IsOpens[4] = {
		player->IsLeftOpen,
		player->IsUpOpen,
		player->IsRightOpen,
		player->IsDownOpen,
	};

	stage->Updata();
	stage->FoldAndOpen(player->CenterPosition, playerTile, PlayerBodyStatus, player->leg.FootIsAction, IsFolds, player->OpenCount, IsOpens);

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

	if (player->IsGoal && !Ischangecount)
	{
		Ischangecount = true;
		changecount = 0;
	}

	if (Ischangecount)
	{
		changecount++;

		if (changecount > 20)
		{
			IsGoSelect = true;
		}
	}
}

void GameMainManager::GameInstanceDraw()
{
	//各ステージの処理
	SpriteManager::Get()->SetCommonBeginDraw();
	Back.DrawExtendSprite(0, 0, 1280, 720);
	Back.Draw();
	Raki_DX12B::Get()->ClearDepthBuffer();
	ui.Draw();
	stage->Draw(drawOffsetX, drawOffsetY);
	player->Draw(drawOffsetX, drawOffsetY);
}
