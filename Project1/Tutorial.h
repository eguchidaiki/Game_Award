#pragma once
#include "Sprite.h"
#include "RVector.h"
#include "Easing.h"

class Tutorial
{
public: //サブクラス
	// チュートリアルの状態
	enum TutorialState
	{
		NO_TUTORIAL,
		MOVE,  //移動のチュートリアル
		JUMP,  //ジャンプのチュートリアル
		FOLD,  //折るのチュートリアル
		SELECT //カーソル移動のチュートリアル
	};
	enum TutorialType
	{
		NORMAL_TYPE, //普通のチュートリアル
		FOLD_TYPE,   //折るのチュートリアル
		SELECT_TYPE  //カーソル移動のチュートリアル
	};

public: //定数
	static const int backFrameWadth; //背景の枠線の幅
	static const int frameWadth;     //枠線の幅
	static const XMFLOAT2 frameSize; //枠のサイズ
	
	static const XMFLOAT2 moveSpriteSize;   //移動のチュートリアルの画像サイズ
	static const XMFLOAT2 jumpSpriteSize;   //ジャンプのチュートリアルの画像サイズ
	static const XMFLOAT2 foldSpriteSize;   //折る開くのチュートリアルの画像サイズ
	static const XMFLOAT2 selectSpriteSize; //カーソル移動のチュートリアルの画像サイズ

public: //メンバ関数
	Tutorial();
	~Tutorial();

	// 初期化
	void Init();
	// 更新
	void Update();
	// 描画
	void Draw(int offsetX = 0, int offsetY = 0);
	// 生成
	void Create();
	// リセット
	void Reset();

	// チュートリアルを開始する
	void StartNormalTutorial();
	// チュートリアルを開始する(折る)
	void StartFoldTutorial();
	// チュートリアルを開始する(ステージの選択)
	void StartSelectTutorial();
	// チュートリアルをスキップする
	void SkipTutorial(const TutorialType& tutorialType);

	// チュートリアル中かどうか
	inline bool GetTutorialFlag() { return isTutorial; }
private:
	// 移動のチュートリアル
	void MoveTutorial(const XMFLOAT2& offset, bool flag);
	// ジャンプのチュートリアル
	void JumpTutorial(const XMFLOAT2& offset, bool flag);
	// 折る開くのチュートリアル
	void FoldTutorial(const XMFLOAT2& offset, bool flag);
	// カーソル移動のチュートリアル
	void SelectTutorial(const XMFLOAT2& offset, bool flag);
	// 文字の描画
	void CharacterDraw();

public: //メンバ変数
	bool isFirstOnly;

private:
	bool isTutorial;
	TutorialState tutorialState;
	bool isFirst;
	TutorialType tutorialType;

	//画像ハンドル
	Sprite frameSprite;
	Sprite moveSprite;
	Sprite moveFontSprite;
	Sprite jumpSprite;
	Sprite jumpFontSprite;
	Sprite foldSprite;
	Sprite foldFontSprite;
	Sprite selectSprite;
	Sprite selectFontSprite;

	XMFLOAT2 frameOffset;
	float drawScale;

	bool isEase;             //イージング中かどうか
	TutorialState easeState; //どのチュートリアルが動いているか
	float timeRate;
	RVector3 startPos; //イージングの開始位置
	RVector3 endPos;   //イージングの終了位置
	RVector3 easePos;  //イージング中の座標
};
