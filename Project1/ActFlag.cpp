#include "ActFlag.h"
#include "InputManger.h"
#include "Player.h"
#include "stage.h"

namespace //関数間の受け渡しは必要無いが変数名が同じ物
{
	static InputManger* inputManger = InputManger::Get(); //インプットマネージャー
	static Player* player = Player::Get(); //プレイヤー
	static Stage* stage = Stage::Get();    //ステージ

	static bool isFold = false;   //折れるかどうか
	static bool isOpen = false;   //開けるかどうか

	static size_t i = 0, j = 0; //ループカウンタ
}

bool ActFlag::isMouse = false;

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
	// キーボード&コントローラー入力
	return inputManger->DecisionTrigger();
}

bool ActFlag::Cancel()
{
	// キーボード&コントローラー入力
	return inputManger->CancelTrigger();
}

bool ActFlag::MoveLeft()
{
	// キーボード&コントローラー入力
	return inputManger->Left();
}

bool ActFlag::MoveRight()
{
	// キーボード&コントローラー入力
	return inputManger->Right();
}

bool ActFlag::Jump()
{
	// キーボード&コントローラー入力
	return inputManger->UpTrigger() && !player->IsJumpOnly;
}

bool ActFlag::FoldUp()
{
	if (player->Player_IsAction == false && player->Body_Two.IsActivate &&
		player->Body_Two.IsFold == false && !player->Body_Two.IsAction &&
		inputManger->SubUpTrigger() && !player->IsPlayerStageOnly())
	{
		return true;
	}
	else
	{
		return false;
	}

	// キーボード&コントローラー入力
	if (inputManger->SubUpTrigger() == false)
	{
		return false;
	}

	// ステージをまたいだら折れる
	isFold = false;
	for (i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			if (stage->IsPlayerTile(i, j) == false)
			{
				continue;
			}

			if (static_cast<int>(player->CenterPosition.y) / Stage::blockSize ==
				stage->GetStageTileOffsetY(i, j))
			{
				isFold = true;
				break;
			}
		}
		if (isFold)
		{
			break;
		}
	}

	return isFold;
}

bool ActFlag::FoldDown()
{
	if (player->Player_IsAction == false && player->Body_Four.IsActivate &&
		player->Body_Four.IsFold == false && !player->Body_Four.IsAction &&
		inputManger->SubDownTrigger() && !player->IsPlayerStageOnly())
	{
		return true;
	}
	else
	{
		return false;
	}

	// キーボード&コントローラー入力
	if (inputManger->SubDownTrigger() == false)
	{
		return false;
	}

	// ジャンプしたら折れる
	if (Jump() == false)
	{
		return false;
	}

	// ステージをまたいだら折れる
	isFold = false;
	for (i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			if (stage->IsPlayerTile(i, j) == false)
			{
				continue;
			}

			if (static_cast<int>(player->CenterPosition.y) / Stage::blockSize ==
				stage->GetStageTileOffsetY(i, j) + stage->GetStageTileHeight(i, j) - 1)
			{
				isFold = true;
				break;
			}
		}
		if (isFold)
		{
			break;
		}
	}

	return isFold;
}

bool ActFlag::FoldLeft()
{
	if (player->Player_IsAction == false && player->Body_One.IsActivate &&
		player->Body_One.IsFold == false && !player->Body_One.IsAction &&
		inputManger->SubLeftTrigger() && !player->IsPlayerStageOnly())
	{
		return true;
	}
	else
	{
		return false;
	}

	// キーボード&コントローラー入力
	if (inputManger->SubLeftTrigger() == false)
	{
		return false;
	}

	// 右に移動したら折れる
	if (MoveRight() == false)
	{
		return false;
	}

	// ステージをまたいだら折れる
	isFold = false;
	for (i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			if (stage->IsPlayerTile(i, j) == false)
			{
				continue;
			}

			if (static_cast<int>(player->CenterPosition.x) / Stage::blockSize ==
				stage->GetStageTileOffsetX(i, j))
			{
				isFold = true;
				break;
			}
		}
		if (isFold)
		{
			break;
		}
	}

	return isFold;
}

bool ActFlag::FoldRight()
{
	if (player->Player_IsAction == false && player->Body_Three.IsActivate &&
		player->Body_Three.IsFold == false && !player->Body_Three.IsAction &&
		inputManger->SubRightTrigger() && !player->IsPlayerStageOnly())
	{
		return true;
	}
	else
	{
		return false;
	}

	// キーボード&コントローラー入力
	if (inputManger->SubRightTrigger() == false)
	{
		//return false;
	}

	// 左に移動したら折れる
	if (MoveLeft() == false)
	{
		return false;
	}

	// ステージをまたいだら折れる
	isFold = false;
	for (i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			if (stage->IsPlayerTile(i, j) == false)
			{
				continue;
			}

			if (static_cast<int>(player->CenterPosition.x) / Stage::blockSize ==
				stage->GetStageTileOffsetX(i, j) + stage->GetStageTileWidth(i, j) - 1)
			{
				isFold = true;
				break;
			}
		}
		if (isFold)
		{
			break;
		}
	}

	return isFold;
}

