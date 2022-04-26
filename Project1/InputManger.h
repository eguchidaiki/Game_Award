#pragma once
#include "Raki_Input.h"

// 入力をまとめたクラス
class InputManger final
{
private: //シングルトン化
	InputManger() {}
	InputManger(const InputManger&) = delete;
	~InputManger() {}
	InputManger operator=(const InputManger&) = delete;

public: //サブクラス
	enum StickDirection
	{
		NONE,  //どの方向にも傾いていない
		LEFT,  //左に傾いている
		UP,    //上に傾いている
		RIGHT, //右に傾いている
		DOWN   //下に傾いている
	};

public: //静的メンバ関数
	// 更新
	static void Update();

	// 終了
	static bool Escape();
	// 終了(押した瞬間の判定)
	static bool EscapeTrigger();
	// 終了(離した瞬間の判定)
	static bool EscapeReturn();
	// 上
	static bool Up();
	// 上(押した瞬間の判定)
	static bool UpTrigger();
	// 上(離した瞬間の判定)
	static bool UpReturn();
	// 下
	static bool Down();
	// 下(押した瞬間の判定)
	static bool DownTrigger();
	// 下(離した瞬間の判定)
	static bool DownReturn();
	// 左
	static bool Left();
	// 左(押した瞬間の判定)
	static bool LeftTrigger();
	// 左(離した瞬間の判定)
	static bool LeftReturn();
	// 右
	static bool Right();
	// 右(押した瞬間の判定)
	static bool RightTrigger();
	// 右(離した瞬間の判定)
	static bool RightReturn();
	// 上(サブ)
	static bool SubUp();
	// 上(サブ)(押した瞬間の判定)
	static bool SubUpTrigger();
	// 上(サブ)(離した瞬間の判定)
	static bool SubUpReturn();
	// 下(サブ)
	static bool SubDown();
	// 下(サブ)(押した瞬間の判定)
	static bool SubDownTrigger();
	// 下(サブ)(離した瞬間の判定)
	static bool SubDownReturn();
	// 左(サブ)
	static bool SubLeft();
	// 左(サブ)(押した瞬間の判定)
	static bool SubLeftTrigger();
	// 左(サブ)(離した瞬間の判定)
	static bool SubLeftReturn();
	// 右(サブ)
	static bool SubRight();
	// 右(サブ)(押した瞬間の判定)
	static bool SubRightTrigger();
	// 右(サブ)(離した瞬間の判定)
	static bool SubRightReturn();
	// 決定
	static bool Decision();
	// 決定(押した瞬間の判定)
	static bool DecisionTrigger();
	// 決定(離した瞬間の判定)
	static bool DecisionReturn();
	// キャンセル
	static bool Cancel();
	// キャンセル(押した瞬間の判定)
	static bool CancelTrigger();
	// キャンセル(離した瞬間の判定)
	static bool CancelReturn();
	/*以下キーボードオンリー*/
	// リセット
	static bool Reset();
	// リセット(押した瞬間の判定)
	static bool ResetTrigger();
	// リセット(離した瞬間の判定)
	static bool ResetReturn();
};
