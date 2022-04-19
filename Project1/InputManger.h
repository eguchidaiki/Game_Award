#pragma once
#include "Raki_Input.h"

// ���͂��܂Ƃ߂��N���X
class InputManger final
{
private: //�V���O���g����
	InputManger() {}
	InputManger(const InputManger&) = delete;
	~InputManger() {}
	InputManger operator=(const InputManger&) = delete;

public: //�T�u�N���X
	enum StickDirection
	{
		NONE,  //�ǂ̕����ɂ��X���Ă��Ȃ�
		LEFT,  //���ɌX���Ă���
		UP,    //��ɌX���Ă���
		RIGHT, //�E�ɌX���Ă���
		DOWN   //���ɌX���Ă���
	};

public: //�ÓI�����o�֐�
	// �X�V
	static void Update();

	// �I��
	static bool Escape();
	// �I��(�������u�Ԃ̔���)
	static bool EscapeTrigger();
	// �I��(�������u�Ԃ̔���)
	static bool EscapeReturn();
	// ��
	static bool Up();
	// ��(�������u�Ԃ̔���)
	static bool UpTrigger();
	// ��(�������u�Ԃ̔���)
	static bool UpReturn();
	// ��
	static bool Down();
	// ��(�������u�Ԃ̔���)
	static bool DownTrigger();
	// ��(�������u�Ԃ̔���)
	static bool DownReturn();
	// ��
	static bool Left();
	// ��(�������u�Ԃ̔���)
	static bool LeftTrigger();
	// ��(�������u�Ԃ̔���)
	static bool LeftReturn();
	// �E
	static bool Right();
	// �E(�������u�Ԃ̔���)
	static bool RightTrigger();
	// �E(�������u�Ԃ̔���)
	static bool RightReturn();
	// ��
	static bool SubUp();
	// ��(�������u�Ԃ̔���)
	static bool SubUpTrigger();
	// ��(�������u�Ԃ̔���)
	static bool SubUpReturn();
	// ��
	static bool SubDown();
	// ��(�������u�Ԃ̔���)
	static bool SubDownTrigger();
	// ��(�������u�Ԃ̔���)
	static bool SubDownReturn();
	// ��
	static bool SubLeft();
	// ��(�������u�Ԃ̔���)
	static bool SubLeftTrigger();
	// ��(�������u�Ԃ̔���)
	static bool SubLeftReturn();
	// �E
	static bool SubRight();
	// �E(�������u�Ԃ̔���)
	static bool SubRightTrigger();
	// �E(�������u�Ԃ̔���)
	static bool SubRightReturn();
	// ���Z�b�g
	static bool Reset();
	// ���Z�b�g(�������u�Ԃ̔���)
	static bool ResetTrigger();
	// ���Z�b�g(�������u�Ԃ̔���)
	static bool ResetReturn();
	// �A�N�V����1
	static bool Act1();
	// �A�N�V����1(�������u�Ԃ̔���)
	static bool Act1Trigger();
	// �A�N�V����1(�������u�Ԃ̔���)
	static bool Act1Return();
};
