#include "Stage.h"
#include "LoadFile.h"
#include "General.h"
#include "Player.h"
#include "NY_random.h"
#include <Raki_WinAPI.h>
#include "InputManger.h"

#define EF (-1) //Error Function

#define ContainerClear(container)\
container.clear();\
container.shrink_to_fit();

namespace
{
	static size_t i = 0, j = 0; //for文のループカウンタ
	static size_t x = 0, y = 0; //マップチップ上の座標

	static size_t mapchipPos = 0; //マップチップの要素番号
	static size_t reverseMapchipPos = 0; //反転したマップチップの要素番号

	static Player* player = Player::Get(); //プレイヤー
	static InputManger* inputManeger = InputManger::Get();
}

const int Stage::blockSize = 60;
const int Stage::halfBlockSize = Stage::blockSize / 2;
const int Stage::lineWidth = 5;
const int Stage::foldLineCount = 2;
const XMFLOAT4 Stage::backColor[4] = {
	{ 0x6e / 255.0f, 0x93 / 255.0f, 0xa8 / 255.0f, 0.9f }, //0x6e93a8
	{ 0x76 / 255.0f, 0xa8 / 255.0f, 0x6e / 255.0f, 0.9f }, //0x76a86e
	{ 0xa8 / 255.0f, 0x6e / 255.0f, 0x70 / 255.0f, 0.9f }, //0xa86e70
	{ 0xa8 / 255.0f, 0xa5 / 255.0f, 0x75 / 255.0f, 0.9f }  //0xa8a575
};
const XMFLOAT4 Stage::lineColor[4] = {
	{ 0x6e / 255.0f, 0x93 / 255.0f, 0xa8 / 255.0f, 0.9f }, //0x6e93a8
	{ 0x76 / 255.0f, 0xa8 / 255.0f, 0x6e / 255.0f, 0.9f }, //0x76a86e
	{ 0xa8 / 255.0f, 0x6e / 255.0f, 0x70 / 255.0f, 0.9f }, //0xa86e70
	{ 0xa8 / 255.0f, 0xa5 / 255.0f, 0x75 / 255.0f, 0.9f }  //0xa8a575
};

int Stage::drawOffsetX = 0;
int Stage::drawOffsetY = 0;
bool Stage::isMoveSelectCursor = false;
int Stage::startPlayerPosX = 0;
int Stage::startPlayerPosY = 0;
char Stage::nowPlayerStage = 0;
unsigned char Stage::initFoldCount[4] = { 0 };

Stage* Stage::Get()
{
	static Stage instance = {};
	return &instance;
}

Stage::Stage() :
	stageData{},
	initStageData{},
	reverseMapchip(nullptr),
	lineHandle(0),
	AllBlockHandle(),
	EmptyHandle(0),
	GoalHandle(0),
	lineSprite{},
	AllBlockSprite{},
	MapchipSpriteEmpty{},
	MapchipSpriteGoal{},
	IsParticleTrigger(false),
	particleManager(nullptr),
	FoldParticle(new ParticleSingle())
{
	Init();
}

Stage::~Stage()
{
	DataClear();
	DeleteStageSound();
}

void Stage::GetInitFoldCount(unsigned char foldCount[4])
{
	for (i = 0; i < sizeof(initFoldCount) / sizeof(initFoldCount[0]); i++)
	{
		foldCount[i] = initFoldCount[i];
	}
}

void Stage::Init()
{

}

void Stage::Updata()
{
	static int posX = 0;
	static int posY = 0;

	isMoveSelectCursor = false;
	SelectingStageTile();

	EaseingUpdate();

	particleManager->Prototype_Update();

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			if (stageData[i].stageTileData[j].stageEase.isMove)
			{
				for (y = 0; y < stageData[i].stageTileData[j].height; y++)
				{
					for (x = 0; x < stageData[i].stageTileData[j].width; x++)
					{
						static size_t easeMapchipPos = 0;

						mapchipPos = y * stageData[i].stageTileData[j].width + x;

						switch ((stageData[i].stageTileData[j].direction - 1) % 4)
						{
						case BodyType::up:
						{
							reverseMapchipPos = (stageData[i].stageTileData[j].height - y - 1) * stageData[i].stageTileData[j].width + x;

							posX = static_cast<int>(stageData[i].stageTileData[j].easePos[reverseMapchipPos].x);
							posY = static_cast<int>(stageData[i].stageTileData[j].easePos[reverseMapchipPos].y);

							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].x = static_cast<float>(posX);
							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].y = static_cast<float>(posY);
							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].z = stageData[i].stageTileData[j].easePos[mapchipPos].z;

							if ((stageData[i].stageTileData[j].height - y - 1) + 1 == static_cast<size_t>(stageData[i].stageTileData[j].height))
							{
								posX = static_cast<int>(stageData[i].stageTileData[j].startPos[reverseMapchipPos].x);
								posY = static_cast<int>(stageData[i].stageTileData[j].startPos[reverseMapchipPos].y + blockSize);
							}
							else
							{
								easeMapchipPos = ((stageData[i].stageTileData[j].height - y - 1) + 1) * stageData[i].stageTileData[j].width + x;

								posX = static_cast<int>(stageData[i].stageTileData[j].easePos[easeMapchipPos].x);
								posY = static_cast<int>(stageData[i].stageTileData[j].easePos[easeMapchipPos].y);
							}

							stageData[i].stageTileData[j].drawRightDown[mapchipPos].x = static_cast<float>(posX + blockSize);
							stageData[i].stageTileData[j].drawRightDown[mapchipPos].y = static_cast<float>(posY);
							stageData[i].stageTileData[j].drawRightDown[mapchipPos].z =
								static_cast<float>(stageData[i].stageTileData[j].easePos[mapchipPos].z);
							break;
						}
						case BodyType::down:
						{
							reverseMapchipPos = (stageData[i].stageTileData[j].height - y - 1) * stageData[i].stageTileData[j].width + x;

							posX = static_cast<int>(stageData[i].stageTileData[j].easePos[reverseMapchipPos].x);
							posY = static_cast<int>(stageData[i].stageTileData[j].easePos[reverseMapchipPos].y);

							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].x = static_cast<float>(posX);
							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].y = static_cast<float>(posY);
							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].z = stageData[i].stageTileData[j].easePos[mapchipPos].z;

							if ((stageData[i].stageTileData[j].height - y - 1) - 1 == static_cast<size_t>(-1))
							{
								posX = static_cast<int>(stageData[i].stageTileData[j].startPos[reverseMapchipPos].x);
								posY = static_cast<int>(stageData[i].stageTileData[j].startPos[reverseMapchipPos].y - blockSize);
							}
							else
							{
								easeMapchipPos = ((stageData[i].stageTileData[j].height - y - 1) - 1) * stageData[i].stageTileData[j].width + x;

								posX = static_cast<int>(stageData[i].stageTileData[j].easePos[easeMapchipPos].x);
								posY = static_cast<int>(stageData[i].stageTileData[j].easePos[easeMapchipPos].y);
							}

							stageData[i].stageTileData[j].drawRightDown[mapchipPos].x = static_cast<float>(posX + blockSize);
							stageData[i].stageTileData[j].drawRightDown[mapchipPos].y = static_cast<float>(posY);
							stageData[i].stageTileData[j].drawRightDown[mapchipPos].z =
								static_cast<float>(stageData[i].stageTileData[j].easePos[mapchipPos].z);
							break;
						}
						case BodyType::left:
						{
							reverseMapchipPos = y * stageData[i].stageTileData[j].width + (stageData[i].stageTileData[j].width - x - 1);

							posX = static_cast<int>(stageData[i].stageTileData[j].easePos[reverseMapchipPos].x);
							posY = static_cast<int>(stageData[i].stageTileData[j].easePos[reverseMapchipPos].y);

							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].x = static_cast<float>(posX);
							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].y = static_cast<float>(posY);
							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].z = stageData[i].stageTileData[j].easePos[mapchipPos].z;

							if ((stageData[i].stageTileData[j].width - x - 1) + 1 == static_cast<size_t>(stageData[i].stageTileData[j].width))
							{
								posX = static_cast<int>(stageData[i].stageTileData[j].startPos[reverseMapchipPos].x + blockSize);
								posY = static_cast<int>(stageData[i].stageTileData[j].startPos[reverseMapchipPos].y);
							}
							else
							{
								easeMapchipPos = y * stageData[i].stageTileData[j].width + ((stageData[i].stageTileData[j].width - x - 1) + 1);

								posX = static_cast<int>(stageData[i].stageTileData[j].easePos[easeMapchipPos].x);
								posY = static_cast<int>(stageData[i].stageTileData[j].easePos[easeMapchipPos].y);
							}

							stageData[i].stageTileData[j].drawRightDown[mapchipPos].x = static_cast<float>(posX);
							stageData[i].stageTileData[j].drawRightDown[mapchipPos].y = static_cast<float>(posY + blockSize);
							stageData[i].stageTileData[j].drawRightDown[mapchipPos].z =
								static_cast<float>(stageData[i].stageTileData[j].easePos[mapchipPos].z);
							break;
						}
						case BodyType::right:
						{
							reverseMapchipPos = y * stageData[i].stageTileData[j].width + (stageData[i].stageTileData[j].width - x - 1);

							posX = static_cast<int>(stageData[i].stageTileData[j].easePos[reverseMapchipPos].x);
							posY = static_cast<int>(stageData[i].stageTileData[j].easePos[reverseMapchipPos].y);

							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].x = static_cast<float>(posX);
							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].y = static_cast<float>(posY);
							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].z = stageData[i].stageTileData[j].easePos[mapchipPos].z;

							if ((stageData[i].stageTileData[j].width - x - 1) - 1 == static_cast<size_t>(-1))
							{
								posX = static_cast<int>(stageData[i].stageTileData[j].startPos[reverseMapchipPos].x - blockSize);
								posY = static_cast<int>(stageData[i].stageTileData[j].startPos[reverseMapchipPos].y);
							}
							else
							{
								easeMapchipPos = y * stageData[i].stageTileData[j].width + ((stageData[i].stageTileData[j].width - x - 1) - 1);

								posX = static_cast<int>(stageData[i].stageTileData[j].easePos[easeMapchipPos].x);
								posY = static_cast<int>(stageData[i].stageTileData[j].easePos[easeMapchipPos].y);
							}

							stageData[i].stageTileData[j].drawRightDown[mapchipPos].x = static_cast<float>(posX);
							stageData[i].stageTileData[j].drawRightDown[mapchipPos].y = static_cast<float>(posY + blockSize);
							stageData[i].stageTileData[j].drawRightDown[mapchipPos].z =
								static_cast<float>(stageData[i].stageTileData[j].easePos[mapchipPos].z);
							break;
						}
						default:
						{
							posX = static_cast<int>(stageData[i].stageTileData[j].easePos[mapchipPos].x);
							posY = static_cast<int>(stageData[i].stageTileData[j].easePos[mapchipPos].y);

							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].x = static_cast<float>(posX);
							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].y = static_cast<float>(posY);
							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].z = stageData[i].stageTileData[j].easePos[mapchipPos].z;

							stageData[i].stageTileData[j].drawRightDown[mapchipPos].x = static_cast<float>(posX + blockSize);
							stageData[i].stageTileData[j].drawRightDown[mapchipPos].y = static_cast<float>(posY + blockSize);
							stageData[i].stageTileData[j].drawRightDown[mapchipPos].z =
								static_cast<float>(stageData[i].stageTileData[j].easePos[mapchipPos].z + blockSize);
							break;
						}
						}
					}
				}
			}
			else
			{
				for (y = 0; y < stageData[i].stageTileData[j].height; y++)
				{
					for (x = 0; x < stageData[i].stageTileData[j].width; x++)
					{
						posX = static_cast<int>(x + stageData[i].stageTileData[j].offsetX);
						posY = static_cast<int>(y + stageData[i].stageTileData[j].offsetY);
						mapchipPos = y * stageData[i].stageTileData[j].width + x;

						stageData[i].stageTileData[j].drawLeftUp[mapchipPos].x = static_cast<float>(posX * blockSize);
						stageData[i].stageTileData[j].drawLeftUp[mapchipPos].y = static_cast<float>(posY * blockSize);
						stageData[i].stageTileData[j].drawLeftUp[mapchipPos].z = 0.0f;
						stageData[i].stageTileData[j].drawRightDown[mapchipPos].x = static_cast<float>((posX + 1) * blockSize);
						stageData[i].stageTileData[j].drawRightDown[mapchipPos].y = static_cast<float>((posY + 1) * blockSize);
						stageData[i].stageTileData[j].drawRightDown[mapchipPos].z = 0.0f;
					}
				}
			}
		}
	}

}

void Stage::Draw(const int offsetX, const int offsetY)
{
	static int posX = 0;
	static int posY = 0;
	static RVector3 pos1, pos2;

	static DirectX::XMFLOAT2 drawOffset = {};
	drawOffset.x = static_cast<float>(drawOffsetX) + static_cast<float>(offsetX);
	drawOffset.y = static_cast<float>(drawOffsetY) + static_cast<float>(offsetY);

	for (i = 0; i < stageData.size(); i++)
	{
		if (i == static_cast<size_t>(nowPlayerStage))
		{
			continue;
		}

		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			StageTileDraw(i, j, drawOffset);
		}

		LineDraw(i, drawOffset);
	}

	if (nowPlayerStage >= 0)
	{
		for (j = 0; j < stageData[nowPlayerStage].stageTileData.size(); j++)
		{
			StageTileDraw(nowPlayerStage, j, drawOffset, 1.1f);
		}

		LineDraw(nowPlayerStage, drawOffset, 1.1f);
	}

	// 色の初期化
	Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);

