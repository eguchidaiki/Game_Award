#include "Raki_Input.h"
#include "Raki_WinAPI.h"

#include <stdio.h>

BYTE Input::keys[256]    = {};
BYTE Input::oldkeys[256] = {};
DIMOUSESTATE Input::mouseState;
DIMOUSESTATE Input::oldMouseState;
POINT        Input::pos;
XINPUT_STATE Input::xInputState;
XINPUT_STATE Input::oldxInputState;
//�f�b�h�]�[���萔�i�����l��XINPUT���C�u�����񋟂̒萔���g�p�j
SHORT Input::XPAD_RS_DEADZONE = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
SHORT Input::XPAD_LS_DEADZONE = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

IDirectInput8       *Input::dinput      = nullptr;
IDirectInputDevice8 *Input::devkeyBoard = nullptr;
IDirectInputDevice8 *Input::devMouse	= nullptr;

bool Input::Init(WNDCLASSEX w, HWND hwnd)
{
	HRESULT result;
	//������
	result = DirectInput8Create(w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&dinput, nullptr);
	if (result != S_OK) { return false; }

	//�L�[�{�[�h�f�o�C�X����
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyBoard, NULL);
	if (result != S_OK) { return false; }
	//�}�E�X�f�o�C�X����
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	if (result != S_OK) { return false; }

	//���̓f�[�^�`���Z�b�g
	result = devkeyBoard->SetDataFormat(&c_dfDIKeyboard);//�W��
	if (result != S_OK) { return false; }
	result = devMouse->SetDataFormat(&c_dfDIMouse);
	if (result != S_OK) { return false; }

	//�r�����䃌�x���Z�b�g
	result = devkeyBoard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (result != S_OK) { return false; }
	result = devMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (result != S_OK) { return false; }

	//xinput�̓��͏�Ԃ̃N���A
	ZeroMemory(&xInputState, sizeof(xInputState));
	//xinput�̓��͏�Ԃ̃e�X�g�擾
	auto xresult = XInputGetState(0, &xInputState);
	//�擾���s���A���b�Z�[�W�o��
	if (xresult != ERROR_SUCCESS) {
		printf("FAILED : xInput : Failed to get input from xInput.\n");
	}
	else {
		printf("SUCCESSED : xInput : Get input state.\n");
	}

	return true;
}

void Input::StartGetInputState()
{
	//�O�t���[���̓��͏����擾
	for (int i = 0; i < 256; i++) {
		oldkeys[i] = keys[i];
	}
	//�L�[���͏�Ԏ擾�J�n
	devkeyBoard->Acquire();
	//�S�L�[�̓��͏�Ԃ��擾
	devkeyBoard->GetDeviceState(sizeof(keys), keys);

	//�}�E�X�̑O�t���[���̓��͏�Ԃ��擾
	oldMouseState = mouseState;
	//�}�E�X���͏�Ԏ擾�J�n
	devMouse->Acquire();
	//�}�E�X�̓��͏�Ԃ��擾
	devMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);

	//�O�t���[���̓��͏�Ԃ�ۑ�
	oldxInputState = xInputState;
	//xinput�̓��͏�Ԃ̃N���A
	ZeroMemory(&xInputState, sizeof(xInputState));
	//xinput�̓��͏�Ԃ̎擾�J�n
	auto result = XInputGetState(0, &xInputState);
	//�f�b�h�]�[���͈͂ɂ܂Ƃ߂�
	XpadStickTiltRoundOffToDeadzone();
}

bool Input::isKey(int key)
{
	return keys[key];
}

bool Input::isKeyTrigger(int key)
{
	return keys[key] && !oldkeys[key];
}

bool Input::isKeyReleased(int key)
{
	return oldkeys[key] && !keys[key];
}

bool Input::isMouseClicking(int keyCode)
{
	//�}�E�X��������Ă���
	return mouseState.rgbButtons[keyCode] & (0x80);
}

bool Input::isMouseClickTrigger(int keyCode)
{
	BYTE onValue = 0x80;
	//�O�t���[���͉�����Ă��炸�A���t���[���ŉ�����Ă���
	return (mouseState.rgbButtons[keyCode] == onValue) & (oldMouseState.rgbButtons[keyCode] != onValue);
}

bool Input::isMouseClicked(int keyCode)
{
	BYTE onValue = 0x80;
	//�O�t���[���ŉ�����Ă���A���t���[���ŉ�����Ă��Ȃ�
	return (mouseState.rgbButtons[keyCode] != onValue) & (oldMouseState.rgbButtons[keyCode] == onValue);
}

