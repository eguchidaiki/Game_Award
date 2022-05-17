#include "Player.h"
#include "Stage.h"
#include "InputManger.h"
#include "ActFlag.h"
#include "Raki_imguiMgr.h"

namespace
{
	static size_t i = 0;
	static ActFlag* actFlag = ActFlag::Get();

	static int PlayerOffsetX = 0;
	static int PlayerOffsetY = 0;

	static InputManger* inputManger = InputManger::Get(); //インプットマネージャー
	static Stage* stage = Stage::Get();
}

Player* Player::Get()
{
	static Player instance = {};
	return &instance;
}

Player::Player() :
	FloorHeight(640.0f),
	CenterPosition{ 100.0f, 100.0f, 0.0f },
	IsLeft(),
	IsRight(),
	Body_One{},
	Body_Two{},
	Body_Three{},
	Body_Four{},
	IsUpBlocked(true),
	IsLeftFold(),
	IsUpFold(),
	IsRightFold(),
	IsDownFold(),
	SideMoveSpeed(2.0f),
	IsJump(false),
	JumpSpeed(3.0f),
	FallSpeed(3.0f),
	IsFaceFall(),
	IsAllFall(false),
	IsInputjump(false),
	Player_IsAction(false),
	FaceHandle{},
	IsStart(false),
	IsGoal(false),
	IsColide(false),
	IsDownBody(false),
	leg{},
	goalParticle{}
{
}

Player::~Player()
{
}

void Player::Init()
{
	Create();
	CenterPosition.x = static_cast<float>(Stage::GetStartPlayerPosX() * Stage::blockSize + Stage::halfBlockSize);
	CenterPosition.y = static_cast<float>(Stage::GetStartPlayerPosY() * Stage::blockSize + Stage::halfBlockSize);

	FallSpeed = 3.0f;
	IsAllFall = true;
	IsJump = false;
	Player_IsAction = false;
	IsColide = false;

	Body_One.Create();
	Body_Two.Create();
	Body_Three.Create();
	Body_Four.Create();

	Body_One.Init(CenterPosition, BodyType::left);
	Body_Two.Init(CenterPosition, BodyType::up);
	Body_Three.Init(CenterPosition, BodyType::right);
	Body_Four.Init(CenterPosition, BodyType::down);

	leg.Create();
	leg.Init();
	IsLeft = true;
	IsRight = false;

	IsStart = true;

	goalParticle.Create();
}

