#pragma once

class Tutorial
{
public: //メンバ関数
	Tutorial();
	~Tutorial();

	void Update();

	// チュートリアルを開始する
	void StartTutorial();
	// チュートリアル中かどうか
	inline bool GetTutorialFlag() { return isTutorial; }

private: //メンバ変数
	bool isTutorial;
	bool isMoveTutorial;
	bool isFoldTutorial;
};
