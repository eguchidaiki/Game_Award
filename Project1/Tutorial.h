#pragma once
#include "Sprite.h"

class Tutorial
{
public: //サブクラス
	enum TutorialState
	{
		NO_TUTORIAL,
		MOVE,  //移動のチュートリアル
		JUMP,  //ジャンプのチュートリアル
		FOLD,  //折るのチュートリアル
		SELECT //カーソル移動のチュートリアル
	};

public: //定数
	static const int backFrameWadth; //背景の枠線の幅
	
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
	// チュートリアルを開始する
	void StartSelectTutorial();
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

public: //メンバ変数
	bool isFirstOnly;

private:
	bool isTutorial;
	TutorialState tutorialState;
	bool isFirst;
	bool isNormal;
	bool isSelect;

	//画像ハンドル
	Sprite frameSprite;
	Sprite moveSprite;
	Sprite jumpSprite;
	Sprite foldSprite;
	Sprite selectSprite;
};
