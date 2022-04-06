#pragma once
#include <vector>
#include "Vector3.h"
#include "Easing.h"

/*メモリが大きくなりすぎる懸念がある箇所はchar型にしています*/

enum MapchipData
{
	EMPTY_STAGE = 0,
	NONE = 1,
	BLOCK = 2,
	GOAL = 3,
	START = 4
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
		std::vector<Vector3> drawLeftUp = {};
		std::vector<Vector3> drawRightDown = {};
		size_t size = 1;
		unsigned char width = 1;
		unsigned char height = 1;
		unsigned char direction = 0;
		bool isFold = false;

		Easing stageEase = {};
		std::vector<Vector3> startPos = {};
		std::vector<Vector3> endPos = {};
		std::vector<Vector3> easePos = {};

		bool isTop = true;
	};
	struct StageData
	{
		char* stageTile = nullptr;
		char offsetX = 0;
		char offsetY = 0;
		unsigned char width = 1;
		unsigned char height = 1;
		std::vector<StageTileData> stageTileData;
		std::vector<char> stageOffsetX;
		std::vector<char> stageOffsetY;
	};

public: //定数
	static const int blockSize;
	static const int halfBlockSize;

	Vector3 offset = { 0,0,0 };

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
	int FoldAndOpen(const Vector3& playerPos, unsigned char foldCount[4]);
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
	inline StageTileData* GetStageTileData(const short& stageNumber, const short& stageTileNumber);
	// ステージのデータを取得
	inline StageData* GetStageData(const short& stageNumber);
	// 全ステージのデータを取得
	inline StageData* GetAllStageData();
	// Stagedataのサイズを取得
	size_t GetStageDataSize();
	// Stagetiledataのサイズを取得
	size_t GetStageTileDataSize(int i);
	// ステージの横幅を取得
	char GetStageWidth(int i);
	// ステージの縦幅を取得
	char GetStageHeight(int i);
	// 任意のStageTileの高さを取得
	char GetStageTileHeight(int i, int j);
	// 任意のStageTileの幅を取得
	char GetStageTileWidth(int i, int j);
	// 任意の場所のマップチップ情報を取得
	char GetStageMapchip(int i, int j, int mapchipPos);
	// 任意の座標からどのStageTileにいるかを取得
	bool GetPositionTile(Vector3 center, int i, int j);
	// ステージタイルのX軸のオフセットを返す
	int GetStageTileOffsetX(int i, int j);
	// ステージタイルのY軸のオフセットを返す
	int GetStageTileOffsetY(int i, int j);
	//任意の方向に特定のマップチップがあるかどうか
	bool IsNeighberMapchip(int i, int j, int MapchipPos, MapchipData mapchipData);

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
};
