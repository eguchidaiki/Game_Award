#include "Title.h"
#include "Raki_imguiMgr.h"
#include "TexManager.h"

#include "InputManger.h"
#include "NY_random.h"

using namespace myImgui;

Title::Title(ISceneChanger* changer) : BaseScene(changer) {


}

//初期化
void Title::Initialize() {




}

void Title::Finalize()
{

}

//更新
void Title::Update() {

	
}

//描画
void Title::Draw() {

	//背景に常にいる
	Raki_DX12B::Get()->StartDrawRenderTarget();

	Raki_DX12B::Get()->StartDrawBackbuffer();

	// 描画処理
	//DrawGraph(0, 0, Back, true);
	//DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), true);


	//描画終了
	Raki_DX12B::Get()->CloseDraw();

}