#pragma  region 選択しているステージのフレーム
	//左フレーム
	for (int l = 0; l < SelectFrame_L.size(); l++)
	{
		SelectIconSprite[0].DrawExtendSprite(
			SelectFrame_L[l].x + drawOffset.x,
			SelectFrame_L[l].y + drawOffset.y,
			SelectFrame_L[l].x + 10 + drawOffset.x,
			SelectFrame_L[l].z + drawOffset.y
		);

		for (int u = 0; u < SelectFrame_U.size(); u++)
		{
			if (SelectFrame_L[l].x == SelectFrame_U[u].y &&
				SelectFrame_L[l].z == SelectFrame_U[u].z)
			{
				SelectIconSprite[0].DrawExtendSprite(
					SelectFrame_L[l].x + drawOffset.x,
					SelectFrame_L[l].y + drawOffset.y,
					SelectFrame_L[l].x + 10 + drawOffset.x,
					SelectFrame_L[l].z + 10 + drawOffset.y
				);
			}
		}

		for (int d = 0; d < SelectFrame_D.size(); d++)
		{
			if (SelectFrame_L[l].x == SelectFrame_D[d].y &&
				SelectFrame_L[l].y == SelectFrame_D[d].z)
			{
				SelectIconSprite[0].DrawExtendSprite(
					SelectFrame_L[l].x + drawOffset.x,
					SelectFrame_L[l].y + drawOffset.y,
					SelectFrame_L[l].x + 10 + drawOffset.x,
					SelectFrame_L[l].z - 10 + drawOffset.y
				);
			}
		}
	}

	//上フレーム
	for (int u = 0; u < SelectFrame_U.size(); u++)
	{
		SelectIconSprite[1].DrawExtendSprite(
			SelectFrame_U[u].x + drawOffset.x,
			SelectFrame_U[u].z + drawOffset.y,
			SelectFrame_U[u].y + drawOffset.x,
			SelectFrame_U[u].z + 10 + drawOffset.y
		);
	}

	//右フレーム
	for (int r = 0; r < SelectFrame_R.size(); r++)
	{
		SelectIconSprite[2].DrawExtendSprite(
			SelectFrame_R[r].x + drawOffset.x,
			SelectFrame_R[r].y + drawOffset.y,
			SelectFrame_R[r].x - 10 + drawOffset.x,
			SelectFrame_R[r].z + drawOffset.y
		);

		for (int u = 0; u < SelectFrame_U.size(); u++)
		{
			if (SelectFrame_R[r].x == SelectFrame_U[u].x &&
				SelectFrame_R[r].z == SelectFrame_U[u].z)
			{
				SelectIconSprite[2].DrawExtendSprite(
					SelectFrame_R[r].x + drawOffset.x,
					SelectFrame_R[r].y + drawOffset.y,
					SelectFrame_R[r].x - 10 + drawOffset.x,
					SelectFrame_R[r].z + 10 + drawOffset.y
				);
			}
		}

		for (int d = 0; d < SelectFrame_D.size(); d++)
		{
			if (SelectFrame_R[r].x == SelectFrame_D[d].x &&
				SelectFrame_R[r].y == SelectFrame_D[d].z)
			{
				SelectIconSprite[2].DrawExtendSprite(
					SelectFrame_R[r].x + drawOffset.x,
					SelectFrame_R[r].y + drawOffset.y,
					SelectFrame_R[r].x - 10 + drawOffset.x,
					SelectFrame_R[r].z - 10 + drawOffset.y
				);
			}
		}
	}

	//下フレーム
	for (int d = 0; d < SelectFrame_D.size(); d++)
	{
		SelectIconSprite[3].DrawExtendSprite(
			SelectFrame_D[d].x + drawOffset.x,
			SelectFrame_D[d].z + drawOffset.y,
			SelectFrame_D[d].y + drawOffset.x,
			SelectFrame_D[d].z - 10 + drawOffset.y
		);
	}

#pragma endregion

	//エフェクト描画
	oriEffect.Draw();

	particleManager->Prototype_Draw(EmptyHandle);

	SpriteManager::Get()->SetCommonBeginDraw();

	MapchipSpriteEmpty.Draw();
	MapchipSpriteGoal.Draw();
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 16; j++)
		{
			AllBlockSprite[i][j].Draw();
		}
	}

	lineSprite.Draw();

	for (int a = 0; a < 4; a++)
	{
		SelectIconSprite[a].Draw();
	}
}

void Stage::Create()
{
	LoadBlocksHandle();
	CreateBlocksSprite();

	if ((MapchipSpriteEmpty.spdata->size.x <= 0) || (MapchipSpriteEmpty.spdata->size.y <= 0))
	{
		EmptyHandle = TexManager::LoadTexture("Resources/stage_empty.png");
		MapchipSpriteEmpty.Create(EmptyHandle);
	}

	if ((MapchipSpriteGoal.spdata->size.x <= 0) || (MapchipSpriteGoal.spdata->size.y <= 0))
	{
		GoalHandle = TexManager::LoadTexture("Resources/goal/goal1.png");
		MapchipSpriteGoal.Create(GoalHandle);
	}

	if ((lineSprite.spdata->size.x <= 0) || (lineSprite.spdata->size.y <= 0))
	{
		lineHandle = TexManager::LoadTexture("Resources/WhitePixle.png");
		lineSprite.Create(lineHandle);
	}

	if (particleManager == nullptr)
	{
		particleManager = ParticleManager::Create();
		particleManager->Prototype_Set(FoldParticle);
	}

	SelectFrameHandle = TexManager::LoadTexture("Resources/Blocks/SelectFrame.png");

	for (int a = 0; a < 4; a++)
	{
		SelectIconSprite[a].Create(SelectFrameHandle);
	}

	LoadStageSound();
}

void Stage::LoadBlocksHandle()
{
	//画像ハンドルの読み込み
	std::string BasePath = "Resources/Blocks/";
	std::string fileType = ".png";

	std::string Bule = "Block_Bule/blockB";
	std::string Green = "Block_Green/blockG";
	std::string Red = "Block_Red/blockR";
	std::string Yellow = "Block_Yellow/blockY";

	std::string Horizontal = "_=";
	std::string Vertical = "_ll";
	std::string NoFrame = "_NoFrame";

	for (i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
		{
			//青色のブロックの画像ハンドル
			AllBlockHandle[0][0] = TexManager::LoadTexture(BasePath + Bule + fileType);
			AllBlockHandle[0][1] = TexManager::LoadTexture(BasePath + Bule + Horizontal + fileType);
			AllBlockHandle[0][2] = TexManager::LoadTexture(BasePath + Bule + Vertical + fileType);
			AllBlockHandle[0][3] = TexManager::LoadTexture(BasePath + Bule + "_I_Left" + fileType);
			AllBlockHandle[0][4] = TexManager::LoadTexture(BasePath + Bule + "_I_Up" + fileType);
			AllBlockHandle[0][5] = TexManager::LoadTexture(BasePath + Bule + "_I_Right" + fileType);
			AllBlockHandle[0][6] = TexManager::LoadTexture(BasePath + Bule + "_I_Down" + fileType);
			AllBlockHandle[0][7] = TexManager::LoadTexture(BasePath + Bule + "_L_Left" + fileType);
			AllBlockHandle[0][8] = TexManager::LoadTexture(BasePath + Bule + "_L_Up" + fileType);
			AllBlockHandle[0][9] = TexManager::LoadTexture(BasePath + Bule + "_L_right" + fileType);
			AllBlockHandle[0][10] = TexManager::LoadTexture(BasePath + Bule + "_L_Down" + fileType);
			AllBlockHandle[0][11] = TexManager::LoadTexture(BasePath + Bule + "_U_Left" + fileType);
			AllBlockHandle[0][12] = TexManager::LoadTexture(BasePath + Bule + "_U_Up" + fileType);
			AllBlockHandle[0][13] = TexManager::LoadTexture(BasePath + Bule + "_U_Right" + fileType);
			AllBlockHandle[0][14] = TexManager::LoadTexture(BasePath + Bule + "_U_Down" + fileType);
			AllBlockHandle[0][15] = TexManager::LoadTexture(BasePath + Bule + NoFrame + fileType);

			break;
		}
		case 1:
		{
			//緑色のブロックの画像ハンドル
			AllBlockHandle[1][0] = TexManager::LoadTexture(BasePath + Green + fileType);
			AllBlockHandle[1][1] = TexManager::LoadTexture(BasePath + Green + Horizontal + fileType);
			AllBlockHandle[1][2] = TexManager::LoadTexture(BasePath + Green + Vertical + fileType);
			AllBlockHandle[1][3] = TexManager::LoadTexture(BasePath + Green + "_I_Left" + fileType);
			AllBlockHandle[1][4] = TexManager::LoadTexture(BasePath + Green + "_I_Up" + fileType);
			AllBlockHandle[1][5] = TexManager::LoadTexture(BasePath + Green + "_I_Right" + fileType);
			AllBlockHandle[1][6] = TexManager::LoadTexture(BasePath + Green + "_I_Down" + fileType);
			AllBlockHandle[1][7] = TexManager::LoadTexture(BasePath + Green + "_L_Left" + fileType);
			AllBlockHandle[1][8] = TexManager::LoadTexture(BasePath + Green + "_L_Up" + fileType);
			AllBlockHandle[1][9] = TexManager::LoadTexture(BasePath + Green + "_L_right" + fileType);
			AllBlockHandle[1][10] = TexManager::LoadTexture(BasePath + Green + "_L_Down" + fileType);
			AllBlockHandle[1][11] = TexManager::LoadTexture(BasePath + Green + "_U_Left" + fileType);
			AllBlockHandle[1][12] = TexManager::LoadTexture(BasePath + Green + "_U_Up" + fileType);
			AllBlockHandle[1][13] = TexManager::LoadTexture(BasePath + Green + "_U_Right" + fileType);
			AllBlockHandle[1][14] = TexManager::LoadTexture(BasePath + Green + "_U_Down" + fileType);
			AllBlockHandle[1][15] = TexManager::LoadTexture(BasePath + Green + NoFrame + fileType);
			break;
		}
		case 2:
		{
			//赤色のブロックの画像ハンドル
			AllBlockHandle[2][0] = TexManager::LoadTexture(BasePath + Red + fileType);
			AllBlockHandle[2][1] = TexManager::LoadTexture(BasePath + Red + Horizontal + fileType);
			AllBlockHandle[2][2] = TexManager::LoadTexture(BasePath + Red + Vertical + fileType);
			AllBlockHandle[2][3] = TexManager::LoadTexture(BasePath + Red + "_I_Left" + fileType);
			AllBlockHandle[2][4] = TexManager::LoadTexture(BasePath + Red + "_I_Up" + fileType);
			AllBlockHandle[2][5] = TexManager::LoadTexture(BasePath + Red + "_I_Right" + fileType);
			AllBlockHandle[2][6] = TexManager::LoadTexture(BasePath + Red + "_I_Down" + fileType);
			AllBlockHandle[2][7] = TexManager::LoadTexture(BasePath + Red + "_L_Left" + fileType);
			AllBlockHandle[2][8] = TexManager::LoadTexture(BasePath + Red + "_L_Up" + fileType);
			AllBlockHandle[2][9] = TexManager::LoadTexture(BasePath + Red + "_L_right" + fileType);
			AllBlockHandle[2][10] = TexManager::LoadTexture(BasePath + Red + "_L_Down" + fileType);
			AllBlockHandle[2][11] = TexManager::LoadTexture(BasePath + Red + "_U_Left" + fileType);
			AllBlockHandle[2][12] = TexManager::LoadTexture(BasePath + Red + "_U_Up" + fileType);
			AllBlockHandle[2][13] = TexManager::LoadTexture(BasePath + Red + "_U_Right" + fileType);
			AllBlockHandle[2][14] = TexManager::LoadTexture(BasePath + Red + "_U_Down" + fileType);
			AllBlockHandle[2][15] = TexManager::LoadTexture(BasePath + Red + NoFrame + fileType);
			break;
		}
		case 3:
		{
			//黄色のブロックの画像ハンドル
			AllBlockHandle[3][0] = TexManager::LoadTexture(BasePath + Yellow + fileType);
			AllBlockHandle[3][1] = TexManager::LoadTexture(BasePath + Yellow + Horizontal + fileType);
			AllBlockHandle[3][2] = TexManager::LoadTexture(BasePath + Yellow + Vertical + fileType);
			AllBlockHandle[3][3] = TexManager::LoadTexture(BasePath + Yellow + "_I_Left" + fileType);
			AllBlockHandle[3][4] = TexManager::LoadTexture(BasePath + Yellow + "_I_Up" + fileType);
			AllBlockHandle[3][5] = TexManager::LoadTexture(BasePath + Yellow + "_I_Right" + fileType);
			AllBlockHandle[3][6] = TexManager::LoadTexture(BasePath + Yellow + "_I_Down" + fileType);
			AllBlockHandle[3][7] = TexManager::LoadTexture(BasePath + Yellow + "_L_Left" + fileType);
			AllBlockHandle[3][8] = TexManager::LoadTexture(BasePath + Yellow + "_L_Up" + fileType);
			AllBlockHandle[3][9] = TexManager::LoadTexture(BasePath + Yellow + "_L_right" + fileType);
			AllBlockHandle[3][10] = TexManager::LoadTexture(BasePath + Yellow + "_L_Down" + fileType);
			AllBlockHandle[3][11] = TexManager::LoadTexture(BasePath + Yellow + "_U_Left" + fileType);
			AllBlockHandle[3][12] = TexManager::LoadTexture(BasePath + Yellow + "_U_Up" + fileType);
			AllBlockHandle[3][13] = TexManager::LoadTexture(BasePath + Yellow + "_U_Right" + fileType);
			AllBlockHandle[3][14] = TexManager::LoadTexture(BasePath + Yellow + "_U_Down" + fileType);
			AllBlockHandle[3][15] = TexManager::LoadTexture(BasePath + Yellow + NoFrame + fileType);
			break;
		}
		default:
		{
			break;
		}
		}
	}
}

void Stage::CreateBlocksSprite()
{
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 16; j++)
		{
			if ((AllBlockSprite[i][j].spdata->size.x <= 0) || (AllBlockSprite[i][j].spdata->size.y <= 0))
			{
				AllBlockSprite[i][j].Create(AllBlockHandle[i][j]);
			}
		}
	}

	oriEffect.Init();
}

