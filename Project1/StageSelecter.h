#pragma once
#include <memory>
#include <array>
#include <TexManager.h>
#include <Sprite.h>
#include <Raki_Input.h>
#include <Audio.h>

#include "ButtonUI.h"

//ページ1枚分（これはあくまでも表示と描画座標の管理のみを担う。実際の選択や遷移の処理はSelecterの仕事）
class Page
{
public:
	Page(){};
	~Page(){};

	//初期化（アイコン中心座標の定義配列、グラフィックハンドルの配列、カーソルのグラフィックハンドルの配列）
	void Init(float xicons[], float yicons[], std::array<UINT, 4> uiGraphHandles, UINT cursorR, UINT cursorL,RVector3 easeTarget);
	//更新
	void Update();
	//描画（左上基準）
	void Draw();

	void ChangeDisplayMode();

	//各アイコンの中心座標
	std::array<float, 4> iconX, iconY;
	//ボタンリソース(Selecterがアクセスする)
	std::array<UI_Button, 4> stageIconButton;

	//イージング目標に移動するかのフラグ
	bool isDisplay = true;
	bool isMustMoving = true;
	//イージング関連
	RVector3 easeTarget;
	RVector3 easeStart = { -1280,0,0 };
	//描画座標左上
	RVector3 drawLTpos;
	//イージング最大フレーム
	const int EASE_FRAME = 60;
	int frame = 0;


private:
	//背景
	Sprite backSprite;
	//レンダーテクスチャハンドル
	int rtHandle;
	//レンダーテクスチャスプライト
	Sprite rtSprite;
	//左右スプライト
	Sprite cursorR, cursorL;

};

//ステージ選択を管理する
class StageSelecter
{
private:
	//ステージ本編を管理するクラスのポインタ（参照しているだけなのでweak_ptrにしたい）

public:
	StageSelecter();
	~StageSelecter();

	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw();
	//終了
	void Finalize();

	//ゲーム本編へ遷移するフラグ
	bool GetMoveGameMain() { return isChanging_GameMain; }

	//ゲームメインへ遷移するフラグ
	bool isChanging_GameMain = false;

	//選択したステージ番号(GameSceneに渡す用)
	int SelectStageNum = 0;

	//現在の状態
	enum NOW_SELECTER_STATE
	{
		is_selecting,				//選択中（ステージ番号の入力を受け付ける）
		is_pageChange_waiting,		//ページチェンジ演出中
		is_stageSelected_waiting,	//ステージ選択完了、遷移演出実行中
	};
	NOW_SELECTER_STATE state;

	void GoNextStage();

	void LoadStage(int stagenum);

private:
	int _nowStageNumber = 0;

	//番号
	std::array<Sprite, 20> stageNumIcon;
	//ステージ背景
	std::array<Sprite, 20> stageBack;

	//ページ
	std::array<Page, 5> stagePage;

	//カーソル
	Sprite selectCursor;
	const int cursorSpriteCount = 4;

	Sprite SelectLeft;
	Sprite SelectRight;

	//現在表示するステージのページ
	enum STAGE_PAGE
	{
		page_1_4,
		page_5_8,
		page_9_12,
		page_13_16,
		page_17_20,
	};
	STAGE_PAGE nowpage;		//現在のページ
	STAGE_PAGE nextpage;	//前のページ
	STAGE_PAGE displayPage;	//表示するページ

	enum NOW_SELECTING
	{
		//戻る
		UI_BACK,
		//ステージアイコン
		UI_STAGEBOX_1,
		UI_STAGEBOX_2,
		UI_STAGEBOX_3,
		UI_STAGEBOX_4,
		//進む
		UI_FRONT,
	};
	NOW_SELECTING user_selecting;
	void Changing_UI_Number();

	enum PAGE_MOVE_DIR
	{
		is_front,
		is_back,
	};
	PAGE_MOVE_DIR pageMoveDir;

	//表示番号
	int nowDisplayNum = 0;

	//ロード関数（連番画像のためにint -> string変換）
	void LoadSprite();
	
	//ステージアイコン画像のロード
	std::array<UINT,4> LoadStageIcons(int pageNumber);

	//ページ遷移のための入力検知、ページ状態切替
	void CheckToPageChangeInput();

	//ページ状態に応じた処理
	int animationFrame = 0;
	const int perFrame = 3;
	void PageChange();

	//ステージ遷移のための入力検知
	const int NUMBOX_START_X = 284;
	const int NUMBOX_START_Y = 325;
	const int NUMBOX_SIZE = 129;
	const int NUMBOX_SPACE = 65;
	const int PAGEMOVE_LEFT_X = 29;
	const int PAGEMOVE_RIGHT_X = 1184;
	const int PAGEMOVE_Y = 623;
	const int PAGEMOVE_SIZE = 67;
	void CheckLoadStage(int boxnum);
	void CheckToStageChangeInput();

	//描画関数系
	void DrawCursor();


	void DrawPages();

	//BGM
	SoundData menuBGM;
};