bool ActFlag::OpenUp()
{
	if (player->IsUpOpen)
	{
		//return false;
	}

	if (player->Body_Two.IsActivate && player->Body_Two.IsFold &&
		player->Body_Two.AfterBodyFoldCount == 0 && !player->Body_Two.IsAction &&
		player->IsUpBlocked && inputManger->SubUpTrigger() && !player->IsPlayerStageOnly())
	{
		return true;
	}
	else
	{
		return false;
	}

	// キーボード&コントローラー入力
	if (inputManger->SubUpTrigger() == false)
	{
		return false;
	}

	// ジャンプしたら折れる
	if (Jump() == false)
	{
		return false;
	}

	// ステージをまたいだら開く
	isOpen = false;
	stage->GetPositionInitTile(player->CenterPosition, &i, &j);
	if (i == static_cast<size_t>(-1) || j == static_cast<size_t>(-1))
	{
		return false;
	}

	if (static_cast<int>(player->CenterPosition.y) / Stage::blockSize ==
		stage->GetStageTileInitOffsetY(i, j) + stage->GetStageTileInitHeight(i, j) - 1)
	{
		isOpen = true;
	}

	if (isOpen == false)
	{
		return false;
	}

	return isOpen;
}

bool ActFlag::OpenDown()
{
	// 折る処理中かどうか
	if (player->IsDownOpen)
	{
		//return false;
	}

	if (player->Body_Four.IsActivate && player->Body_Four.IsFold &&
		player->Body_Four.AfterBodyFoldCount == 0 && !player->Body_Four.IsAction &&
		inputManger->SubDownTrigger() && !player->IsPlayerStageOnly())
	{
		return true;
	}
	else
	{
		return false;
	}

	// キーボード&コントローラー入力
	if (inputManger->SubDownTrigger() == false)
	{
		return false;
	}

	// ステージをまたいだら開く
	isOpen = false;
	stage->GetPositionInitTile(player->CenterPosition, &i, &j);
	if (i == static_cast<size_t>(-1) || j == static_cast<size_t>(-1))
	{
		return false;
	}

	if (static_cast<int>(player->CenterPosition.y) / Stage::blockSize ==
		stage->GetStageTileInitOffsetY(i, j))
	{
		isOpen = true;
	}

	if (isOpen == false)
	{
		return false;
	}

	return isOpen;
}

bool ActFlag::OpenLeft()
{
	// 折る処理中かどうか
	if (player->IsLeftOpen)
	{
		//return false;
	}

	if (player->Body_One.IsActivate && player->Body_One.IsFold &&
		player->Body_One.AfterBodyFoldCount == 0 && !player->Body_One.IsAction &&
		inputManger->SubLeftTrigger() && !player->IsPlayerStageOnly())
	{
		return true;
	}
	else
	{
		return false;
	}

	// キーボード&コントローラー入力
	if (inputManger->SubLeftTrigger() == false)
	{
		return false;
	}

	// 左に移動したら開く
	if (MoveLeft() == false)
	{
		return false;
	}

	// ステージをまたいだら開く
	isOpen = false;
	stage->GetPositionInitTile(player->CenterPosition, &i, &j);
	if (i == static_cast<size_t>(-1) || j == static_cast<size_t>(-1))
	{
		return false;
	}

	if (static_cast<int>(player->CenterPosition.x) / Stage::blockSize ==
		stage->GetStageTileInitOffsetX(i, j) + stage->GetStageTileInitWidth(i, j) - 1)
	{
		isOpen = true;
	}

	if (isOpen == false)
	{
		return false;
	}

	return true;
}

bool ActFlag::OpenRight()
{
	// 折る処理中かどうか
	if (player->IsRightOpen)
	{
		//return false;
	}

	if (player->Body_Three.IsActivate && player->Body_Three.IsFold &&
		player->Body_Three.AfterBodyFoldCount == 0 && !player->Body_Three.IsAction &&
		inputManger->SubRightTrigger() && !player->IsPlayerStageOnly())
	{
		return true;
	}
	else
	{
		return false;
	}

	// キーボード&コントローラー入力
	if (inputManger->SubRightTrigger() == false)
	{
		return false;
	}

	// 左に移動したら開く
	if (MoveRight() == false)
	{
		return false;
	}

	// ステージをまたいだら開く
	isOpen = false;
	stage->GetPositionInitTile(player->CenterPosition, &i, &j);
	if (i == static_cast<size_t>(-1) || j == static_cast<size_t>(-1))
	{
		return false;
	}

	if (static_cast<int>(player->CenterPosition.x) / Stage::blockSize ==
		stage->GetStageTileInitOffsetX(i, j))
	{
		isOpen = true;
	}

	if (isOpen == false)
	{
		return false;
	}

	return true;
}