int Stage::LoadStage(const char* filePath, unsigned char foldCount[4])
{
	DataClear();

	static const char endCharacter = '@';
	static FILE* fileHandle;
	static errno_t err;
	static size_t lastIndex = 0;

	err = fopen_s(&fileHandle, filePath, "r");
	if (err != 0)
	{
		return err;
	}

	if (LoadFile::LoadCSV((char*)initFoldCount, sizeof(initFoldCount) / sizeof(initFoldCount[0]), fileHandle, endCharacter) == EF)
	{
		// 関数失敗
		return EF;
	}

	for (i = 0; i < sizeof(initFoldCount) / sizeof(initFoldCount[0]); i++)
	{
		foldCount[i] = initFoldCount[i];
		//foldCount[i] = 1;
		//initFoldCount[i] = 1;
	}

	static char stageCount = 0;

	if (LoadFile::LoadCSV(&stageCount, 1, fileHandle, endCharacter) == EF)
	{
		// 関数失敗
		return EF;
	}

	static char size[2] = { 0 };
	static char tilePos[2] = { 0 };

	for (i = 0; i < static_cast<size_t>(stageCount); i++)
	{
		stageData.push_back({});

		if (LoadFile::LoadCSV(tilePos, sizeof(tilePos) / sizeof(tilePos[0]), fileHandle, INT_MIN) == EF)
		{
			// 関数失敗
			return EF;
		}
		if (LoadFile::LoadCSV(size, sizeof(size) / sizeof(size[0]), fileHandle, INT_MIN) == EF)
		{
			// 関数失敗
			return EF;
		}

		if (size[0] <= 0 || size[1] <= 0)
		{
			// 一つ以上の値が間違ってる
			return EF;
		}

		stageData[i].offsetX = tilePos[0];
		stageData[i].offsetY = tilePos[1];
		stageData[i].width = size[0];
		stageData[i].height = size[1];

		stageData[i].stageTile = (char*)malloc(sizeof(char) * stageData[i].width * stageData[i].height);

		if (LoadFile::LoadCSV(stageData[i].stageTile, static_cast<size_t>(stageData[i].width * stageData[i].height), fileHandle, endCharacter))
		{
			// 関数失敗
			return EF;
		}
	}

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < static_cast<size_t>(stageData[i].width * stageData[i].height); j++)
		{
			//CSVのステージの情報の中で0になっている所は存在しない扱いにする
			if (stageData[i].stageTile[j] == MapchipData::EMPTY_STAGE)
			{
				continue;
			}

			//push_backして末尾に代入
			stageData[i].stageTile[j] = static_cast<char>(stageData[i].stageTileData.size() + 1);
			stageData[i].stageTileData.push_back({});
			lastIndex = stageData[i].stageTileData.size() - 1;
			stageData[i].stageTileData[lastIndex].stageNumber = static_cast<char>(j);

			if (LoadFile::LoadCSV(tilePos, sizeof(tilePos) / sizeof(tilePos[0]), fileHandle, INT_MIN) == EF)
			{
				// 関数失敗
				return EF;
			}
			if (LoadFile::LoadCSV(size, sizeof(size) / sizeof(size[0]), fileHandle, INT_MIN) == EF)
			{
				// 関数失敗
				return EF;
			}

			if (size[0] <= 0 || size[1] <= 0)
			{
				// 一つ以上の値が間違ってる
				return EF;
			}

			stageData[i].stageTileData[lastIndex].offsetX = tilePos[0];
			stageData[i].stageTileData[lastIndex].offsetY = tilePos[1];
			stageData[i].stageTileData[lastIndex].width = size[0];
			stageData[i].stageTileData[lastIndex].height = size[1];
			stageData[i].stageTileData[lastIndex].size =
				stageData[i].stageTileData[lastIndex].width *
				stageData[i].stageTileData[lastIndex].height;
			stageData[i].stageTileData[lastIndex].mapchip =
				(char*)malloc(sizeof(char) * stageData[i].stageTileData[lastIndex].size);

			for (size_t k = 0; k < stageData[i].stageTileData[lastIndex].size; k++)
			{
				stageData[i].stageTileData[lastIndex].drawLeftUp.push_back({});
				stageData[i].stageTileData[lastIndex].drawRightDown.push_back({});
				stageData[i].stageTileData[lastIndex].startPos.push_back({});
				stageData[i].stageTileData[lastIndex].endPos.push_back({});
				stageData[i].stageTileData[lastIndex].easePos.push_back({});
			}

			if (stageData[i].stageTileData[lastIndex].mapchip == nullptr)
			{
				// 関数失敗
				return EF;
			}

			if (LoadFile::LoadCSV(stageData[i].stageTileData[lastIndex].mapchip,
				stageData[i].stageTileData[lastIndex].width *
				stageData[i].stageTileData[lastIndex].height, fileHandle, endCharacter) != 0)
			{
				// 関数失敗
				return EF;
			}
		}
	}

	fclose(fileHandle);

	//ステージグループの設定
	SetStageGroup();

	// オフセット値の計算
	x = 0;
	y = 0;
	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			static unsigned char tileNumber = 0;
			tileNumber = stageData[i].stageTileData[j].stageNumber;

			if (stageData[i].stageTile[tileNumber] == MapchipData::EMPTY_STAGE)
			{
				continue;
			}

			stageData[i].stageTileData[j].offsetX += stageData[i].offsetX;
			stageData[i].stageTileData[j].offsetY += stageData[i].offsetY;

			if (x < stageData[i].stageTileData[j].offsetX + stageData[i].stageTileData[j].width)
			{
				x = stageData[i].stageTileData[j].offsetX + stageData[i].stageTileData[j].width;
			}
			if (y < stageData[i].stageTileData[j].offsetY + stageData[i].stageTileData[j].height)
			{
				y = stageData[i].stageTileData[j].offsetY + stageData[i].stageTileData[j].height;
			}
		}
	}
	drawOffsetX = Raki_WinAPI::window_width / 2 - static_cast<int>(halfBlockSize * x);
	drawOffsetY = Raki_WinAPI::window_height / 2 - static_cast<int>(halfBlockSize * y);

	static bool end = false;
	end = false;

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			// プレイヤーの開始位置の探索
			for (mapchipPos = 0; mapchipPos < stageData[i].stageTileData[j].size; mapchipPos++)
			{
				if (stageData[i].stageTileData[j].mapchip[mapchipPos] != MapchipData::START)
				{
					continue;
				}

				x = mapchipPos % stageData[i].stageTileData[j].width;
				y = mapchipPos / stageData[i].stageTileData[j].width;

				startPlayerPosX = static_cast<int>(x + stageData[i].stageTileData[j].offsetX);
				startPlayerPosY = static_cast<int>(y + stageData[i].stageTileData[j].offsetY);

				nowPlayerStage = static_cast<char>(i);

				end = true;
				break;
			}
			if (end)
			{
				break;
			}
		}
		if (end)
		{
			break;
		}
	}

	if (end == false)
	{
		// 開始位置が無い
		return EF;
	}

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			InitOverlap(i, j);
		}
		SetFoldType(i);
	}

	for (i = 0; i < stageData.size(); i++)
	{
		initStageData.push_back(stageData[i]);
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			char* initMapchip = static_cast<char*>(malloc(sizeof(char) * stageData[i].stageTileData[j].size));
			memcpy_s(initMapchip, sizeof(char) * stageData[i].stageTileData[j].size,
				stageData[i].stageTileData[j].mapchip, sizeof(char) * stageData[i].stageTileData[j].size);
			initStageData[i].stageTileData[j].mapchip = initMapchip;
		}
	}

	SelectTile = &stageData[0].stageTileData[0];

	return 0;
}

void Stage::SetFoldType(size_t stagenum)
{
	int a = 0;
	int b = 0;

	//隣り合ったタイルが何枚あるのか
	int NextCount = 0;

	//対象のタイルの外枠
	float NowTile_L;
	float NowTile_R;
	float NowTile_U;
	float NowTile_D;

	//対象外のタイルの外枠
	float OtherTile_L;
	float OtherTile_U;
	float OtherTile_R;
	float OtherTile_D;

	//対象のタイルを順番に見ていく
	for (a = 0; a < stageData[stagenum].stageTileData.size(); a++)
	{
		NowTile_L = (stageData[stagenum].stageTileData[a].offsetX) * blockSize;
		NowTile_R = (stageData[stagenum].stageTileData[a].offsetX + stageData[stagenum].stageTileData[a].width) * blockSize;
		NowTile_U = (stageData[stagenum].stageTileData[a].offsetY) * blockSize;
		NowTile_D = (stageData[stagenum].stageTileData[a].offsetY + stageData[stagenum].stageTileData[a].height) * blockSize;

		//ほかのタイルとの位置関係を調べる
		for (b = 0; b < stageData[stagenum].stageTileData.size(); b++)
		{
			if (a == b)
			{
				continue;
			}

			//対象外タイルの外枠をセット
			OtherTile_L = (stageData[stagenum].stageTileData[b].offsetX) * blockSize;
			OtherTile_R = (stageData[stagenum].stageTileData[b].offsetX + stageData[stagenum].stageTileData[b].width) * blockSize;
			OtherTile_U = (stageData[stagenum].stageTileData[b].offsetY) * blockSize;
			OtherTile_D = (stageData[stagenum].stageTileData[b].offsetY + stageData[stagenum].stageTileData[b].height) * blockSize;

			if (NowTile_L == OtherTile_L || NowTile_R == OtherTile_R)
			{
				stageData[stagenum].stageTileData[a].NextTileCount++;

				if (stageData[stagenum].stageTileData[a].FoldType < 0)
				{
					if (NowTile_U < OtherTile_U)
					{
						stageData[stagenum].stageTileData[a].FoldType = BodyType::up;
					}

					if (NowTile_D > OtherTile_D)
					{
						stageData[stagenum].stageTileData[a].FoldType = BodyType::down;
					}
				}
			}

			if (NowTile_U == OtherTile_U || NowTile_D == OtherTile_D)
			{
				stageData[stagenum].stageTileData[a].NextTileCount++;

				if (stageData[stagenum].stageTileData[a].FoldType < 0)
				{
					if (NowTile_L < OtherTile_L)
					{
						stageData[stagenum].stageTileData[a].FoldType = BodyType::left;
					}

					if (NowTile_R > OtherTile_R)
					{
						stageData[stagenum].stageTileData[a].FoldType = BodyType::right;
					}
				}
			}
		}

		if (stageData[stagenum].stageTileData[a].NextTileCount >= 2)
		{
			stageData[stagenum].stageTileData[a].FoldType = -1;
		}
	}
}

void Stage::SelectingStageTile()
{
	std::vector < size_t > AllTiles = {};

	//ステージの数だけリストをセット
	for (int a = 0; a < stageData.size(); a++)
	{
		AllTiles.push_back(a);
	}

	static size_t selectCount;

	//現在選択しているタイルに合わせてカウントを初期化
	for (int c = 0; c < AllTiles.size(); c++)
	{
		if (AllTiles[c] == selectStageNum)
		{
			selectCount = c;
		}
	}

	//セレクト入力が入ったら対象ステージを変更(単ボタンループ)
	if (inputManeger->FoldSelectTrigger())
	{
		if (selectCount + 1 < AllTiles.size())
		{
			selectCount++;
		}
		else
		{
			selectCount = 0;
		}
		Audio::PlayLoadedSound(selectSound);
		isMoveSelectCursor = true;
	}

	selectStageNum = (AllTiles[selectCount]);
	SelectTile = &stageData[selectStageNum].stageTileData[0];

	SelectStage = &stageData[selectCount];

	SetSelectStageFrame(selectStageNum);
}

void Stage::SetSelectStageFrame(size_t SelectStageNum)
{
	SelectFrame_L.clear();
	SelectFrame_R.clear();
	SelectFrame_U.clear();
	SelectFrame_D.clear();

	int a = 0;
	int b = 0;

	//対象のタイルの外枠
	float NowTile_L;
	float NowTile_U;
	float NowTile_R;
	float NowTile_D;

	//対象外のタイルの外枠
	float OtherTile_L;
	float OtherTile_U;
	float OtherTile_R;
	float OtherTile_D;

	//四方向それぞれ隣り合っているタイルがあるか
	bool IsNext_L = false;
	bool IsNext_U = false;
	bool IsNext_R = false;
	bool IsNext_D = false;

	//セットする用の変数
	RVector3 Set_L = {};
	RVector3 Set_U = {};
	RVector3 Set_R = {};
	RVector3 Set_D = {};

	//指定したステージの頭からタイルを見ていく
	for (a = 0; a < stageData[SelectStageNum].stageTileData.size(); a++)
	{
		//対象タイルの外枠をセット
		NowTile_L = (stageData[SelectStageNum].stageTileData[a].offsetX) * blockSize;
		NowTile_R = (stageData[SelectStageNum].stageTileData[a].offsetX + stageData[SelectStageNum].stageTileData[a].width) * blockSize;
		NowTile_U = (stageData[SelectStageNum].stageTileData[a].offsetY) * blockSize;
		NowTile_D = (stageData[SelectStageNum].stageTileData[a].offsetY + stageData[SelectStageNum].stageTileData[a].height) * blockSize;

		IsNext_L = false;
		IsNext_U = false;
		IsNext_R = false;
		IsNext_D = false;

		Set_L = { -1,-1,-1 };
		Set_U = { -1,-1,-1 };
		Set_R = { -1,-1,-1 };
		Set_D = { -1,-1,-1 };

		//ほかのタイルと比べていく
		for (b = 0; b < stageData[SelectStageNum].stageTileData.size(); b++)
		{
			//同じタイルは見ない
			if (a == b)
			{
				continue;
			}

			//対象外タイルの外枠をセット
			OtherTile_L = (stageData[SelectStageNum].stageTileData[b].offsetX) * blockSize;
			OtherTile_R = (stageData[SelectStageNum].stageTileData[b].offsetX + stageData[SelectStageNum].stageTileData[b].width) * blockSize;
			OtherTile_U = (stageData[SelectStageNum].stageTileData[b].offsetY) * blockSize;
			OtherTile_D = (stageData[SelectStageNum].stageTileData[b].offsetY + stageData[SelectStageNum].stageTileData[b].height) * blockSize;


			//縦並びだった場合
			if (NowTile_L == OtherTile_L || NowTile_R == OtherTile_R)
			{
				//上側にあるかどうか
				if (NowTile_U > OtherTile_U)
				{
					IsNext_U = true;
				}
				//下側にあるかどうか
				if (NowTile_D < OtherTile_D)
				{
					IsNext_D = true;
				}

				//片方の幅が大きかった場合
				if (stageData[SelectStageNum].stageTileData[a].width > stageData[SelectStageNum].stageTileData[b].width)
				{
					if (NowTile_L < OtherTile_L)
					{
						Set_U =
						{
							OtherTile_L - (OtherTile_L - NowTile_L),
							OtherTile_L,
							NowTile_U
						};
						Set_D =
						{
							OtherTile_L - (OtherTile_L - NowTile_L),
							OtherTile_L,
							NowTile_D
						};
					}

					if (NowTile_R > OtherTile_R)
					{
						Set_U =
						{
							OtherTile_R,
							OtherTile_R + (NowTile_R - OtherTile_R),
							NowTile_U
						};
						Set_D =
						{
							OtherTile_R,
							OtherTile_R + (NowTile_R - OtherTile_R),
							NowTile_D
						};
					}
				}
			}

			//横並びだった場合
			if (NowTile_U == OtherTile_U || NowTile_D == OtherTile_D)
			{
				//左側にあるかどうか
				if (NowTile_L > OtherTile_L)
				{
					IsNext_L = true;
				}
				//右側にあるかどうか
				if (NowTile_R < OtherTile_R)
				{
					IsNext_R = true;
				}

				//片方の幅が大きかった場合
				if (stageData[SelectStageNum].stageTileData[a].height > stageData[SelectStageNum].stageTileData[b].height)
				{
					if (NowTile_U < OtherTile_U)
					{
						Set_L =
						{
							NowTile_L,
							NowTile_U,
							NowTile_U + (OtherTile_U - NowTile_U)
						};
						Set_R =
						{
							NowTile_R,
							NowTile_U,
							NowTile_U + (OtherTile_U - NowTile_U)
						};
					}

					if (NowTile_D > OtherTile_D)
					{
						Set_L =
						{
							NowTile_L,
							NowTile_D - (NowTile_D - OtherTile_D),
							NowTile_D
						};
						Set_R =
						{
							NowTile_R,
							NowTile_D - (NowTile_D - OtherTile_D),
							NowTile_D
						};
					}
				}
			}
		}

		//上下左右それぞれ端だった場合のみフレーム描画座標を格納
		if (!IsNext_U)
		{
			SelectFrame_U.push_back({ NowTile_L,NowTile_R,NowTile_U });
		}
		if (!IsNext_D)
		{
			SelectFrame_D.push_back({ NowTile_L,NowTile_R,NowTile_D });
		}
		if (!IsNext_L)
		{
			SelectFrame_L.push_back({ NowTile_L,NowTile_U,NowTile_D });
		}
		if (!IsNext_R)
		{
			SelectFrame_R.push_back({ NowTile_R,NowTile_U,NowTile_D });
		}

		if (Set_U.x != -1)
		{
			SelectFrame_U.push_back(Set_U);
		}
		if (Set_D.x != -1)
		{
			SelectFrame_D.push_back(Set_D);
		}
		if (Set_L.x != -1)
		{
			SelectFrame_L.push_back(Set_L);
		}
		if (Set_R.x != -1)
		{
			SelectFrame_R.push_back(Set_R);
		}
	}

	return;
}

