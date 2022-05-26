#pragma once
#include "Raki_Input.h"
#include <DirectXMath.h>

// 入力をまとめたクラス
class InputManger final
{
public: //シングルトン化
	static InputManger* Get();
private:
	InputManger();
	InputManger(const InputManger&) = delete;
	~InputManger()
	{
	}
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
	// マウスの入力状態更新
	static void MouseInputUpdate();

public: //静的メンバ変数
	static bool isMenu; //メニューの有無
private:
	static DirectX::XMFLOAT2 pressPos;
	static DirectX::XMFLOAT2 releasePos;
	static DirectX::XMFLOAT2 dragDis;

public: //メンバ関数
	// 終了
	bool Escape();
	// 終了(押した瞬間の判定)
	bool EscapeTrigger();
	// 終了(離した瞬間の判定)
	bool EscapeReturn();
	// 上
	bool Up();
	// 上(押した瞬間の判定)
	bool UpTrigger();
	// 上(離した瞬間の判定)
	bool UpReturn();
	// 下
	bool Down();
	// 下(押した瞬間の判定)
	bool DownTrigger();
	// 下(離した瞬間の判定)
	bool DownReturn();
	// 左
	bool Left();
	// 左(押した瞬間の判定)
	bool LeftTrigger();
	// 左(離した瞬間の判定)
	bool LeftReturn();
	// 右
	bool Right();
	// 右(押した瞬間の判定)
	bool RightTrigger();
	// 右(離した瞬間の判定)
	bool RightReturn();
	// 上(サブ)
	bool SubUp();
	// 上(サブ)(押した瞬間の判定)
	bool SubUpTrigger();
	// 上(サブ)(離した瞬間の判定)
	bool SubUpReturn();
	// 下(サブ)
	bool SubDown();
	// 下(サブ)(押した瞬間の判定)
	bool SubDownTrigger();
	// 下(サブ)(離した瞬間の判定)
	bool SubDownReturn();
	// 左(サブ)
	bool SubLeft();
	// 左(サブ)(押した瞬間の判定)
	bool SubLeftTrigger();
	// 左(サブ)(離した瞬間の判定)
	bool SubLeftReturn();
	// 右(サブ)
	bool SubRight();
	// 右(サブ)(押した瞬間の判定)
	bool SubRightTrigger();
	// 右(サブ)(離した瞬間の判定)
	bool SubRightReturn();
	// 決定
	bool Decision();
	// 決定(押した瞬間の判定)
	bool DecisionTrigger();
	// 決定(離した瞬間の判定)
	bool DecisionReturn();
	// キャンセル
	bool Cancel();
	// キャンセル(押した瞬間の判定)
	bool CancelTrigger();
	// キャンセル(離した瞬間の判定)
	bool CancelReturn();
	// リセット
	bool Reset();
	// リセット(押した瞬間の判定)
	bool ResetTrigger();
	// リセット(離した瞬間の判定)
	bool ResetReturn();
	// メニュー
	bool Menu();
	// メニュー(押した瞬間の判定)
	bool MenuTrigger();
	// メニュー(離した瞬間の判定)
	bool MenuReturn();
	// ジャンプ
	bool Jump();
	// ジャンプ(押した瞬間の判定)
	bool JumpTrigger();
	// ジャンプ(離した瞬間の判定)
	bool JumpReturn();

	//折る操作のトリガー
	bool FoldLeftTrigger();
	bool FoldUpTrigger();
	bool FoldRightTrigger();
	bool FoldDownTrigger();

	//開く操作のトリガー
	bool OpenLeftTrigger();
	bool OpenUpTrigger();
	bool OpenRightTrigger();
	bool OpenDownTrigger();
};
