#include "InputManger.h"

namespace
{
static char decisionKey = DIK_RETURN; //決定キー
static char cancelKey = DIK_BACK;     //キャンセルキー

static XPAD_INPUT_CODE decisionXpad = XPAD_BUTTON_B; //決定ボタン
static XPAD_INPUT_CODE cancelXpad = XPAD_BUTTON_A;   //キャンセルボタン
}

DirectX::XMFLOAT2 InputManger::pressPos = {0.0f, 0.0f};
DirectX::XMFLOAT2 InputManger::releasePos = {0.0f, 0.0f};
DirectX::XMFLOAT2 InputManger::dragDis = {0.0f, 0.0f};

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

	dragDis = {releasePos.x - pressPos.x , releasePos.y - pressPos.y};
}

bool InputManger::Escape()
{
	return Input::isKey(DIK_ESCAPE) || Input::isXpadButtonPushing(XPAD_BUTTON_OPTION_L);
}

bool InputManger::EscapeTrigger()
{
	return Input::isKeyTrigger(DIK_ESCAPE) || Input::isXpadButtonPushTrigger(XPAD_BUTTON_OPTION_L);
}

bool InputManger::EscapeReturn()
{
	return Input::isKeyReleased(DIK_ESCAPE) || Input::isXpadButtonPushed(XPAD_BUTTON_OPTION_L);
}

bool InputManger::Up()
{
	return Input::isKey(DIK_W) || Input::isXpadButtonPushing(XPAD_BUTTON_CROSS_UP) || Input::isXpadStickTilt(XPAD_LSTICK_DIR_UP);
}

bool InputManger::UpTrigger()
{
	return Input::isKeyTrigger(DIK_W) || Input::isXpadButtonPushTrigger(XPAD_BUTTON_CROSS_UP);
}

bool InputManger::UpReturn()
{
	return Input::isKeyReleased(DIK_W) || Input::isXpadButtonPushed(XPAD_BUTTON_CROSS_UP);
}

bool InputManger::Down()
{
	return Input::isKey(DIK_S) || Input::isXpadButtonPushing(XPAD_BUTTON_CROSS_DOWN) || Input::isXpadStickTilt(XPAD_LSTICK_DIR_DOWN);
}

bool InputManger::DownTrigger()
{
	return Input::isKeyTrigger(DIK_S) || Input::isXpadButtonPushTrigger(XPAD_BUTTON_CROSS_DOWN);
}

bool InputManger::DownReturn()
{
	return Input::isKeyReleased(DIK_S) || Input::isXpadButtonPushed(XPAD_BUTTON_CROSS_DOWN);
}

bool InputManger::Left()
{
	return Input::isKey(DIK_A) || Input::isXpadButtonPushing(XPAD_BUTTON_CROSS_LEFT) || Input::isXpadStickTilt(XPAD_LSTICK_DIR_LEFT);
}

bool InputManger::LeftTrigger()
{
	return Input::isKeyTrigger(DIK_A) || Input::isXpadButtonPushTrigger(XPAD_BUTTON_CROSS_LEFT);
}

bool InputManger::LeftReturn()
{
	return Input::isKeyReleased(DIK_A) || Input::isXpadButtonPushed(XPAD_BUTTON_CROSS_LEFT);
}

bool InputManger::Right()
{
	return Input::isKey(DIK_D) || Input::isXpadButtonPushing(XPAD_BUTTON_CROSS_RIGHT) || Input::isXpadStickTilt(XPAD_LSTICK_DIR_RIGHT);
}

bool InputManger::RightTrigger()
{
	return Input::isKeyTrigger(DIK_D) || Input::isXpadButtonPushTrigger(XPAD_BUTTON_CROSS_RIGHT);
}

bool InputManger::RightReturn()
{
	return Input::isKeyReleased(DIK_D) || Input::isXpadButtonPushed(XPAD_BUTTON_CROSS_RIGHT);
}

bool InputManger::SubUp()
{
	return Input::isKey(DIK_UP) || Input::isXpadButtonPushing(XPAD_BUTTON_Y) || Input::isXpadStickTilt(XPAD_RSTICK_DIR_UP);
}

bool InputManger::SubUpTrigger()
{
	return Input::isKeyTrigger(DIK_UP) || Input::isXpadButtonPushTrigger(XPAD_BUTTON_Y);
}

bool InputManger::SubUpReturn()
{
	return Input::isKeyReleased(DIK_UP) || Input::isXpadButtonPushed(XPAD_BUTTON_Y);
}

bool InputManger::SubDown()
{
	return Input::isKey(DIK_DOWN) || Input::isXpadButtonPushing(XPAD_BUTTON_A) || Input::isXpadStickTilt(XPAD_RSTICK_DIR_DOWN);
}

bool InputManger::SubDownTrigger()
{
	return Input::isKeyTrigger(DIK_DOWN) || Input::isXpadButtonPushTrigger(XPAD_BUTTON_A);
}

bool InputManger::SubDownReturn()
{
	return Input::isKeyReleased(DIK_DOWN) || Input::isXpadButtonPushed(XPAD_BUTTON_A);
}

bool InputManger::SubLeft()
{
	return Input::isKey(DIK_LEFT) || Input::isXpadButtonPushing(XPAD_BUTTON_X) || Input::isXpadStickTilt(XPAD_RSTICK_DIR_LEFT);
}

bool InputManger::SubLeftTrigger()
{
	return Input::isKeyTrigger(DIK_LEFT) || Input::isXpadButtonPushTrigger(XPAD_BUTTON_X);
}

bool InputManger::SubLeftReturn()
{
	return Input::isKeyReleased(DIK_LEFT) || Input::isXpadButtonPushed(XPAD_BUTTON_X);
}

bool InputManger::SubRight()
{
	return Input::isKey(DIK_RIGHT) || Input::isXpadButtonPushing(XPAD_BUTTON_B) || Input::isXpadStickTilt(XPAD_RSTICK_DIR_RIGHT);
}

bool InputManger::SubRightTrigger()
{
	return Input::isKeyTrigger(DIK_RIGHT) || Input::isXpadButtonPushTrigger(XPAD_BUTTON_B);
}

bool InputManger::SubRightReturn()
{
	return Input::isKeyReleased(DIK_RIGHT) || Input::isXpadButtonPushed(XPAD_BUTTON_B);
}

bool InputManger::Decision()
{
	return Input::isKey(decisionKey) || Input::isXpadButtonPushing(decisionXpad);
}

bool InputManger::DecisionTrigger()
{
	return Input::isKeyTrigger(decisionKey) || Input::isXpadButtonPushing(decisionXpad);
}

bool InputManger::DecisionReturn()
{
	return Input::isKeyReleased(decisionKey) || Input::isXpadButtonPushing(decisionXpad);
}

bool InputManger::Cancel()
{
	return Input::isKey(cancelKey) || Input::isXpadButtonPushing(cancelXpad);
}

bool InputManger::CancelTrigger()
{
	return Input::isKeyTrigger(cancelKey) || Input::isXpadButtonPushing(cancelXpad);
}

bool InputManger::CancelReturn()
{
	return Input::isKeyReleased(cancelKey) || Input::isXpadButtonPushing(cancelXpad);
}

bool InputManger::Reset()
{
	return Input::isKey(DIK_R);
}

bool InputManger::ResetTrigger()
{
	return Input::isKeyTrigger(DIK_R);
}

bool InputManger::ResetReturn()
{
	return Input::isKeyReleased(DIK_R);
}
