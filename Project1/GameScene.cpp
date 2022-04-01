#include "GameScene.h"

GameScene::GameScene(ISceneChanger *changer) : BaseScene(changer) {


}

//‰Šú‰»
void GameScene::Initialize() {

}

//XV
void GameScene::Update() {



}

//•`‰æ
void GameScene::Draw() {

	Raki_DX12B::Get()->StartDraw();

	Raki_DX12B::Get()->EndDraw();
}