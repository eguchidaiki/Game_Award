#include "InputManger.h"

namespace
{
	static const char decisionKey = DIK_RETURN; //決定キー
	static const char cancelKey = DIK_BACK;     //キャンセルキー

	static const XPAD_INPUT_CODE decisionXpad = XPAD_BUTTON_A; //決定ボタン
	static const XPAD_INPUT_CODE cancelXpad = XPAD_BUTTON_B;   //キャンセルボタン
}

bool InputManger::isMenu = false;
bool InputManger::isInversionX = false;
bool InputManger::isInversionY = false;
DirectX::XMFLOAT2 InputManger::pressPos = { 0.0f, 0.0f };
DirectX::XMFLOAT2 InputManger::releasePos = { 0.0f, 0.0f };
DirectX::XMFLOAT2 InputManger::dragDis = { 0.0f, 0.0f };

InputManger::InputManger()
{
}

InputManger* InputManger::Get()
{
	static InputManger ins;
	return &ins;
}

void InputManger::Update()
{
	Input::StartGetInputState();
	MouseInputUpdate();
}

void InputManger::MouseInputUpdate()
{
	//マウス左ボタンを押したときの座標
	if (Input::isMouseClickTrigger(0))
	{
		pressPos = Input::getMousePos();
		//PressCount = 0;
	}

	//マウスを押している間はカウントを進める
	if (Input::isMouseClicking(0))
	{
		//PressCount++;
	}

	//マウス左ボタンを離したときの座標
	if (Input::isMouseClicked(0))
	{
		releasePos = Input::getMousePos();
	}

	dragDis = { releasePos.x - pressPos.x , releasePos.y - pressPos.y };
}

bool InputManger::Escape()
{
	return (isMenu == false && Input::isKey(DIK_ESCAPE)) || Input::isXpadButtonPushing(XPAD_BUTTON_OPTION_L);
}

bool InputManger::EscapeTrigger()
{
	return (isMenu == false && Input::isKeyTrigger(DIK_ESCAPE)) || Input::isXpadButtonPushTrigger(XPAD_BUTTON_OPTION_L);
}

bool InputManger::EscapeRelease()
{
	return (isMenu == false && Input::isKeyReleased(DIK_ESCAPE)) || Input::isXpadButtonPushed(XPAD_BUTTON_OPTION_L);
}

bool InputManger::Up()
{
	return Input::isKey(DIK_W) || Input::isXpadButtonPushing(XPAD_BUTTON_CROSS_UP) || Input::isXpadStickTilt(XPAD_LSTICK_DIR_UP);
}

bool InputManger::UpTrigger()
{
	return Input::isKeyTrigger(DIK_W) || Input::isXpadButtonPushTrigger(XPAD_BUTTON_CROSS_UP) || Input::isXpadStickTiltTrigger(XPAD_LSTICK_DIR_UP);
}

bool InputManger::UpRelease()
{
	return Input::isKeyReleased(DIK_W) || Input::isXpadButtonPushed(XPAD_BUTTON_CROSS_UP) || Input::isXpadStickTiltReleased(XPAD_LSTICK_DIR_UP);
}

bool InputManger::Down()
{
	return Input::isKey(DIK_S) || Input::isXpadButtonPushing(XPAD_BUTTON_CROSS_DOWN) || Input::isXpadStickTilt(XPAD_LSTICK_DIR_DOWN);
}

bool InputManger::DownTrigger()
{
	return Input::isKeyTrigger(DIK_S) || Input::isXpadButtonPushTrigger(XPAD_BUTTON_CROSS_DOWN) || Input::isXpadStickTiltTrigger(XPAD_LSTICK_DIR_DOWN);
}

bool InputManger::DownRelease()
{
	return Input::isKeyReleased(DIK_S) || Input::isXpadButtonPushed(XPAD_BUTTON_CROSS_DOWN) || Input::isXpadStickTiltReleased(XPAD_LSTICK_DIR_DOWN);
}

bool InputManger::Left()
{
	return Input::isKey(DIK_A) || Input::isXpadButtonPushing(XPAD_BUTTON_CROSS_LEFT) || Input::isXpadStickTilt(XPAD_LSTICK_DIR_LEFT);
}

bool InputManger::LeftTrigger()
{
	return Input::isKeyTrigger(DIK_A) || Input::isXpadButtonPushTrigger(XPAD_BUTTON_CROSS_LEFT) || Input::isXpadStickTiltTrigger(XPAD_LSTICK_DIR_LEFT);
}