XMFLOAT2 Input::getMousePos()
{
	//�f�X�N�g�b�v���W���擾
	GetCursorPos(&pos);
	//�N���C�A���g�̈���W�ϊ�
	ScreenToClient(Raki_WinAPI::GetHWND(), &pos);
	//�߂�l�Ɋi�[
	XMFLOAT2 result((float)pos.x, (float)pos.y);
	//�ԋp
	return result;
}

XMFLOAT2 Input::getMouseVelocity()
{
	XMFLOAT2 result;
	result.x = (float)mouseState.lX;
	result.y = (float)mouseState.lY;

	return result;
}

bool Input::isXpadButtonPushing(XPAD_INPUT_CODE code)
{
	switch (code)
	{
	case XPAD_BUTTON_A:
		return xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_A;
		break;
	case XPAD_BUTTON_B:
		return xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_B;
		break;
	case XPAD_BUTTON_X:
		return xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_X;
		break;
	case XPAD_BUTTON_Y:
		return xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
		break;
	case XPAD_BUTTON_CROSS_UP:
		return xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
		break;
	case XPAD_BUTTON_CROSS_DOWN:
		return xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
		break;
	case XPAD_BUTTON_CROSS_LEFT:
		return xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
		break;
	case XPAD_BUTTON_CROSS_RIGHT:
		return xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
		break;
	case XPAD_TRIGGER_LB:
		return xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
		break;
	case XPAD_TRIGGER_LT:
		return xInputState.Gamepad.bLeftTrigger != 0;
		break;
	case XPAD_TRIGGER_RB:
		return xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
		break;
	case XPAD_TRIGGER_RT:
		return xInputState.Gamepad.bRightTrigger != 0;
		break;
	case XPAD_BUTTON_LSTICK:
		return xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
		break;
	case XPAD_BUTTON_RSTICK:
		return xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
		break;
	case XPAD_BUTTON_OPTION_L:
		return xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
		break;
	case XPAD_BUTTON_OPTION_R:
		return xInputState.Gamepad.wButtons & XINPUT_GAMEPAD_START;
		break;
	default:
		break;
	}
}

bool Input::isXpadButtonPushTrigger(XPAD_INPUT_CODE code)
{
	return isXpadButtonPushing(code) == true && isOldXpadPushing(code) == false;
}

bool Input::isXpadButtonPushed(XPAD_INPUT_CODE code)
{
	return isXpadButtonPushing(code) == false && isOldXpadPushing(code) == true;
}

int Input::GetXpadRTStrength()
{
	return static_cast<int>(xInputState.Gamepad.bRightTrigger);
}

int Input::GetXpadLTStrength()
{
	return static_cast<int>(xInputState.Gamepad.bLeftTrigger);
}

bool Input::isXpadStickTilt(XPAD_STICK_DIRECTION_CODE dircode)
{
	switch (dircode)
	{
	case XPAD_LSTICK_DIR_UP:
		return isLSTiltUp();
		break;
	case XPAD_LSTICK_DIR_DOWN:
		return isLSTiltDown();
		break;
	case XPAD_LSTICK_DIR_LEFT:
		return isLSTiltLeft();
		break;
	case XPAD_LSTICK_DIR_RIGHT:
		return isLSTiltRight();
		break;
	case XPAD_LSTICK_DIR_UR:
		return isLSTiltUp() && isLSTiltRight();
		break;
	case XPAD_LSTICK_DIR_UL:
		return isLSTiltUp() && isLSTiltLeft();
		break;
	case XPAD_LSTICK_DIR_DR:
		return isLSTiltDown() && isLSTiltRight();
		break;
	case XPAD_LSTICK_DIR_DL:
		return isLSTiltDown() && isLSTiltLeft();
		break;
	case XPAD_RSTICK_DIR_UP:
		return isRSTiltUp();
		break;
	case XPAD_RSTICK_DIR_DOWN:
		return isRSTiltDown();
		break;
	case XPAD_RSTICK_DIR_LEFT:
		return isRSTiltLeft();
		break;
	case XPAD_RSTICK_DIR_RIGHT:
		return isRSTiltRight();
		break;
	case XPAD_RSTICK_DIR_UR:
		return isRSTiltUp() && isRSTiltRight();
		break;
	case XPAD_RSTICK_DIR_UL:
		return isRSTiltUp() && isRSTiltRight();
		break;
	case XPAD_RSTICK_DIR_DR:
		return isRSTiltUp() && isRSTiltRight();
		break;
	case XPAD_RSTICK_DIR_DL:
		return isRSTiltUp() && isRSTiltRight();
		break;
	default:
		break;
	}
}