int Stage::FoldAndOpen(const RVector3& playerPos, bool BodyStatus[4], bool IsFootAction, bool IsFolds[4], int OpenCount, bool IsOpens[4])
{
	int anyActionCount = 0;

	for (int a = 0; a < stageData.size(); a++)
	{
		for (int b = 0; b < stageData[a].stageTileData.size(); b++)
		{
			StageTileData* tile = &stageData[a].stageTileData[b];
			if (tile->stageEase.isMove)
			{
				return 0;
			}
		}
	}

	if (player->Player_IsAction)
	{
		return 0;
	}

	if (player->Body_One.Ease.isMove ||
		player->Body_Two.Ease.isMove ||
		player->Body_Three.Ease.isMove ||
		player->Body_Four.Ease.isMove)
	{
		return 0;
	}

	static size_t NowStage = -1;
	static size_t NowTile = -1;

	static size_t OldStage = -1;
	static size_t OldTile = -1;

	OldStage = NowStage;
	OldTile = NowTile;

	static char direction = -1;
	static size_t onPlayerStageTile = -1;
	static size_t moveStageTile = 0;
	static size_t moveStageData = 0;

	std::vector<size_t> onplayerstage = {};
	std::vector<size_t> onplayerstageOpen = {};
	std::vector<size_t> movestagetile = {};
	std::vector<size_t> movestagetileOpen = {};
	std::vector<size_t> movestagedata = {};
	std::vector<size_t> movestagedataOpen = {};
	std::vector<size_t> stagenumber = {};

	GetPositionStage(playerPos);

	direction = -1;

	GetPositionTile(player->CenterPosition, &NowStage, &NowTile);

	for (int a = 0; a < SelectStage->stageTileData.size(); a++)
	{
		if (!SelectStage->stageTileData[a].isFold && SelectStage->stageTileData[a].Overlap == 0)
		{
			if (inputManeger->FoldUpTrigger() &&
				SelectStage->stageTileData[a].FoldType == BodyType::up)
			{
				if (IsTileFoldDirection(selectStageNum, BodyType::up))
				{
					direction = BodyType::up;
					break;
				}
			}
			else if (inputManeger->FoldDownTrigger() &&
				SelectStage->stageTileData[a].FoldType == BodyType::down)
			{
				if (IsTileFoldDirection(selectStageNum, BodyType::down))
				{
					direction = BodyType::down;
					break;
				}
			}
			else if (inputManeger->FoldLeftTrigger() &&
				SelectStage->stageTileData[a].FoldType == BodyType::left)
			{
				if (IsTileFoldDirection(selectStageNum, BodyType::left))
				{
					direction = BodyType::left;
					break;
				}
			}
			else if (inputManeger->FoldRightTrigger() &&
				SelectStage->stageTileData[a].FoldType == BodyType::right)
			{
				if (IsTileFoldDirection(selectStageNum, BodyType::right))
				{
					direction = BodyType::right;
					break;
				}
			}
		}
		else
		{
			continue;
		}
	}

	for (int a = 0; a < SelectStage->stageTileData.size(); a++)
	{
		if (SelectStage->stageTileData[a].isFold)
		{
			if (inputManeger->OpenUpTrigger() &&
				SelectStage->stageTileData[a].FoldType == BodyType::up)
			{
				direction = BodyType::up;
				break;
			}
			else if (inputManeger->OpenDownTrigger() &&
				SelectStage->stageTileData[a].FoldType == BodyType::down)
			{
				direction = BodyType::down;
				break;
			}
			else if (inputManeger->OpenLeftTrigger() &&
				SelectStage->stageTileData[a].FoldType == BodyType::left)
			{
				direction = BodyType::left;
				break;
			}
			else if (inputManeger->OpenRightTrigger() &&
				SelectStage->stageTileData[a].FoldType == BodyType::right)
			{
				direction = BodyType::right;
				break;
			}
		}
		else
		{
			continue;
		}
	}

	static bool isAct = false;

	isAct = false;

	if (direction < 0)
	{
		return 0;
	}

	//Fold専用のコンテナに格納
	SetOnPlayerStageTileFold(stagenumber, onplayerstage, movestagetile, movestagedata, direction);

	//Open専用のコンテナに格納
	SetOnPlayerStageTileOpen(stagenumber, onplayerstageOpen, movestagetileOpen, movestagedataOpen, direction);

	//Fold専用のコンテナのサイズ分回す
	for (int a = 0; a < onplayerstage.size(); a++)
	{
		if (Fold(direction, stagenumber[a], onplayerstage[a], movestagedata[a], onplayerstage.size()) != EF)
		{
			isAct = true;
		}
	}

	//Open専用のコンテナのサイズ分回す
	for (int a = 0; a < onplayerstageOpen.size(); a++)
	{
		if (Open(direction, stagenumber[a], movestagedataOpen[a], onplayerstageOpen.size()) != EF)
		{
			isAct = true;
		}
	}

	for (int a = 0; a < stageData.size(); a++)
	{
		for (int b = 0; b < stageData[a].stageTileData.size(); b++)
		{
			SetOverlap(a, b);
		}
	}

	return 0;
}

bool Stage::IsTileFoldDirection(size_t stage, int direction)
{
	size_t NowStage = 0;
	size_t NowTile = 0;
	GetPositionTile(player->CenterPosition, &NowStage, &NowTile);

	if (stageData[stage].stageTileData.size() <= 1)
	{
		return false;
	}

	for (int tile = 0; tile < SelectStage->stageTileData.size(); tile++)
	{
		if (SelectStage->stageTileData[tile].isFold)
		{
			continue;
		}

		switch (direction)
		{
		case BodyType::up:
		{
			if (IsPlayerPositionFold(direction))
			{
				if (player->IsDirectionFoldAll(BodyType::up))
				{
					player->Player_IsAction = true;
					player->IsUpFold = true;
					player->FaceLeg.Set();
				}
				else
				{
					return false;
				}
			}
			return true;
			break;
		}
		case BodyType::down:
		{
			if (IsPlayerPositionFold(direction))
			{
				if (player->IsDirectionFoldAll(BodyType::down))
				{
					player->Player_IsAction = true;
					player->IsDownFold = true;
					player->FaceLeg.Set();
				}
				else
				{
					return false;
				}
			}
			return true;
			break;
		}
		case BodyType::left:
		{
			if (IsPlayerPositionFold(direction))
			{
				if (player->Player_IsAction == false && player->Body_One.IsActivate &&
					player->Body_One.IsFold == false && !player->Body_One.IsAction)
				{
					if (player->IsDirectionFoldAll(BodyType::left))
					{
						player->Player_IsAction = true;
						player->IsLeftFold = true;
						player->FaceLeg.Set();
					}
					else
					{
						return false;
					}
				}
			}
			return true;
			break;
		}
		case BodyType::right:
		{
			if (IsPlayerPositionFold(direction))
			{
				if (player->IsDirectionFoldAll(BodyType::right))
				{
					player->Player_IsAction = true;
					player->IsRightFold = true;
					player->FaceLeg.Set();
				}
				else
				{
					return false;
				}
			}
			return true;
			break;
		}
		default:
			break;
		}
	}

	return false;
}

bool Stage::IsPlayerPositionFold(int FoldType)
{
	StageTileData* ThisTile = {};

	//セレクトステージのタイル数分だけ回す
	for (int a = 0; a < SelectStage->stageTileData.size(); a++)
	{
		//プレイヤーのいるタイルは飛ばす
		if ((IsPlayerTile(selectStageNum, a) && !IsNowTileOver(selectStageNum, a)) ||
			stageData[selectStageNum].stageTileData[a].FoldType != FoldType)
		{
			continue;
		}

		ThisTile = &stageData[selectStageNum].stageTileData[a];

		switch (FoldType)
		{
		case BodyType::left:
			if ((ThisTile->offsetX + ThisTile->width) * blockSize + 25 < player->CenterPosition.x &&
				(ThisTile->offsetX + ThisTile->width) * blockSize + 300 > player->CenterPosition.x &&
				(ThisTile->offsetY) * blockSize < player->CenterPosition.y &&
				(ThisTile->offsetY + ThisTile->height) * blockSize > player->CenterPosition.y)
			{
				return true;
			}
			break;

		case BodyType::right:
			if ((ThisTile->offsetX) * blockSize - 25 > player->CenterPosition.x &&
				(ThisTile->offsetX) * blockSize - 300 < player->CenterPosition.x &&
				(ThisTile->offsetY) * blockSize < player->CenterPosition.y &&
				(ThisTile->offsetY + ThisTile->height) * blockSize > player->CenterPosition.y)
			{
				return true;
			}
			break;

		case BodyType::up:
			if ((ThisTile->offsetY + ThisTile->height) * blockSize + 25 < player->CenterPosition.y &&
				(ThisTile->offsetY + ThisTile->height) * blockSize + 300 > player->CenterPosition.y &&
				(ThisTile->offsetX) * blockSize < player->CenterPosition.x &&
				(ThisTile->offsetX + ThisTile->width) * blockSize > player->CenterPosition.x)
			{
				return true;
			}
			break;

		case BodyType::down:
			if ((ThisTile->offsetY) * blockSize - 25 > player->CenterPosition.y &&
				(ThisTile->offsetY) * blockSize - 300 < player->CenterPosition.y &&
				(ThisTile->offsetX) * blockSize < player->CenterPosition.x &&
				(ThisTile->offsetX + ThisTile->width) * blockSize > player->CenterPosition.x)
			{
				return true;
			}
			break;
		}
	}

	return false;
}