void Player::Update(int offsetX, int offsetY)
{
	PlayerOffsetX = offsetX;
	PlayerOffsetY = offsetY;

	//マウス入力
	//Mouse_Input(offsetX, offsetY);

	//マウス移動
	//Mouse_Move(offsetX, offsetY);

	//キー移動
	Key_Move();

	//ジャンプ中の処理
	if (IsJump == true)
	{
		if (Player_IsAction == false)
		{
			FallSpeed += 0.2f;
		}

		if (Leftjump == true)
		{
			CenterPosition.x -= 2.0f;
		}
		if (Rightjump == true)
		{
			CenterPosition.x += 2.0f;
		}

		if (FallSpeed > 0)
		{
			Leftjump = false;
			Rightjump = false;
			IsJump = false;
			IsAllFall = true;
		}
	}

	//落下判定
	if (IsJump == false && IsAllFall == true && Player_IsAction == false)
	{
		if (FallSpeed < 5.0)
		{
			FallSpeed += 0.2f;
		}
	}

	//どの体にも引っかからなかったら落ちる
	if (IsAllFall == true && Player_IsAction == false)
	{
		CenterPosition.y += FallSpeed;
	}

	//顔の当たり判定
	IsHitPlayerBody();
	IsAroundBlock();

	//キー折る・開く入力
	Key_FoldOpen();

	//キースライド
	//Key_Slide();

	//マウス折る・開く入力
	//Mouse_FoldOpen(offsetX, offsetY);

	//足を上げる演出が終わったら折る処理に入る
	Fold();

	//PlayerIsactionのfalse条件
	if (leg.FootIsAction == false &&
		Body_One.IsAction == false &&
		Body_Two.IsAction == false &&
		Body_Three.IsAction == false &&
		Body_Four.IsAction == false)
	{
		Player_IsAction = false;
		leg.IsFootUp = false;
	}

	if (IsOpenCountStart == true)
	{
		OpenCount++;
	}

	//開く処理(OpenCountを参照するのでこの順番でOK)
	Open();

	//顔の下に体があるかどうか
	IsdownBody();

	//足のy座標はこっちで判断して引数で渡す
	RVector3 FootUpPosition = { 0,0,0 };
	if (Body_Four.IsActivate && Body_Four.IsOpen)
	{
		if (Body_Four.BodyStartPos.y < Body_Four.BodyEndPos.y)
		{
			FootUpPosition =
			{
				CenterPosition.x - 30,
				Body_Four.BodyEndPos.y,
				0
			};
		}
		else
		{
			FootUpPosition =
			{
				CenterPosition.x - 30,
				Body_Four.BodyStartPos.y,
				0
			};
		}
	}
	else
	{
		FootUpPosition =
		{
			CenterPosition.x - 30,
			CenterPosition.y + 25,
			0
		};
	}

	leg.Update(FootUpPosition, IsDownBody, 1);

	//それぞれの体のアップデート処理(有効化されているときのみ)
	if (Body_One.IsActivate == true)
	{
		Body_One.IsHitBody(&CenterPosition, FallSpeed, IsAllFall, IsJump, IsColide);
		Body_One.Update(CenterPosition);
		Body_One.IsAroundBlock();
	}
	if (Body_Two.IsActivate == true)
	{
		Body_Two.IsHitBody(&CenterPosition, FallSpeed, IsAllFall, IsJump, IsColide);
		Body_Two.Update(CenterPosition);
		Body_Two.IsAroundBlock();
	}
	if (Body_Three.IsActivate == true)
	{
		Body_Three.IsHitBody(&CenterPosition, FallSpeed, IsAllFall, IsJump, IsColide);
		Body_Three.Update(CenterPosition);
		Body_Three.IsAroundBlock();
	}
	if (Body_Four.IsActivate == true)
	{
		Body_Four.IsHitBody(&CenterPosition, FallSpeed, IsAllFall, IsJump, IsColide);
		Body_Four.Update(CenterPosition);
		Body_Four.IsAroundBlock();
	}

	//ゴール演出
	if (IsGoal)
	{
		goalParticle.Init({ 0.0f, 0.0f, 0.0f }, 1.0f, 10, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	}
	goalParticle.Update();
}

void Player::Draw(int offsetX, int offsetY)
{
	leg.Draw(offsetX, offsetY, IsLeft, IsRight);
	if (IsLeft)
	{
		if (Player_IsAction)
		{
			PlayerSpriteAction.DrawExtendSprite(
				(CenterPosition.x - 25) + offsetX, (CenterPosition.y - 25) + offsetY,
				(CenterPosition.x + 25) + offsetX, (CenterPosition.y + 25) + offsetY);
		}
		else
		{
			PlayerSprite.DrawExtendSprite(
				(CenterPosition.x - 25) + offsetX, (CenterPosition.y - 25) + offsetY,
				(CenterPosition.x + 25) + offsetX, (CenterPosition.y + 25) + offsetY);
		}
	}
	if (IsRight)
	{
		if (Player_IsAction)
		{
			PlayerSpriteAction.DrawExtendSprite(
				(CenterPosition.x + 25) + offsetX, (CenterPosition.y - 25) + offsetY,
				(CenterPosition.x - 25) + offsetX, (CenterPosition.y + 25) + offsetY);
		}
		else
		{
			PlayerSprite.DrawExtendSprite(
				(CenterPosition.x + 25) + offsetX, (CenterPosition.y - 25) + offsetY,
				(CenterPosition.x - 25) + offsetX, (CenterPosition.y + 25) + offsetY);
		}
	}

#pragma region body_draw

	if (Body_One.AfterBodyFoldCount == 2)
	{
		Body_One.Draw(offsetX, offsetY);
	}
	if (Body_Two.AfterBodyFoldCount == 2)
	{
		Body_Two.Draw(offsetX, offsetY);
	}
	if (Body_Three.AfterBodyFoldCount == 2)
	{
		Body_Three.Draw(offsetX, offsetY);
	}
	if (Body_Four.AfterBodyFoldCount == 2)
	{
		Body_Four.Draw(offsetX, offsetY);
	}

	if (Body_One.AfterBodyFoldCount == 1)
	{
		Body_One.Draw(offsetX, offsetY);
	}
	if (Body_Two.AfterBodyFoldCount == 1)
	{
		Body_Two.Draw(offsetX, offsetY);
	}
	if (Body_Three.AfterBodyFoldCount == 1)
	{
		Body_Three.Draw(offsetX, offsetY);
	}
	if (Body_Four.AfterBodyFoldCount == 1)
	{
		Body_Four.Draw(offsetX, offsetY);
	}

	if (Body_One.AfterBodyFoldCount == 0)
	{
		Body_One.Draw(offsetX, offsetY);
	}
	if (Body_Two.AfterBodyFoldCount == 0)
	{
		Body_Two.Draw(offsetX, offsetY);
	}
	if (Body_Three.AfterBodyFoldCount == 0)
	{
		Body_Three.Draw(offsetX, offsetY);
	}
	if (Body_Four.AfterBodyFoldCount == 0)
	{
		Body_Four.Draw(offsetX, offsetY);
	}

#pragma endregion //body_draw

	if (Player_IsAction)
	{
		PlayerSpriteAction.Draw();
	}
	else
	{
		PlayerSprite.Draw();
	}

	//goalParticle.Draw();

#ifdef _DEBUG
	//ImguiMgr::Get()->StartDrawImgui("IsGoal state", 0.0f, 100.0f);
	//ImGui::Text("flag:%d", Player_IsAction);
	//ImGui::Text("1:%d", Body_One.AfterBodyFoldCount);
	//ImGui::Text("2:%d", Body_Two.AfterBodyFoldCount);
	//ImGui::Text("3:%d", Body_Three.AfterBodyFoldCount);
	//ImGui::Text("4:%d", Body_Four.AfterBodyFoldCount);
	//ImGui::Text("x:%f", CenterPosition.x);
	//ImGui::Text("y:%f", CenterPosition.y);
	//ImGui::Text("z:%f", CenterPosition.z);
	//ImGui::Text("IsLeftSlide:%d", IsLeftSlide);
	//ImGui::Text("IsRightSlide:%d", IsRightSlide);
	//ImGui::Text("IsUpSlide:%d", IsUpSlide);
	//ImGui::Text("IsDownSlide:%d", IsDownSlide);
	//ImguiMgr::Get()->EndDrawImgui();
#endif // _DEBUG


}

void Player::Create()
{
	if ((PlayerSprite.spdata->size.x <= 0) || (PlayerSprite.spdata->size.y <= 0))
	{
		FaceHandle[0] = TexManager::LoadTexture("Resources/player.png");
		PlayerSprite.Create(FaceHandle[0]);
	}

	if ((PlayerSpriteAction.spdata->size.x <= 0) || (PlayerSpriteAction.spdata->size.y <= 0))
	{
		FaceHandle[1] = TexManager::LoadTexture("Resources/playerBody02.png");
		PlayerSpriteAction.Create(FaceHandle[1]);
	}
}

void Player::Key_Move()
{
	IsWalk = (actFlag->MoveLeft() || actFlag->MoveRight()) && (!Player_IsAction);

	//左右移動
	if (IsJumpOnly == false)
	{
		if (actFlag->MoveRight() && Player_IsAction == false)
		{
			CenterPosition.x += SideMoveSpeed;
			IsLeft = false;
			IsRight = true;
		}

		if (actFlag->MoveLeft() && Player_IsAction == false)
		{
			CenterPosition.x -= SideMoveSpeed;
			IsLeft = true;
			IsRight = false;
		}
	}
	else
	{
		if (IsJump == false && inputManger->LeftTrigger() && IsInputjump == true)
		{
			Leftjump = true;
			Rightjump = false;
			IsJump = true;
			FallSpeed = -5.0f;
		}

		if (IsJump == false && inputManger->RightTrigger() && IsInputjump == true)
		{
			Rightjump = true;
			Leftjump = false;
			IsJump = true;
			FallSpeed = -5.0f;
		}
	}

	//ジャンプ入力できるかどうか
	if (IsJump == false && IsFall() == false)
	{
		IsInputjump = true;
		FallSpeed = 0.0f;
	}
	else
	{
		IsInputjump = false;
	}

	//ジャンプ
	if (actFlag->Jump() && IsInputjump == true)
	{
		IsJump = true;
		FallSpeed = -5.0f;
	}
}

void Player::Key_FoldOpen()
{
	//折る入力
	if (actFlag->FoldLeft())
	{ //左に折る
		if (IsDirectionFoldAll(BodyType::left))
		{
			Player_IsAction = true;
			IsLeftFold = true;
			leg.Set();
			return;
		}
	}
	if (actFlag->FoldUp())
	{ //上に折る
		if (IsDirectionFoldAll(BodyType::up))
		{
			Player_IsAction = true;
			IsUpFold = true;
			leg.Set();
			return;
		}
	}
	if (actFlag->FoldRight())
	{ //右に折る
		if (IsDirectionFoldAll(BodyType::right))
		{
			Player_IsAction = true;
			IsRightFold = true;
			leg.Set();
			return;
		}
	}
	if (actFlag->FoldDown())
	{ //下に折る
		if (IsDirectionFoldAll(BodyType::down))
		{
			Player_IsAction = true;
			IsDownFold = true;
			leg.Set();
			return;
		}
	}

	//開く入力
	if (actFlag->OpenLeft())
	{ //左に開く
		OpenCount = 0;
		IsOpenCountStart = true;
		IsLeftOpen = true;
		return;
	}
	if (actFlag->OpenUp())
	{ //上に開く
		OpenCount = 0;
		IsOpenCountStart = true;
		IsUpOpen = true;
		return;
	}
	if (actFlag->OpenRight())
	{ //右に開く
		OpenCount = 0;
		IsOpenCountStart = true;
		IsRightOpen = true;
		return;
	}
	if (actFlag->OpenDown())
	{ //下に開く
		OpenCount = 0;
		IsOpenCountStart = true;
		IsDownOpen = true;
		return;
	}
}

void Player::Key_Slide()
{
	IsSlideBlock();

	if (Player_IsAction == true)
	{
		return;
	}
	//左にスライド
	if (Input::isKeyTrigger(DIK_A) && IsLeftSlide)
	{
		if (Body_One.IsActivate == true && Body_Three.IsActivate == true)
		{
			if (Body_One.Body_Type == BodyType::right)
			{
				Body_One.AfterBodyFoldCount = 0;
				Body_One.setslide(-1, 2);
				Body_Three.BodyDistance = 1;
				Body_Three.setslide(-1, 1);
			}
			if (Body_One.Body_Type == BodyType::left && Body_One.BodyDistance == 1 && Body_Three.IsFold == false)
			{
				if (Body_One.IsFold == true)
				{
					Body_Three.AfterBodyFoldCount = 1;
					Body_One.BodyDistance = 2;
					Body_One.setslide(-1, 1);
					Body_Three.setslide(-1, 2);

					if (Body_Two.IsFold == true)
					{
						Body_Two.AfterBodyFoldCount = 0;
					}
				}
				else
				{
					Body_One.BodyDistance = 2;
					Body_One.setslide(-1, 1);
					Body_Three.setslide(-1, 2);
				}
			}
		}
		else if (Body_Three.IsActivate == true && Body_Three.Body_Type == BodyType::right && Body_Three.IsFold == false && Body_Three.IsAction == false)
		{
			Body_Three.setslide(-1, 2);
		}
		else if (Body_One.IsActivate == true && Body_One.Body_Type == BodyType::right && Body_One.IsFold == false)
		{
			Body_One.setslide(-1, 2);
		}
	}

	//右にスライド
	if (Input::isKeyTrigger(DIK_D) && IsRightSlide)
	{
		if (Body_One.IsActivate == true && Body_Three.IsActivate == true)
		{
			if (Body_Three.Body_Type == BodyType::left)
			{
				Body_Three.AfterBodyFoldCount = 0;
				Body_Three.setslide(1, 2);
				Body_One.BodyDistance = 1;
				Body_One.setslide(1, 1);
			}
			if (Body_Three.Body_Type == BodyType::right && Body_Three.BodyDistance == 1 && Body_One.IsFold == false)
			{
				if (Body_Three.IsFold == true)
				{
					Body_One.AfterBodyFoldCount = 1;
					Body_Three.BodyDistance = 2;
					Body_Three.setslide(1, 1);
					Body_One.setslide(1, 2);

					if (Body_Two.IsFold == true)
					{
						Body_Two.AfterBodyFoldCount = 0;
					}
				}
				else
				{
					Body_Three.BodyDistance = 2;
					Body_Three.setslide(1, 1);
					Body_One.setslide(1, 2);
				}
			}
		}
		else if (Body_One.IsActivate == true && Body_One.Body_Type == BodyType::left && Body_One.IsFold == false && Body_One.IsAction == false)
		{
			Body_One.setslide(1, 2);
		}
		else if (Body_Three.IsActivate == true && Body_Three.Body_Type == BodyType::left && Body_Three.IsFold == false)
		{
			Body_Three.setslide(1, 2);
		}
	}

	//上にスライド
	if (Input::isKeyTrigger(DIK_W) && IsUpSlide)
	{
		if (Body_Two.IsActivate == true && Body_Four.IsActivate == true)
		{
			if (Body_Two.Body_Type == BodyType::down)
			{
				Body_Two.AfterBodyFoldCount = 0;
				Body_Two.setslide(-1, 2);
				Body_Four.BodyDistance = 1;
				Body_Four.setslide(-1, 1);
			}
			if (Body_Two.Body_Type == BodyType::up && Body_Two.BodyDistance == 1 && Body_Four.IsFold == false)
			{
				if (Body_Two.IsFold == true)
				{
					Body_Four.AfterBodyFoldCount = 1;
					Body_Two.BodyDistance = 2;
					Body_Two.setslide(-1, 1);
					Body_Four.setslide(-1, 2);

					if (Body_One.IsFold == true)
					{
						Body_One.AfterBodyFoldCount = 0;
					}
				}
				else
				{
					Body_Two.BodyDistance = 2;
					Body_Two.setslide(-1, 1);
					Body_Four.setslide(-1, 2);
				}
			}
		}
		else if (Body_Four.IsActivate == true && Body_Four.Body_Type == BodyType::down && Body_Four.IsFold == false && Body_Four.IsAction == false)
		{
			Body_Four.setslide(-1, 2);
		}
		else if (Body_Two.IsActivate == true && Body_Two.Body_Type == BodyType::down && Body_Two.IsFold == false)
		{
			Body_Two.setslide(-1, 2);
		}
	}

	//下にスライド
	if (Input::isKeyTrigger(DIK_S) && IsDownSlide)
	{
		if (Body_Two.IsActivate == true && Body_Four.IsActivate == true)
		{
			if (Body_Two.Body_Type == BodyType::up)
			{
				Body_Two.AfterBodyFoldCount = 0;
				Body_Two.setslide(-1, 2);
				Body_Four.BodyDistance = 1;
				Body_Four.setslide(-1, 1);
			}
			if (Body_Two.Body_Type == BodyType::down && Body_Two.BodyDistance == 1 && Body_Four.IsFold == false)
			{
				if (Body_Two.IsFold == true)
				{
					Body_Four.AfterBodyFoldCount = 1;
					Body_Two.BodyDistance = 2;
					Body_Two.setslide(-1, 1);
					Body_Four.setslide(-1, 2);

					if (Body_One.IsFold == true)
					{
						Body_One.AfterBodyFoldCount = 0;
					}
				}
				else
				{
					Body_Two.BodyDistance = 2;
					Body_Two.setslide(-1, 1);
					Body_Four.setslide(-1, 2);
				}
			}
		}
		else if (Body_Four.IsActivate == true && Body_Four.Body_Type == BodyType::up && Body_Four.IsFold == false && Body_Four.IsAction == false)
		{
			Body_Four.setslide(-1, 2);
		}
		else if (Body_Two.IsActivate == true && Body_Two.Body_Type == BodyType::up && Body_Two.IsFold == false)
		{
			Body_Two.setslide(-1, 2);
		}
	}
}

void Player::Mouse_Input(int offsetX, int offsetY)
{
	//マウス左ボタンを押したときの座標
	if (Input::isMouseClickTrigger(MOUSE_L))
	{
		PressPos = Input::getMousePos();
		PressPos.x -= offsetX;
		PressPos.y -= offsetY;
		PressCount = 0;
	}

	//マウスを押している間はカウントを進める
	if (Input::isMouseClicking(MOUSE_L))
	{
		PressCount++;
	}

	//マウス左ボタンを離したときの座標
	if (Input::isMouseClicked(MOUSE_L))
	{
		ReleasePos = Input::getMousePos();
		ReleasePos.x -= offsetX;
		ReleasePos.y -= offsetY;
	}

	DragDis = { ReleasePos.x - PressPos.x , ReleasePos.y - PressPos.y };
}

void Player::Mouse_Move(int offsetX, int offsetY)
{
	if (IsStart == false)
	{
		return;
	}

	if (ReleasePos.x != 0.0f &&
		ReleasePos.y != 0.0f &&
		PressCount != 0 &&
		PressCount < 30 &&
		Input::isMouseClicked(MOUSE_L) &&
		IsPressInStage())
	{
		IsWalk = true;
	}

	if (IsWalk == true && Player_IsAction == false)
	{
		if (PressPos.x < CenterPosition.x)
		{
			CenterPosition.x -= SideMoveSpeed;
			IsLeft = true;
			IsRight = false;

			if (CenterPosition.x - SideMoveSpeed * 2 < PressPos.x)
			{
				IsWalk = false;
			}
		}
		else if (PressPos.x > CenterPosition.x)
		{
			CenterPosition.x += SideMoveSpeed;
			IsLeft = false;
			IsRight = true;

			if (CenterPosition.x + SideMoveSpeed * 2 > PressPos.x)
			{
				IsWalk = false;
			}
		}
		else
		{
			IsWalk = false;
		}
	}
}

void Player::Mouse_FoldOpen(int offsetX, int offsetY)
{
	if (ReleasePos.x != 0.0f &&
		ReleasePos.y != 0.0f &&
		PressCount >= 30 &&
		Input::isMouseClicked(MOUSE_L))
	{
		if (fabs(ReleasePos.x - PressPos.x) > fabs(ReleasePos.y - PressPos.y))
		{
			if (DragDis.x > 0)
			{
				if (IsDirectionFoldAll(BodyType::left) && IsMouseClickFold(BodyType::left))
				{
					Player_IsAction = true;
					IsLeftFold = true;
					leg.Set();
					return;
				}
				if (((Body_Three.IsActivate == true && Body_Three.IsFold == true &&
					Body_Three.AfterBodyFoldCount == 0 && Body_Three.Body_Type == BodyType::right) ||
					(Body_One.IsActivate == true && Body_One.IsFold == true &&
						Body_One.AfterBodyFoldCount == 0 && Body_One.Body_Type == BodyType::right)) &&
					IsMouseClickOpen(BodyType::right))
				{
					OpenCount = 0;
					IsOpenCountStart = true;
					IsRightOpen = true;
					return;
				}
			}
			else
			{
				if (IsDirectionFoldAll(BodyType::right) && IsMouseClickFold(BodyType::right))
				{
					Player_IsAction = true;
					IsRightFold = true;
					leg.Set();
					return;
				}
				if ((Body_One.IsActivate == true && Body_One.IsFold == true &&
					Body_One.AfterBodyFoldCount == 0 && Body_One.Body_Type == BodyType::left ||
					Body_Three.IsActivate == true && Body_Three.IsFold == true &&
					Body_Three.AfterBodyFoldCount == 0 && Body_Three.Body_Type == BodyType::left) &&
					IsMouseClickOpen(BodyType::left))
				{
					OpenCount = 0;
					IsOpenCountStart = true;
					IsLeftOpen = true;
					return;
				}
			}
		}
		else
		{
			if (DragDis.y > 0)
			{
				if (IsDirectionFoldAll(BodyType::up) && IsMouseClickFold(BodyType::up))
				{
					Player_IsAction = true;
					IsUpFold = true;
					IsWalk = false;
					leg.Set();
					return;
				}
				if ((Body_Four.IsActivate == true && Body_Four.IsFold == true &&
					Body_Four.AfterBodyFoldCount == 0 && Body_Four.Body_Type == BodyType::down ||
					Body_Two.IsActivate == true && Body_Two.IsFold == true &&
					Body_Two.AfterBodyFoldCount == 0 && Body_Two.Body_Type == BodyType::down) &&
					IsMouseClickOpen(BodyType::down))
				{
					OpenCount = 0;
					IsOpenCountStart = true;
					IsDownOpen = true;
					IsWalk = false;
					return;
				}
			}
			else
			{
				if (IsDirectionFoldAll(BodyType::down) && IsMouseClickFold(BodyType::down))
				{
					Player_IsAction = true;
					IsDownFold = true;
					IsWalk = false;
					leg.Set();
					return;
				}
				if ((Body_Two.IsActivate == true && Body_Two.IsFold == true &&
					Body_Two.AfterBodyFoldCount == 0 && Body_Two.Body_Type == BodyType::up ||
					Body_Four.IsActivate == true && Body_Four.IsFold == true &&
					Body_Four.AfterBodyFoldCount == 0 && Body_Four.Body_Type == BodyType::up) &&
					IsUpBlocked == true && IsMouseClickOpen(BodyType::up))
				{
					OpenCount = 0;
					IsOpenCountStart = true;
					IsUpOpen = true;
					IsWalk = false;
					return;
				}
			}
		}
	}
}

bool Player::IsMouseClickFold(BodyType Direction)
{
	int PlayerStage = -1;
	int PlayerTile = -1;

	int ReleaseStage = -1;
	int ReleaseTile = -1;

	for (int i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (int j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			if (stage->IsPositionTile(CenterPosition, i, j))
			{
				PlayerStage = i;
				PlayerTile = j;
			}

			if (stage->IsPositionTile({ ReleasePos.x, ReleasePos.y, 0.0f }, i, j))
			{
				ReleaseStage = i;
				ReleaseTile = j;
			}
		}
	}

	float TileLeft = -1;
	float TileRight = -1;
	float TileUp = -1;
	float TileDown = -1;

	if (PlayerStage != -1 && ReleaseStage != -1 &&
		PlayerStage == ReleaseStage && PlayerTile == ReleaseTile)
	{
		switch (Direction)
		{
		case BodyType::left:
		{
			TileLeft = (stage->GetStageTileOffsetX(PlayerStage, PlayerTile) * 60) - 300;
			TileRight = (stage->GetStageTileOffsetX(PlayerStage, PlayerTile) * 60);
			TileUp = (stage->GetStageTileOffsetY(PlayerStage, PlayerTile) * 60);
			TileDown = (stage->GetStageTileOffsetY(PlayerStage, PlayerTile) * 60) + 300;
			break;
		}
		case BodyType::right:
		{
			TileLeft = (stage->GetStageTileOffsetX(PlayerStage, PlayerTile) * 60) + 300;
			TileRight = (stage->GetStageTileOffsetX(PlayerStage, PlayerTile) * 60) + 600;
			TileUp = (stage->GetStageTileOffsetY(PlayerStage, PlayerTile) * 60);
			TileDown = (stage->GetStageTileOffsetY(PlayerStage, PlayerTile) * 60) + 300;
			break;
		}
		case BodyType::up:
		{
			TileLeft = (stage->GetStageTileOffsetX(PlayerStage, PlayerTile) * 60);
			TileRight = (stage->GetStageTileOffsetX(PlayerStage, PlayerTile) * 60) + 300;
			TileUp = (stage->GetStageTileOffsetY(PlayerStage, PlayerTile) * 60) - 300;
			TileDown = (stage->GetStageTileOffsetY(PlayerStage, PlayerTile) * 60);
			break;
		}
		case BodyType::down:
		{
			TileLeft = (stage->GetStageTileOffsetX(PlayerStage, PlayerTile) * 60);
			TileRight = (stage->GetStageTileOffsetX(PlayerStage, PlayerTile) * 60) + 300;
			TileUp = (stage->GetStageTileOffsetY(PlayerStage, PlayerTile) * 60) + 300;
			TileDown = (stage->GetStageTileOffsetY(PlayerStage, PlayerTile) * 60) + 600;
			break;
		}

		}
	}
	else
	{
		return false;
	}

	if (TileLeft <= PressPos.x &&
		TileRight >= PressPos.x &&
		TileUp <= PressPos.y &&
		TileDown >= PressPos.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Player::IsMouseClickOpen(BodyType Direction)
{
	int PlayerStage = -1;
	int PlayerTile = -1;

	int PressStage = -1;
	int PressTile = -1;

	for (int i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (int j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			if (stage->IsPositionTile(CenterPosition, i, j))
			{
				PlayerStage = i;
				PlayerTile = j;
			}

			if (stage->IsPositionTile({ PressPos.x, PressPos.y, 0.0f }, i, j))
			{
				PressStage = i;
				PressTile = j;
			}
		}
	}

	float TileLeft = -1;
	float TileRight = -1;
	float TileUp = -1;
	float TileDown = -1;

	if (PlayerStage != -1 && PressStage != -1 &&
		PlayerStage == PressStage && PlayerTile == PressTile)
	{
		switch (Direction)
		{
		case BodyType::left:
		{
			TileLeft = (stage->GetStageTileOffsetX(PlayerStage, PlayerTile) * 60) - 300;
			TileRight = (stage->GetStageTileOffsetX(PlayerStage, PlayerTile) * 60);
			TileUp = (stage->GetStageTileOffsetY(PlayerStage, PlayerTile) * 60);
			TileDown = (stage->GetStageTileOffsetY(PlayerStage, PlayerTile) * 60) + 300;
			break;
		}
		case BodyType::right:
		{
			TileLeft = (stage->GetStageTileOffsetX(PlayerStage, PlayerTile) * 60) + 300;
			TileRight = (stage->GetStageTileOffsetX(PlayerStage, PlayerTile) * 60) + 600;
			TileUp = (stage->GetStageTileOffsetY(PlayerStage, PlayerTile) * 60);
			TileDown = (stage->GetStageTileOffsetY(PlayerStage, PlayerTile) * 60) + 300;
			break;
		}
		case BodyType::up:
		{
			TileLeft = (stage->GetStageTileOffsetX(PlayerStage, PlayerTile) * 60);
			TileRight = (stage->GetStageTileOffsetX(PlayerStage, PlayerTile) * 60) + 300;
			TileUp = (stage->GetStageTileOffsetY(PlayerStage, PlayerTile) * 60) - 300;
			TileDown = (stage->GetStageTileOffsetY(PlayerStage, PlayerTile) * 60);
			break;
		}
		case BodyType::down:
		{
			TileLeft = (stage->GetStageTileOffsetX(PlayerStage, PlayerTile) * 60);
			TileRight = (stage->GetStageTileOffsetX(PlayerStage, PlayerTile) * 60) + 300;
			TileUp = (stage->GetStageTileOffsetY(PlayerStage, PlayerTile) * 60) + 300;
			TileDown = (stage->GetStageTileOffsetY(PlayerStage, PlayerTile) * 60) + 600;
			break;
		}

		}
	}
	else
	{
		false;
	}

	if (TileLeft <= ReleasePos.x &&
		TileRight >= ReleasePos.x &&
		TileUp <= ReleasePos.y &&
		TileDown >= ReleasePos.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Player::IsPressInStage()
{
	for (int i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (int j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			//左上
			if (stage->IsPositionTile({ PressPos.x,PressPos.y,0.0f }, i, j))
			{
				return true;
			}
		}
	}

	return false;
}

void Player::BodySetUp(bool one, int one_type, bool two, int two_type, bool three, int three_type, bool four, int four_type)
{
	Body_One.IsActivate = one;
	Body_One.Body_Type = one_type;
	//Bodys[0].IsActivate = true;
	//Bodys[0].Body_Type = one_type;

	Body_Two.IsActivate = two;
	Body_Two.Body_Type = two_type;
	//Bodys[1].IsActivate = true;
	//Bodys[1].Body_Type = one_type;

	Body_Three.IsActivate = three;
	Body_Three.Body_Type = three_type;
	//Bodys[2].IsActivate = true;
	//Bodys[2].Body_Type = one_type;

	Body_Four.IsActivate = four;
	Body_Four.Body_Type = four_type;
	//Bodys[3].IsActivate = true;
	//Bodys[3].Body_Type = one_type;

	Body_One.setactivate(CenterPosition);
	Body_Two.setactivate(CenterPosition);
	Body_Three.setactivate(CenterPosition);
	Body_Four.setactivate(CenterPosition);

	for (int i = 0; i < 4; i++)
	{
		//Bodys[i].setactivate(CenterPosition);
	}

	CenterPosition.y += 1;
}

void Player::BodySetUp(const unsigned char foldCount[4])
{
	playerTile[0] = foldCount[0];
	playerTile[1] = foldCount[1];
	playerTile[2] = foldCount[2];
	playerTile[3] = foldCount[3];
	//static int bodyTile[4] = { 0 };
	static size_t j = 0;

	j = 0;

	BodySetUp(
		playerTile[0] != 0, BodyType::left,
		playerTile[1] != 0, BodyType::up,
		playerTile[2] != 0, BodyType::right,
		playerTile[3] != 0, BodyType::down);
}

bool Player::IsPlayerStageOnly()
{
	int onlycount = 0;

	for (int i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (int j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			if (stage->IsPositionTile(CenterPosition, i, j))
			{
				if (stage->GetStageTileDataSize(i) <= 1)
				{
					onlycount++;
				}
				else
				{
					onlycount = 0;
				}
			}
		}
	}

	if (onlycount > 0)
	{
		return true;
	}

	return false;
}

void Player::Fold()
{
	//足を上げ終わったら折る
	if (leg.FootIsAction == false)
	{
		if (IsLeftFold == true)
		{
			//body1のみ有効
			if (Body_One.IsActivate == true)
			{
				Body_One.Ease.addTime = 0.1f;
				Body_One.Ease.maxTime = 1.2f;
				Body_One.Ease.timeRate = 0.0f;

				//折る
				if (Body_One.IsFold == false && Body_One.IsOpen == true && Body_One.IsAction == false)
				{
					Body_One.Ease.isMove = true;
					Body_One.IsFold = true;
					Body_One.IsOpen = false;
					Body_One.IsAction = true;


					if (Body_Two.IsActivate == true)
					{
						if (Body_Two.IsFold == true)
						{
							Body_Two.AfterBodyFoldCount++;
						}
					}
					if (Body_Three.IsActivate == true)
					{
						if (Body_Three.IsFold == true)
						{
							Body_Three.AfterBodyFoldCount++;
						}
					}
					if (Body_Four.IsActivate == true)
					{
						if (Body_Four.IsFold == true)
						{
							Body_Four.AfterBodyFoldCount++;
						}
					}
				}
			}

			IsLeftFold = false;
		}
		if (IsUpFold == true)
		{
			//body2のみ有効
			if (Body_Two.IsActivate == true)
			{
				Body_Two.Ease.addTime = 0.1f;
				Body_Two.Ease.maxTime = 1.2f;
				Body_Two.Ease.timeRate = 0.0f;

				//折る
				if (Body_Two.IsFold == false && Body_Two.IsOpen == true && Body_Two.IsAction == false)
				{
					Body_Two.Ease.isMove = true;
					Body_Two.IsFold = true;
					Body_Two.IsOpen = false;
					Body_Two.IsAction = true;

					if (Body_One.IsActivate == true)
					{
						if (Body_One.IsFold == true)
						{
							Body_One.AfterBodyFoldCount++;
						}
					}
					if (Body_Three.IsActivate == true)
					{
						if (Body_Three.IsFold == true)
						{
							Body_Three.AfterBodyFoldCount++;
						}
					}
					if (Body_Four.IsActivate == true)
					{
						if (Body_Four.IsFold == true)
						{
							Body_Four.AfterBodyFoldCount++;
						}
					}
				}
			}

			IsUpFold = false;
		}
		if (IsRightFold == true)
		{
			//body3のみ有効
			if (Body_Three.IsActivate == true)
			{
				Body_Three.Ease.addTime = 0.1f;
				Body_Three.Ease.maxTime = 1.2f;
				Body_Three.Ease.timeRate = 0.0f;

				//折る
				if (Body_Three.IsFold == false && Body_Three.IsOpen == true && Body_Three.IsAction == false)
				{
					Body_Three.Ease.isMove = true;
					Body_Three.IsFold = true;
					Body_Three.IsOpen = false;
					Body_Three.IsAction = true;

					if (Body_One.IsActivate == true)
					{
						if (Body_One.IsFold == true)
						{
							Body_One.AfterBodyFoldCount++;
						}
					}
					if (Body_Two.IsActivate == true)
					{
						if (Body_Two.IsFold == true)
						{
							Body_Two.AfterBodyFoldCount++;
						}
					}
					if (Body_Four.IsActivate == true)
					{
						if (Body_Four.IsFold == true)
						{
							Body_Four.AfterBodyFoldCount++;
						}
					}
				}
			}

			IsRightFold = false;
			return;
		}
		if (IsDownFold == true)
		{
			//body4のみ有効
			if (Body_Four.IsActivate == true)
			{
				Body_Four.Ease.addTime = 0.1f;
				Body_Four.Ease.maxTime = 1.2f;
				Body_Four.Ease.timeRate = 0.0f;

				//折る
				if (Body_Four.IsFold == false && Body_Four.IsOpen == true && Body_Four.IsAction == false)
				{
					Body_Four.Ease.isMove = true;
					Body_Four.IsFold = true;
					Body_Four.IsOpen = false;
					Body_Four.IsAction = true;

					if (Body_One.IsActivate == true)
					{
						if (Body_One.IsFold == true)
						{
							Body_One.AfterBodyFoldCount++;
						}
					}
					if (Body_Two.IsActivate == true)
					{
						if (Body_Two.IsFold == true)
						{
							Body_Two.AfterBodyFoldCount++;
						}
					}
					if (Body_Three.IsActivate == true)
					{
						if (Body_Three.IsFold == true)
						{
							Body_Three.AfterBodyFoldCount++;
						}
					}
				}
			}

			IsDownFold = false;
		}
	}
}

void Player::Open()
{
	if (OpenCount >= 2)
	{
		//左を開く
		if (IsLeftOpen == true)
		{
			if (Body_One.IsActivate == true && Body_One.Body_Type == BodyType::left &&
				Body_One.IsFold == true)
			{
				Body_One.Ease.addTime = 0.1f;
				Body_One.Ease.maxTime = 1.2f;
				Body_One.Ease.timeRate = 0.0f;

				Body_One.Ease.isMove = true;
				Body_One.IsFold = false;
				Body_One.IsOpen = true;
				Body_One.IsAction = true;

				if (Body_Two.IsActivate == true && Body_Two.IsFold == true)
				{
					Body_Two.AfterBodyFoldCount--;
				}
				if (Body_Three.IsActivate == true && Body_Three.IsFold == true)
				{
					Body_Three.AfterBodyFoldCount--;
				}
				if (Body_Four.IsActivate == true && Body_Four.IsFold == true)
				{
					Body_Four.AfterBodyFoldCount--;
				}
			}

			//IsLeftOpen = false;
			return;
		}
		//上を開く
		if (IsUpOpen == true)
		{
			if (Body_Two.IsActivate == true && Body_Two.Body_Type == BodyType::up &&
				Body_Two.IsFold == true)
			{
				Body_Two.Ease.addTime = 0.1f;
				Body_Two.Ease.maxTime = 1.2f;
				Body_Two.Ease.timeRate = 0.0f;

				Body_Two.Ease.isMove = true;
				Body_Two.IsFold = false;
				Body_Two.IsOpen = true;
				Body_Two.IsAction = true;

				if (Body_One.IsActivate == true && Body_One.IsFold == true)
				{
					Body_One.AfterBodyFoldCount--;
				}
				if (Body_Three.IsActivate == true && Body_Three.IsFold == true)
				{
					Body_Three.AfterBodyFoldCount--;
				}
				if (Body_Four.IsActivate == true && Body_Four.IsFold == true)
				{
					Body_Four.AfterBodyFoldCount--;
				}
			}

			//IsUpOpen = false;
			return;
		}
		//右を開く
		if (IsRightOpen == true)
		{
			if (Body_Three.IsActivate == true && Body_Three.Body_Type == BodyType::right &&
				Body_Three.IsFold == true)
			{
				Body_Three.Ease.addTime = 0.1f;
				Body_Three.Ease.maxTime = 1.2f;
				Body_Three.Ease.timeRate = 0.0f;

				Body_Three.Ease.isMove = true;
				Body_Three.IsFold = false;
				Body_Three.IsOpen = true;
				Body_Three.IsAction = true;

				if (Body_One.IsActivate == true && Body_One.IsFold == true)
				{
					Body_One.AfterBodyFoldCount--;
				}
				if (Body_Two.IsActivate == true && Body_Two.IsFold == true)
				{
					Body_Two.AfterBodyFoldCount--;
				}
				if (Body_Four.IsActivate == true && Body_Four.IsFold == true)
				{
					Body_Four.AfterBodyFoldCount--;
				}
			}

			//IsRightOpen = false;
			return;
		}
		//下を開く
		if (IsDownOpen == true)
		{
			if (Body_Four.IsActivate == true && Body_Four.Body_Type == BodyType::down &&
				Body_Four.IsFold == true)
			{
				Body_Four.Ease.addTime = 0.1f;
				Body_Four.Ease.maxTime = 1.2f;
				Body_Four.Ease.timeRate = 0.0f;

				Body_Four.Ease.isMove = true;
				Body_Four.IsFold = false;
				Body_Four.IsOpen = true;
				Body_Four.IsAction = true;

				if (Body_One.IsActivate == true && Body_One.IsFold == true)
				{
					Body_One.AfterBodyFoldCount--;
				}
				if (Body_Two.IsActivate == true && Body_Two.IsFold == true)
				{
					Body_Two.AfterBodyFoldCount--;
				}
				if (Body_Three.IsActivate == true && Body_Three.IsFold == true)
				{
					Body_Three.AfterBodyFoldCount--;
				}
			}

			//IsDownOpen = false;
			return;
		}
	}
}

void Player::IsHitPlayerBody()
{
	//ステージの数
	size_t i = 0;
	//タイルの数
	size_t j = 0;

	float FaceLeft = CenterPosition.x - 25;
	float FaceUp = CenterPosition.y - 25;
	float FaceRight = CenterPosition.x + 25;
	float FaceDown = CenterPosition.y + 25;
	float FaceAndLegDown = CenterPosition.y + 33;

	//上下左右(プレイヤーの顔)
	int left_mapchip = (int)(FaceLeft - stage->offset.x) / 60;
	int up_mapchip = (int)(FaceUp - stage->offset.y) / 60;
	int right_mapchip = (int)(FaceRight - stage->offset.x) / 60;
	int down_mapchip = (int)(FaceDown - stage->offset.y) / 60;
	int FaceAndLegdown_mapchip = (int)(FaceAndLegDown - stage->offset.y) / 60;

	//タイル内のマップチップ座標
	int left_mapchip_tile;
	int up_mapchip_tile;
	int right_mapchip_tile;
	int down_mapchip_tile;
	int FaceAndLegdown_mapchip_tile;

	//マップチップの場所(共通)
	int MapchipPos = 0;

	//マップチップの場所(ゴール用)
	int MapchipPos_Goal[3] = { 0 };

	//押し出す方向を決めるための距離
	float BuriedX = 0;
	float BuriedY = 0;

	int FallCount = 0;

	int JumpCountLeft = 0;
	int jumpCountRight = 0;

	if ((CenterPosition.x - 25) <= stage->offset.x)
	{
		CenterPosition.x = 25;
	}
	if ((CenterPosition.y - 25) <= stage->offset.y)
	{
		CenterPosition.y = 25;
	}

	//顔の四隅との当たり判定
	for (i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			//左上
			if (stage->IsPositionTile({ FaceLeft, CenterPosition.y - 30 ,0.0f }, i, j))
			{
				left_mapchip_tile = left_mapchip % stage->GetStageTileWidth(i, j);
				up_mapchip_tile = up_mapchip % stage->GetStageTileHeight(i, j);

				MapchipPos = (up_mapchip_tile)*stage->GetStageTileWidth(i, j) + (left_mapchip_tile);
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, MapchipPos)))
				{
					BuriedX = (left_mapchip * 60) - (CenterPosition.x - 30);
					BuriedY = (up_mapchip * 60) - (CenterPosition.y - 30);

					if (BuriedX > BuriedY)
					{
						CenterPosition.y = static_cast<float>(up_mapchip + 1) * stage->blockSize + 25.0f;
					}
					else if (BuriedX < BuriedY)
					{
						CenterPosition.x = static_cast<float>(left_mapchip + 1) * stage->blockSize + 25.0f;
						JumpCountLeft += IsLeft;
					}
				}
				if (up_mapchip_tile > 0)
				{
					MapchipPos = (up_mapchip_tile - 1) * stage->GetStageTileWidth(i, j) + (left_mapchip_tile);
					if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, MapchipPos)))
					{
						IsUpBlocked = false;
					}
					else
					{
						IsUpBlocked = true;
					}
				}
			}
			//左下
			if (stage->IsPositionTile({ FaceLeft, FaceAndLegDown, 0.0f }, i, j))
			{
				left_mapchip_tile = left_mapchip % stage->GetStageTileWidth(i, j);
				FaceAndLegdown_mapchip_tile = FaceAndLegdown_mapchip % stage->GetStageTileHeight(i, j);

				MapchipPos = (FaceAndLegdown_mapchip_tile)*stage->GetStageTileWidth(i, j) + (left_mapchip_tile);
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, MapchipPos)))
				{
					BuriedX = (left_mapchip * 60) - (CenterPosition.x - 30);
					BuriedY = ((CenterPosition.y + 29) - 60) - (FaceAndLegdown_mapchip * 60);

					if (BuriedX > BuriedY)
					{
						CenterPosition.y = static_cast<float>(FaceAndLegdown_mapchip * stage->blockSize) - 33.0f;
						FallCount++;
						IsInitJump = false;
					}
					else if (BuriedX < BuriedY)
					{
						CenterPosition.x = static_cast<float>(left_mapchip + 1) * stage->blockSize + 25.0f;
						JumpCountLeft += IsLeft;
					}
				}
			}
			//右上
			if (stage->IsPositionTile({ FaceRight, CenterPosition.y - 30, 0.0f }, i, j))
			{
				right_mapchip_tile = right_mapchip % stage->GetStageTileWidth(i, j);
				up_mapchip_tile = up_mapchip % stage->GetStageTileHeight(i, j);

				MapchipPos = (up_mapchip_tile)*stage->GetStageTileWidth(i, j) + (right_mapchip_tile);
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, MapchipPos)))
				{
					BuriedX = ((CenterPosition.x + 29) - 60) - (right_mapchip * 60);
					BuriedY = (up_mapchip * 60) - (CenterPosition.y - 30);

					if (BuriedX > BuriedY)
					{
						CenterPosition.y = static_cast<float>(up_mapchip + 1) * stage->blockSize + 25.0f;
					}
					else if (BuriedX < BuriedY)
					{
						CenterPosition.x = static_cast<float>(right_mapchip * stage->blockSize) - 26.0f;
						jumpCountRight += IsRight;
					}
				}
				if (up_mapchip_tile > 0)
				{
					MapchipPos = (up_mapchip_tile - 1) * stage->GetStageTileWidth(i, j) + (right_mapchip_tile);
					if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, MapchipPos)))
					{
						IsUpBlocked = false;
					}
					else
					{
						IsUpBlocked = true;
					}
				}

			}
			//右下
			if (stage->IsPositionTile({ FaceRight, FaceAndLegDown, 0.0f }, i, j))
			{
				right_mapchip_tile = right_mapchip % stage->GetStageTileWidth(i, j);
				FaceAndLegdown_mapchip_tile = FaceAndLegdown_mapchip % stage->GetStageTileHeight(i, j);

				MapchipPos = (FaceAndLegdown_mapchip_tile)*stage->GetStageTileWidth(i, j) + (right_mapchip_tile);
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, MapchipPos)))
				{
					BuriedX = ((CenterPosition.x + 29) - 60) - (right_mapchip * 60);
					BuriedY = ((CenterPosition.y + 29) - 60) - (FaceAndLegdown_mapchip * 60);

					if (BuriedX > BuriedY)
					{
						CenterPosition.y = static_cast<float>(FaceAndLegdown_mapchip * stage->blockSize) - 33.0f;
						FallCount++;
						IsInitJump = false;
					}
					else if (BuriedX < BuriedY)
					{
						CenterPosition.x = static_cast<float>(right_mapchip * stage->blockSize) - 26.0f;
						jumpCountRight += IsRight;
					}
				}
			}

			//ゴール判定
			if (stage->IsPositionTile(CenterPosition, i, j))
			{
				left_mapchip_tile = left_mapchip % stage->GetStageTileWidth(i, j);
				up_mapchip_tile = up_mapchip % stage->GetStageTileHeight(i, j);
				right_mapchip_tile = right_mapchip % stage->GetStageTileWidth(i, j);
				down_mapchip_tile = down_mapchip % stage->GetStageTileHeight(i, j);

				//左上
				MapchipPos = up_mapchip_tile * stage->GetStageTileWidth(i, j) + (left_mapchip_tile);
				MapchipPos_Goal[0] = up_mapchip_tile * stage->GetStageTileWidth(i, j) + (right_mapchip_tile);
				MapchipPos_Goal[1] = down_mapchip_tile * stage->GetStageTileWidth(i, j) + (left_mapchip_tile);
				MapchipPos_Goal[2] = down_mapchip_tile * stage->GetStageTileWidth(i, j) + (right_mapchip_tile);

				if (Player_IsAction == false)
				{
					int GoalCount = 0;
					if (stage->GetStageMapchip(i, j, MapchipPos) == MapchipData::GOAL)
					{
						GoalCount++;
					}
					//右上
					if (stage->GetStageMapchip(i, j, MapchipPos_Goal[0]) == MapchipData::GOAL)
					{
						GoalCount++;
					}
					//左下
					if (stage->GetStageMapchip(i, j, MapchipPos_Goal[1]) == MapchipData::GOAL)
					{
						GoalCount++;
					}
					//右下
					if (stage->GetStageMapchip(i, j, MapchipPos_Goal[2]) == MapchipData::GOAL)
					{
						GoalCount++;
					}

					if (GoalCount >= 4)
					{
						IsGoal = true;
					}
					else
					{
						IsGoal = false;
					}
				}
			}
		}
	}

	bool DiagonallyUpLeft = false;
	bool DiagonallyUpRight = false;

	//進む方向の斜め上にブロックがあるかどうか
	for (i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			if (stage->IsPositionTile({ FaceLeft - 20, FaceUp - 20, 0.0f }, i, j))
			{
				left_mapchip_tile = (left_mapchip - 1) % stage->GetStageTileWidth(i, j);
				up_mapchip_tile = (up_mapchip - 1) % stage->GetStageTileHeight(i, j);

				MapchipPos = up_mapchip_tile * stage->GetStageTileWidth(i, j) + left_mapchip_tile;
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, MapchipPos)) &&
					Player::Get()->IsLeft == true)
				{
					DiagonallyUpLeft = true;
				}
			}

			if (stage->IsPositionTile({ FaceRight + 20, FaceUp - 20, 0.0f }, i, j))
			{
				right_mapchip_tile = (right_mapchip) % stage->GetStageTileWidth(i, j);
				up_mapchip_tile = (up_mapchip - 1) % stage->GetStageTileHeight(i, j);

				MapchipPos = up_mapchip_tile * stage->GetStageTileWidth(i, j) + right_mapchip_tile;
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, MapchipPos)) &&
					Player::Get()->IsRight == true)
				{
					DiagonallyUpRight = true;
				}
			}
		}
	}

	if ((JumpCountLeft <= 1 && IsLeft && Body_Two.IsHitLeft) || (jumpCountRight <= 1 && IsRight && Body_Two.IsHitRight))
	{
		/*IsJump = false;
		IsWalk = false;
		IsInitJump = true;*/
	}
	else if (((JumpCountLeft > 0 && DiagonallyUpLeft == false) || (jumpCountRight > 0 && DiagonallyUpRight == false)) && IsInitJump == false)
	{
		/*IsJump = true;
		FallSpeed = -5.6f;
		IsWalk = true;
		IsInitJump = true;*/
	}

	if (FallCount > 0)
	{
		IsFaceFall = false;
	}
	else
	{
		IsFaceFall = true;
	}
}

