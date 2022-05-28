#pragma once
#include <vector>
#include <RVector.h>
#include "Easing.h"
#include "Sprite.h"
#include "ParticleManager.h"

//折るエフェクト
#include "OriEffects.h"

//サウンド
#include <Audio.h>

enum MapchipData
{
	EMPTY_STAGE = 0,
	NONE = 1,
	BLOCK = 2,
	GOAL = 3,
	START = 4,

	HORIZONTAL = 10,
	VERTICAL = 11,

	LEFTONLY = 20,
	UPONLY = 21,
	RIGHTONLY = 22,
	DOWNONLY = 23,

	LEFTL = 30,
	UPL = 31,
	RIGHTL = 32,
	DOWNL = 33,

	LEFTU = 40,
	UPU = 41,
	RIGHTU = 42,
	DOWNU = 43,

	NOFRAME = 50
};

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
	~ParticleSingle() {};

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
		//ステージの構成(CSVのステージの情報)の中で1になっている順番
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

		//相対的な位置
		int RelativePositionFold = -1;
		int RelativePositionOpen = -1;

		//どのステージのタイルなのか
		int StageGroup = -1;

		//折られている場合の方向
		int FoldDirection = -1;

		//上に重なっている枚数
		int Overlap = 0;

		//Overlapを設定したかどうか
		bool IsOverSet = false;
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
	static const int blockSize;     //ブロックの大きさ
	static const int halfBlockSize; //ブロックの半分の大きさ

	static const int lineWidth;         //線の太さ
	static const int foldLineCount;     //折れ目の間隔
	static const XMFLOAT4 backColor[4]; //ステージの何もない場所の色
	static const XMFLOAT4 lineColor[4]; //線の色

	RVector3 offset = { 0,0,0 };

public: //静的メンバ関数
	// プレイヤーのX軸上の開始位置を取得
	static inline int GetStartPlayerPosX() { return startPlayerPosX; }
	// プレイヤーのY軸上の開始位置を取得
	static inline int GetStartPlayerPosY() { return startPlayerPosY; }
	// プレイヤーの折れる回数の初期状態を取得
	static void GetInitFoldCount(unsigned char foldCount[4]);

public: //静的メンバ変数
	static int drawOffsetX;
	static int drawOffsetY;

	static bool isMoveSelectCursor;
private:
	static int startPlayerPosX;
	static int startPlayerPosY;
	static char nowPlayerStage; //プレイヤーがいるステージ
	static unsigned char initFoldCount[4];

public: //メンバ関数
	// 初期化
	void Init();
	// 更新
	void Updata();
	// 描画
	void Draw(const int offsetX = 0, const int offsetY = 0);
	// 生成
	void Create();

	//ブロックの画像ハンドルの読み込み
	void LoadBlocksHandle();

	//ブロックスライトの生成
	void CreateBlocksSprite();

	/// <summary>
	/// ステージファイルの読み込み
	/// </summary>
	/// <param name="foldCount"> プレイヤーが折れる回数(0番から上、下、左、右) </param>
	/// <param name="fileHandle"> ステージファイルのパス </param>
	/// <returns> 0で成功、0以外で失敗 </returns>
	int LoadStage(const char* fileHandle, unsigned char playerTileArray[4]);

	void SelectingStageTile();

	// ステージを折る・開く
	int FoldAndOpen(const RVector3& playerPos, bool BodyStatus[4], bool IsFootAction, bool IsFolds[4], int OpenCount, bool IsOpens[4]);

	//セレクトしたタイルが指定の方向に折れるかどうか
	bool IsTileFoldDirection(size_t stage, int direction);

	//今いるタイルがほかのタイルと重なっているかどうか
	bool IsNowTileOver(size_t stage, size_t tile);

	/// <summary>
	/// ステージがどう折れるかの予測
	/// </summary>
	/// <param name="playerPos"> プレイヤーの座標 </param>
	/// <param name="direction"> 折りたい方向 </param>
	/// <param name="returnMapchip"> マップチップ(出力) </param>
	/// <returns> 0で折れる、1で開ける、-1で失敗 </returns>
	int FoldSimulation(const RVector3& playerPos, const unsigned char& direction, char** returnMapchip);
	// リセット
	void Reset(unsigned char foldCount[4]);

	// 内部データ全削除
	void DataClear();

	//Overlapの設定
	void SetOverlap(size_t stagenum, size_t tilenum);

	//Overlapの更新
	void UpdateOverlap() {}

	// 任意の座標が任意のステージにいるかどうか
	bool IsPositionStage(const RVector3& center, const size_t& stageNumber);

	//プレイヤーがどのタイルにいるか
	bool IsPlayerTile(const size_t& stageNumber, const size_t& TileNumber);

	//onplayerstageを設定(折る時)
	void SetOnPlayerStageTileFold(std::vector<size_t>& stagenumber, std::vector<size_t>& onplayerstage,
		std::vector<size_t>& movestagetile, std::vector<size_t>& moveStageData, const unsigned char& direction);

	//onplayerstageを設定(開く時)
	void SetOnPlayerStageTileOpen(std::vector<size_t>& stagenumber, std::vector<size_t>& onplayerstage,
		std::vector<size_t>& movestagetile, std::vector<size_t>& moveStageData, const unsigned char& direction);

	//ステージの中で指定した方向のoffset血を比べる
	XMFLOAT2 ReturnMostOffset(const unsigned char& direction, const size_t& stageNumber);

	//ステージグループのセッティング
	void SetStageGroup();

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
	// ステージタイルの幅を取得
	inline size_t GetStageTileWidth(const size_t& stageNumber, const size_t& stageTileNumber)
	{
		return stageData[stageNumber].stageTileData[stageTileNumber].width;
	}
	// ステージタイルの高さを取得
	inline size_t GetStageTileHeight(const size_t& stageNumber, const size_t& stageTileNumber)
	{
		return stageData[stageNumber].stageTileData[stageTileNumber].height;
	}
	// ステージタイルのX軸のオフセットの初期値を返す
	inline char GetStageTileInitOffsetX(const size_t& stageNumber, const size_t& stageTileNumber)
	{
		return initStageData[stageNumber].stageTileData[stageTileNumber].offsetX;
	}
	// ステージタイルのY軸のオフセットの初期値を返す
	inline char GetStageTileInitOffsetY(const size_t& stageNumber, const size_t& stageTileNumber)
	{
		return initStageData[stageNumber].stageTileData[stageTileNumber].offsetY;
	}
	// ステージタイルの幅の初期値を取得
	inline size_t GetStageTileInitWidth(const size_t& stageNumber, const size_t& stageTileNumber)
	{
		return initStageData[stageNumber].stageTileData[stageTileNumber].width;
	}
	// ステージタイルの高さの初期値を取得
	inline size_t GetStageTileInitHeight(const size_t& stageNumber, const size_t& stageTileNumber)
	{
		return initStageData[stageNumber].stageTileData[stageTileNumber].height;
	}
	// 任意の場所のマップチップ情報を取得
	inline char GetStageMapchip(const size_t& stageNumber, const size_t& stageTileNumber, int mapchipPos)
	{
		return stageData[stageNumber].stageTileData[stageTileNumber].mapchip[mapchipPos];
	}
	//引数で取ったマップチップがブロックかどうか
	bool IsMapchipBlocks(char mapchip);
	// プレイヤーがいるステージを取得
	char GetPositionStage(const RVector3& playerPos);
	// 任意の座標からどのステージタイルにいるかを取得
	void GetPositionTile(const RVector3& center, size_t* stageNumber, size_t* stageTileNumber,
						 bool isSkip = false, const size_t& skipStageNumber = 0);
	// 任意の座標からどのステージタイルにいるかを取得(初期状態)
	void GetPositionInitTile(const RVector3& center, size_t* stageNumber, size_t* stageTileNumber,
							 bool isSkip = false, const size_t& skipStageNumber = 0);
	//指定した場所のステージタイルにプレイヤーがいるかどうか
	bool IsPositionInitTile(size_t StageNum, size_t StageTileNum);
	//任意の座標がどこかのタイルにいたらtrue
	bool IsAnyTile(const RVector3& center);
	//任意の座標がステージタイルにいるのか
	bool IsPositionTile(const RVector3& center, const size_t& stageNumber, const size_t& stageTileNumber);

	//パーティクル生成
	void CreateParticle(const size_t& StageDataNum, const size_t& StageTileDataNum);

	// ステージのサウンドの読み込み
	void LoadStageSound();
	// ステージのサウンドデータの削除
	void DeleteStageSound();
