#pragma once
#include <vector>
#include <RVector.h>
#include "Easing.h"
#include "Sprite.h"
#include "ParticleManager.h"

/*メモリが大きくなりすぎる懸念がある箇所はchar型にしています*/

enum MapchipData
{
	EMPTY_STAGE = 0,
	NONE = 1,
	BLOCK = 2,
	GOAL = 3,
	START = 4
};

class PlayerBody;

//パーティクル派生クラス
class ParticleSingle : public ParticlePrototype
{
public:
	//開始位置保存用
	RVector3 spos;

	ParticleSingle()
	{
		Init();
	}
	~ParticleSingle();

	void Init() override;
	void Update() override;
	ParticlePrototype* clone(RVector3 start) override;
};

class Stage final
{
public: //シングルトン化
	static Stage* Get();
private:
	Stage();
	Stage(const Stage&) = delete;
	~Stage();
	Stage operator=(const Stage&) = delete;

public: //サブクラス
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

public: //定数
	static const int blockSize;
	static const int halfBlockSize;

	RVector3 offset = { 0,0,0 };

private: //静的メンバ変数
	static int startPlayerPosX;
	static int startPlayerPosY;
	static unsigned char initFoldCount[4];

public: //メンバ関数
	// 初期化
	void Init();
	// 更新
	void Updata();
	// 描画
	void Draw(int offsetX, int offsetY);

	/// <summary>
	/// ステージファイルの読み込み
	/// </summary>
	/// <param name="foldCount"> プレイヤーが折れる回数(0番から上、下、左、右) </param>
	/// <param name="fileHandle"> ステージファイルのパス </param>
	/// <returns> 0で成功、0以外で失敗 </returns>
	int LoadStage(const char* fileHandle, unsigned char playerTileArray[4]);

	// ステージを折る・開く
	int FoldAndOpen(const RVector3& playerPos, unsigned char foldCount[4], PlayerBody BodyStatus[4], bool IsFootAction, bool IsFolds[4], int OpenCount, bool IsOpens[4]);
	// リセット
	void Reset();
	// 内部データ全削除
	void DataClear();

	// プレイヤーのx軸上の開始位置を取得
	inline static int GetStartPlayerPosX() { return startPlayerPosX; }
	// プレイヤーのx軸上の開始位置を取得
	inline static int GetStartPlayerPosY() { return startPlayerPosY; }
	// プレイヤーの折れる回数の初期状態を取得
	static void GetInitFoldCount(unsigned char foldCount[4]);
	// ステージタイルのデータを取得
	inline StageTileData* GetStageTileData(const size_t& stageNumber, const size_t& stageTileNumber);
	// ステージのデータを取得
	inline StageData* GetStageData(const size_t& stageNumber);
	// 全ステージのデータを取得
	inline StageData* GetAllStageData();
	// Stagedataのサイズを取得
	inline size_t GetStageDataSize() { return stageData.size(); }
	// Stagetiledataのサイズを取得
	inline size_t GetStageTileDataSize(const size_t& stageNumber)
	{
		return stageData[stageNumber].stageTileData.size();
	}
	// ステージの横幅を取得
	inline size_t GetStageWidth(const size_t& stageNumber) { return stageData[stageNumber].width; }
	// ステージの縦幅を取得
	inline size_t GetStageHeight(const size_t& stageNumber) { return stageData[stageNumber].height; }
	// 任意のStageTileの幅を取得
	inline size_t GetStageTileWidth(const size_t& stageNumber, const size_t& stageTileNumber)
	{
		return stageData[stageNumber].stageTileData[stageTileNumber].width;
	}
	// 任意のStageTileの高さを取得
	inline size_t GetStageTileHeight(const size_t& stageNumber, const size_t& stageTileNumber)
	{
		return stageData[stageNumber].stageTileData[stageTileNumber].height;
	}
	// 任意の場所のマップチップ情報を取得
	inline char GetStageMapchip(const size_t& stageNumber, const size_t& stageTileNumber, int mapchipPos)
	{
		return stageData[stageNumber].stageTileData[stageTileNumber].mapchip[mapchipPos];
	}
	// 任意の座標からどのStageTileにいるかを取得
	bool GetPositionTile(RVector3 center, int i, int j);
	// ステージタイルのX軸のオフセットを返す
	inline char GetStageTileOffsetX(const size_t& stageNumber, const size_t& stageTileNumber)
	{
		return stageData[stageNumber].stageTileData[stageTileNumber].offsetX;
	}
	// ステージタイルのY軸のオフセットを返す
	inline char GetStageTileOffsetY(const size_t& stageNumber, const size_t& stageTileNumber)
	{
		return stageData[stageNumber].stageTileData[stageTileNumber].offsetY;
	}
	//パーティクル生成
	void CreateParticle(const size_t& StageDataNum, const size_t& StageTileDataNum);

private:
	// ステージを折る
	int Fold(unsigned char playerTile[4], const unsigned char& direction, const size_t& onPlayerStage, const size_t& onPlayerStageTile, const size_t& moveStageData);
	// ステージを開く
	int Open(unsigned char playerTile[4], const unsigned char& direction, const size_t& onPlayerStage, const size_t& moveStageTile, const size_t& moveStageData);

	// イージングの初期化
	void EaseingInit(const size_t& onPlayerStage, const size_t& moveStageData, const int& direction);
	// イージングの更新
	void EaseingUpdate();
	// 一番上のステージタイルを探す
	int SearchTopStageTile();

private: //メンバ変数
	std::vector<StageData> stageData;
	std::vector<StageData> initStageData;

	UINT BlockHandle;
	UINT EnptyHandle;
	UINT GoalHandle;

	UINT ParticleHandle;

	Sprite MapchipSpriteBlock;
	Sprite MapchipSpriteEnpty;
	Sprite MapchipSpriteGoal;

	bool IsParticleTrigger;
	ParticleManager* Particlemanager = ParticleManager::Create();
	ParticleSingle* FoldParticle = new ParticleSingle();
};
