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
	void Update(int offsetX, int offsetY);
	// 描画
	void Draw(int offsetX, int offsetY);
	// 生成
	void Create();

	//キー操作
	//移動
	void Key_Move();
	//折る・開く
	void Key_FoldOpen();
	//スライド
	void Key_Slide();

	//マウス操作
	//入力状態
	void Mouse_Input(int offsetX, int offsetY);
	//移動
	void Mouse_Move(int offsetX, int offsetY);
	//折る・開く
	void Mouse_FoldOpen(int offsetX, int offsetY);
	//クリックした時のタイル状のマウス座標とプレイヤーのタイル状の座標を比較
	bool IsMouseClickFold(BodyType Direction);
	bool IsMouseClickOpen(BodyType Direction);


	/// <summary>
	/// どの体を有効化するか
	/// </summary>
	/// <param name="one">body_one</param>
	/// <param name="two">body_two</param>
	/// <param name="three">body_three</param>
	void BodySetUp(bool one, int one_type, bool two, int two_type, bool three, int three_type, bool four, int four_type);
	void BodySetUp(const unsigned char foldCount[4]);

	//折る
	void Fold();

	//開く
	void Open();

	//顔の当たり判定
	void IsHitPlayerBody();

	//各体・顔の落下判定
	bool IsFall();

	//各体の状態を配列にセット(セットする順番はstage.FoldAndOpenの判定順)
	void SetBodyStatus(bool arrangement[4]);

	//反転したタイルと顔の四隅との判定
	bool IsReverseHitFace(const unsigned char& direction);

	//任意の方向に折れるかどうか
	bool IsDirectionFoldAll(BodyType foldtype);

	//有効化されている体の数を取得
	int ActivateBodyCount();

	//Body配列の中で指定した方向に折れる体があるかどうか
	bool IsBodysFold(BodyType FoldType);

	//Body配列の中で指定した方向に開ける体があるかどうか
	bool IsBodysOpen(BodyType OpenType);

	//顔の下に体があるかどうか
	void IsdownBody();

	//ブロックに邪魔されずにスライドできるかどうか
	bool IsSlideBlock(BodyType SlideType);

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

	//マウスを押している間のカウント
	int PressCount = 0;

	//体(折るほう)
	PlayerBody Body_One;
	PlayerBody Body_Two;
	PlayerBody Body_Three;
	PlayerBody Body_Four;

	//PlayerBody Bodys[4];

	//上がふさがっていないか
	bool IsUpBlocked;

	//どの方向を折るか(4方向)
	bool IsLeftFold;
	bool IsUpFold;
	bool IsRightFold;
	bool IsDownFold;

	//どの方向を開くか(4方向)
	bool IsLeftOpen;
	bool IsUpOpen;
	bool IsRightOpen;
	bool IsDownOpen;

	//スライドできるかどうか
	bool IsLeftSlide;
	bool IsUpSlide;
	bool IsRightSlide;
	bool IsDownSlide;

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


	//落下判定(顔のみ)
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
	XMFLOAT2 PressPos = {0,0};
	XMFLOAT2 ReleasePos = {0,0};

	// ゴールパーティクル
	GoalParticle goalParticle;

};
