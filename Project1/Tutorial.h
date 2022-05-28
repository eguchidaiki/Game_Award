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
	void StartTutorial();
	// チュートリアル中かどうか
	inline bool GetTutorialFlag() { return isTutorial; }

public: //メンバ変数
	bool isFirstOnly;

private:
	bool isTutorial;
	TutorialState tutorialState;
	bool isMoveTutorial;
	bool isJumpTutorial;
	bool isFoldTutorial;
	bool isFirst;

	//画像ハンドル
	Sprite moveSprite;
	Sprite jumpSprite;
	Sprite foldSprite;
	Sprite selectSprite;

	XMFLOAT2 spriteSize;
};
