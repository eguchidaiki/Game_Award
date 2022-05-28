#include "StageClearedControler.h"
#include "InputManger.h"

namespace
{
InputManger* inputManger = InputManger::Get();
}

void StageClearedControler::Init()
{

	//���o������
	goalEffect.reset(new GoalEffects);
	goalEffect->Init();

	//UI������
	_go_select_button.Init(60, TexManager::LoadTexture("Resources/back.png"), 200, 90);
	x1 = 1280.0f / 3.0f;
	y1 = 720 * (2.0f / 3.0f);
	_go_next_button.Init(60, TexManager::LoadTexture("Resources/next.png"), 200, 90);
	x2 = 1280.0f * (2.0f / 3.0f);
	y2 = 720 * (2.0f / 3.0f);

	//���͋֎~
	ctrl_state = CONTROL_NOT_ALLOW;
	//�ŏ��Ƀ��[�U�[���I��ł���
	_user_selecting = USER_SELECT_NEXT;

	frameCount = 0;

	isAllowSwitching = false;
}

void StageClearedControler::Update()
{
	Update_CheckControlStates();

	Update_ControlMain();

	//�I�����m�F�ł�����A���o��؂�

}

void StageClearedControler::Draw()
{
	if (ctrl_state == CONTROL_NOT_ALLOW) { return; }

	//���̃N���X���̕`�惊�\�[�X�́A���삪�֎~����ĂȂ��Ƃ��͏�ɕ`�悳���

	//�S�[�����o�`��
	goalEffect->Draw();

	//UI�`��
	Draw_UI();

}

void StageClearedControler::ControlActivate()
{
	if (ctrl_state != CONTROL_NOT_ALLOW) { return; }

	//���o��ԂɈڍs
	ctrl_state = CONTROL_ACTIVE;


	goalEffect->Play();
}

void StageClearedControler::ControlDisable()
{
	//�R���g���[��������
	ctrl_state = CONTROL_NOT_ALLOW;
}

void StageClearedControler::Update_CheckControlStates()
{
	//�L���X�g���Ă��̃^�C�~���O�ł͐����ň����悤�ɂ���
	int user_selecting = static_cast<int>(_user_selecting);

	//��ԑJ�ڂ̔�����s��
	switch (ctrl_state)
	{
	case StageClearedControler::CONTROL_NOT_ALLOW:
		//���ɂȂ��iActive�֐����Ă΂��܂Łj
		break;
	case StageClearedControler::CONTROL_DIRECING:
		//�K��t���[���o�߁A�܂��͓��͂����m
		frameCount++;
		if (frameCount < DIRECTING_FRAME)
		{
			//UI�I��L����
			ctrl_state = CONTROL_ACTIVE;
		}
		if (inputManger->DecisionTrigger())
		{
			frameCount = DIRECTING_FRAME;
		}

		break;
	case StageClearedControler::CONTROL_ACTIVE:
		//UI�I���̓��́i�ǂ�����I�����Ă邩�H�j
		if (inputManger->LeftTrigger()) { user_selecting = 0; }
		if (inputManger->RightTrigger()) { user_selecting = 1; }
		_user_selecting = static_cast<STAGE_CLEARED_USER_SELECTING>(user_selecting);

		//UI�I�����ꂽ
		if (inputManger->DecisionTrigger())
		{
			//�{�^��������
			if (user_selecting == 0) { _go_next_button.UI_Push(); }
			if (user_selecting == 1) { _go_select_button.UI_Push(); }

			ctrl_state = CONTROL_UI_SELECTED;
		}

		break;

	case StageClearedControler::CONTROL_UI_SELECTED:

		break;

	default:
		break;
	}
}

void StageClearedControler::Update_ControlMain()
{
	switch (ctrl_state)
	{
	case StageClearedControler::CONTROL_NOT_ALLOW:
		//�������Ȃ�
		break;

	case StageClearedControler::CONTROL_DIRECING:
		//�C�[�W���O����Ȃ炱����



		break;

	case StageClearedControler::CONTROL_ACTIVE:
		//�I�𒆁A���͈ȊO�X�V


		break;

	case StageClearedControler::CONTROL_UI_SELECTED:
		//UI�I����X�V����

		//���X�e�[�W�ւ̈ړ�����
		if (_go_next_button.isFunctionActivate || _go_select_button.isFunctionActivate) { isAllowSwitching = true; }

		break;

	default:
		break;
	}

	//UI�X�V
	_go_next_button.Update();
	_go_select_button.Update();

}

void StageClearedControler::Draw_UI()
{
	switch (_user_selecting)
	{
	case StageClearedControler::USER_SELECT_NEXT:
		_go_next_button.Draw(x1, y1, SELECTING_SCALE, SELECTING_SCALE);
		_go_select_button.Draw(x2, y2);
		break;
	case StageClearedControler::USER_SELECT_BACK:
		_go_next_button.Draw(x1, y1);
		_go_select_button.Draw(x2, y2, SELECTING_SCALE, SELECTING_SCALE);
		break;
	default:
		break;
	}
}

void UI_Button::Init(int activateTimerag, UINT graphHandle, float baseWidth, float baseHeight)
{
	this->activateTimerag = activateTimerag;
	uiSprite.Create(graphHandle);
	width = baseWidth / 2.0f;
	height = baseHeight / 2.0f;

	isUserPushed = false;
	isFunctionActivate = false;
}

void UI_Button::Update()
{

	//�����ꂽ��
	if (isUserPushed) {
		count++;
		if (count > activateTimerag) {
			isFunctionActivate = true;
		}
	}
	//������Ă��Ȃ���
	else {

	}
}

void UI_Button::Draw(float centerX, float centerY, float x_scale, float y_scale)
{
	uiSprite.DrawExtendSprite(centerX - width * x_scale, centerY - height * y_scale,
							  centerX + width * x_scale, centerY + height * y_scale);
	uiSprite.Draw();
}

void UI_Button::UI_Push()
{
	isUserPushed = true;
}