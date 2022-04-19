#pragma once
#include <vector>
#include <RVector.h>
#include "Easing.h"
#include "Sprite.h"
#include "ParticleManager.h"

/*���������傫���Ȃ肷���錜�O������ӏ���char�^�ɂ��Ă��܂�*/

enum MapchipData
{
	EMPTY_STAGE = 0,
	NONE = 1,
	BLOCK = 2,
	GOAL = 3,
	START = 4
};

class PlayerBody;

class ParticleSingle : public ParticlePrototype
{
public:
	//�J�n�ʒu�ۑ��p
	RVector3 spos;

	ParticleSingle(RVector3 start)
	{
		Init();
	}

	void Init();
	void Update();
	ParticlePrototype* clone(RVector3 start);
};

class Stage final
{
public: //�V���O���g����
	static Stage* Get();
private:
	Stage();
	Stage(const Stage&) = delete;
	~Stage();
	Stage operator=(const Stage&) = delete;

public: //�T�u�N���X
	struct StageTileData
	{
		char* mapchip = nullptr;
		char stageNumber = 0;
		char offsetX = 0;
		char offsetY = 0;
		std::vector<RVector3> drawLeftUp = {};
		std::vector<RVector3> drawRightDown = {};
		size_t size = 1;
		size_t width = 1;
		size_t height = 1;
		unsigned char direction = 0;
		bool isFold = false;

		Easing stageEase = {};
		std::vector<RVector3> startPos = {};
		std::vector<RVector3> endPos = {};
		std::vector<RVector3> easePos = {};

		bool isTop = true;
	};
	struct StageData
	{
		char* stageTile = nullptr;
		char offsetX = 0;
		char offsetY = 0;
		size_t width = 1;
		size_t height = 1;
		std::vector<StageTileData> stageTileData;
		std::vector<char> stageOffsetX;
		std::vector<char> stageOffsetY;
	};

public: //�萔
	static const int blockSize;
	static const int halfBlockSize;

	RVector3 offset = { 0,0,0 };

private: //�ÓI�����o�ϐ�
	static int startPlayerPosX;
	static int startPlayerPosY;
	static unsigned char initFoldCount[4];

public: //�����o�֐�
	// ������
	void Init();
	// �X�V
	void Updata();
	// �`��
	void Draw(int offsetX, int offsetY);

	/// <summary>
	/// �X�e�[�W�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="foldCount"> �v���C���[���܂���(0�Ԃ����A���A���A�E) </param>
	/// <param name="fileHandle"> �X�e�[�W�t�@�C���̃p�X </param>
	/// <returns> 0�Ő����A0�ȊO�Ŏ��s </returns>
	int LoadStage(const char* fileHandle, unsigned char playerTileArray[4]);

	// �X�e�[�W��܂�E�J��
	int FoldAndOpen(const RVector3& playerPos, unsigned char foldCount[4], PlayerBody BodyStatus[4], bool IsFootAction, bool IsFolds[4], int OpenCount, bool IsOpens[4]);
	// ���Z�b�g
	void Reset();
	// �����f�[�^�S�폜
	void DataClear();

	// �v���C���[��x����̊J�n�ʒu���擾
	inline static int GetStartPlayerPosX() { return startPlayerPosX; }
	// �v���C���[��x����̊J�n�ʒu���擾
	inline static int GetStartPlayerPosY() { return startPlayerPosY; }
	// �v���C���[�̐܂��񐔂̏�����Ԃ��擾
	static void GetInitFoldCount(unsigned char foldCount[4]);
	// �X�e�[�W�^�C���̃f�[�^���擾
	inline StageTileData* GetStageTileData(const size_t& stageNumber, const size_t& stageTileNumber);
	// �X�e�[�W�̃f�[�^���擾
	inline StageData* GetStageData(const size_t& stageNumber);
	// �S�X�e�[�W�̃f�[�^���擾
	inline StageData* GetAllStageData();
	// Stagedata�̃T�C�Y���擾
	inline size_t GetStageDataSize() { return stageData.size(); }
	// Stagetiledata�̃T�C�Y���擾
	inline size_t GetStageTileDataSize(const size_t& stageNumber)
	{
		return stageData[stageNumber].stageTileData.size();
	}
	// �X�e�[�W�̉������擾
	inline size_t GetStageWidth(const size_t& stageNumber) { return stageData[stageNumber].width; }
	// �X�e�[�W�̏c�����擾
	inline size_t GetStageHeight(const size_t& stageNumber) { return stageData[stageNumber].height; }
	// �C�ӂ�StageTile�̕����擾
	inline size_t GetStageTileWidth(const size_t& stageNumber, const size_t& stageTileNumber)
	{
		return stageData[stageNumber].stageTileData[stageTileNumber].width;
	}
	// �C�ӂ�StageTile�̍������擾
	inline size_t GetStageTileHeight(const size_t& stageNumber, const size_t& stageTileNumber)
	{
		return stageData[stageNumber].stageTileData[stageTileNumber].height;
	}
	// �C�ӂ̏ꏊ�̃}�b�v�`�b�v�����擾
	inline char GetStageMapchip(const size_t& stageNumber, const size_t& stageTileNumber, int mapchipPos)
	{
		return stageData[stageNumber].stageTileData[stageTileNumber].mapchip[mapchipPos];
	}
	// �C�ӂ̍��W����ǂ�StageTile�ɂ��邩���擾
	bool GetPositionTile(RVector3 center, int i, int j);
	// �X�e�[�W�^�C����X���̃I�t�Z�b�g��Ԃ�
	inline char GetStageTileOffsetX(const size_t& stageNumber, const size_t& stageTileNumber)
	{
		return stageData[stageNumber].stageTileData[stageTileNumber].offsetX;
	}
	// �X�e�[�W�^�C����Y���̃I�t�Z�b�g��Ԃ�
	inline char GetStageTileOffsetY(const size_t& stageNumber, const size_t& stageTileNumber)
	{
		return stageData[stageNumber].stageTileData[stageTileNumber].offsetY;
	}
	//�p�[�e�B�N������
	void CreateParticle(const size_t& StageDataNum, const size_t& StageTileDataNum);

private:
	// �X�e�[�W��܂�
	int Fold(unsigned char playerTile[4], const unsigned char& direction, const size_t& onPlayerStage, const size_t& onPlayerStageTile, const size_t& moveStageData);
	// �X�e�[�W���J��
	int Open(unsigned char playerTile[4], const unsigned char& direction, const size_t& onPlayerStage, const size_t& moveStageTile, const size_t& moveStageData);

	// �C�[�W���O�̏�����
	void EaseingInit(const size_t& onPlayerStage, const size_t& moveStageData, const int& direction);
	// �C�[�W���O�̍X�V
	void EaseingUpdate();
	// ��ԏ�̃X�e�[�W�^�C����T��
	int SearchTopStageTile();

private: //�����o�ϐ�
	std::vector<StageData> stageData;
	std::vector<StageData> initStageData;

	UINT BlockHandle;
	UINT EnptyHandle;
	UINT GoalHandle;

	Sprite MapchipSpriteBlock;
	Sprite MapchipSpriteEnpty;
	Sprite MapchipSpriteGoal;

	ParticleManager* Particlemanager;

	bool IsParticleTrigger;

	ParticleSingle* FoldParticle;
};