bool Stage::IsNowTileOver(size_t stage, size_t tile)
{
	int overcount = 0;

	for (int a = 0; a < stageData.size(); a++)
	{
		for (int b = 0; b < stageData[a].stageTileData.size(); b++)
		{
			if (static_cast<size_t>(stageData[stage].stageTileData[tile].offsetX + 1) <= (stageData[a].stageTileData[b].offsetX + stageData[a].stageTileData[b].width) - 1 &&
				static_cast<size_t>(stageData[a].stageTileData[b].offsetX + 1) <= (stageData[stage].stageTileData[tile].offsetX + stageData[stage].stageTileData[tile].width) - 1 &&
				static_cast<size_t>(stageData[stage].stageTileData[tile].offsetY + 1) <= (stageData[a].stageTileData[b].offsetY + stageData[a].stageTileData[b].height) - 1 &&
				static_cast<size_t>(stageData[a].stageTileData[b].offsetY + 1) <= (stageData[stage].stageTileData[tile].offsetY + stageData[stage].stageTileData[tile].height) - 1)
			{
				overcount++;
			}
		}
	}

	if (overcount > 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Stage::FoldSimulation(const RVector3& playerPos, const unsigned char& direction, char** returnMapchip)
{
	static size_t onPlayerStageData = 0;
	static size_t moveStageTile = 0;
	static size_t moveStageData = 0;
	static bool isFold = false; //折れる物があるかどうか

	isFold = false;

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			if ((playerPos.x / blockSize >= stageData[i].stageTileData[j].offsetX &&
				playerPos.x / blockSize < stageData[i].stageTileData[j].offsetX + stageData[i].stageTileData[j].width) &&
				(playerPos.y / blockSize >= stageData[i].stageTileData[j].offsetY &&
					playerPos.y / blockSize < stageData[i].stageTileData[j].offsetY + stageData[i].stageTileData[j].height))
			{
				onPlayerStageData = i;
			}
			else
			{
				continue;
			}

			switch (direction)
			{
			case BodyType::up: //上入力
			{
				if (static_cast<unsigned char>(stageData[i].stageTileData[j].stageNumber) / stageData[i].width <= 0)
				{
					// プレイヤーがいるステージタイルが端
					break;
				}

				moveStageTile = stageData[i].stageTileData[j].stageNumber - stageData[i].width;
				moveStageData = static_cast<size_t>(stageData[i].stageTile[moveStageTile]) - 1;

				if (moveStageTile < 0 ||
					stageData[i].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
				{
					// ステージタイルがその方向に折れない
					break;
				}

				isFold = true;
				break;
			}
			case BodyType::down: //下入力
			{
				if (static_cast<unsigned char>(stageData[i].stageTileData[j].stageNumber) / stageData[i].width >= stageData[i].height - 1)
				{
					// プレイヤーがいるステージタイルが端
					break;
				}

				moveStageTile = stageData[i].stageTileData[j].stageNumber + stageData[i].width;
				moveStageData = static_cast<size_t>(stageData[i].stageTile[moveStageTile]) - 1;

				if (moveStageTile >= static_cast<size_t>(stageData[i].width * stageData[i].height) ||
					stageData[i].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
				{
					// ステージタイルがその方向に折れない
					break;
				}

				isFold = true;
				break;
			}
			case BodyType::left: //左入力
			{
				if (static_cast<unsigned char>(stageData[i].stageTileData[j].stageNumber) % stageData[i].width <= 0)
				{
					// プレイヤーがいるステージタイルが端
					break;
				}

				moveStageTile = static_cast<size_t>(stageData[i].stageTileData[j].stageNumber) - 1;
				moveStageData = static_cast<size_t>(stageData[i].stageTile[moveStageTile]) - 1;

				if (moveStageTile < 0 ||
					stageData[i].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
				{
					// ステージタイルがその方向に折れない
					break;
				}

				isFold = true;
				break;
			}
			case BodyType::right: //右入力
			{
				if (static_cast<unsigned char>(stageData[i].stageTileData[j].stageNumber) % stageData[i].width >= stageData[i].width - 1)
				{
					// プレイヤーがいるステージタイルが端
					break;
				}

				moveStageTile = static_cast<size_t>(stageData[i].stageTileData[j].stageNumber) + 1;
				moveStageData = static_cast<size_t>(stageData[i].stageTile[moveStageTile]) - 1;

				if (moveStageTile >= static_cast<size_t>(stageData[i].width * stageData[i].height) ||
					stageData[i].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
				{
					// ステージタイルがその方向に折れない
					break;
				}

				isFold = true;
				break;
			}
			default:
			{
				*returnMapchip = nullptr;

				return EF;
				break;
			}
			}

			if (isFold)
			{
				break;
			}
		}

		if (isFold)
		{
			break;
		}
	}

	if (isFold)
	{
		if (reverseMapchip == nullptr)
		{
			reverseMapchip =
				static_cast<char*>(malloc(sizeof(char) * stageData[onPlayerStageData].stageTileData[moveStageData].size));
		}

		for (y = 0; y < static_cast<size_t>(stageData[onPlayerStageData].stageTileData[moveStageData].height); y++)
		{
			for (x = 0; x < stageData[onPlayerStageData].stageTileData[moveStageData].width; x++)
			{
				if (direction == BodyType::up || direction == BodyType::down)
				{
					mapchipPos = y * stageData[onPlayerStageData].stageTileData[moveStageData].width + x;
					reverseMapchipPos = (stageData[onPlayerStageData].stageTileData[moveStageData].height - y - 1) * stageData[onPlayerStageData].stageTileData[moveStageData].width + x;
				}
				if (direction == BodyType::left || direction == BodyType::right)
				{
					mapchipPos = y * stageData[onPlayerStageData].stageTileData[moveStageData].width + x;
					reverseMapchipPos = y * stageData[onPlayerStageData].stageTileData[moveStageData].width + (stageData[onPlayerStageData].stageTileData[moveStageData].width - x - 1);
				}

				reverseMapchip[mapchipPos] = stageData[onPlayerStageData].stageTileData[moveStageData].mapchip[reverseMapchipPos];
			}
		}

		*returnMapchip = reverseMapchip;

		if (stageData[onPlayerStageData].stageTileData[moveStageData].isFold)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		*returnMapchip = nullptr;

		// 折れる物が無い
		return EF;
	}

	return 0;
}

void Stage::Reset(unsigned char foldCount[4])
{
	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			stageData[i].stageTileData[j].offsetX = initStageData[i].stageTileData[j].offsetX;
			stageData[i].stageTileData[j].offsetY = initStageData[i].stageTileData[j].offsetY;
			stageData[i].stageTileData[j].stageNumber = initStageData[i].stageTileData[j].stageNumber;
			stageData[i].stageTileData[j].direction = initStageData[i].stageTileData[j].direction;
			for (size_t k = 0; k < stageData[i].stageTileData[j].size; k++)
			{
				stageData[i].stageTileData[j].mapchip[k] = initStageData[i].stageTileData[j].mapchip[k];
			}

			stageData[i].stageTileData[j].stageEase.isMove = false;
			stageData[i].stageTileData[j].stageEase.splineIndex = 0;
			stageData[i].stageTileData[j].stageEase.timeRate = 0.0f;
			stageData[i].stageTileData[j].isFold = false;

			stageData[i].stageTileData[j].Overlap = initStageData[i].stageTileData[j].Overlap;
		}
	}

	selectStageNum = 0;
	selectTileNum = 0;

	GetInitFoldCount(foldCount);
}

void Stage::DataClear()
{
	for (i = 0; i < stageData.size(); i++)
	{
		if (stageData[i].stageTile != nullptr)
		{
			free(stageData[i].stageTile);
			stageData[i].stageTile = nullptr;
		}
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			if (stageData[i].stageTileData[j].mapchip != nullptr)
			{
				free(stageData[i].stageTileData[j].mapchip);
				stageData[i].stageTileData[j].mapchip = nullptr;
			}

			ContainerClear(stageData[i].stageTileData[j].drawLeftUp);
			ContainerClear(stageData[i].stageTileData[j].drawRightDown);
			ContainerClear(stageData[i].stageTileData[j].startPos);
			ContainerClear(stageData[i].stageTileData[j].endPos);
		}

		ContainerClear(stageData[i].stageTileData);
		ContainerClear(stageData[i].stageOffsetX);
		ContainerClear(stageData[i].stageOffsetY);
	}

	for (i = 0; i < initStageData.size(); i++)
	{
		//if (initStageData[i].stageTile != nullptr)
		//{
		//	free(initStageData[i].stageTile);
		//	initStageData[i].stageTile = nullptr;
		//}
		for (j = 0; j < initStageData[i].stageTileData.size(); j++)
		{
			if (initStageData[i].stageTileData[j].mapchip != nullptr)
			{
				free(initStageData[i].stageTileData[j].mapchip);
				initStageData[i].stageTileData[j].mapchip = nullptr;
			}

			ContainerClear(initStageData[i].stageTileData[j].drawLeftUp);
			ContainerClear(initStageData[i].stageTileData[j].drawRightDown);
			ContainerClear(initStageData[i].stageTileData[j].startPos);
			ContainerClear(initStageData[i].stageTileData[j].endPos);
		}

		ContainerClear(initStageData[i].stageTileData);
		ContainerClear(initStageData[i].stageOffsetX);
		ContainerClear(initStageData[i].stageOffsetY);
	}

	ContainerClear(stageData);
	ContainerClear(initStageData);
}

void Stage::InitOverlap(size_t stagenum, size_t tilenum)
{
	for (int a = 0; a < stageData.size(); a++)
	{
		for (int b = 0; b < stageData[a].stageTileData.size(); b++)
		{
			if (a == stagenum && b == tilenum)
			{
				continue;
			}
			if (static_cast<size_t>(GetStageTileOffsetX(a, b) + 1) <= (GetStageTileOffsetX(stagenum, tilenum) + GetStageTileWidth(stagenum, tilenum)) - 1 &&
				(GetStageTileOffsetX(a, b) + GetStageTileWidth(a, b)) - 1 >= static_cast<size_t>(GetStageTileOffsetX(stagenum, tilenum) + 1) &&
				static_cast<size_t>(GetStageTileOffsetY(a, b) + 1) <= (GetStageTileOffsetY(stagenum, tilenum) + GetStageTileHeight(stagenum, tilenum)) - 1 &&
				(GetStageTileOffsetY(a, b) + GetStageTileHeight(a, b)) - 1 >= static_cast<size_t>(GetStageTileOffsetY(stagenum, tilenum) + 1))
			{
				if (stageData[a].stageTileData[b].StageGroup > stageData[stagenum].stageTileData[tilenum].StageGroup &&
					stageData[stagenum].stageTileData[tilenum].IsOverSet == false)
				{
					stageData[stagenum].stageTileData[tilenum].Overlap++;
					//stageData[stagenum].stageTileData[tilenum].IsOverSet = true;
					break;
				}
			}
		}
	}
}

void Stage::SetOverlap(size_t stagenum, size_t tilenum)
{
	if (!stageData[stagenum].stageTileData[tilenum].stageEase.isMove)
	{
		return;
	}

	StageTileData* Tile = &stageData[stagenum].stageTileData[tilenum];

	//対象のタイルの外枠(折った後・開いた後)
	float NowTile_L = (Tile->offsetX) * blockSize;
	float NowTile_R = (Tile->offsetX + Tile->width) * blockSize;
	float NowTile_U = (Tile->offsetY) * blockSize;
	float NowTile_D = (Tile->offsetY + Tile->height) * blockSize;

	//対象のタイルが以前いた場所
	float OldTile_L = -1;
	float OldTile_R = -1;
	float OldTile_U = -1;
	float OldTile_D = -1;

	//FoldTypeや折る場合・開く場合で以前いた場所が変わる
	switch (Tile->FoldType)
	{
	case BodyType::left:
	{
		OldTile_U = NowTile_U;
		OldTile_D = NowTile_D;

		if (Tile->isFold)
		{
			OldTile_L = NowTile_L - (Tile->width * blockSize);
			OldTile_R = NowTile_L;
		}
		else
		{
			OldTile_L = NowTile_R;
			OldTile_R = NowTile_R + (Tile->width * blockSize);
		}
		break;
	}
	case BodyType::right:
	{
		OldTile_U = NowTile_U;
		OldTile_D = NowTile_D;

		if (Tile->isFold)
		{
			OldTile_L = NowTile_R;
			OldTile_R = NowTile_R + (Tile->width * blockSize);
		}
		else
		{
			OldTile_L = NowTile_L - (Tile->width * blockSize);
			OldTile_R = NowTile_L;
		}
		break;
	}
	case BodyType::up:
	{
		OldTile_L = NowTile_L;
		OldTile_R = NowTile_R;

		if (Tile->isFold)
		{
			OldTile_U = NowTile_U - (Tile->height * blockSize);
			OldTile_D = NowTile_U;
		}
		else
		{
			OldTile_U = NowTile_D;
			OldTile_D = NowTile_D + (Tile->height * blockSize);
		}
		break;
	}
	case BodyType::down:
	{
		OldTile_L = NowTile_L;
		OldTile_R = NowTile_R;

		if (Tile->isFold)
		{
			OldTile_U = NowTile_D;
			OldTile_D = NowTile_D + (Tile->height * blockSize);
		}
		else
		{
			OldTile_U = NowTile_U - (Tile->height * blockSize);
			OldTile_D = NowTile_U;
		}
		break;
	}
	default:
		break;
	}

	//対象外のタイルの外枠
	float OtherTile_L;
	float OtherTile_U;
	float OtherTile_R;
	float OtherTile_D;

	//重なっている状態を方向別に見る
	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			if (i == stagenum && j == tilenum)
			{
				continue;
			}

			//今見ているタイルの外枠をセット
			OtherTile_L = (stageData[i].stageTileData[j].offsetX) * blockSize;
			OtherTile_R = (stageData[i].stageTileData[j].offsetX + stageData[i].stageTileData[j].width) * blockSize;
			OtherTile_U = (stageData[i].stageTileData[j].offsetY) * blockSize;
			OtherTile_D = (stageData[i].stageTileData[j].offsetY + stageData[i].stageTileData[j].height) * blockSize;

			//現在の位置とかぶっていればOverlapを加算・以前いた位置とかぶっていればOerlapを減算する
			if (OtherTile_L < NowTile_R && OtherTile_R > NowTile_L &&
				OtherTile_U < NowTile_D && OtherTile_D > NowTile_U)
			{
				stageData[i].stageTileData[j].Overlap++;
			}

			if (OtherTile_L < OldTile_R && OtherTile_R > OldTile_L &&
				OtherTile_U < OldTile_D && OtherTile_D > OldTile_U)
			{
				stageData[i].stageTileData[j].Overlap--;
			}

		}
	}

	return;
}

bool Stage::IsPositionStage(const RVector3& center, const size_t& stageNumber)
{
	int NowStageCount = 0;

	for (j = 0; j < stageData[stageNumber].stageTileData.size(); j++)
	{
		float left = (float)stageData[stageNumber].stageTileData[j].offsetX * blockSize;
		float up = (float)stageData[stageNumber].stageTileData[j].offsetY * blockSize;
		float right = left + blockSize * (float)stageData[stageNumber].stageTileData[j].width;
		float down = up + blockSize * (float)stageData[stageNumber].stageTileData[j].height;

		if (center.x >= left && center.x < right && center.y >= up && center.y < down)
		{
			NowStageCount++;
		}
	}

	if (NowStageCount > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Stage::IsPlayerTile(const size_t& stageNumber, const size_t& TileNumber)
{
	float left = (float)stageData[stageNumber].stageTileData[TileNumber].offsetX * blockSize;
	float up = (float)stageData[stageNumber].stageTileData[TileNumber].offsetY * blockSize;
	float right = left + blockSize * (float)stageData[stageNumber].stageTileData[TileNumber].width;
	float down = up + blockSize * (float)stageData[stageNumber].stageTileData[TileNumber].height;

	if (player->CenterPosition.x >= left && player->CenterPosition.x < right && player->CenterPosition.y >= up && player->CenterPosition.y < down)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Stage::SetOnPlayerStageTileFold(std::vector<size_t>& stagenumber, std::vector<size_t>& onplayerstage, std::vector<size_t>& movestagetile, std::vector<size_t>& moveStageData, const unsigned char& direction)
{
	if (direction < 0)
	{
		return;
	}



	for (int b = 0; b < SelectStage->stageTileData.size(); b++)
	{
		if (IsPlayerTile(selectStageNum, b) && !IsNowTileOver(selectStageNum, b))
		{
			continue;
		}

		if (stageData[selectStageNum].stageTileData[b].FoldType == direction &&
			stageData[selectStageNum].stageTileData[b].Overlap <= 0)
		{
			switch (direction)
			{
			case BodyType::up:
			{
				stagenumber.push_back(selectStageNum);
				onplayerstage.push_back(stageData[selectStageNum].stageTileData[b].stageNumber);
				moveStageData.push_back(b);
				break;
			}
			case BodyType::down:
			{
				stagenumber.push_back(selectStageNum);
				onplayerstage.push_back(stageData[selectStageNum].stageTileData[b].stageNumber);
				moveStageData.push_back(b);
				break;
			}
			case BodyType::left:
			{
				stagenumber.push_back(selectStageNum);
				onplayerstage.push_back(stageData[selectStageNum].stageTileData[b].stageNumber);
				moveStageData.push_back(b);
				break;
			}
			case BodyType::right:
			{
				stagenumber.push_back(selectStageNum);
				onplayerstage.push_back(stageData[selectStageNum].stageTileData[b].stageNumber);
				moveStageData.push_back(b);
				break;
			}
			}
		}
	}

	return;
}

void Stage::SetOnPlayerStageTileOpen(std::vector<size_t>& stagenumber, std::vector<size_t>& onplayerstage, std::vector<size_t>& movestagetile, std::vector<size_t>& moveStageData, const unsigned char& direction)
{
	if (direction < 0)
	{
		return;
	}

	size_t NowStage = -1;
	size_t NowTile = -1;
	GetPositionTile(player->CenterPosition, &NowStage, &NowTile);

	StageTileData* playertiles = &stageData[NowStage].stageTileData[NowTile];


	for (int b = 0; b < SelectStage->stageTileData.size(); b++)
	{
		//タイルが折られているかつ、折られた方向が開く方向と一致していたら
		if (SelectStage->stageTileData[b].isFold == true &&
			SelectStage->stageTileData[b].FoldType == direction &&
			SelectStage->stageTileData[b].Overlap <= 0)
		{
			//方向ごとに格納していく
			switch (direction)
			{
			case BodyType::up:
			{
				stagenumber.push_back(selectStageNum);
				onplayerstage.push_back(stageData[selectStageNum].stageTileData[b].stageNumber);
				moveStageData.push_back(b);

				if (IsPositionTile(player->CenterPosition, selectStageNum, b) ||
					(NowStage == selectStageNum &&
						(playertiles->offsetX <= stageData[selectStageNum].stageTileData[b].offsetX + stageData[selectStageNum].stageTileData[b].width) &&
						playertiles->offsetX + playertiles->width >= stageData[selectStageNum].stageTileData[b].offsetX))
				{
					if (player->Body_Two.IsActivate && player->Body_Two.IsFold &&
						!player->Body_Two.IsAction &&
						player->IsUpBlocked)
					{
						if (player->IsOpenBlock(BodyType::up))
						{
							player->OpenCount = 0;
							player->IsOpenCountStart = true;
							player->IsUpOpen = true;
						}
					}
				}
				break;
			}
			case BodyType::down:
			{
				stagenumber.push_back(selectStageNum);
				onplayerstage.push_back(stageData[selectStageNum].stageTileData[b].stageNumber);
				moveStageData.push_back(b);

				if (IsPositionTile(player->CenterPosition, selectStageNum, b) ||
					(NowStage == selectStageNum &&
						(playertiles->offsetX <= stageData[selectStageNum].stageTileData[b].offsetX + stageData[selectStageNum].stageTileData[b].width) &&
						playertiles->offsetX + playertiles->width >= stageData[selectStageNum].stageTileData[b].offsetX))
				{
					if (player->Body_Four.IsActivate && player->Body_Four.IsFold &&
						!player->Body_Four.IsAction)
					{
						if (player->IsOpenBlock(BodyType::down))
						{
							player->OpenCount = 0;
							player->IsOpenCountStart = true;
							player->IsDownOpen = true;
						}
					}
				}
				break;
			}
			case BodyType::left:
			{
				stagenumber.push_back(selectStageNum);
				onplayerstage.push_back(stageData[selectStageNum].stageTileData[b].stageNumber);
				moveStageData.push_back(b);

				if (IsPositionTile(player->CenterPosition, selectStageNum, b) ||
					(NowStage == selectStageNum &&
						(playertiles->offsetY <= stageData[selectStageNum].stageTileData[b].offsetY + stageData[selectStageNum].stageTileData[b].height) &&
						playertiles->offsetY + playertiles->height >= stageData[selectStageNum].stageTileData[b].offsetY))
				{
					if (player->Body_One.IsActivate && player->Body_One.IsFold &&
						!player->Body_One.IsAction)
					{
						if (player->IsOpenBlock(BodyType::left))
						{
							player->OpenCount = 0;
							player->IsOpenCountStart = true;
							player->IsLeftOpen = true;
						}
					}
				}
				break;
			}
			case BodyType::right:
			{
				stagenumber.push_back(selectStageNum);
				onplayerstage.push_back(stageData[selectStageNum].stageTileData[b].stageNumber);
				moveStageData.push_back(b);

				if (IsPositionTile(player->CenterPosition, selectStageNum, b) ||
					(NowStage == selectStageNum &&
						(playertiles->offsetY <= stageData[selectStageNum].stageTileData[b].offsetY + stageData[selectStageNum].stageTileData[b].height) &&
						playertiles->offsetY + playertiles->height >= stageData[selectStageNum].stageTileData[b].offsetY))
				{
					if (player->Body_Three.IsActivate && player->Body_Three.IsFold &&
						!player->Body_Three.IsAction)
					{
						if (player->IsOpenBlock(BodyType::right))
						{
							player->OpenCount = 0;
							player->IsOpenCountStart = true;
							player->IsRightOpen = true;
						}
					}
				}
				break;
			}
			default:
				break;
			}
		}
	}
}

XMFLOAT2 Stage::ReturnMostOffset(const unsigned char& direction, const size_t& stageNumber)
{
	int mostOffsetX = stageData[stageNumber].stageTileData[0].offsetX;
	int mostOffsetY = stageData[stageNumber].stageTileData[0].offsetY;

	XMFLOAT2 mostOffsets = { (float)mostOffsetX,(float)mostOffsetY };

	for (int a = 0; a < stageData[stageNumber].stageTileData.size(); a++)
	{
		switch (direction)
		{
		case BodyType::up:
			if (mostOffsetY > stageData[stageNumber].stageTileData[a].offsetY)
			{
				mostOffsets.x = stageData[stageNumber].stageTileData[a].offsetX;
				mostOffsets.y = stageData[stageNumber].stageTileData[a].offsetY;
			}
			break;
		case BodyType::down:
			if (mostOffsetY < stageData[stageNumber].stageTileData[a].offsetY)
			{
				mostOffsets.x = stageData[stageNumber].stageTileData[a].offsetX;
				mostOffsets.y = stageData[stageNumber].stageTileData[a].offsetY;
			}
			break;
		case BodyType::left:
			if (mostOffsetX > stageData[stageNumber].stageTileData[a].offsetX)
			{
				mostOffsets.x = stageData[stageNumber].stageTileData[a].offsetX;
				mostOffsets.y = stageData[stageNumber].stageTileData[a].offsetY;
			}
			break;
		case BodyType::right:
			if (mostOffsetX < stageData[stageNumber].stageTileData[a].offsetX)
			{
				mostOffsets.x = stageData[stageNumber].stageTileData[a].offsetX;
				mostOffsets.y = stageData[stageNumber].stageTileData[a].offsetY;
			}
			break;
		}
	}

	return mostOffsets;
}

void Stage::SetStageGroup()
{
	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			stageData[i].stageTileData[j].StageGroup = static_cast<int>(i);
		}
	}
}

inline Stage::StageTileData* Stage::GetStageTileData(const size_t& stageNumber, const size_t& stageTileNumber)
{
	if (stageNumber >= stageData.size())
	{
		return nullptr;
	}
	if (stageTileNumber >= stageData[stageNumber].stageTileData.size())
	{
		return nullptr;
	}
	return &stageData[stageNumber].stageTileData[stageTileNumber];
}

inline Stage::StageData* Stage::GetStageData(const size_t& stageNumber)
{
	if (stageNumber >= stageData.size())
	{
		return nullptr;
	}
	return &stageData[stageNumber];
}

inline Stage::StageData* Stage::GetAllStageData()
{
	return stageData.data();
}

bool Stage::IsMapchipBlocks(char mapchip)
{
	if (mapchip == MapchipData::BLOCK)
	{
		return true;
	}

	if (mapchip == MapchipData::HORIZONTAL || mapchip == MapchipData::VERTICAL)
	{
		return true;
	}

	if (mapchip >= MapchipData::LEFTONLY && mapchip <= MapchipData::DOWNONLY)
	{
		return true;
	}

	if (mapchip >= MapchipData::LEFTL && mapchip <= MapchipData::DOWNL)
	{
		return true;
	}

	if (mapchip >= MapchipData::LEFTU && mapchip <= MapchipData::DOWNU)
	{
		return true;
	}

	if (mapchip == MapchipData::NOFRAME)
	{
		return true;
	}

	return false;
}

char Stage::GetPositionStage(const RVector3& playerPos)
{
	nowPlayerStage = -1;

	//ステージが重なっている場合は上にある方が優先

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			if ((playerPos.x / blockSize >= stageData[i].stageTileData[j].offsetX &&
				playerPos.x / blockSize < stageData[i].stageTileData[j].offsetX + stageData[i].stageTileData[j].width) &&
				(playerPos.y / blockSize >= stageData[i].stageTileData[j].offsetY &&
					playerPos.y / blockSize < stageData[i].stageTileData[j].offsetY + stageData[i].stageTileData[j].height))
			{
				nowPlayerStage = static_cast<char>(i);
				break;
			}
			else
			{
				continue;
			}
		}
	}

	return nowPlayerStage;
}

