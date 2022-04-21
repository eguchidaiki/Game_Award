#pragma once
#include "PlayerBody.h"
#include "PlayerFoot.h"
#include <RVector.h>
#include <TexManager.h>
#include "Sprite.h"
#include "goalParticle.h"

class Player final
{
public: //シングルトン化
	static Player* Get();
private:
	Player();
	Player(const Player&) = delete;
	~Player();
	Player operator=(const Player&) = delete;

public: //メンバ関数
	// 初期化
	void Init();
	// 更新
	void Update(Stage& stage, int offsetX, int offsetY);
	// 描画
	void Draw(int offsetX, int offsetY);

	/// <summary>
	/// どの体を有効化するか
	/// </summary>
	/// <param name="one">body_one</param>
	/// <param name="two">body_two</param>
	/// <param name="three">body_three</param>
	void bodysetup(bool one, int one_type, bool two, int two_type, bool three, int three_type, bool four, int four_type);
	void bodysetup(const unsigned char foldCount[4]);

	//顔の当たり判定
	void IsHitPlayerBody(Stage& stage);
	//押し出し処理(使ってない)
	void ExtrudePlayer(RVector3 ExtrudePos, float ExtrudeDis, BodyType ExtrudeType);
	//各体・顔の落下判定
	bool IsFall();
	//各体の状態を配列にセット(セットする順番はstage.FoldAndOpenの判定順)
	void SetBodyStatus(PlayerBody* arrangement, int size);

public: //メンバ変数
	//床の高さ
	float FloorHeight;

	//顔の中心座標
	RVector3 CenterPosition;

	//向いている方向
	bool IsLeft;
	bool IsRight;

	//歩くかどうか
	bool IsWalk = false;

	//マウスドラッグで折るかどうか
	bool IsDragFold = false;
	XMFLOAT2 DragDis = {};

	//体(折るほう)
	PlayerBody Body_One;
	PlayerBody Body_Two;
	PlayerBody Body_Three;
	PlayerBody Body_Four;

	//body_twoを優先的に開くか
	bool IsOpenTwo;

	//どの方向を折るか(4方向)
	bool IsLeftFold;
	bool IsUpFold;
	bool IsRightFold;
	bool IsDownFold;

	//どの方向を折るか(4方向)
	bool IsLeftOpen;
	bool IsUpOpen;
	bool IsRightOpen;
	bool IsDownOpen;

	//開くまでに数フレーム空ける用
	int OpenCount = 0;

	//カウントを始めるかどうか
	bool IsOpenCountStart = false;

	//移動速度
	float SideMoveSpeed = 3.0f;

	//ジャンプ
	bool IsJump;
	float JumpSpeed;
	float FallSpeed;

	//落下判定(顔のみ
	bool IsFaceFall;

	//体と顔すべてを考慮した落下判定
	bool IsAllFall;

	//落下中・ジャンプ中にジャンプ入力が入っているかどうか
	bool IsInputjump;

	//どれか一つでも体を動かしていたらtrue
	bool Player_IsAction;

	//画像ハンドル(顔)
	UINT FaceHandle[2];

	//ゴールに触れているかどうか
	bool IsGoal;

	//ブロックに当たっているかどうか
	bool IsColide;

	//下に体があるかどうか
	bool IsDownBody;

	//足
	PlayerFoot leg;

	//描画用スプライト
	Sprite PlayerSprite;
	Sprite PlayerSpriteAction;

	//マウスのクリック座標
	XMFLOAT2 PressPos = { 0,0 };
	XMFLOAT2 ReleasePos = { 0,0 };

	//ゴール時のパーティクル
	GoalParticle goalParticle;

};