bool Player::IsFall()
{
	int FallCount = 0;

	if (Body_One.IsActivate == true && Body_One.BodyIsFall == false)
	{
		FallCount++;
	}
	if (Body_Two.IsActivate == true && Body_Two.BodyIsFall == false)
	{
		FallCount++;
	}
	if (Body_Three.IsActivate == true && Body_Three.BodyIsFall == false)
	{
		FallCount++;
	}
	if (Body_Four.IsActivate == true && Body_Four.BodyIsFall == false)
	{
		FallCount++;
	}
	if (IsFaceFall == false)
	{
		FallCount++;
	}

	if (FallCount > 0)
	{
		if (IsDownBody == true && Body_Four.BodyIsFall)
		{
			IsJumpOnly = true;
		}
		else if (IsDownBody == false && IsFaceFall)
		{
			IsJumpOnly = true;
		}
		else
		{
			IsJumpOnly = false;
		}
		return false;
	}
	else
	{
		return true;
	}
}

void Player::SetBodyStatus(bool arrangement[4])
{
	arrangement[0] = (Body_One.IsActivate && Body_One.IsOpen) || (Body_Three.IsActivate && Body_Three.IsOpen);
	arrangement[1] = (Body_Two.IsActivate && Body_Two.IsOpen) || (Body_Four.IsActivate && Body_Four.IsOpen);
	arrangement[2] = (Body_One.IsActivate && Body_One.IsOpen) || (Body_Three.IsActivate && Body_Three.IsOpen);
	arrangement[3] = (Body_Two.IsActivate && Body_Two.IsOpen) || (Body_Four.IsActivate && Body_Four.IsOpen);
}