bool InputManger::LeftRelease()
{
	return Input::isKeyReleased(DIK_A) || Input::isXpadButtonPushed(XPAD_BUTTON_CROSS_LEFT) || Input::isXpadStickTiltReleased(XPAD_LSTICK_DIR_LEFT);
}

bool InputManger::Right()
{
	return Input::isKey(DIK_D) || Input::isXpadButtonPushing(XPAD_BUTTON_CROSS_RIGHT) || Input::isXpadStickTilt(XPAD_LSTICK_DIR_RIGHT);
}

bool InputManger::RightTrigger()
{
	return Input::isKeyTrigger(DIK_D) || Input::isXpadButtonPushTrigger(XPAD_BUTTON_CROSS_RIGHT) || Input::isXpadStickTiltTrigger(XPAD_LSTICK_DIR_RIGHT);
}

bool InputManger::RightRelease()
{
	return Input::isKeyReleased(DIK_D) || Input::isXpadButtonPushed(XPAD_BUTTON_CROSS_RIGHT) || Input::isXpadStickTiltReleased(XPAD_LSTICK_DIR_RIGHT);
}

bool InputManger::SubUp()
{
	return Input::isKey(DIK_UP) || Input::isXpadStickTilt(XPAD_RSTICK_DIR_UP);
}

bool InputManger::SubUpTrigger()
{
	return Input::isKeyTrigger(DIK_UP) || Input::isXpadStickTiltTrigger(XPAD_RSTICK_DIR_UP);
}

bool InputManger::SubUpRelease()
{
	return Input::isKeyReleased(DIK_UP) || Input::isXpadStickTiltReleased(XPAD_RSTICK_DIR_UP);
}

bool InputManger::SubDown()
{
	return Input::isKey(DIK_DOWN) || Input::isXpadStickTilt(XPAD_RSTICK_DIR_DOWN);
}

bool InputManger::SubDownTrigger()
{
	return Input::isKeyTrigger(DIK_DOWN) || Input::isXpadStickTiltTrigger(XPAD_RSTICK_DIR_DOWN);
}

bool InputManger::SubDownRelease()
{
	return Input::isKeyReleased(DIK_DOWN) || Input::isXpadStickTiltReleased(XPAD_RSTICK_DIR_DOWN);
}

bool InputManger::SubLeft()
{
	return Input::isKey(DIK_LEFT) || Input::isXpadStickTilt(XPAD_RSTICK_DIR_LEFT);
}

bool InputManger::SubLeftTrigger()
{
	return Input::isKeyTrigger(DIK_LEFT) || Input::isXpadStickTiltTrigger(XPAD_RSTICK_DIR_LEFT);
}

bool InputManger::SubLeftRelease()
{
	return Input::isKeyReleased(DIK_LEFT) || Input::isXpadStickTiltReleased(XPAD_RSTICK_DIR_LEFT);
}

bool InputManger::SubRight()
{
	return Input::isKey(DIK_RIGHT) || Input::isXpadStickTilt(XPAD_RSTICK_DIR_RIGHT);
}

bool InputManger::SubRightTrigger()
{
	return Input::isKeyTrigger(DIK_RIGHT) || Input::isXpadStickTiltTrigger(XPAD_RSTICK_DIR_RIGHT);
}

bool InputManger::SubRightRelease()
{
	return Input::isKeyReleased(DIK_RIGHT) || Input::isXpadStickTiltReleased(XPAD_RSTICK_DIR_RIGHT);
}

bool InputManger::Decision()
{
	return Input::isKey(decisionKey) || Input::isXpadButtonPushing(decisionXpad);
}

bool InputManger::DecisionTrigger()
{
	return Input::isKeyTrigger(decisionKey) || Input::isXpadButtonPushTrigger(decisionXpad);
}

bool InputManger::DecisionRelease()
{
	return Input::isKeyReleased(decisionKey) || Input::isXpadButtonPushed(decisionXpad);
}

bool InputManger::Cancel()
{
	return Input::isKey(cancelKey) || Input::isXpadButtonPushing(cancelXpad);
}

bool InputManger::CancelTrigger()
{
	return Input::isKeyTrigger(cancelKey) || Input::isXpadButtonPushTrigger(cancelXpad);
}

bool InputManger::CancelRelease()
{
	return Input::isKeyReleased(cancelKey) || Input::isXpadButtonPushed(cancelXpad);
}

bool InputManger::Reset()
{
	return Input::isKey(DIK_R) || Input::isXpadButtonPushing(XPAD_BUTTON_X);
}

bool InputManger::ResetTrigger()
{
	return Input::isKeyTrigger(DIK_R) || Input::isXpadButtonPushTrigger(XPAD_BUTTON_X);
}

