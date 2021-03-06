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

#include "StageSelecter.h"
#include "GameMainManager.h"

class GameScene : public BaseScene {
public:
    GameScene::GameScene(ISceneChanger *changer);
    ~GameScene() {}
    void Initialize() override;    //初期化処理をオーバーライド。
    //void Finalize() override;        //終了処理をオーバーライド。
    void Update() override;        //更新処理をオーバーライド。
    void Draw() override;            //描画処理をオーバーライド。

    //シーンで使うインスタンス
    XMFLOAT3 eye = { 0,0,-150 };
    XMFLOAT3 target = { 0,0,0 };
    XMFLOAT3 up = { 0,1,0 };

    // ゲームループで使う変数の宣言
    Player* player = Player::Get();
    Stage* stage = Stage::Get();

    //ステージ選択かゲーム中か
    enum NowGameState
    {
        is_Select,
        is_Game,
    };
    NowGameState nowState = is_Select;

    std::shared_ptr<StageSelecter> selecter;
    std::shared_ptr<GameMainManager> gamemain;




};

