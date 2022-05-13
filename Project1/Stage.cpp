#include "Stage.h"
#include "LoadFile.h"
#include "General.h"
#include "Player.h"
#include "NY_random.h"
#include <Raki_DX12B.h>

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
}

const int Stage::blockSize = 60;
const int Stage::halfBlockSize = Stage::blockSize / 2;
const int Stage::lineWidth = 5;
const int Stage::foldLineCount = 2;
const XMFLOAT4 Stage::lineColor[2] = {
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 0.5f, 0.5f, 0.5f, 1.0f }
};

int Stage::drawOffsetX = 0;
int Stage::drawOffsetY = 0;
int Stage::startPlayerPosX = 0;
int Stage::startPlayerPosY = 0;
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
}

void Stage::Init()
{
}

void Stage::Updata()
{
	static int posX = 0;
	static int posY = 0;

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
		lineSprite.spdata->color = lineColor[i % 2]; //色設定

		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			for (y = 0; y < stageData[i].stageTileData[j].height; y++)
			{
				for (x = 0; x < stageData[i].stageTileData[j].width; x++)
				{
					mapchipPos = y * stageData[i].stageTileData[j].width + x;

					pos1.x = stageData[i].stageTileData[j].drawLeftUp[mapchipPos].x + drawOffset.x;
					pos1.y = stageData[i].stageTileData[j].drawLeftUp[mapchipPos].y + drawOffset.y;
					pos1.z = stageData[i].stageTileData[j].drawLeftUp[mapchipPos].z;
					pos2.x = stageData[i].stageTileData[j].drawRightDown[mapchipPos].x + drawOffset.x;
					pos2.y = stageData[i].stageTileData[j].drawRightDown[mapchipPos].y + drawOffset.y;
					pos2.z = stageData[i].stageTileData[j].drawRightDown[mapchipPos].z;

					switch (stageData[i].stageTileData[j].mapchip[mapchipPos])
					{
					case MapchipData::EMPTY_STAGE:
					{
						continue;
						break;
					}
					case MapchipData::BLOCK:
					{
						AllBlockSprite[i][0].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
						break;
					}
					case MapchipData::GOAL:
					{
						MapchipSpriteGoal.DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
						break;
					}
					case MapchipData::HORIZONTAL:
						AllBlockSprite[i][1].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
						break;

					case MapchipData::VERTICAL:
						AllBlockSprite[i][2].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
						break;

					case MapchipData::LEFTONLY:
						AllBlockSprite[i][3].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
						break;
						
					case MapchipData::UPONLY:
						AllBlockSprite[i][4].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
						break;

					case MapchipData::RIGHTONLY:
						AllBlockSprite[i][5].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
						break;

					case MapchipData::DOWNONLY:
						AllBlockSprite[i][6].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
						break;

					case MapchipData::LEFTL:
						AllBlockSprite[i][7].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
						break;

					case MapchipData::UPL:
						AllBlockSprite[i][8].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
						break;

					case MapchipData::RIGHTL:
						AllBlockSprite[i][9].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
						break;

					case MapchipData::DOWNL:
						AllBlockSprite[i][10].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
						break;

					case MapchipData::LEFTU:
						AllBlockSprite[i][10].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
						break;

					case MapchipData::UPU:
						AllBlockSprite[i][11].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
						break;

					case MapchipData::RIGHTU:
						AllBlockSprite[i][11].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
						break;

					case MapchipData::DOWNU:
						AllBlockSprite[i][11].DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
						break;

					case MapchipData::NONE:
					case MapchipData::START:
					default:
					{
						MapchipSpriteEmpty.DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
						break;
					}
					}
				}
			}
		}

		// 折り目・枠線の描画
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			static char sideStageTile = 0;
			static char sideStageData = 0;

			if (static_cast<INT64>(stageData[i].stageTileData[j].stageNumber % stageData[i].width) - 1 >= 0)
			{
				sideStageTile = stageData[i].stageTileData[j].stageNumber - 1;

				if (stageData[i].stageTile[sideStageTile] != MapchipData::EMPTY_STAGE)
				{
					FoldDraw(i, j, BodyType::left, drawOffset.x, drawOffset.y);
				}
				else
				{
					FlameDraw(i, j, BodyType::left, drawOffset.x, drawOffset.y);
				}
			}
			else
			{
				FlameDraw(i, j, BodyType::left, drawOffset.x, drawOffset.y);
			}
			if (static_cast<size_t>(stageData[i].stageTileData[j].stageNumber % stageData[i].width) + 1 < stageData[i].width)
			{
				sideStageTile = stageData[i].stageTileData[j].stageNumber + 1;

				if (stageData[i].stageTile[sideStageTile] != MapchipData::EMPTY_STAGE)
				{
					FoldDraw(i, j, BodyType::right, drawOffset.x, drawOffset.y);
				}
				else
				{
					FlameDraw(i, j, BodyType::right, drawOffset.x, drawOffset.y);
				}
			}
			else
			{
				FlameDraw(i, j, BodyType::right, drawOffset.x, drawOffset.y);
			}
			if (static_cast<INT64>(stageData[i].stageTileData[j].stageNumber / stageData[i].width) - 1 >= 0)
			{
				sideStageTile = stageData[i].stageTileData[j].stageNumber - static_cast<char>(stageData[i].width);
				sideStageData = stageData[i].stageTile[sideStageTile];

				if (sideStageData != MapchipData::EMPTY_STAGE)
				{
					FoldDraw(i, j, BodyType::up, drawOffset.x, drawOffset.y);
				}
				else
				{
					FlameDraw(i, j, BodyType::up, drawOffset.x, drawOffset.y);
				}
			}
			else
			{
				FlameDraw(i, j, BodyType::up, drawOffset.x, drawOffset.y);
			}
			if (static_cast<size_t>(stageData[i].stageTileData[j].stageNumber / stageData[i].width) + 1 < stageData[i].height)
			{
				sideStageTile = stageData[i].stageTileData[j].stageNumber + static_cast<char>(stageData[i].width);

				if (stageData[i].stageTile[sideStageTile] != MapchipData::EMPTY_STAGE)
				{
					FoldDraw(i, j, BodyType::down, drawOffset.x, drawOffset.y);
				}
				else
				{
					FlameDraw(i, j, BodyType::down, drawOffset.x, drawOffset.y);
				}
			}
			else
			{
				FlameDraw(i, j, BodyType::down, drawOffset.x, drawOffset.y);
			}
		}
	}

	particleManager->Prototype_Draw(EmptyHandle);

	SpriteManager::Get()->SetCommonBeginDraw();

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < 15; j++)
		{
			AllBlockSprite[i][j].Draw();
		}
	}

	MapchipSpriteGoal.Draw();
	MapchipSpriteEmpty.Draw();
	lineSprite.Draw();
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
		GoalHandle = TexManager::LoadTexture("Resources/goal.png");
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
}