private:
	// ステージを折る
	int Fold(const unsigned char& direction, const size_t& onPlayerStage, const size_t& onPlayerStageTile,
		const size_t& moveStageData, size_t datasize);
	// ステージを開く
	int Open(const unsigned char& direction, const size_t& onPlayerStage,
		const size_t& moveStageData, size_t datasize);

	// ステージタイルの描画
	// saturationColorが1だと元の色で表示する
	int StageTileDraw(const size_t& stageNumber, const size_t& stageTileNumber, const XMFLOAT2& offset, const float saturationColor = 1.0f);
	// 枠線・折り目の描画
	// saturationColorが1だと元の色で表示する
	int LineDraw(const size_t& stageNumber, const XMFLOAT2& offset, const float saturationColor = 1.0f);
	// 枠線の描画
	int FlameDraw(const size_t& stageNumber, const size_t& stageTileNumber, const unsigned char direction,
		const int offsetX, const int offsetY);
	// 折り目の描画
	int FoldDraw(const size_t& stageNumber, const size_t& stageTileNumber, const unsigned char direction,
		const int offsetX, const int offsetY);

	// イージングの初期化
	void EaseingInit(const size_t& moveStage, const size_t& moveTile, const int& direction);
	// イージングの更新
	void EaseingUpdate();
	// 一番上のステージタイルを探す
	int SearchTopStageTile();

private: //メンバ変数
	std::vector<StageData> stageData;
	std::vector<StageData> initStageData;

	StageTileData* SelectTile;
	size_t selectStageNum = 0;
	size_t selectTileNum = 0;

	char* reverseMapchip;

	//折り目の画像ハンドル
	UINT lineHandle;
	//ブロックの画像ハンドル
	//UINT BlocksHandle[4];
	UINT AllBlockHandle[4][16];
	UINT Bule_BlocksHandle[15];
	UINT Green_BlocksHandle[15];
	UINT Red_BlocksHandle[15];
	UINT Yellow_BlocksHandle[15];
	//空白の画像ハンドル
	UINT EmptyHandle;
	//ゴールの画像ハンドル
	UINT GoalHandle;

	//折り目のスプライト
	Sprite lineSprite;
	//ブロックのスプライト
	//Sprite MapchipSpriteBlocks[4];
	Sprite AllBlockSprite[4][16];
	//空白のスプライト
	Sprite MapchipSpriteEmpty;
	//ゴールのスプライト
	Sprite MapchipSpriteGoal;

	//タイル選択アイコンのスプライト
	Sprite SelectIconSprite[4];

	bool IsParticleTrigger;
	ParticleManager* particleManager;
	ParticleSingle* FoldParticle;

	//折るエフェクト
	OriEffects oriEffect;

	// サウンド
	SoundData foldSound; //折る時のSE
	SoundData selectSound; //枠線選択時のSE
};
