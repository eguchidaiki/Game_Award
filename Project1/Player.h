#pragma once
#include "PlayerBody.h"
#include "PlayerFoot.h"
#include <RVector.h>
#include <TexManager.h>
#include "Sprite.h"

class Player final
{
public: //�V���O���g����
	static Player* Get();
private:
	Player();
	Player(const Player&) = delete;
	~Player();
	Player operator=(const Player&) = delete;

public: //�����o�֐�
	// ������
	void Init();
	// �X�V
	void Update(Stage& stage);
	// �`��
	void Draw(int offsetX, int offsetY);

	/// <summary>
	/// �ǂ̑̂�L�������邩
	/// </summary>
	/// <param name="one">body_one</param>
	/// <param name="two">body_two</param>
	/// <param name="three">body_three</param>
	void bodysetup(bool one, int one_type, bool two, int two_type, bool three, int three_type, bool four, int four_type);
	void bodysetup(const unsigned char foldCount[4]);

	//��̓����蔻��
	void IsHitPlayerBody(Stage& stage);
	//�����o������(�g���ĂȂ�)
	void ExtrudePlayer(RVector3 ExtrudePos, float ExtrudeDis, BodyType ExtrudeType);
	//�e�́E��̗�������
	bool IsFall();
	//�e�̂̏�Ԃ�z��ɃZ�b�g(�Z�b�g���鏇�Ԃ�stage.FoldAndOpen�̔��菇)
	void SetBodyStatus(PlayerBody* arrangement, int size);

public: //�����o�ϐ�
	//���̍���
	float FloorHeight;

	//��̒��S���W
	RVector3 CenterPosition;

	//�����Ă������
	bool IsLeft;
	bool IsRight;

	//��(�܂�ق�)
	PlayerBody Body_One;
	PlayerBody Body_Two;
	PlayerBody Body_Three;
	PlayerBody Body_Four;

	//body_two��D��I�ɊJ����
	bool IsOpenTwo;

	//�ǂ̕�����܂邩(4����)
	bool IsLeftFold;
	bool IsUpFold;
	bool IsRightFold;
	bool IsDownFold;

	//�ǂ̕�����܂邩(4����)
	bool IsLeftOpen;
	bool IsUpOpen;
	bool IsRightOpen;
	bool IsDownOpen;

	//�J���܂łɐ��t���[���󂯂�p
	int OpenCount = 0;

	//�J�E���g���n�߂邩�ǂ���
	bool IsOpenCountStart = false;

	//�ړ����x
	float SideMoveSpeed = 3.0f;

	//�W�����v
	bool IsJump;
	float JumpSpeed;
	float FallSpeed;

	//��������(��̂�
	bool IsFaceFall;

	//�̂Ɗ炷�ׂĂ��l��������������
	bool IsAllFall;

	//�������E�W�����v���ɃW�����v���͂������Ă��邩�ǂ���
	bool IsInputjump;

	//�ǂꂩ��ł��̂𓮂����Ă�����true
	bool Player_IsAction;

	//�摜�n���h��(��)
	UINT FaceHandle[2];

	//�S�[���ɐG��Ă��邩�ǂ���
	bool IsGoal;

	//�u���b�N�ɓ������Ă��邩�ǂ���
	bool IsColide;

	//���ɑ̂����邩�ǂ���
	bool IsDownBody;

	//��
	PlayerFoot leg;

	//�`��p�X�v���C�g
	Sprite PlayerSprite;
	Sprite PlayerSpriteAction;

	//�}�E�X�̃N���b�N���W
	XMFLOAT2 PressPos;
	XMFLOAT2 ReleasePos;
};
