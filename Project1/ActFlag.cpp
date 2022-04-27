#include "ActFlag.h"
#include "InputManger.h"
#include "Player.h"
#include "Stage.h"

namespace //関数間の受け渡しは必要無いが変数名が同じ物
{
	/*各種フラグ*/
	static bool isFold = false; //折れるかどうか
	static bool isOpen = false; //開けるかどうか

	/*フラグ以外*/
	static Player* player = Player::Get(); //プレイヤー
	static Stage* stage = Stage::Get();    //ステージ
}

ActFlag* ActFlag::Get()
{
	static ActFlag ins{};
	return &ins;
}

ActFlag::ActFlag()
{
}

ActFlag::~ActFlag()
{
}

bool ActFlag::Decision()
{
	return InputManger::DecisionTrigger();
}

bool ActFlag::Cancel()
{
	return InputManger::CancelTrigger();
}

bool ActFlag::MoveLeft()
{
	return InputManger::Left();
}

bool ActFlag::MoveRight()
{
	return InputManger::Right();
}

bool ActFlag::Jump()
{
	return false;
}

bool ActFlag::FoldUp()
{
	isFold = InputManger::SubUpTrigger() && player->Body_Two.IsActivate && player->Body_Two.IsFold && player->Body_Two.AfterBodyFoldCount == 0;
	//if (isFold)
	//{
	//    isFold = InputManger::SubUpTrigger();
	//}

	return isFold;
}

bool ActFlag::FoldDown()
{
	isFold = InputManger::SubDownTrigger() && player->Body_Four.IsActivate && player->Body_Four.IsFold && player->Body_Four.AfterBodyFoldCount == 0;
	//if (isFold)
	//{
	//    isFold = InputManger::SubDownTrigger();
	//}

	return isFold;
}

bool ActFlag::FoldLeft()
{
	isFold = InputManger::SubLeftTrigger() && player->Body_One.IsActivate && player->Body_One.IsFold && player->Body_One.AfterBodyFoldCount == 0;
	//if (isFold)
	//{
	//    isFold = InputManger::SubLeftTrigger();
	//}

	return isFold;
}

bool ActFlag::FoldRight()
{
	if (isFold)
	{
		isFold = InputManger::SubRightTrigger();
	}

	return isFold;
}

bool ActFlag::OpenUp()
{
	// キーボード&コントローラー入力
	isOpen = InputManger::SubUpTrigger() && player->Body_Two.IsActivate && player->Body_Two.IsFold && player->Body_Two.AfterBodyFoldCount == 0;
	//if (isOpen)
	//{
	//    isOpen = InputManger::SubUpTrigger();
	//}

	return isOpen;
}

bool ActFlag::OpenDown()
{
	// キーボード&コントローラー入力
	isOpen = InputManger::SubDownTrigger() && player->Body_Four.IsActivate && player->Body_Four.IsFold && player->Body_Four.AfterBodyFoldCount == 0;
	//if (isOpen)
	//{
	//    isOpen = InputManger::SubDownTrigger();
	//}

	return isOpen;
}

bool ActFlag::OpenLeft()
{
	// キーボード&コントローラー入力
	isOpen = InputManger::SubLeftTrigger() && player->Body_One.IsActivate && player->Body_One.IsFold && player->Body_One.AfterBodyFoldCount == 0;
	//if (isOpen)
	//{
	//    isOpen = InputManger::SubLeftTrigger();
	//}

	return isOpen;
}

bool ActFlag::OpenRight()
{
	// キーボード&コントローラー入力
	isOpen = InputManger::SubDownTrigger() && player->Body_Four.IsActivate && player->Body_Four.IsFold && player->Body_Four.AfterBodyFoldCount == 0;
	//if (isOpen)
	//{
	//	isOpen = InputManger::SubRightTrigger();
	//}

	return isOpen;
}
