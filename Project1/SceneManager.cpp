#include "SceneManager.h"
#include "GameScene.h"
#include "Title.h"

//Raki_DX12B         *SceneManager::dx12b  = nullptr;
//NY_Object3DManager *SceneManager::objmgr = nullptr;
//SpriteManager      *SceneManager::spmgr  = nullptr;

SceneManager::SceneManager() :mNextScene(eScene_None) {

    //初回軌道するシーンはここで定義
    nowScene = (BaseScene*)new Title(this);
    Initialize();
}

void SceneManager::Initialize()
{
    //シーンの初期化
	nowScene->Initialize();
}

void SceneManager::Finalize()
{
    //シーンの終了
    delete nowScene;
}

void SceneManager::Update()
{
    if (mNextScene != eScene_None) {    //次のシーンがセットされていたら
        delete nowScene;//現在のシーンの終了処理を実行
        switch (mNextScene) {       //シーンによって処理を分岐
        case eScene_Title:        //次の画面がメニューなら
            nowScene = (BaseScene*)new Title(this);
            break;//以下略
        case eScene_Game:
            nowScene = (BaseScene*)new GameScene(this);
            break;
            // ----- シーンを追加するときは、上のようにenumに定義した定数で分岐させて、nowSceneに該当シーンをnewで生成すること ----- //


        }
        mNextScene = eScene_None;    //次のシーン情報をクリア
        nowScene->Initialize();    //シーンを初期化
    }

    nowScene->Update(); //シーンの更新
}

void SceneManager::Draw()
{
    //現在シーンの描画処理を実行
    nowScene->Draw();
}

void SceneManager::ChangeScene(eScene NextScene)
{
    //次のシーン番号をセット
    mNextScene = NextScene;
}
