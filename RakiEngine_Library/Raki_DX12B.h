#pragma once



#include <Windows.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl.h>
#include <dxgi1_6.h>
#include <cstdlib>

#include "Raki_WinAPI.h"
#include "Raki_Input.h"

//�}���`�p�X���ʕ`��p�|���S�����_�f�[�^
struct mpVertex {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

class Raki_DX12B
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	Raki_DX12B() {};
	~Raki_DX12B() {
	
	};

public:
	//--------------------�����o�֐�--------------------//

	//�R�s�[�R���X�g���N�^�A������Z�q������
	Raki_DX12B &operator=(const Raki_DX12B &obj) = delete;
	Raki_DX12B(const Raki_DX12B &obj) = delete;

	static Raki_DX12B *Get() {
		static Raki_DX12B dx12b;
		return &dx12b;
	}

	/// <summary>
	/// DirectX12�̈ꊇ������
	/// </summary>
	void Initialize(Raki_WinAPI *win);

	/// <summary>
	/// 1�p�X�ڕ`��J�n
	/// </summary>
	void StartDraw();

	/// <summary>
	/// 1�p�X�ڕ`��I��
	/// </summary>
	void EndDraw();

	/// <summary>
	/// �]���̕`��J�n
	/// </summary>
	void StartDraw2();

	//�o�b�N�o�b�t�@�`��J�n
	void StartDrawBackbuffer();

	//�����_�[�^�[�Q�b�g�`��J�n�i�����Ƀ����_�����O�������ʂ��摜�Ƃ��Ďg����j
	void StartDrawRenderTarget();

	//�`�揈���I��
	void CloseDraw();

	/// <summary>
	/// �]���̕`��I��
	/// </summary>
	void EndDraw2();

	/// <summary>
	/// �����_�[�^�[�Q�b�g�̃N���A
	/// </summary>
	void ClearRenderTarget();

	/// <summary>
	/// �[�x�o�b�t�@�[�̃N���A
	/// </summary>
	void ClearDepthBuffer();

	/// <summary>
	/// ID3D12GraphicsCommandList�̃Q�b�^
	/// </summary>
	/// <returns>ID3D12GraphicsCommandList�̃|�C���^</returns>
	ID3D12GraphicsCommandList *GetGCommandList() { return commandList.Get(); }

	/// <summary>
	/// ID3D12Device�̃Q�b�^
	/// </summary>
	/// <returns></returns>
	ID3D12Device *GetDevice() { return device.Get(); }

	/// <summary>
	/// imgui�p�f�X�N���v�^�q�[�v�̃Q�b�^
	/// </summary>
	/// <returns></returns>
	ComPtr<ID3D12DescriptorHeap> GetDsvHeapForImgui() { return dsvHeapForImgui; }

	/// <summary>
	/// �}���`�p�X�p�f�B�X�N���v�^�q�[�v
	/// </summary>
	/// <returns></returns>
	ID3D12DescriptorHeap *GetMuliPassSrvDescHeap() { return mpSrvHeap.Get(); }


	void ManualRelease() {
		dxgiFactory.ReleaseAndGetAddressOf();
		device.ReleaseAndGetAddressOf();
		commandAllocator.ReleaseAndGetAddressOf();
		commandList.ReleaseAndGetAddressOf();
		commandQueue.ReleaseAndGetAddressOf();
		swapchain.ReleaseAndGetAddressOf();
		depthBuffer.ReleaseAndGetAddressOf();
		rtvHeaps.ReleaseAndGetAddressOf();
		dsvHeap.ReleaseAndGetAddressOf();
		fence.ReleaseAndGetAddressOf();
	}

private:
	//DirectX12�v�f�̃����o�ϐ�

	// �E�B���h�E�Y�A�v���P�[�V�����Ǘ�
	Raki_WinAPI *winApp = nullptr;

	// Direct3D�֘A
	ComPtr<IDXGIFactory6>				dxgiFactory;
	ComPtr<ID3D12Device>				device;
	ComPtr<ID3D12GraphicsCommandList>	commandList;
	ComPtr<ID3D12CommandAllocator>		commandAllocator;
	ComPtr<ID3D12CommandQueue>			commandQueue;
	ComPtr<IDXGISwapChain4>				swapchain;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	ComPtr<ID3D12Resource>				depthBuffer;
	ComPtr<ID3D12DescriptorHeap>		rtvHeaps;
	ComPtr<ID3D12DescriptorHeap>		dsvHeap;
	ComPtr<ID3D12Fence>					fence;
	UINT64								fenceVal = 0;

	//�I�v�V����

	//�N���A�J���[
	float clearColor_r = 0.1f;
	float clearColor_g = 0.25f;
	float clearColor_b = 0.5f;
	float clearColor_a = 0.0f;

	//imgui�֘A
	ComPtr<ID3D12DescriptorHeap>		dsvHeapForImgui;

	//�}���`�p�X�֘A

	//���\�[�X
	ComPtr<ID3D12Resource>				mpResource;	//�y���|���S�����\�[�X
	ComPtr<ID3D12DescriptorHeap>		mpRtvHeap;	//�����_�[�^�[�Q�b�g�p
	ComPtr<ID3D12DescriptorHeap>		mpSrvHeap;	//�e�N�X�`���p

	ComPtr<ID3D12Resource>				mpVertBuff; //���ʕ`��p���_�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW			mpvbView;	//���ʕ`��p���_�o�b�t�@�r���[
	ComPtr<ID3D12PipelineState>			mpPipeline;	//�y���|���S���p�p�C�v���C��
	ComPtr<ID3D12RootSignature>			mpRootsig;	//�y���|���S���p���[�g�V�O�l�`��
	ComPtr<ID3D12Resource>				mpDepth;

	bool isRenderTarget = false;
	bool isBackBuffer = false;


private:
	//DirectX12�̏������֐��Q

	/// <summary>
	/// DXGI�f�o�C�X������
	/// </summary>
	/// <returns>����</returns>
	bool InitDXGIDevice();

	/// <summary>
	/// �R�}���h�֘A�̐���
	/// </summary>
	/// <returns>����</returns>
	bool CreateCommand();

	/// <summary>
	/// �X���b�v�`�F�[������
	/// </summary>
	/// <returns>����</returns>
	bool CreateSwapChain();

	/// <summary>
	/// �����_�[�^�[�Q�b�g�r���[����
	/// </summary>
	/// <returns>����</returns>
	bool CreateRenderTargetView();

	bool CreateSecondRenderTargetAndResource();

	/// <summary>
	/// �[�x�o�b�t�@�[����
	/// </summary>
	/// <returns>����</returns>
	bool CreateDepthBuffer();

	/// <summary>
	/// �t�F���X����
	/// </summary>
	/// <returns>����</returns>
	bool CreateFence();

	/// <summary>
	/// ���͌n������
	/// </summary>
	bool InitInput(Raki_WinAPI *win);

	/// <summary>
	/// imgui�p�f�X�N���v�^�q�[�v
	/// </summary>
	/// <returns></returns>
	bool CreateDsvHeapForIngui();

};

//<<<<<<< HEAD
//
//=======
//>>>>>>> master
#define RAKI_DX12B_DEV (Raki_DX12B::Get()->GetDevice())
#define RAKI_DX12B_CMD (Raki_DX12B::Get()->GetGCommandList())
#define RAKI_DX12B_GET (Raki_DX12B::Get())
