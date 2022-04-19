#pragma once

#include <RVector.h>
#include "Easing.h"
#include "Sprite.h"

class Stage;

enum BodyType
{
	left,
	up,
	right,
	down
};

class PlayerBody
{
public: //�萔
	//�̂̑傫��
	static const float BodySize;
	static const float HalfBodySize;

public: //�����o�֐�
	PlayerBody();
	~PlayerBody();

	// ������
	void Init(RVector3 position, BodyType number);
	// �X�V
	void Update(RVector3& center);
	/// <summary>
	// �`��
	void Draw(int offsetX, int offsetY);

	/// <summary>
	/// �̂�L�����������̐ݒ�
	/// </summary>
	/// <param name="center">�L�����������̍��W�Q�Ɛ�</param>
	void setactivate(RVector3 center);
	/// �̂̃X���C�h�̃Z�b�g�A�b�v
	/// </summary>
	/// <param name="slidepat">�X���C�h�������(����:-1 �E��:1)</param>
	/// <param name="move_dis">�X���C�h���鋗��(��:1 ����܂���:2)</param>
	void setslide(int slidepat, int move_dis);

	/// <summary>
	/// �v���C���[�̓����蔻��
	/// </summary>
	/// <param name="stage">�X�e�[�W�f�[�^</param>
	/// <param name="center">�v���C���[�̒��S</param>
	/// <returns></returns>
	void IsHitBody(Stage& stage, RVector3* center, float& FallSpeed, bool& isfall, bool& isjump, bool& iscolide);

	/// <summary>
	/// �����o������
	/// </summary>
	/// <param name="center">�����o����鑤�̍��W</param>
	/// <param name="extrudepos">�����o�����̍��W</param>
	/// <param name="extrudedis">�����o������</param>
	/// <param name="extrudetype">�ǂ̕���"����"�����o����</param>
	void Extrude(RVector3* center, RVector3 extrudepos, float extrudedis, BodyType extrudetype, bool& isfall, bool& isjump, bool& iscolide);

public: //�����o�ϐ�
	//�L�����t���O
	bool IsActivate;

	//�炩�猩���̂̈ʒu
	int Body_Type;

	//�̂̍��W(������ԁF����)
	RVector3 BodyStartPos;

	//�̂̍��W(������ԁF�E��)
	RVector3 BodyEndPos;

	//�̂̒��S���W
	RVector3 BodyCenterPos;

	//�X���C�h�̊J�n���W
	RVector3 SlideStartPos;

	//�܂�E�J���t���O
	bool IsFold;
	bool IsOpen;

	//���̑̂��܂�ꂽ��
	int FoldCount;

	//�X���C�h�t���O�E�X���C�h�������
	bool IsSlide;
	int SlidePat;

	//�܂�E�J���E�X���C�h�����Ă���r�����ǂ���
	bool IsAction;

	//�㉺���E���ꂼ��̓����蔻��
	bool IsHitLeft;
	bool IsHitUp;
	bool IsHitRight;
	bool IsHitDown;

	//body�ʗ�������
	bool BodyIsFall;

	//�X���C�h���鋗��
	int SlideDis;

	//��ɏd�Ȃ��Ă��銚�݂̐�
	int Overlap;

	//�{�̂���ǂꂾ������Ă��邩(���E��p)
	int BodyDistance;

	//�C�[�W���O
	Easing Ease;

	//�̂̐F
	//int BodyColor;

	//�摜�n���h��
	UINT Bodyhandle;

	Sprite BodySprite;
};
