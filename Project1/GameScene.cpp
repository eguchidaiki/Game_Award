#include "GameScene.h"

GameScene::GameScene(ISceneChanger *changer) : BaseScene(changer) {


}

//初期化
void GameScene::Initialize() {

}

//更新
void GameScene::Update() {



}

//描画
void GameScene::Draw() {

	Raki_DX12B::Get()->StartDraw();

	Raki_DX12B::Get()->EndDraw();
}