Input *Input::Get()
{
	static Input instance;
	return &instance;
}

bool Input::isOldXpadPushing(XPAD_INPUT_CODE code)
{
	switch (code)
	{
	case XPAD_BUTTON_A:
		return oldxInputState.Gamepad.wButtons & XINPUT_GAMEPAD_A;
		break;
	case XPAD_BUTTON_B:
		return oldxInputState.Gamepad.wButtons & XINPUT_GAMEPAD_B;
		break;
	case XPAD_BUTTON_X:
		return oldxInputState.Gamepad.wButtons & XINPUT_GAMEPAD_X;
		break;
	case XPAD_BUTTON_Y:
		return oldxInputState.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
		break;
	case XPAD_BUTTON_CROSS_UP:
		return oldxInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
		break;
	case XPAD_BUTTON_CROSS_DOWN:
		return oldxInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
		break;
	case XPAD_BUTTON_CROSS_LEFT:
		return oldxInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
		break;
	case XPAD_BUTTON_CROSS_RIGHT:
		return oldxInputState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
		break;
	case XPAD_TRIGGER_LB:
		return oldxInputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
		break;
	case XPAD_TRIGGER_LT:
		return oldxInputState.Gamepad.bLeftTrigger != 0;
		break;
	case XPAD_TRIGGER_RB:
		return oldxInputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
		break;
	case XPAD_TRIGGER_RT:
		return oldxInputState.Gamepad.bRightTrigger != 0;
		break;
	case XPAD_BUTTON_LSTICK:
		return oldxInputState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
		break;
	case XPAD_BUTTON_RSTICK:
		return oldxInputState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
		break;
	case XPAD_BUTTON_OPTION_L:
		return oldxInputState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
		break;
	case XPAD_BUTTON_OPTION_R:
		return oldxInputState.Gamepad.wButtons & XINPUT_GAMEPAD_START;
		break;
	default:
		return false;
		break;
	}

	return false;
}

void Input::XpadStickTiltRoundOffToDeadzone()
{
	//�X�������𔻕ʂ���֐��ŁA�ǂ����̕����ɂ��X���Ă��Ȃ��ꍇ
	//�X�e�B�b�N�̌X���̓f�b�h�]�[���͈͓̔��Ɣ��f�ł���̂ŁA���̏ꍇ�͌X����0�ɂ���

	if (!isRSTiltUp() && !isRSTiltDown()) { xInputState.Gamepad.sThumbRY = (SHORT)0; }
	if (!isRSTiltRight() && !isRSTiltLeft()) { xInputState.Gamepad.sThumbRX = (SHORT)0; }
	if (!isLSTiltUp() && !isLSTiltDown()) { xInputState.Gamepad.sThumbLY = (SHORT)0; }
	if (!isLSTiltRight() && !isLSTiltLeft()) { xInputState.Gamepad.sThumbLX = (SHORT)0; }
}

bool Input::isRSTiltRight() { return xInputState.Gamepad.sThumbRX > XPAD_RS_DEADZONE; }

bool Input::isRSTiltLeft() { return xInputState.Gamepad.sThumbRX < -XPAD_RS_DEADZONE; }

bool Input::isRSTiltUp() { return xInputState.Gamepad.sThumbRY > XPAD_RS_DEADZONE; }

bool Input::isRSTiltDown() { return xInputState.Gamepad.sThumbRY < -XPAD_RS_DEADZONE; }

bool Input::isLSTiltRight() { return xInputState.Gamepad.sThumbLX > XPAD_LS_DEADZONE; }

bool Input::isLSTiltLeft() { return xInputState.Gamepad.sThumbLX < -XPAD_LS_DEADZONE; }

bool Input::isLSTiltUp() { return xInputState.Gamepad.sThumbLY > XPAD_LS_DEADZONE; }

bool Input::isLSTiltDown() { return xInputState.Gamepad.sThumbLY < -XPAD_LS_DEADZONE; }

Input::StickTiltParam Input::GetXpadRStickTilt()
{
	StickTiltParam result;
	result.x = (int)xInputState.Gamepad.sThumbRX;
	result.y = (int)xInputState.Gamepad.sThumbRY;

	return result;
}

Input::StickTiltParam Input::GetXpadLStickTilt()
{
	StickTiltParam result;
	result.x = (int)xInputState.Gamepad.sThumbLX;
	result.y = (int)xInputState.Gamepad.sThumbLY;

	return result;
}