bool Player::IsReverseHitFace(const unsigned char& direction)
{
	//顔の四辺
	float BodyLeft;
	float BodyRight;
	float BodyUp;
	float BodyDown;

	BodyLeft = CenterPosition.x - 25;
	BodyRight = CenterPosition.x + 25;
	BodyUp = CenterPosition.y - 25;
	BodyDown = CenterPosition.y + 25;

	//上下左右(プレイヤーの顔)
	int left_mapchip = (int)(BodyLeft - stage->offset.x) / 60;
	int up_mapchip = (int)(BodyUp - stage->offset.y) / 60;
	int right_mapchip = (int)(BodyRight - stage->offset.x) / 60;
	int down_mapchip = (int)(BodyDown - stage->offset.y) / 60;

	//タイル内のマップチップ座標
	int left_mapchip_tile;
	int up_mapchip_tile;
	int right_mapchip_tile;
	int down_mapchip_tile;

	//反転したマップチップたち
	char* ReverseMapchips = { 0 };

	//折れるかどうか(プレイヤーの中心座標)
	int CenterPositionFold = stage->FoldSimulation(CenterPosition, direction, &ReverseMapchips);

	//playerの中心座標
	if (CenterPositionFold == -1)
	{
		return false;
	}

	//マップチップの場所(25個)
	int MapchipPos = 0;

	//反転したブロックマップチップと顔の四隅との判定
	for (int i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (int j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			//左上
			if (stage->IsPositionTile({ CenterPosition.x - 25, CenterPosition.y - 30, 0.0f }, i, j))
			{
				left_mapchip_tile = left_mapchip % stage->GetStageTileWidth(i, j);
				up_mapchip_tile = up_mapchip % stage->GetStageTileHeight(i, j);

				MapchipPos = (up_mapchip_tile)*stage->GetStageTileWidth(i, j) + (left_mapchip_tile);

				if (stage->IsMapchipBlocks(ReverseMapchips[MapchipPos]))
				{
					return true;
				}
			}
			//左下
			if (stage->IsPositionTile({ CenterPosition.x - 25, CenterPosition.y + 33, 0.0f }, i, j))
			{
				left_mapchip_tile = left_mapchip % stage->GetStageTileWidth(i, j);
				down_mapchip_tile = down_mapchip % stage->GetStageTileHeight(i, j);

				MapchipPos = (down_mapchip_tile)*stage->GetStageTileWidth(i, j) + (left_mapchip_tile);

				if (stage->IsMapchipBlocks(ReverseMapchips[MapchipPos]))
				{
					return true;
				}
			}
			//右上
			if (stage->IsPositionTile({ CenterPosition.x + 25, CenterPosition.y - 30, 0.0f }, i, j))
			{
				right_mapchip_tile = right_mapchip % stage->GetStageTileWidth(i, j);
				up_mapchip_tile = up_mapchip % stage->GetStageTileHeight(i, j);

				MapchipPos = (up_mapchip_tile)*stage->GetStageTileWidth(i, j) + (right_mapchip_tile);

				if (stage->IsMapchipBlocks(ReverseMapchips[MapchipPos]))
				{
					return true;
				}
			}
			//右下
			if (stage->IsPositionTile({ CenterPosition.x + 25, CenterPosition.y + 33, 0.0f }, i, j))
			{
				right_mapchip_tile = right_mapchip % stage->GetStageTileWidth(i, j);
				down_mapchip_tile = down_mapchip % stage->GetStageTileHeight(i, j);

				MapchipPos = (down_mapchip_tile)*stage->GetStageTileWidth(i, j) + (right_mapchip_tile);

				if (stage->IsMapchipBlocks(ReverseMapchips[MapchipPos]))
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool Player::IsDirectionFoldAll(BodyType foldtype)
{
	int BodyCanFoldCount = 0;

	if (Body_One.IsActivate == true && Body_One.IsReverseHitBody(foldtype) == true && Body_One.Body_Type != foldtype)
	{
		BodyCanFoldCount++;
	}
	if (Body_Two.IsActivate == true && Body_Two.IsReverseHitBody(foldtype) == true && Body_Two.Body_Type != foldtype)
	{
		BodyCanFoldCount++;
	}
	if (Body_Three.IsActivate == true && Body_Three.IsReverseHitBody(foldtype) == true && Body_Three.Body_Type != foldtype)
	{
		BodyCanFoldCount++;
	}
	if (Body_Four.IsActivate == true && Body_Four.IsReverseHitBody(foldtype) == true && Body_Four.Body_Type != foldtype)
	{
		BodyCanFoldCount++;
	}

	bool ReverseHitFace = IsReverseHitFace(foldtype);

	if (ReverseHitFace == true || BodyCanFoldCount > 0)
	{
		return false;
	}
	else if (ReverseHitFace == false && BodyCanFoldCount == 0)
	{
		return true;
	}
}

int Player::ActivateBodyCount()
{
	int count = 0;

	if (Body_One.IsActivate == true)
	{
		count++;
	}
	if (Body_Two.IsActivate == true)
	{
		count++;
	}
	if (Body_Three.IsActivate == true)
	{
		count++;
	}
	if (Body_Four.IsActivate == true)
	{
		count++;
	}

	return count;
}

bool Player::IsBodysFold(BodyType FoldType)
{
	for (int i = 0; i < 4; i++)
	{
		/*if (Bodys[i].IsActivate == true &&
			Bodys[i].Body_Type == FoldType &&
			Bodys[i].IsOpen == true)
		{
			return true;
		}*/
	}
	return false;
}

bool Player::IsBodysOpen(BodyType OpenType)
{
	for (int i = 0; i < 4; i++)
	{
		/*if (Bodys[i].IsActivate == true &&
			Bodys[i].Body_Type == OpenType &&
			Bodys[i].IsFold == true &&
			Bodys[i].AfterBodyFoldCount <= 0)
		{
			return true;
		}*/
	}
	return false;
}

void Player::IsdownBody()
{
	int DownBodyCount = 0;

	if (Body_Two.IsActivate && Body_Two.Body_Type == BodyType::down && Body_Two.IsFold == false)
	{
		DownBodyCount++;
	}

	if (Body_Four.IsActivate && Body_Four.Body_Type == BodyType::down && Body_Four.IsFold == false)
	{
		DownBodyCount++;
	}

	if (DownBodyCount > 0)
	{
		IsDownBody = true;
	}
	else
	{
		IsDownBody = false;
	}

	return;
}

void Player::IsAroundBlock()
{
	//中心座標をブロックサイズで割った数
	int Center_X_mapchip = (int)(CenterPosition.x - stage->offset.x) / 60;
	int Center_Y_mapchip = (int)(CenterPosition.y - stage->offset.x) / 60;

	//体の四辺
	float NextLeft = CenterPosition.x - PlayerBody::BodySize;
	float NextRight = CenterPosition.x + PlayerBody::BodySize;
	float NextUp = CenterPosition.y - PlayerBody::BodySize;
	float NextDown = CenterPosition.y + PlayerBody::BodySize;

	//四辺をブロックサイズで割った数
	int NextLeft_mapchip = (int)(NextLeft - stage->offset.x) / 60;
	int NextUp_mapchip = (int)(NextUp - stage->offset.y) / 60;
	int NextRight_mapchip = (int)(NextRight - stage->offset.x) / 60;
	int NextDown_mapchip = (int)(NextDown - stage->offset.y) / 60;

	//タイル内のマップチップ座標
	int X_mapchip_tile;
	int Y_mapchip_tile;

	//マップチップの座標
	int mapchipPos = 0;

	for (int i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (int j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			//左隣
			if (stage->IsPositionTile({ NextLeft,CenterPosition.y,0.0 }, i, j))
			{
				X_mapchip_tile = NextLeft_mapchip % stage->GetStageTileWidth(i, j);
				Y_mapchip_tile = Center_Y_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = Y_mapchip_tile * stage->GetStageTileWidth(i, j) + X_mapchip_tile;
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, mapchipPos)))
				{
					IsLeftBlockFace = true;
				}
				else
				{
					IsLeftBlockFace = false;
				}
			}

			//右隣
			if (stage->IsPositionTile({ NextRight,CenterPosition.y,0.0 }, i, j))
			{
				X_mapchip_tile = NextRight_mapchip % stage->GetStageTileWidth(i, j);
				Y_mapchip_tile = Center_Y_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = Y_mapchip_tile * stage->GetStageTileWidth(i, j) + X_mapchip_tile;
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, mapchipPos)))
				{
					IsRightBlockFace = true;
				}
				else
				{
					IsRightBlockFace = false;
				}
			}

			//上隣
			if (stage->IsPositionTile({ CenterPosition.x,NextUp,0.0 }, i, j))
			{
				X_mapchip_tile = Center_X_mapchip % stage->GetStageTileWidth(i, j);
				Y_mapchip_tile = NextUp_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = Y_mapchip_tile * stage->GetStageTileWidth(i, j) + X_mapchip_tile;
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, mapchipPos)))
				{
					IsUpBlockFace = true;
				}
				else
				{
					IsUpBlockFace = false;
				}
			}

			//下隣
			if (stage->IsPositionTile({ CenterPosition.x,NextDown,0.0 }, i, j))
			{
				X_mapchip_tile = Center_X_mapchip % stage->GetStageTileWidth(i, j);
				Y_mapchip_tile = NextDown_mapchip % stage->GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = Y_mapchip_tile * stage->GetStageTileWidth(i, j) + X_mapchip_tile;
				if (stage->IsMapchipBlocks(stage->GetStageMapchip(i, j, mapchipPos)))
				{
					IsDownBlockFace = true;
				}
				else
				{
					IsDownBlockFace = false;
				}
			}
		}
	}
}