void Stage::GetPositionTile(const RVector3& center, size_t* stageNumber, size_t* stageTileNumber,
	bool isSkip, const size_t& skipStageNumber)
{
	*stageNumber = static_cast<size_t>(-1);
	*stageTileNumber = static_cast<size_t>(-1);
	x = static_cast<size_t>(center.x) / blockSize;
	y = static_cast<size_t>(center.y) / blockSize;

	for (i = 0; i < stageData.size(); i++)
	{
		if (isSkip && i <= skipStageNumber)
		{
			//continue;
		}

		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			auto& stageTile = stageData[i].stageTileData[j];

			if (x >= stageTile.offsetX && x <= stageTile.offsetX + stageTile.width &&
				y >= stageTile.offsetY && y <= stageTile.offsetY + stageTile.height)
			{
				*stageNumber = i;
				*stageTileNumber = j;
				//return;
			}
		}
	}
}

void Stage::GetPositionInitTile(const RVector3& center, size_t* stageNumber, size_t* stageTileNumber,
	bool isSkip, const size_t& skipStageNumber)
{
	*stageNumber = static_cast<size_t>(-1);
	*stageTileNumber = static_cast<size_t>(-1);
	x = static_cast<size_t>(center.x) / blockSize;
	y = static_cast<size_t>(center.y) / blockSize;

	for (i = 0; i < initStageData.size(); i++)
	{
		if (isSkip && i <= skipStageNumber)
		{
			continue;
		}

		for (j = 0; j < initStageData[i].stageTileData.size(); j++)
		{
			auto& stageTile = initStageData[i].stageTileData[j];

			if (x >= stageTile.offsetX && x < stageTile.offsetX + stageTile.width &&
				y >= stageTile.offsetY && y < stageTile.offsetY + stageTile.height)
			{
				*stageNumber = i;
				*stageTileNumber = j;
				return;
			}
		}
	}
}

bool Stage::IsPositionInitTile(size_t StageNum, size_t StageTileNum)
{
	if ((player->CenterPosition.x / blockSize >= initStageData[StageNum].stageTileData[StageTileNum].offsetX &&
		player->CenterPosition.x / blockSize < initStageData[StageNum].stageTileData[StageTileNum].offsetX + stageData[StageNum].stageTileData[StageTileNum].width) &&
		(player->CenterPosition.y / blockSize >= initStageData[StageNum].stageTileData[StageTileNum].offsetY &&
			player->CenterPosition.y / blockSize < initStageData[StageNum].stageTileData[StageTileNum].offsetY + stageData[StageNum].stageTileData[StageTileNum].height))
	{
		return true;
	}
	return false;
}

bool Stage::IsAnyTile(const RVector3& center)
{
	float left;
	float up;
	float right;
	float down;

	for (int a = 0; a < stageData.size(); a++)
	{
		for (int b = 0; b < stageData[a].stageTileData.size(); b++)
		{
			left = (float)stageData[a].stageTileData[b].offsetX * blockSize;
			up = (float)stageData[a].stageTileData[b].offsetY * blockSize;
			right = left + blockSize * (float)stageData[a].stageTileData[b].width;
			down = up + blockSize * (float)stageData[a].stageTileData[b].height;

			if (center.x >= left && center.x <= right && center.y >= up && center.y <= down)
			{
				return true;
			}
		}
	}

	return false;
}

