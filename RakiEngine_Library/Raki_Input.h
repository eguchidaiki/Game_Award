#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <xinput.h>
#include <wrl.h>
#include <utility>

#include "RVector.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")
#pragma comment(lib,"dxguid.lib")


#pragma region InputKeyCode
// �L�[�{�[�h�͂��Ȃ�����

// �}�E�X����
#define MOUSE_L			0x00		//�}�E�X��
#define MOUSE_R			0x01		//�}�E�X�E
#define MOUSE_CENTER	0x02		//�}�E�X����

//�Q�[���p�b�h���ʃR�[�h
enum XPAD_INPUT_CODE
{
	XPAD_BUTTON_A,
	XPAD_BUTTON_B,
	XPAD_BUTTON_X,
	XPAD_BUTTON_Y,

	XPAD_BUTTON_CROSS_UP,
	XPAD_BUTTON_CROSS_DOWN,
	XPAD_BUTTON_CROSS_LEFT,
	XPAD_BUTTON_CROSS_RIGHT,

	XPAD_TRIGGER_LB,
	XPAD_TRIGGER_LT,
	XPAD_TRIGGER_RB,
	XPAD_TRIGGER_RT,

	XPAD_BUTTON_LSTICK,
	XPAD_BUTTON_RSTICK,

	XPAD_BUTTON_OPTION_R,
	XPAD_BUTTON_OPTION_L,
};
//xInput�X�e�B�b�N�X������
enum XPAD_STICK_DIRECTION_CODE
{
	///4��������
	XPAD_LSTICK_DIR_UP,
	XPAD_LSTICK_DIR_DOWN,
	XPAD_LSTICK_DIR_LEFT,
	XPAD_LSTICK_DIR_RIGHT,

	///�΂ߓ���
	XPAD_LSTICK_DIR_UR,
	XPAD_LSTICK_DIR_UL,
	XPAD_LSTICK_DIR_DR,
	XPAD_LSTICK_DIR_DL,

	///4��������
	XPAD_RSTICK_DIR_UP,
	XPAD_RSTICK_DIR_DOWN,
	XPAD_RSTICK_DIR_LEFT,
	XPAD_RSTICK_DIR_RIGHT,

	///�΂ߓ���
	XPAD_RSTICK_DIR_UR,
	XPAD_RSTICK_DIR_UL,
	XPAD_RSTICK_DIR_DR,
	XPAD_RSTICK_DIR_DL,
};


#pragma endregion InputKeyCode

using namespace Microsoft::WRL;
/// <summary>
/// �V���O���g���p�^�[���̗p�A�L�[���̓N���X
/// </summary>
class Input final
{
private:
	/// �f�o�C�X

	//���̓f�o�C�X
	static IDirectInput8       *dinput      ;
	//�L�[�{�[�h
	static IDirectInputDevice8 *devkeyBoard ;
	//�}�E�X
	static IDirectInputDevice8 *devMouse;


	/// �e����͏��

	//�L�[�{�[�h
	static BYTE keys[256];
	static BYTE oldkeys[256];
	//�}�E�X
	static DIMOUSESTATE mouseState;		//�}�E�X���͏��
	static DIMOUSESTATE oldMouseState;	//1F�O�̃}�E�X���͏��
	static POINT		pos;			//�}�E�X���W
	//xinput
	static XINPUT_STATE xInputState;	//xinput�̓��͏��
	static XINPUT_STATE oldxInputState;	//1F�O
	static SHORT XPAD_RS_DEADZONE, XPAD_LS_DEADZONE;

	//�R���X�g���N�^�A�f�X�g���N�^���B��
	Input() {}
	~Input() {}

	

public:

	/// <summary>
	/// ���͌n������
	/// </summary>
	/// <param name="w">�E�B���h�E�N���X</param>
	/// <param name="hwnd">�E�B���h�E�n���h��</param>
	/// <returns>����</returns>
	bool Init(WNDCLASSEX w, HWND hwnd);

	//���͏��̎擾�J�n
	static void StartGetInputState();

	// ---------- �L�[���� ---------- //

	//�L�[�����͂���Ă��邩�H
	static bool isKey(int key);
	//�g���K�[����
	static bool isKeyTrigger(int key);
	//�����[�X����
	static bool isKeyReleased(int key);

	// ---------- �}�E�X���� ---------- //

	static bool isMouseClicking(int keyCode);

	static bool isMouseClickTrigger(int keyCode);

	static bool isMouseClicked(int keyCode);

	static XMFLOAT2 getMousePos();

	static XMFLOAT2 getMouseVelocity();

	//----------- xinput�R���g���[���[���� ----------//

	static bool isXpadButtonPushing(XPAD_INPUT_CODE code);

	static bool isXpadButtonPushTrigger(XPAD_INPUT_CODE code);

	static bool isXpadButtonPushed(XPAD_INPUT_CODE code);

	static int GetXpadRTStrength();

	static int GetXpadLTStrength();

	static bool isXpadStickTilt(XPAD_STICK_DIRECTION_CODE dircode);

	struct StickTiltParam
	{
		int x;
		int y;
	};

	static StickTiltParam GetXpadRStickTilt();

	static StickTiltParam GetXpadLStickTilt();

	//�C���X�^���X�擾
	static Input *Get();

	//�R�s�[�R���X�g���N�^�A������Z�q������
	Input(const Input &obj) = delete;
	Input &operator=(const Input &obj) = delete;

private:
	//1F�O�̓��͂��m�F
	static bool isOldXpadPushing(XPAD_INPUT_CODE code);

	//�X�e�B�b�N�̌X�����f�b�h�]�[���͈͂ɂ܂Ƃ߂�
	static void XpadStickTiltRoundOffToDeadzone();

	//�X�e�B�b�N�̌X���������m�F
	static bool isRSTiltRight();
	static bool isRSTiltLeft();
	static bool isRSTiltUp();
	static bool isRSTiltDown();
	static bool isLSTiltRight();
	static bool isLSTiltLeft();
	static bool isLSTiltUp();
	static bool isLSTiltDown();
};

//�}�E�X���́i���b�v�֐��j
namespace mouse {



}

//�Q�[���p�b�h����
namespace pad {




}