void Stage::LoadBlocksHandle()
{
	//画像ハンドルの読み込み
	Bule_BlocksHandle[0] = TexManager::LoadTexture("Resources/Blocks/Block_Bule/blockB.png");//青
	Green_BlocksHandle[0] = TexManager::LoadTexture("Resources/Blocks/Block_Green/blockG.png");//緑
	Red_BlocksHandle[0] = TexManager::LoadTexture("Resources/Blocks/Block_Red/blockR.png");//赤
	Yellow_BlocksHandle[0] = TexManager::LoadTexture("Resources/Blocks/Block_Yellow/blockY.png");//黄

	std::string BasePath = "Resources/Blocks/";
	std::string fileType = ".png";

	std::string Bule = "Block_Bule/blockB";
	std::string Green = "Block_Green/blockG";
	std::string Red = "Block_Red/blockR";
	std::string Yellow = "Block_Yellow/blockY";

	std::string Horizontal = "_=";
	std::string Vertical = "_ll";

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
		for (j = 0; j < 15; j++)
		{
			if ((AllBlockSprite[i][j].spdata->size.x <= 0) || (AllBlockSprite[i][j].spdata->size.y <= 0))
			{
				AllBlockSprite[i][j].Create(AllBlockHandle[i][j]);
			}
		}
	}
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
			if (stageData[i].stageTile[j] == MapchipData::EMPTY_STAGE)
			{
				continue;
			}

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

	// オフセット値の計算
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
		}
	}

	static bool end = false;
	end = false;

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			// プレイヤーの開始位置の探索
			for (y = 0; y < stageData[i].stageTileData[j].height; y++)
			{
				for (x = 0; x < stageData[i].stageTileData[j].width; x++)
				{
					mapchipPos = y * stageData[i].stageTileData[j].height + x;
					if (stageData[i].stageTileData[j].mapchip[mapchipPos] == MapchipData::START)
					{
						startPlayerPosX = static_cast<int>(x + stageData[i].stageTileData[j].offsetX);
						startPlayerPosY = static_cast<int>(y + stageData[i].stageTileData[j].offsetY);

						end = true;
						break;
					}
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
		initStageData.push_back(stageData[i]);
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			char* initMapchip = static_cast<char*>(malloc(sizeof(char) * stageData[i].stageTileData[j].size));
			memcpy_s(initMapchip, sizeof(char) * stageData[i].stageTileData[j].size,
				stageData[i].stageTileData[j].mapchip, sizeof(char) * stageData[i].stageTileData[j].size);
			initStageData[i].stageTileData[j].mapchip = initMapchip;
		}
	}

	return 0;
}

