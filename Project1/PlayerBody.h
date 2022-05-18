#pragma once

#include <RVector.h>
#include "Easing.h"
#include "Sprite.h"

class Stage;
class Player;

enum BodyType
{
	left,
	up,
	right,
	down
};

class PlayerBody
{
public: //定数
	//体の大きさ
	static const float BodySize;
	static const float HalfBodySize;

public: //メンバ関数
	PlayerBody();
	~PlayerBody();

	// 初期化
	void Init(RVector3 position, BodyType number);
	// 更新
	void Update(RVector3& center);
	// 描画
	void Draw(int offsetX, int offsetY);
	// 生成
	void Create();

	//折る
	void Body_Fold(RVector3& center);

	//開く
	void Body_Open(RVector3& center);

	//スライド
	void Body_Slide(RVector3& center);

	/// <summary>
	/// 体を有効化した時の設定
	/// </summary>
	/// <param name="center">有効化した時の座標参照先</param>
	void setactivate(RVector3 center);
	/// 体のスライドのセットアップ
	/// </summary>
	/// <param name="slidepat">スライドする向き(左上:-1 右下:1)</param>
	/// <param name="move_dis">スライドする距離(隣:1 顔をまたぐ:2)</param>
	void setslide(int slidepat, int move_dis);

	/// <summary>
	/// プレイヤーの当たり判定
	/// </summary>
	/// <param name="stage">ステージデータ</param>
	/// <param name="center">プレイヤーの中心</param>
	/// <returns></returns>
	void IsHitBody(RVector3* center, float& FallSpeed, bool& isfall, bool& isjump, bool& iscolide);

	/// <summary>
	/// 場外判定(体)
	/// </summary>
	void IsOutsideBody();

	/// <summary>
	/// 反転したタイルと体の四隅の判定
	/// </summary>
	/// <param name="stage">ステージデータ</param>
	bool IsReverseHitBody(const unsigned char& direction);

	/// <summary>
	/// 十字方向にブロックがあるかどうか
	/// </summary>
	/// <param name="stage">ステージ情報</param>
	void IsAroundBlock();

public: //メンバ変数
	//有効化フラグ
	bool IsActivate;

	//顔から見た体の位置
	int Body_Type;

	//体の座標(初期状態：左上)
	RVector3 BodyStartPos;

	//体の座標(初期状態：右下)
	RVector3 BodyEndPos;

	//スライドの開始座標
	RVector3 SlideStartPos;

	//折る・開くフラグ
	bool IsFold;
	bool IsOpen;

	//この体が折られた回数
	int FoldCount;

	//スライドフラグ・スライドする方向
	bool IsSlide;
	int SlidePat;

	//折る・開く・スライドをしている途中かどうか
	bool IsAction;

	//上下左右それぞれの当たり判定
	bool IsHitLeft;
	bool IsHitUp;
	bool IsHitRight;
	bool IsHitDown;

	//十字方向にブロックがあるかどうか
	bool IsLeftBlock;
	bool IsRightBlock;
	bool IsUpBlock;
	bool IsDownBlock;

	//反転したタイルとの当たり判定
	BodyType IsReverse;
	bool IsCanFold;

	//body別落下判定
	bool BodyIsFall;

	//スライドする距離
	int SlideDis;

	//この体より後にほかの体を折った数
	int AfterBodyFoldCount;

	//本体からどれだけ離れているか(左右専用)
	int BodyDistance;

	//イージング
	Easing Ease;

	//画像ハンドル
	UINT Bodyhandle;

	Sprite BodySprite;
};
