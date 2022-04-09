#include "Title.h"
#include "Raki_imguiMgr.h"
#include "TexManager.h"

#include "NY_random.h"

using namespace myImgui;

Title::Title(ISceneChanger *changer) : BaseScene(changer) {

    
    camera->SetViewStatusEyeTargetUp(eye, target, up);

    stage->LoadStage("./Resources/stage1.csv", playerTile);
    player->Init();
    player->bodysetup(playerTile);

    UINT Back = TexManager::LoadTexture("Resources/back.png");

}

//初期化
void Title::Initialize() {
    /// <summary>
    /// フィールド管理部初期化
    /// </summary>

    //StageMoveParticle::Get()->Init(&cam);
}

void Title::Finalize()
{

}

//更新
void Title::Update() {
    camera->SetViewStatusEyeTargetUp(eye, target, up);

    player->Update(*stage);
}

//描画
void Title::Draw() {

    //背景に常にいる
    Raki_DX12B::Get()->StartDrawRenderTarget();

    Raki_DX12B::Get()->StartDrawBackbuffer();

    // 描画処理
    //DrawGraph(0, 0, Back, true);
    //DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), true);
    SpriteManager::Get()->SetCommonBeginDraw();
    stage->Draw(drawOffsetX, drawOffsetY);
    player->Draw(drawOffsetX, drawOffsetY);

    //描画終了
    Raki_DX12B::Get()->CloseDraw();

}