bool InputManger::ResetRelease()
{
	return Input::isKeyReleased(DIK_R) || Input::isXpadButtonPushed(XPAD_BUTTON_X);
}

bool InputManger::Menu()
{
	return (isMenu && Input::isKey(DIK_ESCAPE)) || Input::isXpadButtonPushing(XPAD_BUTTON_OPTION_R);
}

bool InputManger::MenuTrigger()
{
	return (isMenu && Input::isKeyTrigger(DIK_ESCAPE)) || Input::isXpadButtonPushTrigger(XPAD_BUTTON_OPTION_R);
}

bool InputManger::MenuRelease()
{
	return (isMenu && Input::isKeyReleased(DIK_ESCAPE)) || Input::isXpadButtonPushed(XPAD_BUTTON_OPTION_R);
}

bool InputManger::Jump()
{
	return Input::isKey(DIK_W) || Input::isXpadButtonPushing(XPAD_BUTTON_A);
}

bool InputManger::JumpTrigger()
{
	return Input::isKeyTrigger(DIK_W) || Input::isXpadButtonPushTrigger(XPAD_BUTTON_A);
}

bool InputManger::JumpRelease()
{
	return Input::isKeyReleased(DIK_W) || Input::isXpadButtonPushed(XPAD_BUTTON_A);
}

bool InputManger::FoldLeft()
{
	if (isInversionX)
	{
		return SubLeft();
	}
	return SubRight();
}

bool InputManger::FoldUp()
{
	if (isInversionY)
	{
		return SubUp();
	}
	return SubDown();
}

bool InputManger::FoldRight()
{
	if (isInversionX)
	{
		return SubRight();
	}
	return SubLeft();
}

bool InputManger::FoldDown()
{
	if (isInversionY)
	{
		return SubDown();
	}
	return SubUp();
}

bool InputManger::FoldLeftTrigger()
{
	if (isInversionX)
	{
		return SubLeftTrigger();
	}
	return SubRightTrigger();
}

bool InputManger::FoldUpTrigger()
{
	if (isInversionY)
	{
		return SubUpTrigger();
	}
	return SubDownTrigger();
}

bool InputManger::FoldRightTrigger()
{
	if (isInversionX)
	{
		return SubRightTrigger();
	}
	return SubLeftTrigger();
}

bool InputManger::FoldDownTrigger()
{
	if (isInversionY)
	{
		return SubDownTrigger();
	}
	return SubUpTrigger();
}

bool InputManger::FoldLeftRelease()
{
	if (isInversionX)
	{
		return SubLeftRelease();
	}
	return SubRightRelease();
}

bool InputManger::FoldUpRelease()
{
	if (isInversionY)
	{
		return SubUpRelease();
	}
	return SubDownRelease();
}

bool InputManger::FoldRightRelease()
{
	if (isInversionX)
	{
		return SubRightRelease();
	}
	return SubLeftRelease();
}

bool InputManger::FoldDownRelease()
{
	if (isInversionY)
	{
		return SubDownRelease();
	}
	return SubUpRelease();
}

bool InputManger::OpenLeftTrigger()
{
	if (isInversionX)
	{
		return SubRightTrigger();
	}
	return SubLeftTrigger();
}

bool InputManger::OpenUpTrigger()
{
	if (isInversionY)
	{
		return SubDownTrigger();
	}
	return SubUpTrigger();
}

bool InputManger::OpenRightTrigger()
{
	if (isInversionX)
	{
		return SubLeftTrigger();
	}
	return SubRightTrigger();
}

bool InputManger::OpenDownTrigger()
{
	if (isInversionY)
	{
		return SubUpTrigger();
	}
	return SubDownTrigger();
}

bool InputManger::OpenLeftRelease()
{
	if (isInversionX)
	{
		return SubRightRelease();
	}
	return SubLeftRelease();
}

bool InputManger::OpenUpRelease()
{
	if (isInversionY)
	{
		return SubDownRelease();
	}
	return SubUpRelease();
}

bool InputManger::OpenRightRelease()
{
	if (isInversionX)
	{
		return SubLeftRelease();
	}
	return SubRightRelease();
}

bool InputManger::OpenDownRelease()
{
	if (isInversionY)
	{
		return SubUpRelease();
	}
	return SubDownRelease();
}

bool InputManger::FoldSelectTrigger()
{
	return Input::isKeyTrigger(DIK_SPACE) || Input::isXpadButtonPushTrigger(XPAD_TRIGGER_RB);
}