bool Stage::IsPositionTile(const RVector3& center, const size_t& stageNumber, const size_t& stageTileNumber)
{
	float left = (float)stageData[stageNumber].stageTileData[stageTileNumber].offsetX * blockSize;
	float up = (float)stageData[stageNumber].stageTileData[stageTileNumber].offsetY * blockSize;
	float right = left + blockSize * (float)stageData[stageNumber].stageTileData[stageTileNumber].width;
	float down = up + blockSize * (float)stageData[stageNumber].stageTileData[stageTileNumber].height;

	if (center.x >= left && center.x < right && center.y >= up && center.y < down)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Stage::CreateParticle(const size_t& StageDataNum, const size_t& StageTileDataNum)
{
	for (int a = 0; a < 40; a++)
	{
		//位置設定
		float xpos = NY_random::floatrand_sl(
			static_cast<float>(stageData[StageDataNum].stageTileData[StageTileDataNum].offsetX + stageData[StageDataNum].stageTileData[StageTileDataNum].width) * blockSize,
			static_cast<float>(stageData[StageDataNum].stageTileData[StageTileDataNum].offsetX * blockSize));
		float ypos = NY_random::floatrand_sl(
			static_cast<float>(stageData[StageDataNum].stageTileData[StageTileDataNum].offsetY + stageData[StageDataNum].stageTileData[StageTileDataNum].height) * blockSize,
			static_cast<float>(stageData[StageDataNum].stageTileData[StageTileDataNum].offsetY * blockSize));

		RVector3 world_startpos = RV3Colider::CalcScreen2World({ xpos,ypos }, 0.0f);
		//this->particleManager->Prototype_Add(1, { world_startpos.x,world_startpos.y,0.0f });
		this->particleManager->Prototype_Add(1, { 10,10,0.0f });
	}
}

void Stage::LoadStageSound()
{
	foldSound = Audio::LoadSound_wav("./Resources/sound/SE/fold.wav");
	selectSound = Audio::LoadSound_wav("./Resources/sound/SE/select.wav");
}

void Stage::DeleteStageSound()
{
	Audio::UnloadSound(&foldSound);
	Audio::UnloadSound(&selectSound);
}

int Stage::Fold(const unsigned char& direction, const size_t& onPlayerStage, const size_t& onPlayerStageTile, const size_t& moveStageData, size_t datasize)
{
	//そもそも折れない用だったらreturn
	if (player->playerTile[direction] <= 0)
	{
		//return EF;
	}
	//すでにこのタイルが折られていたらreturn
	if (stageData[onPlayerStage].stageTileData[moveStageData].isFold)
	{
		return EF;
	}
	//すでに動いてるタイルを折ろうとしたら動きを止める
	if (stageData[onPlayerStage].stageTileData[moveStageData].stageEase.isMove)
	{
		stageData[onPlayerStage].stageTileData[moveStageData].stageEase.isMove = false;
		stageData[onPlayerStage].stageTileData[moveStageData].stageEase.splineIndex = 0;
		stageData[onPlayerStage].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
	}

	//イージングのセッティング
	stageData[onPlayerStage].stageTileData[moveStageData].stageEase.isMove = true;
	stageData[onPlayerStage].stageTileData[moveStageData].stageEase.splineIndex = 0;
	stageData[onPlayerStage].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
	stageData[onPlayerStage].stageTileData[moveStageData].stageEase.addTime = 0.1f;
	stageData[onPlayerStage].stageTileData[moveStageData].stageEase.maxTime = 1.2f;

	static int datacount = 0;

	//上下に折る場合
	if (direction == BodyType::up || direction == BodyType::down)
	{
		//イージング初期化
		EaseingInit(onPlayerStage, moveStageData, direction);

		for (y = 0; y < static_cast<size_t>(stageData[onPlayerStage].stageTileData[moveStageData].height / 2); y++)
		{
			//yがタイルの縦幅より大きければbreak
			if (y >= stageData[onPlayerStage].stageTileData[moveStageData].height - y - 1)
			{
				break;
			}

			for (x = 0; x < stageData[onPlayerStage].stageTileData[moveStageData].width; x++)
			{
				mapchipPos = y * stageData[onPlayerStage].stageTileData[moveStageData].width + x;
				reverseMapchipPos = (stageData[onPlayerStage].stageTileData[moveStageData].height - y - 1) * stageData[onPlayerStage].stageTileData[moveStageData].width + x;

				Swap(&stageData[onPlayerStage].stageTileData[moveStageData].mapchip[mapchipPos], &stageData[onPlayerStage].stageTileData[moveStageData].mapchip[reverseMapchipPos]);
			}
		}

		//directionを2で割った数(up = 1,down = 3)がint型キャストで0か1か
		if (direction / 2 == 0)
		{
			stageData[onPlayerStage].stageTileData[moveStageData].offsetY += static_cast<char>(stageData[onPlayerStage].stageTileData[moveStageData].height);
		}
		else
		{
			stageData[onPlayerStage].stageTileData[moveStageData].offsetY -= static_cast<char>(stageData[onPlayerStage].stageTileData[moveStageData].height);
		}
	}
	//左右に折る場合
	if (direction == BodyType::left || direction == BodyType::right)
	{
		EaseingInit(onPlayerStage, moveStageData, direction);

		for (y = 0; y < stageData[onPlayerStage].stageTileData[moveStageData].height; y++)
		{
			for (x = 0; x < static_cast<size_t>(stageData[onPlayerStage].stageTileData[moveStageData].width / 2); x++)
			{
				//xがタイルの横幅より大きければbreak
				if (x >= stageData[onPlayerStage].stageTileData[moveStageData].width - x - 1)
				{
					break;
				}

				mapchipPos = y * stageData[onPlayerStage].stageTileData[moveStageData].width + x;
				reverseMapchipPos = y * stageData[onPlayerStage].stageTileData[moveStageData].width + (stageData[onPlayerStage].stageTileData[moveStageData].width - x - 1);

				Swap(&stageData[onPlayerStage].stageTileData[moveStageData].mapchip[mapchipPos], &stageData[onPlayerStage].stageTileData[moveStageData].mapchip[reverseMapchipPos]);
			}
		}

		//directionを2で割った数(left = 0,right = 2)がint型キャストで0か1か
		if (direction / 2 == 0)
		{
			stageData[onPlayerStage].stageTileData[moveStageData].offsetX += static_cast<char>(stageData[onPlayerStage].stageTileData[moveStageData].width);
		}
		else
		{
			stageData[onPlayerStage].stageTileData[moveStageData].offsetX -= static_cast<char>(stageData[onPlayerStage].stageTileData[moveStageData].width);
		}
	}

	stageData[onPlayerStage].stageTileData[moveStageData].stageNumber = static_cast<char>(onPlayerStageTile);
	stageData[onPlayerStage].stageTileData[moveStageData].direction = direction + 1;
	stageData[onPlayerStage].stageTileData[moveStageData].isFold = true;
	stageData[onPlayerStage].stageTileData[moveStageData].FoldDirection = direction;

	datacount++;

	Audio::volume = 0.75f;
	Audio::PlayLoadedSound(foldSound);

	if (datacount >= datasize)
	{
		datacount = 0;
		player->playerTile[direction]--;
	}

	return 0;
}

int Stage::Open(const unsigned char& direction, const size_t& onPlayerStage, const size_t& moveStageData, size_t datasize)
{
	if (player->playerTile[direction] > 0)
	{
		//return EF;
	}

	if (stageData[onPlayerStage].stageTileData[moveStageData].isFold == false)
	{
		return EF;
	}

	stageData[onPlayerStage].stageTileData[moveStageData].stageEase.isMove = true;
	stageData[onPlayerStage].stageTileData[moveStageData].stageEase.splineIndex = 0;
	stageData[onPlayerStage].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
	stageData[onPlayerStage].stageTileData[moveStageData].stageEase.addTime = 0.1f;
	stageData[onPlayerStage].stageTileData[moveStageData].stageEase.maxTime = 1.2f;

	static int datacount = 0;

	EaseingInit(onPlayerStage, moveStageData, direction + 2);

	if (direction == BodyType::up || direction == BodyType::down)
	{
		for (y = 0; y < static_cast<size_t>(stageData[onPlayerStage].stageTileData[moveStageData].height / 2); y++)
		{
			if (y >= stageData[onPlayerStage].stageTileData[moveStageData].height - y - 1)
			{
				break;
			}

			for (x = 0; x < stageData[onPlayerStage].stageTileData[moveStageData].width; x++)
			{
				mapchipPos = y * stageData[onPlayerStage].stageTileData[moveStageData].width + x;
				reverseMapchipPos = (stageData[onPlayerStage].stageTileData[moveStageData].height - y - 1) * stageData[onPlayerStage].stageTileData[moveStageData].width + x;

				Swap(&stageData[onPlayerStage].stageTileData[moveStageData].mapchip[mapchipPos], &stageData[onPlayerStage].stageTileData[moveStageData].mapchip[reverseMapchipPos]);
			}
		}

		if (direction / 2 == 0)
		{
			stageData[onPlayerStage].stageTileData[moveStageData].offsetY -= static_cast<char>(stageData[onPlayerStage].stageTileData[moveStageData].height);
		}
		else
		{
			stageData[onPlayerStage].stageTileData[moveStageData].offsetY += static_cast<char>(stageData[onPlayerStage].stageTileData[moveStageData].height);
		}
	}
	if (direction == BodyType::left || direction == BodyType::right)
	{
		for (y = 0; y < stageData[onPlayerStage].stageTileData[moveStageData].height; y++)
		{
			for (x = 0; x < static_cast<size_t>(stageData[onPlayerStage].stageTileData[moveStageData].width / 2); x++)
			{
				if (x == stageData[onPlayerStage].stageTileData[moveStageData].width - x - 1)
				{
					break;
				}

				mapchipPos = y * stageData[onPlayerStage].stageTileData[moveStageData].width + x;
				reverseMapchipPos = y * stageData[onPlayerStage].stageTileData[moveStageData].width + (stageData[onPlayerStage].stageTileData[moveStageData].width - x - 1);

				Swap(&stageData[onPlayerStage].stageTileData[moveStageData].mapchip[mapchipPos], &stageData[onPlayerStage].stageTileData[moveStageData].mapchip[reverseMapchipPos]);
			}
		}

		if (direction / 2 == 0)
		{
			stageData[onPlayerStage].stageTileData[moveStageData].offsetX -= static_cast<char>(stageData[onPlayerStage].stageTileData[moveStageData].width);
		}
		else
		{
			stageData[onPlayerStage].stageTileData[moveStageData].offsetX += static_cast<char>(stageData[onPlayerStage].stageTileData[moveStageData].width);
		}
	}

	stageData[onPlayerStage].stageTileData[moveStageData].stageNumber = initStageData[onPlayerStage].stageTileData[moveStageData].stageNumber;
	stageData[onPlayerStage].stageTileData[moveStageData].direction = direction + 1 + 2;
	stageData[onPlayerStage].stageTileData[moveStageData].isFold = false;
	stageData[onPlayerStage].stageTileData[moveStageData].FoldDirection = -1;

	datacount++;

	Audio::volume = 0.75f;
	Audio::PlayLoadedSound(foldSound);

	if (datacount >= datasize)
	{
		datacount = 0;
		player->playerTile[direction]++;
	}

	return 0;
}

int Stage::StageTileDraw(const size_t& stageNumber, const size_t& stageTileNumber, const XMFLOAT2& offset, const float saturationColor)
{

	if (stageNumber >= stageData.size())
	{
		return EF;
	}
	if (stageTileNumber >= stageData[stageNumber].stageTileData.size())
	{
		return EF;
	}

	static RVector3 pos1 = {}, pos2 = {};
	static XMFLOAT4 color = {};

	for (mapchipPos = 0; mapchipPos < stageData[stageNumber].stageTileData[stageTileNumber].size; mapchipPos++)
	{
		x = mapchipPos % stageData[stageNumber].stageTileData[stageTileNumber].width;
		y = mapchipPos / stageData[stageNumber].stageTileData[stageTileNumber].width;

		pos1.x = stageData[stageNumber].stageTileData[stageTileNumber].drawLeftUp[mapchipPos].x + offset.x;
		pos1.y = stageData[stageNumber].stageTileData[stageTileNumber].drawLeftUp[mapchipPos].y + offset.y;
		pos1.z = stageData[stageNumber].stageTileData[stageTileNumber].drawLeftUp[mapchipPos].z;
		pos2.x = stageData[stageNumber].stageTileData[stageTileNumber].drawRightDown[mapchipPos].x + offset.x;
		pos2.y = stageData[stageNumber].stageTileData[stageTileNumber].drawRightDown[mapchipPos].y + offset.y;
		pos2.z = stageData[stageNumber].stageTileData[stageTileNumber].drawRightDown[mapchipPos].z;

		switch (stageData[stageNumber].stageTileData[stageTileNumber].mapchip[mapchipPos])
		{
		case MapchipData::EMPTY_STAGE:
		{
			continue;
			break;
		}
		case MapchipData::BLOCK:
		{
			// 色の初期化
			Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
			AllBlockSprite[stageNumber][0].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			break;
		}
		case MapchipData::GOAL:
		{
			// 色設定
			Sprite::SetSpriteColorParam(backColor[stageNumber % 4].x * saturationColor, backColor[stageNumber % 4].y * saturationColor,
				backColor[stageNumber % 4].z * saturationColor, backColor[stageNumber % 4].w * saturationColor);
			MapchipSpriteEmpty.DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			// 色の初期化
			Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
			MapchipSpriteGoal.DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			break;
		}
		case MapchipData::HORIZONTAL:
			// 色の初期化
			Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
			AllBlockSprite[stageNumber][1].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			break;

		case MapchipData::VERTICAL:
			// 色の初期化
			Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
			AllBlockSprite[stageNumber][2].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			break;

		case MapchipData::LEFTONLY:
			// 色の初期化
			Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
			AllBlockSprite[stageNumber][3].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			break;

		case MapchipData::UPONLY:
			// 色の初期化
			Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
			AllBlockSprite[stageNumber][4].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			break;

		case MapchipData::RIGHTONLY:
			// 色の初期化
			Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
			AllBlockSprite[stageNumber][5].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			break;

		case MapchipData::DOWNONLY:
			// 色の初期化
			Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
			AllBlockSprite[stageNumber][6].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			break;

		case MapchipData::LEFTL:
			// 色の初期化
			Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
			AllBlockSprite[stageNumber][7].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			break;

		case MapchipData::UPL:
			// 色の初期化
			Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
			AllBlockSprite[stageNumber][8].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			break;

		case MapchipData::RIGHTL:
			// 色の初期化
			Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
			AllBlockSprite[stageNumber][9].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			break;

		case MapchipData::DOWNL:
			// 色の初期化
			Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
			AllBlockSprite[stageNumber][10].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			break;

		case MapchipData::LEFTU:
			// 色の初期化
			Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
			AllBlockSprite[stageNumber][11].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			break;

		case MapchipData::UPU:
			// 色の初期化
			Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
			AllBlockSprite[stageNumber][12].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			break;

		case MapchipData::RIGHTU:
			// 色の初期化
			Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
			AllBlockSprite[stageNumber][13].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			break;

		case MapchipData::DOWNU:
			// 色の初期化
			Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
			AllBlockSprite[stageNumber][14].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			break;

		case MapchipData::NOFRAME:
			// 色の初期化
			Sprite::SetSpriteColorParam(1.0f, 1.0f, 1.0f, 1.0f);
			AllBlockSprite[stageNumber][15].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			break;

		case MapchipData::NONE:
		case MapchipData::START:
		default:
		{
			// 色設定
			Sprite::SetSpriteColorParam(backColor[stageNumber % 4].x * saturationColor, backColor[stageNumber % 4].y * saturationColor,
				backColor[stageNumber % 4].z * saturationColor, backColor[stageNumber % 4].w * saturationColor);
			MapchipSpriteEmpty.DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			break;
		}
		}
	}

	return 0;
}

int Stage::LineDraw(const size_t& stageNumber, const XMFLOAT2& offset, const float saturationColor)
{
	if (stageNumber >= stageData.size())
	{
		return EF;
	}

	static char sideStageTile = 0;
	static char sideStageData = 0;

	// 色設定
	Sprite::SetSpriteColorParam(lineColor[stageNumber % 4].x * saturationColor, lineColor[stageNumber % 4].y * saturationColor,
		lineColor[stageNumber % 4].z * saturationColor, lineColor[stageNumber % 4].w * saturationColor);

	// 折り目・枠線の描画
	for (j = 0; j < stageData[stageNumber].stageTileData.size(); j++)
	{
		if (static_cast<INT64>(stageData[stageNumber].stageTileData[j].stageNumber % stageData[stageNumber].width) - 1 >= 0)
		{
			sideStageTile = stageData[stageNumber].stageTileData[j].stageNumber - 1;
			sideStageData = stageData[stageNumber].stageTile[sideStageTile];

			if (sideStageData != MapchipData::EMPTY_STAGE)
			{
				FoldDraw(stageNumber, j, BodyType::left,
					static_cast<int>(offset.x), static_cast<int>(offset.y));
			}
			else
			{
				FlameDraw(stageNumber, j, BodyType::left,
					static_cast<int>(offset.x), static_cast<int>(offset.y));
			}
		}
		else
		{
			FlameDraw(stageNumber, j, BodyType::left,
				static_cast<int>(offset.x), static_cast<int>(offset.y));
		}
		if (static_cast<size_t>(stageData[stageNumber].stageTileData[j].stageNumber % stageData[stageNumber].width) + 1 < stageData[stageNumber].width)
		{
			sideStageTile = stageData[stageNumber].stageTileData[j].stageNumber + 1;
			sideStageData = stageData[stageNumber].stageTile[sideStageTile];

			if (sideStageData != MapchipData::EMPTY_STAGE)
			{
				FoldDraw(stageNumber, j, BodyType::right,
					static_cast<int>(offset.x), static_cast<int>(offset.y));
			}
			else
			{
				FlameDraw(stageNumber, j, BodyType::right,
					static_cast<int>(offset.x), static_cast<int>(offset.y));
			}
		}
		else
		{
			FlameDraw(stageNumber, j, BodyType::right,
				static_cast<int>(offset.x), static_cast<int>(offset.y));
		}
		if (static_cast<INT64>(stageData[stageNumber].stageTileData[j].stageNumber / stageData[stageNumber].width) - 1 >= 0)
		{
			sideStageTile = stageData[stageNumber].stageTileData[j].stageNumber - static_cast<char>(stageData[stageNumber].width);
			sideStageData = stageData[stageNumber].stageTile[sideStageTile];

			if (sideStageData != MapchipData::EMPTY_STAGE)
			{
				FoldDraw(stageNumber, j, BodyType::up,
					static_cast<int>(offset.x), static_cast<int>(offset.y));
			}
			else
			{
				FlameDraw(stageNumber, j, BodyType::up,
					static_cast<int>(offset.x), static_cast<int>(offset.y));
			}
		}
		else
		{
			FlameDraw(stageNumber, j, BodyType::up,
				static_cast<int>(offset.x), static_cast<int>(offset.y));
		}
		if (static_cast<size_t>(stageData[stageNumber].stageTileData[j].stageNumber / stageData[stageNumber].width) + 1 < stageData[stageNumber].height)
		{
			sideStageTile = stageData[stageNumber].stageTileData[j].stageNumber + static_cast<char>(stageData[stageNumber].width);
			sideStageData = stageData[stageNumber].stageTile[sideStageTile];

			if (sideStageData != MapchipData::EMPTY_STAGE)
			{
				FoldDraw(stageNumber, j, BodyType::down,
					static_cast<int>(offset.x), static_cast<int>(offset.y));
			}
			else
			{
				FlameDraw(stageNumber, j, BodyType::down,
					static_cast<int>(offset.x), static_cast<int>(offset.y));
			}
		}
		else
		{
			FlameDraw(stageNumber, j, BodyType::down,
				static_cast<int>(offset.x), static_cast<int>(offset.y));
		}
	}

	return 0;
}

int Stage::FlameDraw(const size_t& stageNumber, const size_t& stageTileNumber, const unsigned char direction,
	const int offsetX, const int offsetY)
{
	if (stageNumber >= stageData.size())
	{
		return EF;
	}
	if (stageTileNumber >= stageData[stageNumber].stageTileData.size())
	{
		return EF;
	}

	static int posX = 0, posY = 0;
	static XMFLOAT2 pos1 = {}, pos2 = {};

	switch (direction)
	{
	case BodyType::up:
	{
		posX = 0 + stageData[stageNumber].stageTileData[stageTileNumber].offsetX;
		posY = 0 + stageData[stageNumber].stageTileData[stageTileNumber].offsetY;

		pos1.x = static_cast<float>(posX * blockSize - lineWidth);
		pos1.y = static_cast<float>(posY * blockSize);
		pos2.x = static_cast<float>(posX + stageData[stageNumber].stageTileData[stageTileNumber].width) * blockSize;
		pos2.y = static_cast<float>(posY * blockSize - lineWidth);

		break;
	}
	case BodyType::down:
	{
		posX = 0 + stageData[stageNumber].stageTileData[stageTileNumber].offsetX;
		posY = static_cast<int>(stageData[stageNumber].stageTileData[stageTileNumber].height) + stageData[stageNumber].stageTileData[stageTileNumber].offsetY;

		pos1.x = static_cast<float>(posX * blockSize);
		pos1.y = static_cast<float>(posY * blockSize);
		pos2.x = static_cast<float>(posX + stageData[stageNumber].stageTileData[stageTileNumber].width) * blockSize + lineWidth;
		pos2.y = static_cast<float>(posY * blockSize + lineWidth);

		break;
	}
	case BodyType::left:
	{
		posX = 0 + stageData[stageNumber].stageTileData[stageTileNumber].offsetX;
		posY = 0 + stageData[stageNumber].stageTileData[stageTileNumber].offsetY;

		pos1.x = static_cast<float>(posX * blockSize);
		pos1.y = static_cast<float>(posY * blockSize);
		pos2.x = static_cast<float>(posX * blockSize - lineWidth);
		pos2.y = static_cast<float>(posY + stageData[stageNumber].stageTileData[stageTileNumber].height) * blockSize + lineWidth;

		break;
	}
	case BodyType::right:
	{
		posX = static_cast<int>(stageData[stageNumber].stageTileData[stageTileNumber].width) + stageData[stageNumber].stageTileData[stageTileNumber].offsetX;
		posY = 0 + stageData[stageNumber].stageTileData[stageTileNumber].offsetY;

		pos1.x = static_cast<float>(posX * blockSize);
		pos1.y = static_cast<float>(posY * blockSize - lineWidth);
		pos2.x = static_cast<float>(posX * blockSize + lineWidth);
		pos2.y = static_cast<float>(posY + stageData[stageNumber].stageTileData[stageTileNumber].height) * blockSize;

		break;
	}
	default:
	{
		return EF;
		break;
	}
	}
	pos1.x += static_cast<float>(offsetX);
	pos1.y += static_cast<float>(offsetY);
	pos2.x += static_cast<float>(offsetX);
	pos2.y += static_cast<float>(offsetY);

	lineSprite.DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);

	return 0;
}

