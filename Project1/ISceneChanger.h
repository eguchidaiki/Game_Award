#pragma once


//シーン番号定義
typedef enum {
    eScene_Title,   //タイトル画面
    eScene_Game,    //ゲーム画面
    // ----- シーン追加時はここにシーン番号の宣言をすること！ ----- //

    eScene_None,    //無し
} eScene;

//シーンを変更するためのインターフェイスクラス
class ISceneChanger {
public:
    virtual ~ISceneChanger() = 0;
    virtual void ChangeScene(eScene NextScene) = 0;//指定シーンに変更する
    virtual void EndAplication() = 0;
};

