#pragma once

#include <windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>
#include <vector>
#include <memory>

#include "NY_Camera.h"
#include "RVector.h"

struct ParticleGrainState 
{
private:
	using XMFLOAT4 = DirectX::XMFLOAT4;

public:

	//�N�_
	RVector3 position;
	//�����_���X�|�[���t���O
	bool isRandomSpawn;
	//�X�|�[���͈�
	RVector3 position_spawnRange1;
	RVector3 position_spawnRange2;
	//���x
	RVector3 vel;
	//���x���
	RVector3 velMax;
	//���x����
	RVector3 velMin;
	//�����x
	RVector3 acc;

	//�F�֘A
	//�F
	XMFLOAT4 color_start;
	//�F�⊮�I���_
	XMFLOAT4 color_end;

	//�A�t�B���ϊ�
	//�X�P�[�����O
	float scale_start;
	float scale_end;
	//��]
	float rot_start;
	float rot_end;
	//����
	int aliveTime;

};

// �p�[�e�B�N��1���N���X
class Particle {
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//���W
	RVector3 pos;
	//���x
	RVector3 vel;
	//�����x
	RVector3 acc;
	//�F
	XMFLOAT4 color;
	//�X�P�[�����O
	float scale;
	//��]
	float rot;
	//�����l
	XMFLOAT4 s_color = {};
	float s_scale = 1.0f;
	float s_rotation = 0.0f;
	// �ŏI�l
	XMFLOAT4 e_color = {};
	float e_scale = 0.0f;
	float e_rotation = 0.0f;
	// ���݃t���[��
	int nowFrame = 0;
	// �I���t���[��
	int endFrame = 0;
};

//��荂�x�ȃp�t�H�[�}���X�̂��߂ɁA���������ׂ�����`���ē�������p�[�e�B�N���N���X
//�p�����ē������������ƂŁA���������ׂ�����`�\
//Prototype�p�^�[���̗p�A�p�����Clone�̒�`���쐬���Ȃ���΂Ȃ�Ȃ����A
//����͌p�������I�u�W�F�N�g�𓮓I�m�ۂ������̂�Ԃ��Ηǂ�
// ParticleSourcePrototype *clone()
// {
//		return new ParticleSourcePrototype;
// }
//�������z�֐��Ȃ̂Œ�`�����Ȃ��Ɠ����Ȃ�
class ParticlePrototype : public Particle 
{
public:
	ParticlePrototype() {};
	~ParticlePrototype() {
		
	};
	//������
	virtual void Init() = 0;
	//�X�V
	virtual void Update() = 0;
	//�N���[�������i�N�_���W�����擾����?�j
	virtual ParticlePrototype *clone(RVector3 startPos) = 0;

	//�`��A�I���͕K�v�Ȃ��i���ۂ̕`��̓G�~�b�^�[�ł��̂Łj
};

class ParticleManager
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	

public:
	// ���_�f�[�^
	struct PVertex {
		RVector3	pos;
		float		scale;
		XMFLOAT4	color;
	};

	//�萔�o�b�t�@�f�[�^�\����
	struct ConstBufferData {
		XMMATRIX mat;
		XMMATRIX matBillBoard;
	};

private:
	// �ő吶�����_��
	static const int MAX_VERTEX = 65536;


public:
	/// <summary>
	/// �p�[�e�B�N���}�l�[�W���[����
	/// </summary>
	static ParticleManager *Create();

	/// <summary>
	/// �p�[�e�B�N���}�l�[�W���[������
	/// </summary>
	void Initialize();
	
	/// <summary>
	/// �S�p�[�e�B�N���X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �p�[�e�B�N���`��
	/// </summary>
	void Draw(UINT drawTexNum);

	/// <summary>
	/// �p�[�e�B�N���ǉ�
	/// </summary>
	void Add(ParticleGrainState pgState);

	/// ---------------------- �ȉ��APrototype�p�^�[���̗p�p�[�e�B�N���}�l�[�W���[�̋@�\�Q ------------------------------------//

	void Prototype_Set(ParticlePrototype *proto);

	void Prototype_Add(int addCount,RVector3 startPos);

	void Prototype_Update();

	void Prototype_Draw(UINT drawTexNum);
	

private:
	//�f�B�X�N���v�^�q�[�v�A�e�N�X�`���o�b�t�@��TexManager�ˑ�

	/// �����o�ϐ�

	// �f�o�C�X
	ID3D12Device *dev;
	// �R�}���h
	ID3D12GraphicsCommandList *cmd;
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootsig;
	// �O���t�B�b�N�X�p�C�v���C��
	ComPtr<ID3D12PipelineState> pipeline;
	// ���_�o�b�t�@
	ComPtr <ID3D12Resource> vertbuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbview;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	// �p�[�e�B�N���R���e�i
	std::forward_list<Particle> grains;

	HRESULT result = S_FALSE;


	//�p�[�e�B�N���v���g�^�C�v
	std::unique_ptr<ParticlePrototype> prototype_;
	//�p�[�e�B�N���v���g�^�C�v�R���e�i
	std::forward_list<std::unique_ptr<ParticlePrototype>> pplist;


private:

	/// <summary>
	/// �p�[�e�B�N���p�O���t�B�b�N�X�p�C�v���C��������
	/// </summary>
	void InitializeGraphicsPipeline();

	/// <summary>
	/// �p�[�e�B�N���p���f������
	/// </summary>
	void CreateModel();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	/// <param name="cmd">�R�}���h</param>
	/// <param name="cam">�J����</param>
	ParticleManager(ID3D12Device *dev, ID3D12GraphicsCommandList *cmd) {
		this->dev = dev;
		this->cmd = cmd;
	}
};