int Stage::FoldDraw(const size_t& stageNumber, const size_t& stageTileNumber, const unsigned char direction,
	const int offsetX, const int offsetY)
{

	if (stageNumber >= stageData.size())
	{
		return EF;
	}
	if (stageTileNumber >= stageData[stageNumber].stageTileData.size())
	{
		return EF;
	}

	static int posX = 0, posY = 0;
	static XMFLOAT2 pos1 = {}, pos2 = {};

	switch (direction)
	{
	case BodyType::up:
	{
		posY = static_cast<int>(0 + stageData[stageNumber].stageTileData[stageTileNumber].offsetY);
		pos1.y = static_cast<float>(posY * blockSize);
		pos2.y = static_cast<float>(posY * blockSize + (lineWidth + 1));

		pos1.y += static_cast<float>(offsetY);
		pos2.y += static_cast<float>(offsetY);

		for (x = 0; x < stageData[stageNumber].stageTileData[stageTileNumber].width; x++)
		{
			posX = static_cast<int>(x + stageData[stageNumber].stageTileData[stageTileNumber].offsetX);

			for (int k = 0; k < foldLineCount; k++)
			{
				pos1.x = static_cast<float>(posX * blockSize + blockSize * (1 + k * 4) / (4 * foldLineCount));
				pos2.x = static_cast<float>(posX * blockSize + blockSize * (3 + k * 4) / (4 * foldLineCount));

				pos1.x += static_cast<float>(offsetX);
				pos2.x += static_cast<float>(offsetX);

				lineSprite.DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			}
		}
		break;
	}
	case BodyType::down:
	{
		posY = static_cast<int>(stageData[stageNumber].stageTileData[stageTileNumber].height +
			stageData[stageNumber].stageTileData[stageTileNumber].offsetY);
		pos1.y = static_cast<float>(posY * blockSize);
		pos2.y = static_cast<float>(posY * blockSize - (lineWidth + 1));

		pos1.y += static_cast<float>(offsetY);
		pos2.y += static_cast<float>(offsetY);

		for (x = 0; x < stageData[stageNumber].stageTileData[stageTileNumber].width; x++)
		{
			posX = static_cast<int>(x + stageData[stageNumber].stageTileData[stageTileNumber].offsetX);

			for (int k = 0; k < foldLineCount; k++)
			{
				pos1.x = static_cast<float>(posX * blockSize + blockSize * (1 + k * 4) / (4 * foldLineCount));
				pos2.x = static_cast<float>(posX * blockSize + blockSize * (3 + k * 4) / (4 * foldLineCount));

				pos1.x += static_cast<float>(offsetX);
				pos2.x += static_cast<float>(offsetX);

				lineSprite.DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			}
		}

		break;
	}
	case BodyType::left:
	{
		posX = static_cast<int>(0 + stageData[stageNumber].stageTileData[stageTileNumber].offsetX);
		pos1.x = static_cast<float>(posX * blockSize);
		pos2.x = static_cast<float>(posX * blockSize + (lineWidth + 1));

		pos1.x += static_cast<float>(offsetX);
		pos2.x += static_cast<float>(offsetX);

		for (y = 0; y < stageData[stageNumber].stageTileData[stageTileNumber].height; y++)
		{
			posY = static_cast<int>(y + stageData[stageNumber].stageTileData[stageTileNumber].offsetY);

			for (int k = 0; k < foldLineCount; k++)
			{
				pos1.y = static_cast<float>(posY * blockSize + blockSize * (1 + k * 4) / (4 * foldLineCount));
				pos2.y = static_cast<float>(posY * blockSize + blockSize * (3 + k * 4) / (4 * foldLineCount));

				pos1.y += static_cast<float>(offsetY);
				pos2.y += static_cast<float>(offsetY);

				lineSprite.DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			}
		}
		break;
	}
	case BodyType::right:
	{
		posX = static_cast<int>(stageData[stageNumber].stageTileData[stageTileNumber].width +
			stageData[stageNumber].stageTileData[stageTileNumber].offsetX);
		pos1.x = static_cast<float>(posX * blockSize);
		pos2.x = static_cast<float>(posX * blockSize - (lineWidth + 1));

		pos1.x += static_cast<float>(offsetX);
		pos2.x += static_cast<float>(offsetX);

		for (y = 0; y < stageData[stageNumber].stageTileData[stageTileNumber].height; y++)
		{
			posY = static_cast<int>(y + stageData[stageNumber].stageTileData[stageTileNumber].offsetY);

			for (int k = 0; k < foldLineCount; k++)
			{
				pos1.y = static_cast<float>(posY * blockSize + blockSize * (1 + k * 4) / (4 * foldLineCount));
				pos2.y = static_cast<float>(posY * blockSize + blockSize * (3 + k * 4) / (4 * foldLineCount));

				pos1.y += static_cast<float>(offsetY);
				pos2.y += static_cast<float>(offsetY);

				lineSprite.DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
			}
		}
		break;
	}
	default:
	{
		return EF;
		break;
	}
	}

	return 0;
}

void Stage::EaseingInit(const size_t& moveStage, const size_t& moveTile, const int& direction)
{
	static float keepA = 0.0f, keepB = 0.0f;

	for (y = 0; y < static_cast<size_t>(stageData[moveStage].stageTileData[moveTile].height); y++)
	{
		for (x = 0; x < stageData[moveStage].stageTileData[moveTile].width; x++)
		{
			mapchipPos = y * stageData[moveStage].stageTileData[moveTile].width + x;

			switch (direction % 4)
			{
			case BodyType::up:
			{
				stageData[moveStage].stageTileData[moveTile].startPos[mapchipPos].x = static_cast<float>(x + stageData[moveStage].stageTileData[moveTile].offsetX);
				stageData[moveStage].stageTileData[moveTile].startPos[mapchipPos].y = static_cast<float>(y + stageData[moveStage].stageTileData[moveTile].offsetY);
				stageData[moveStage].stageTileData[moveTile].startPos[mapchipPos].z = 0.0f;
				stageData[moveStage].stageTileData[moveTile].startPos[mapchipPos] *= blockSize;

				keepA = static_cast<float>(stageData[moveStage].stageTileData[moveTile].height - y - 1);
				keepB = static_cast<float>(stageData[moveStage].stageTileData[moveTile].offsetY + stageData[moveStage].stageTileData[moveTile].height);

				stageData[moveStage].stageTileData[moveTile].endPos[mapchipPos].x = static_cast<float>(x + stageData[moveStage].stageTileData[moveTile].offsetX);
				stageData[moveStage].stageTileData[moveTile].endPos[mapchipPos].y = keepA + keepB + 1.0f;
				stageData[moveStage].stageTileData[moveTile].endPos[mapchipPos].z = 0.0f;
				stageData[moveStage].stageTileData[moveTile].endPos[mapchipPos] *= blockSize;
				break;
			}
			case BodyType::down:
			{
				stageData[moveStage].stageTileData[moveTile].startPos[mapchipPos].x = static_cast<float>(x + stageData[moveStage].stageTileData[moveTile].offsetX);
				stageData[moveStage].stageTileData[moveTile].startPos[mapchipPos].y = static_cast<float>(y + stageData[moveStage].stageTileData[moveTile].offsetY) + 1.0f;
				stageData[moveStage].stageTileData[moveTile].startPos[mapchipPos].z = 0.0f;
				stageData[moveStage].stageTileData[moveTile].startPos[mapchipPos] *= blockSize;

				keepA = static_cast<float>(stageData[moveStage].stageTileData[moveTile].height - y - 1);
				keepB = static_cast<float>(stageData[moveStage].stageTileData[moveTile].offsetY - stageData[moveStage].stageTileData[moveTile].height);

				stageData[moveStage].stageTileData[moveTile].endPos[mapchipPos].x = static_cast<float>(x + stageData[moveStage].stageTileData[moveTile].offsetX);
				stageData[moveStage].stageTileData[moveTile].endPos[mapchipPos].y = keepA + keepB;
				stageData[moveStage].stageTileData[moveTile].endPos[mapchipPos].z = 0.0f;
				stageData[moveStage].stageTileData[moveTile].endPos[mapchipPos] *= blockSize;
				break;
			}
			case BodyType::left:
			{
				stageData[moveStage].stageTileData[moveTile].startPos[mapchipPos].x = static_cast<float>(x + stageData[moveStage].stageTileData[moveTile].offsetX);
				stageData[moveStage].stageTileData[moveTile].startPos[mapchipPos].y = static_cast<float>(y + stageData[moveStage].stageTileData[moveTile].offsetY);
				stageData[moveStage].stageTileData[moveTile].startPos[mapchipPos].z = 0.0f;
				stageData[moveStage].stageTileData[moveTile].startPos[mapchipPos] *= blockSize;

				keepA = static_cast<float>(stageData[moveStage].stageTileData[moveTile].width - x - 1);
				keepB = static_cast<float>(stageData[moveStage].stageTileData[moveTile].offsetX + stageData[moveStage].stageTileData[moveTile].width);

				stageData[moveStage].stageTileData[moveTile].endPos[mapchipPos].x = keepA + keepB + 1.0f;
				stageData[moveStage].stageTileData[moveTile].endPos[mapchipPos].y = static_cast<float>(y + stageData[moveStage].stageTileData[moveTile].offsetY);
				stageData[moveStage].stageTileData[moveTile].endPos[mapchipPos].z = 0.0f;
				stageData[moveStage].stageTileData[moveTile].endPos[mapchipPos] *= blockSize;

				break;
			}
			case BodyType::right:
			{
				stageData[moveStage].stageTileData[moveTile].startPos[mapchipPos].x = static_cast<float>(x + stageData[moveStage].stageTileData[moveTile].offsetX) + 1.0f;
				stageData[moveStage].stageTileData[moveTile].startPos[mapchipPos].y = static_cast<float>(y + stageData[moveStage].stageTileData[moveTile].offsetY);
				stageData[moveStage].stageTileData[moveTile].startPos[mapchipPos].z = 0.0f;
				stageData[moveStage].stageTileData[moveTile].startPos[mapchipPos] *= blockSize;

				keepA = static_cast<float>(stageData[moveStage].stageTileData[moveTile].width - x - 1);
				keepB = static_cast<float>(stageData[moveStage].stageTileData[moveTile].offsetX - stageData[moveStage].stageTileData[moveTile].width);

				stageData[moveStage].stageTileData[moveTile].endPos[mapchipPos].x = keepA + keepB;
				stageData[moveStage].stageTileData[moveTile].endPos[mapchipPos].y = static_cast<float>(y + stageData[moveStage].stageTileData[moveTile].offsetY);
				stageData[moveStage].stageTileData[moveTile].endPos[mapchipPos].z = 0.0f;
				stageData[moveStage].stageTileData[moveTile].endPos[mapchipPos] *= blockSize;

				break;
			}
			default:
				break;
			}
		}
	}
}

void Stage::EaseingUpdate()
{
	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			if (stageData[i].stageTileData[j].stageEase.isMove == false)
			{
				continue;
			}
			if ((stageData[i].stageTileData[j].direction - 1) < 0)
			{
				continue;
			}

			static float ease = 0.0f;

			stageData[i].stageTileData[j].stageEase.addTime += stageData[i].stageTileData[j].stageEase.maxTime / 20.0f;
			stageData[i].stageTileData[j].stageEase.timeRate =
				min(stageData[i].stageTileData[j].stageEase.addTime / stageData[i].stageTileData[j].stageEase.maxTime, 1.0f);
			ease = Easing::easeOut(0.0f, 1.0f, stageData[i].stageTileData[j].stageEase.timeRate);

			for (mapchipPos = 0; mapchipPos < stageData[i].stageTileData[j].size; mapchipPos++)
			{
				static RVector3 axisPos = {};

				switch ((stageData[i].stageTileData[j].direction - 1) % 4)
				{
				case BodyType::up:
					axisPos = { static_cast<float>(stageData[i].stageTileData[j].startPos[mapchipPos].x),
						static_cast<float>(stageData[i].stageTileData[j].startPos[0].y + (stageData[i].stageTileData[j].height * blockSize)),
						static_cast<float>(stageData[i].stageTileData[j].height * blockSize) };
					break;
				case BodyType::down:
					axisPos = { static_cast<float>(stageData[i].stageTileData[j].startPos[mapchipPos].x),
						static_cast<float>(stageData[i].stageTileData[j].startPos[0].y - blockSize),
						static_cast<float>(stageData[i].stageTileData[j].height * blockSize) };
					break;
				case BodyType::left:
					axisPos = { static_cast<float>(stageData[i].stageTileData[j].startPos[0].x + (stageData[i].stageTileData[j].width * blockSize)),
						static_cast<float>(stageData[i].stageTileData[j].startPos[mapchipPos].y),
						static_cast<float>(stageData[i].stageTileData[j].width * blockSize) };
					break;
				case BodyType::right:
					axisPos = { static_cast<float>(stageData[i].stageTileData[j].startPos[0].x - blockSize),
						static_cast<float>(stageData[i].stageTileData[j].startPos[mapchipPos].y),
						static_cast<float>(stageData[i].stageTileData[j].width * blockSize) };
					break;
				default:
					break;
				}

				std::vector<RVector3> pos = {
					stageData[i].stageTileData[j].startPos[mapchipPos],
					axisPos,
					stageData[i].stageTileData[j].endPos[mapchipPos]
				};

				stageData[i].stageTileData[j].easePos[mapchipPos] = Easing::SplineCurve(
					pos,
					stageData[i].stageTileData[j].stageEase.splineIndex,
					stageData[i].stageTileData[j].stageEase.timeRate);
			}

			if (stageData[i].stageTileData[j].stageEase.timeRate >= 1.0f)
			{
				if (stageData[i].stageTileData[j].stageEase.splineIndex < 3Ui64 - 2Ui64)
				{
					stageData[i].stageTileData[j].stageEase.splineIndex++;
					stageData[i].stageTileData[j].stageEase.timeRate = 0.0f;
					stageData[i].stageTileData[j].stageEase.addTime = 0.1f;
					stageData[i].stageTileData[j].stageEase.maxTime = 1.5f;
				}
				else
				{
					stageData[i].stageTileData[j].stageEase.isMove = false;

					//ここでパーティクル生成
					float oriEffectPosX = static_cast<float>(stageData[i].stageTileData[j].offsetX) * static_cast<float>(blockSize)
						+ stageData[i].stageTileData[j].width * static_cast<float>(blockSize / 2) + drawOffsetX;
					float oriEffectPosY = static_cast<float>(stageData[i].stageTileData[j].offsetY) * static_cast<float>(blockSize)
						+ stageData[i].stageTileData[j].height * static_cast<float>(blockSize / 2) + drawOffsetY;
					oriEffect.Play(oriEffectPosX, oriEffectPosY);
					break;
				}
			}
		}
	}
}

int Stage::SearchTopStageTile()
{
	std::vector<RVector3> topStageTile; //xの値にstageDataの要素番号、yの値にstageData[i].stageDataの要素番号を格納する
	static bool isTop = false;

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			isTop = false;

			for (size_t k = 0; k < topStageTile.size(); k++)
			{

			}
		}
	}

	return 0;
}

void ParticleSingle::Init()
{
	//開始位置
	spos = RVector3(0, 0, 0);
	pos = spos;

	//終了フレーム
	endFrame = 40;

	//速度設定
	float xvel = NY_random::floatrand_sl(3.0f, -3.0f);
	float yvel = NY_random::floatrand_sl(3.0f, -3.0f);

	vel = RVector3(xvel, yvel, 0.0f);

	acc = RVector3(0.9f, 0.9f, 0.0f);

	scale = 3.0f;
}

void ParticleSingle::Update()
{
	//毎フレーム加算
	vel *= acc;
	pos += vel;
}

ParticlePrototype* ParticleSingle::clone(RVector3 start)
{
	return new ParticleSingle();
}
