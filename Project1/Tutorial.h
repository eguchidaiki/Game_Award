#pragma once
#include "Sprite.h"

class Tutorial
{
public: //メンバ関数
	Tutorial();
	~Tutorial();

	// 更新
	void Update();
	// 描画
	void Draw(int offsetX = 0, int offsetY = 0);
	// 生成
	void Create();

	// チュートリアルを開始する
	void StartTutorial();
	// チュートリアル中かどうか
	inline bool GetTutorialFlag() { return isTutorial; }

public: //メンバ変数
	bool isFirstOnly;
private:
	//画像ハンドル
	UINT moveHandle;
	UINT foldHandle;

	Sprite moveSprite;
	Sprite foldSprite;

	bool isTutorial;
	bool isMoveTutorial;
	bool isFoldTutorial;
	bool isFirst;
};
