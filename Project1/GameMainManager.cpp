#include "GameMainManager.h"
#include "InputManger.h"
#include "Raki_DX12B.h"

GameMainManager::GameMainManager()
{
}

GameMainManager::~GameMainManager()
{
}

void GameMainManager::Init(Stage* stageptr, Player* playerptr)
{
	stage = stageptr;
	player = playerptr;

	BackHandle = TexManager::LoadTexture("Resources/background03.png");
	//BackHandle = TexManager::LoadTexture("Resources/backSin.png");
	this->Back.Create(BackHandle);

	ui.Init();
}

void GameMainManager::Update()
{
	//ゲーム内インスタンスの更新処理（ようは俺が作ってないクラスの更新処理。ややこしくなるからラップした）
	GameInstanceUpdate();
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
	ui.Update(stage, player, playerTile);

	//各ステージの処理
//#ifdef _DEBUG
	if (Input::isKeyTrigger(DIK_1))
	{
		stage->LoadStage("./Resources/stage/stage1.csv", playerTile);
		player->Init();
		player->BodySetUp(playerTile);
	}
	if (Input::isKeyTrigger(DIK_2))
	{
		stage->LoadStage("./Resources/stage/stage5.csv", playerTile);
		player->Init();
		player->BodySetUp(playerTile);
	}
#ifdef _DEBUG
	if (Input::isKeyTrigger(DIK_3))
	{
		stage->LoadStage("./Resources/stage/stage6.csv", playerTile);
		player->Init();
		player->BodySetUp(playerTile);
	}

#endif // _DEBUG

	if (InputManger::ResetTrigger())
	{
		stage->Reset(playerTile);
		player->Init();
		player->BodySetUp(playerTile);
	}
	player->Update(*stage, drawOffsetX, drawOffsetY);
	bool PlayerBodyStatus[4] = {};

	player->SetBodyStatus(PlayerBodyStatus);

	bool IsFolds[4] = {
		player->IsUpFold,
		player->IsDownFold,
		player->IsLeftFold,
		player->IsRightFold
	};

	bool IsOpens[4] = {
		player->IsUpOpen,
		player->IsDownOpen,
		player->IsLeftOpen,
		player->IsRightOpen
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

	ImguiMgr::Get()->StartDrawImgui("Info", 0.0f, 100.0f);
	ImGui::Text("IsGoal:%d", player->IsGoal);
	ImGui::Text("PressCount:%d", player->PressCount);
	ImGui::Text("IsWalk:%d", player->IsWalk);
	ImGui::Text("IsJump:%d", player->IsJump);
	ImGui::Text("IsInitJump:%d", player->IsInitJump);
	ImGui::Text("x:%f", player->CenterPosition.x);
	ImGui::Text("y:%f", player->CenterPosition.y);
	ImGui::Text("z:%f", player->CenterPosition.z);
	ImGui::Text("Reset.x:%f", ui.ResetUISprite.spdata.get()->position.x);
	ImGui::Text("Reset.y:%f", ui.ResetUISprite.spdata.get()->position.y);
	ImguiMgr::Get()->EndDrawImgui();
}