void Player::IsSlideBlock()
{
	int NextLeftCount = 0;
	int NextRightCount = 0;
	int NextUpCount = 0;
	int NextDownCount = 0;

	IsUpSlide = true;
	IsDownSlide = true;
	IsLeftSlide = true;
	IsRightSlide = true;


	//上方向
	if ((Body_One.IsActivate && Body_One.IsDownBlock) || (Body_Three.IsActivate && Body_Three.IsDownBlock))
	{
		if (IsUpBlockFace == true ||
			(Body_Two.IsActivate && Body_Two.IsDownBlock && Body_Two.BodyDistance == 1))
		{
			IsUpSlide = false;
		}
	}

	//下方向
	if ((Body_One.IsActivate && Body_One.IsUpBlock) || (Body_Three.IsActivate && Body_Three.IsUpBlock))
	{
		if (IsDownBlockFace == true ||
			(Body_Four.IsActivate && Body_Four.IsDownBlock && Body_Four.BodyDistance == 1))
		{
			IsDownSlide = false;
		}
	}

	//左方向
	if ((Body_Two.IsActivate && Body_Two.IsRightBlock) || (Body_Four.IsActivate && Body_Four.IsRightBlock))
	{
		if (IsLeftBlockFace == true ||
			(Body_One.IsActivate && Body_One.IsLeftBlock && Body_One.BodyDistance == 1))
		{
			IsLeftSlide = false;
		}
	}

	//右方向
	if ((Body_Two.IsActivate && Body_Two.IsLeftBlock) || (Body_Four.IsActivate && Body_Four.IsLeftBlock))
	{
		if (IsRightBlockFace == true ||
			(Body_Three.IsActivate && Body_Three.IsRightBlock && Body_Three.BodyDistance == 1))
		{
			IsRightSlide = false;
		}
	}
}