int Stage::FoldAndOpen(const RVector3& playerPos, unsigned char playerTile[4], bool BodyStatus[4], bool IsFootAction, bool IsFolds[4], int OpenCount, bool IsOpens[4])
{
	unsigned char direction = -1;
	static size_t onPlayerStageTile = 0;
	static size_t moveStageTile = 0;
	static size_t moveStageData = 0;

	direction = -1;

	//if (IsFolds[BodyType::up] || IsOpens[BodyType::down])
	if (player->Body_Two.IsAction)
	{
		direction = BodyType::up;
	}
	//else if (IsFolds[BodyType::down] || IsOpens[BodyType::up])
	else if (player->Body_Four.IsAction)
	{
		direction = BodyType::down;
	}
	//else if (IsFolds[BodyType::left] || IsOpens[BodyType::right])
	else if (player->Body_One.IsAction)
	{
		direction = BodyType::left;
	}
	//else if (IsFolds[BodyType::right] || IsOpens[BodyType::left])
	else if (player->Body_Three.IsAction)
	{
		direction = BodyType::right;
	}

	static bool isAct = false;

	isAct = false;

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			if ((playerPos.x / blockSize >= initStageData[i].stageTileData[j].offsetX &&
				playerPos.x / blockSize < initStageData[i].stageTileData[j].offsetX + stageData[i].stageTileData[j].width) &&
				(playerPos.y / blockSize >= initStageData[i].stageTileData[j].offsetY &&
					playerPos.y / blockSize < initStageData[i].stageTileData[j].offsetY + stageData[i].stageTileData[j].height))
			{
				onPlayerStageTile = initStageData[i].stageTileData[j].stageNumber;
			}
			else
			{
				continue;
			}

			switch (direction)
			{
			case BodyType::up: //上入力
			{
				if (onPlayerStageTile / initStageData[i].width <= 0)
				{
					break;
				}

				moveStageTile = onPlayerStageTile - initStageData[i].width;
				moveStageData = static_cast<size_t>(initStageData[i].stageTile[moveStageTile]) - 1;

				if (moveStageTile < 0 ||
					stageData[i].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
				{
					break;
				}

				if (BodyStatus[BodyType::up] == false)
				{
					//break;
				}

				if (OpenCount >= 2 && IsOpens[BodyType::up] == true)
				{
					if (Open(playerTile, direction, i, moveStageData) != EF)
					{
						isAct = true;
					}
				}
				else if (player->leg.FootIsAction == false && player->Body_Two.IsFold == true)
				{
					if (Fold(playerTile, direction, i, onPlayerStageTile, moveStageData) != EF)
					{
						isAct = true;
					}
				}

				CreateParticle(i, moveStageData);

				break;
			}
			case BodyType::down: //下入力
			{
				if (onPlayerStageTile / initStageData[i].width >= static_cast<size_t>(initStageData[i].height - 1))
				{
					break;
				}

				moveStageTile = onPlayerStageTile + initStageData[i].width;
				moveStageData = static_cast<size_t>(initStageData[i].stageTile[moveStageTile]) - 1;

				if (moveStageTile >= static_cast<size_t>(stageData[i].width * stageData[i].height) ||
					stageData[i].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
				{
					break;
				}

				if (BodyStatus[BodyType::down] == false)
				{
					//break;
				}

				if (OpenCount >= 2 && IsOpens[BodyType::down] == true)
				{
					if (Open(playerTile, direction, i, moveStageData) != EF)
					{
						isAct = true;
					}
				}
				else if (player->leg.FootIsAction == false && player->Body_Four.IsFold == true)
				{
					if (Fold(playerTile, direction, i, onPlayerStageTile, moveStageData) != EF)
					{
						isAct = true;
					}
				}

				CreateParticle(i, moveStageData);

				break;
			}
			case BodyType::left: //左入力
			{
				if (onPlayerStageTile % initStageData[i].width <= 0)
				{
					break;
				}

				moveStageTile = onPlayerStageTile - 1;
				moveStageData = static_cast<size_t>(initStageData[i].stageTile[moveStageTile]) - 1;

				if (moveStageTile < 0 ||
					stageData[i].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
				{
					break;
				}

				if (BodyStatus[BodyType::left] == false)
				{
					//break;
				}

				if (OpenCount >= 2 && IsOpens[BodyType::left] == true)
				{
					if (Open(playerTile, direction, i, moveStageData) != EF)
					{
						isAct = true;
					}
				}
				else if (player->leg.FootIsAction == false && player->Body_One.IsFold == true)
				{
					if (Fold(playerTile, direction, i, onPlayerStageTile, moveStageData) != EF)
					{
						isAct = true;
					}
				}

				CreateParticle(i, moveStageData);

				break;
			}
			case BodyType::right: //右入力
			{
				if (onPlayerStageTile % initStageData[i].width >= static_cast<size_t>(initStageData[i].width - 1))
				{
					break;
				}

				moveStageTile = onPlayerStageTile + 1;
				moveStageData = static_cast<size_t>(initStageData[i].stageTile[moveStageTile]) - 1;

				if (moveStageTile >= static_cast<size_t>(stageData[i].width * stageData[i].height) ||
					stageData[i].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
				{
					break;
				}

				if (BodyStatus[BodyType::right] == false)
				{
					//break;
				}

				if (OpenCount >= 2 && IsOpens[BodyType::right] == true)
				{
					if (Open(playerTile, direction, i, moveStageData) != EF)
					{
						isAct = true;
					}
				}

				if (player->leg.FootIsAction == false && player->Body_Three.IsFold == true)
				{
					if (Fold(playerTile, direction, i, onPlayerStageTile, moveStageData) != EF)
					{
						isAct = true;
					}
				}

				CreateParticle(i, moveStageData);

				break;
			}
			default:
			{
				return EF;
				break;
			}
			}

			if (isAct)
			{
				break;
			}
		}

		if (isAct)
		{
			break;
		}
	}

	return 0;
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
		}
	}

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

void Stage::GetInitFoldCount(unsigned char foldCount[4])
{
	for (i = 0; i < sizeof(initFoldCount) / sizeof(initFoldCount[0]); i++)
	{
		foldCount[i] = initFoldCount[i];
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

	return false;
}

void Stage::GetPositionTile(const RVector3& center, size_t* stageNumber, size_t* stageTileNumber)
{
	*stageNumber = static_cast<size_t>(-1);
	*stageTileNumber = static_cast<size_t>(-1);
	x = static_cast<size_t>(center.x) / blockSize;
	y = static_cast<size_t>(center.y) / blockSize;

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			auto& stageTile = stageData[i].stageTileData[j];

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

void Stage::GetPositionInitTile(const RVector3& center, size_t* stageNumber, size_t* stageTileNumber)
{
	*stageNumber = static_cast<size_t>(-1);
	*stageTileNumber = static_cast<size_t>(-1);
	x = static_cast<size_t>(center.x) / blockSize;
	y = static_cast<size_t>(center.y) / blockSize;

	for (i = 0; i < initStageData.size(); i++)
	{
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

int Stage::Fold(unsigned char playerTile[4], const unsigned char& direction, const size_t& onPlayerStage, const size_t& onPlayerStageTile, const size_t& moveStageData)
{
	if (playerTile[direction] <= 0)
	{
		return EF;
	}

	if (stageData[i].stageTileData[moveStageData].isFold)
	{
		return EF;
	}

	if (stageData[i].stageTileData[moveStageData].stageEase.isMove)
	{
		stageData[i].stageTileData[moveStageData].stageEase.isMove = false;
		stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
		stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
	}

	stageData[i].stageTileData[moveStageData].stageEase.isMove = true;
	stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
	stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
	stageData[i].stageTileData[moveStageData].stageEase.addTime = 0.1f;
	stageData[i].stageTileData[moveStageData].stageEase.maxTime = 1.2f;

	if (direction == BodyType::up || direction == BodyType::down)
	{
		EaseingInit(onPlayerStage, moveStageData, direction);

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
			stageData[onPlayerStage].stageTileData[moveStageData].offsetY += static_cast<char>(stageData[onPlayerStage].stageTileData[moveStageData].height);
		}
		else
		{
			stageData[onPlayerStage].stageTileData[moveStageData].offsetY -= static_cast<char>(stageData[onPlayerStage].stageTileData[moveStageData].height);
		}
	}
	if (direction == BodyType::left || direction == BodyType::right)
	{
		EaseingInit(onPlayerStage, moveStageData, direction);

		for (y = 0; y < stageData[onPlayerStage].stageTileData[moveStageData].height; y++)
		{
			for (x = 0; x < static_cast<size_t>(stageData[onPlayerStage].stageTileData[moveStageData].width / 2); x++)
			{
				if (x >= stageData[onPlayerStage].stageTileData[moveStageData].width - x - 1)
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
	playerTile[direction]--;

	return 0;
}

int Stage::Open(unsigned char playerTile[4], const unsigned char& direction, const size_t& onPlayerStage, const size_t& moveStageData)
{
	if (playerTile[direction] > 0)
	{
		return EF;
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
	playerTile[direction]++;

	return 0;
}

int Stage::FoldDraw(const size_t& stageNumber, const size_t& stageTileNumber, const unsigned char direction,
	const int offsetX, const int offsetY)
{
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
		posY = static_cast<int>(stageData[i].stageTileData[j].height + stageData[i].stageTileData[j].offsetY);
		pos1.y = static_cast<float>(posY * blockSize);
		pos2.y = static_cast<float>(posY * blockSize - (lineWidth + 1));

		pos1.y += static_cast<float>(offsetY);
		pos2.y += static_cast<float>(offsetY);

		for (x = 0; x < stageData[i].stageTileData[j].width; x++)
		{
			posX = static_cast<int>(x + stageData[i].stageTileData[j].offsetX);

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
		posX = static_cast<int>(stageData[stageNumber].stageTileData[stageTileNumber].width + stageData[stageNumber].stageTileData[stageTileNumber].offsetX);
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

int Stage::FlameDraw(const size_t& stageNumber, const size_t& stageTileNumber, const unsigned char direction,
	const int offsetX, const int offsetY)
{
	static int posX = 0, posY = 0;
	static XMFLOAT2 pos1 = {}, pos2 = {};

	switch (direction)
	{
	case BodyType::up:
	{
		posX = 0 + stageData[stageNumber].stageTileData[stageTileNumber].offsetX;
		posY = 0 + stageData[stageNumber].stageTileData[stageTileNumber].offsetY;

		pos1.x = static_cast<float>(posX * blockSize);
		pos1.y = static_cast<float>(posY * blockSize);
		pos2.x = static_cast<float>(posX + stageData[stageNumber].stageTileData[stageTileNumber].width) * blockSize;
		pos2.y = static_cast<float>(posY * blockSize + lineWidth);

		break;
	}
	case BodyType::down:
	{
		posX = 0 + stageData[stageNumber].stageTileData[stageTileNumber].offsetX;
		posY = static_cast<int>(stageData[stageNumber].stageTileData[stageTileNumber].height) + stageData[stageNumber].stageTileData[stageTileNumber].offsetY;

		pos1.x = static_cast<float>(posX * blockSize);
		pos1.y = static_cast<float>(posY * blockSize);
		pos2.x = static_cast<float>(posX + stageData[stageNumber].stageTileData[stageTileNumber].width) * blockSize;
		pos2.y = static_cast<float>(posY * blockSize - lineWidth);

		break;
	}
	case BodyType::left:
	{
		posX = 0 + stageData[stageNumber].stageTileData[stageTileNumber].offsetX;
		posY = 0 + stageData[stageNumber].stageTileData[stageTileNumber].offsetY;

		pos1.x = static_cast<float>(posX * blockSize);
		pos1.y = static_cast<float>(posY * blockSize);
		pos2.x = static_cast<float>(posX * blockSize + lineWidth);
		pos2.y = static_cast<float>(posY + stageData[stageNumber].stageTileData[stageTileNumber].height) * blockSize;

		break;
	}
	case BodyType::right:
	{
		posX = static_cast<int>(stageData[i].stageTileData[j].width) + stageData[stageNumber].stageTileData[stageTileNumber].offsetX;
		posY = 0 + stageData[stageNumber].stageTileData[stageTileNumber].offsetY;

		pos1.x = static_cast<float>(posX * blockSize);
		pos1.y = static_cast<float>(posY * blockSize);
		pos2.x = static_cast<float>(posX * blockSize - lineWidth);
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

void Stage::EaseingInit(const size_t& onPlayerStage, const size_t& moveStageData, const int& direction)
{
	static float keepA = 0.0f, keepB = 0.0f;

	for (y = 0; y < static_cast<size_t>(stageData[onPlayerStage].stageTileData[moveStageData].height); y++)
	{
		for (x = 0; x < stageData[onPlayerStage].stageTileData[moveStageData].width; x++)
		{
			mapchipPos = y * stageData[onPlayerStage].stageTileData[moveStageData].width + x;

			switch (direction % 4)
			{
			case BodyType::up:
			{
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].x = static_cast<float>(x + stageData[onPlayerStage].stageTileData[moveStageData].offsetX);
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].y = static_cast<float>(y + stageData[onPlayerStage].stageTileData[moveStageData].offsetY);
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].z = 0.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos] *= blockSize;

				keepA = static_cast<float>(stageData[onPlayerStage].stageTileData[moveStageData].height - y - 1);
				keepB = static_cast<float>(stageData[onPlayerStage].stageTileData[moveStageData].offsetY + stageData[onPlayerStage].stageTileData[moveStageData].height);

				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].x = static_cast<float>(x + stageData[onPlayerStage].stageTileData[moveStageData].offsetX);
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].y = keepA + keepB + 1.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].z = 0.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos] *= blockSize;
				break;
			}
			case BodyType::down:
			{
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].x = static_cast<float>(x + stageData[onPlayerStage].stageTileData[moveStageData].offsetX);
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].y = static_cast<float>(y + stageData[onPlayerStage].stageTileData[moveStageData].offsetY) + 1.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].z = 0.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos] *= blockSize;

				keepA = static_cast<float>(stageData[onPlayerStage].stageTileData[moveStageData].height - y - 1);
				keepB = static_cast<float>(stageData[onPlayerStage].stageTileData[moveStageData].offsetY - stageData[onPlayerStage].stageTileData[moveStageData].height);

				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].x = static_cast<float>(x + stageData[onPlayerStage].stageTileData[moveStageData].offsetX);
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].y = keepA + keepB;
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].z = 0.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos] *= blockSize;
				break;
			}
			case BodyType::left:
			{
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].x = static_cast<float>(x + stageData[onPlayerStage].stageTileData[moveStageData].offsetX);
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].y = static_cast<float>(y + stageData[onPlayerStage].stageTileData[moveStageData].offsetY);
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].z = 0.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos] *= blockSize;

				keepA = static_cast<float>(stageData[onPlayerStage].stageTileData[moveStageData].width - x - 1);
				keepB = static_cast<float>(stageData[onPlayerStage].stageTileData[moveStageData].offsetX + stageData[onPlayerStage].stageTileData[moveStageData].width);

				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].x = keepA + keepB + 1.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].y = static_cast<float>(y + stageData[onPlayerStage].stageTileData[moveStageData].offsetY);
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].z = 0.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos] *= blockSize;

				break;
			}
			case BodyType::right:
			{
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].x = static_cast<float>(x + stageData[onPlayerStage].stageTileData[moveStageData].offsetX) + 1.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].y = static_cast<float>(y + stageData[onPlayerStage].stageTileData[moveStageData].offsetY);
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].z = 0.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos] *= blockSize;

				keepA = static_cast<float>(stageData[onPlayerStage].stageTileData[moveStageData].width - x - 1);
				keepB = static_cast<float>(stageData[onPlayerStage].stageTileData[moveStageData].offsetX - stageData[onPlayerStage].stageTileData[moveStageData].width);

				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].x = keepA + keepB;
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].y = static_cast<float>(y + stageData[onPlayerStage].stageTileData[moveStageData].offsetY);
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].z = 0.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos] *= blockSize;

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
				//x = mapchipPos % stageData[i].stageTileData[j].width;
				//y = mapchipPos / stageData[i].stageTileData[j].width;

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
