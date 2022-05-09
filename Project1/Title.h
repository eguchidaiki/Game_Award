#pragma once
#include "BaseScene.h"
#include "ISceneChanger.h"
#include "NY_Object3DMgr.h"
#include "SpriteManager.h"
#include "Audio.h"
#include "../RakiEngine_Library/SpriteManager.h"

#include "testParticle.h"
#include "Player.h"
#include "Stage.h"

enum NowState {
    title,
    game,
    over,
};


class Title : public BaseScene {

public:
    Title(ISceneChanger *changer);
    ~Title() override {};
    void Initialize() override;    //初期化処理をオーバーライド。
    void Finalize() override ;   //終了処理をオーバーライド。
    void Update() override;        //更新処理をオーバーライド。
    void Draw() override;          //描画処理をオーバーライド。

    //シーンで使うインスタンス
    XMFLOAT3 eye = { 0,0,-150 };
    XMFLOAT3 target = { 0,0,0 };
    XMFLOAT3 up = { 0,1,0 };

    // ゲームループで使う変数の宣言

    //タイトルのメインスプライト
    Sprite titleMainSprite;

    const int MENUICON_START_X = 500;
    const int MENUICON_START_Y = 470;
    const int MENUICON_SIZE_Y = 60;
    const int MENUICON_SIZE_X = 280;
    const int MENUICON_OFFSET = 25;

};

