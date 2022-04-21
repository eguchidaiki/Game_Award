#include "Stage.h"
#include "LoadFile.h"
#include "General.h"
#include <Raki_Input.h>
#include "PlayerBody.h"
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
}

const int Stage::blockSize = 60;
const int Stage::halfBlockSize = Stage::blockSize / 2;
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
	initStageData{}
{
	Init();
}

Stage::~Stage()
{
	DataClear();
}

void Stage::Init()
{
	BlockHandle = TexManager::LoadTexture("Resources/block.png");
	EnptyHandle = TexManager::LoadTexture("Resources/stage_enpty.png");
	GoalHandle = TexManager::LoadTexture("Resources/goal.png");

	MapchipSpriteBlock.Create(BlockHandle);
	MapchipSpriteEnpty.Create(EnptyHandle);
	MapchipSpriteGoal.Create(GoalHandle);

	
	
	this->Particlemanager->Prototype_Set(FoldParticle);
}

void Stage::Updata()
{
	static int posX = 0;
	static int posY = 0;

	EaseingUpdate();

	Particlemanager->Prototype_Update();

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
						static size_t reverseMapchipPos = 0;
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

void Stage::Draw(int offsetX, int offsetY)
{
	static int posX = 0;
	static int posY = 0;

	static RVector3 pos1, pos2;

	//SetHierarchyAndColumn();

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			for (y = 0; y < stageData[i].stageTileData[j].height; y++)
			{
				for (x = 0; x < stageData[i].stageTileData[j].width; x++)
				{
					mapchipPos = y * stageData[i].stageTileData[j].width + x;

					pos1.x = stageData[i].stageTileData[j].drawLeftUp[mapchipPos].x + static_cast<float>(offsetX);
					pos1.y = stageData[i].stageTileData[j].drawLeftUp[mapchipPos].y + static_cast<float>(offsetY);
					pos1.z = stageData[i].stageTileData[j].drawLeftUp[mapchipPos].z;
					pos2.x = stageData[i].stageTileData[j].drawRightDown[mapchipPos].x + static_cast<float>(offsetX);
					pos2.y = stageData[i].stageTileData[j].drawRightDown[mapchipPos].y + static_cast<float>(offsetY);
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
						MapchipSpriteBlock.DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
						break;
					}
					case MapchipData::GOAL:
					{
						MapchipSpriteGoal.DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
						break;
					}
					case MapchipData::NONE:
					case MapchipData::START:
					default:
					{
						MapchipSpriteEnpty.DrawExtendSprite(pos1.x, pos1.y, pos2.x, pos2.y);
						break;
					}
					}

					if (j == 2)
					{
						int test = 0;
					}
				}
			}
		}

		// つなぎ目
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			static char sideStageTile = 0;
			static char sideStageData = 0;

			if ((stageData[i].stageTileData[j].stageNumber % stageData[i].width) - 1 >= 0)
			{
				sideStageTile = stageData[i].stageTileData[j].stageNumber - 1;

				if (stageData[i].stageTile[sideStageTile] != MapchipData::EMPTY_STAGE)
				{
					for (y = 0; y < stageData[i].stageTileData[j].height; y++)
					{
						posX = static_cast<int>(0 + stageData[i].stageTileData[j].offsetX);
						posY = static_cast<int>(y + stageData[i].stageTileData[j].offsetY);

						pos1.x = static_cast<float>(posX * blockSize + offsetX);
						pos2.x = static_cast<float>(posX * blockSize + blockSize * 1 / 4 + offsetX);
						pos1.y = static_cast<float>(posY * blockSize + blockSize * 1 / 4 + offsetY);
						pos2.y = static_cast<float>(posY * blockSize + blockSize * 3 / 4 + offsetY);

						//DrawShape::DrawPlane(pos1, pos2, BLACK);
					}
				}
			}
			if ((stageData[i].stageTileData[j].stageNumber % stageData[i].width) + 1 < stageData[i].width)
			{
				sideStageTile = stageData[i].stageTileData[j].stageNumber + 1;

				if (stageData[i].stageTile[sideStageTile] != MapchipData::EMPTY_STAGE)
				{
					for (y = 0; y < stageData[i].stageTileData[j].height; y++)
					{
						posX = static_cast<int>((stageData[i].stageTileData[j].width - 1) + stageData[i].stageTileData[j].offsetX);
						posY = static_cast<int>(y + stageData[i].stageTileData[j].offsetY);

						pos1.x = static_cast<float>(posX * blockSize + blockSize * 3 / 4 + offsetX);
						pos2.x = static_cast<float>((posX + 1) * blockSize + offsetX);
						pos1.y = static_cast<float>(posY * blockSize + blockSize * 1 / 4 + offsetY);
						pos2.y = static_cast<float>(posY * blockSize + blockSize * 3 / 4 + offsetY);

						//DrawShape::DrawPlane(pos1, pos2, BLACK);
					}
				}
			}
			if (static_cast<int>(stageData[i].stageTileData[j].stageNumber / stageData[i].width) - 1 >= 0)
			{
				sideStageTile = stageData[i].stageTileData[j].stageNumber - static_cast<char>(stageData[i].width);
				sideStageData = stageData[i].stageTile[sideStageTile];

				if (sideStageData != MapchipData::EMPTY_STAGE)
				{
					for (x = 0; x < stageData[i].stageTileData[sideStageData].width; x++)
					{
						posX = static_cast<int>(x + stageData[i].stageTileData[sideStageData].offsetX);
						posY = static_cast<int>(0 + stageData[i].stageTileData[sideStageData].offsetY);

						pos1.x = static_cast<float>(posX * blockSize + blockSize * 1 / 4 + offsetX);
						pos2.x = static_cast<float>(posX * blockSize + blockSize * 3 / 4 + offsetX);
						pos1.y = static_cast<float>(posY * blockSize + offsetY);
						pos2.y = static_cast<float>(posY * blockSize + blockSize * 1 / 4 + offsetY);

						//DrawShape::DrawPlane(pos1, pos2, BLACK);
					}
				}
			}
			if ((stageData[i].stageTileData[j].stageNumber / stageData[i].width) + 1 < stageData[i].height)
			{
				sideStageTile = stageData[i].stageTileData[j].stageNumber + static_cast<char>(stageData[i].width);

				if (stageData[i].stageTile[sideStageTile] != MapchipData::EMPTY_STAGE)
				{
					for (x = 0; x < stageData[i].stageTileData[j].width; x++)
					{
						posX = static_cast<int>(x + stageData[i].stageTileData[j].offsetX);
						posY = static_cast<int>((stageData[i].stageTileData[j].height - 1) + stageData[i].stageTileData[j].offsetY);

						pos1.x = static_cast<float>(posX * blockSize + blockSize * 1 / 4 + offsetX);
						pos2.x = static_cast<float>(posX * blockSize + blockSize * 3 / 4 + offsetX);
						pos1.y = static_cast<float>(posY * blockSize + blockSize * 3 / 4 + offsetY);
						pos2.y = static_cast<float>((posY + 1) * blockSize + offsetY);

						//DrawShape::DrawPlane(pos1, pos2, BLACK);
					}
				}
			}
		}
	}

	MapchipSpriteBlock.Draw();
	MapchipSpriteGoal.Draw();
	MapchipSpriteEnpty.Draw();

	Raki_DX12B::Get()->ClearDepthBuffer();

	Particlemanager->Prototype_Draw(BlockHandle);
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

int Stage::FoldAndOpen(const RVector3& playerPos, unsigned char playerTile[4], PlayerBody BodyStatus[4], bool IsFootAction, bool IsFolds[4], int OpenCount, bool IsOpens[4])
{
	unsigned char direction = -1;
	static size_t onPlayerStageTile = 0;
	static size_t moveStageTile = 0;
	static size_t moveStageData = 0;

	if (IsFolds[0] || IsOpens[0])
	{
		direction = BodyType::up;
	}
	else if (IsFolds[1] || IsOpens[1])
	{
		direction = BodyType::down;
	}
	else if (IsFolds[2] || IsOpens[2])
	{
		direction = BodyType::left;
	}
	else if (IsFolds[3] || IsOpens[3])
	{
		direction = BodyType::right;
	}

	static size_t reverseMapchipPos = 0;
	static bool isAct = false;

	isAct = false;

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			if ((playerPos.x / blockSize >= stageData[i].stageTileData[j].offsetX &&
				playerPos.x / blockSize < stageData[i].stageTileData[j].offsetX + stageData[i].stageTileData[j].width) &&
				(playerPos.y / blockSize >= stageData[i].stageTileData[j].offsetY &&
					playerPos.y / blockSize < stageData[i].stageTileData[j].offsetY + stageData[i].stageTileData[j].height))
			{
				onPlayerStageTile = stageData[i].stageTileData[j].stageNumber;
			}
			else
			{
				continue;
			}

			switch (direction)
			{
			case BodyType::up: //上入力
			{
				if (onPlayerStageTile / stageData[i].width <= 0)
				{
					break;
				}

				moveStageTile = onPlayerStageTile - stageData[i].width;
				moveStageData = static_cast<size_t>(stageData[i].stageTile[moveStageTile]) - 1;

				if (moveStageTile < 0 ||
					stageData[i].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
				{
					break;
				}

				if (stageData[i].stageTileData[moveStageData].stageEase.isMove)
				{
					stageData[i].stageTileData[moveStageData].stageEase.isMove = false;
					stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
					stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
				}

				if (stageData[i].stageTileData[moveStageData].isFold)
				{
					if (BodyStatus[0].IsActivate == true && OpenCount == 2 && IsOpens[0] == true)
					{
						Open(playerTile, direction, i, moveStageTile, moveStageData);

						stageData[i].stageTileData[moveStageData].stageEase.isMove = true;
						stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
						stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
						stageData[i].stageTileData[moveStageData].stageEase.addTime = 0.1f;
						stageData[i].stageTileData[moveStageData].stageEase.maxTime = 1.2f;
					}
				}
				else
				{
					if (BodyStatus[0].IsActivate == true && IsFootAction == false && IsFolds[0] == true)
					{
						Fold(playerTile, direction, i, onPlayerStageTile, moveStageData);

						stageData[i].stageTileData[moveStageData].stageEase.isMove = true;
						stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
						stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
						stageData[i].stageTileData[moveStageData].stageEase.addTime = 0.1f;
						stageData[i].stageTileData[moveStageData].stageEase.maxTime = 1.2f;
					}
				}

				isAct = true;

				break;
			}
			case BodyType::down: //下入力
			{
				if (onPlayerStageTile / stageData[i].width >= static_cast<size_t>(stageData[i].height - 1))
				{
					break;
				}

				moveStageTile = onPlayerStageTile + stageData[i].width;
				moveStageData = static_cast<size_t>(stageData[i].stageTile[moveStageTile]) - 1;

				if (moveStageTile >= static_cast<size_t>(stageData[i].width * stageData[i].height) ||
					stageData[i].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
				{
					break;
				}

				if (stageData[i].stageTileData[moveStageData].stageEase.isMove)
				{
					stageData[i].stageTileData[moveStageData].stageEase.isMove = false;
					stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
					stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
				}

				if (stageData[i].stageTileData[moveStageData].isFold)
				{
					if (BodyStatus[1].IsActivate == true && OpenCount == 2 && IsOpens[1] == true)
					{
						Open(playerTile, direction, i, moveStageTile, moveStageData);

						stageData[i].stageTileData[moveStageData].stageEase.isMove = true;
						stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
						stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
						stageData[i].stageTileData[moveStageData].stageEase.addTime = 0.1f;
						stageData[i].stageTileData[moveStageData].stageEase.maxTime = 1.2f;
					}
				}
				else
				{
					if (BodyStatus[1].IsActivate == true && IsFootAction == false && IsFolds[1] == true)
					{
						Fold(playerTile, direction, i, onPlayerStageTile, moveStageData);

						stageData[i].stageTileData[moveStageData].stageEase.isMove = true;
						stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
						stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
						stageData[i].stageTileData[moveStageData].stageEase.addTime = 0.1f;
						stageData[i].stageTileData[moveStageData].stageEase.maxTime = 1.2f;
					}
				}

				isAct = true;

				break;
			}
			case BodyType::left: //左入力
			{
				if (onPlayerStageTile % stageData[i].width <= 0)
				{
					break;
				}

				moveStageTile = onPlayerStageTile - 1;
				moveStageData = static_cast<size_t>(stageData[i].stageTile[moveStageTile]) - 1;

				if (moveStageTile < 0 ||
					stageData[i].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
				{
					break;
				}

				if (stageData[i].stageTileData[moveStageData].stageEase.isMove)
				{
					stageData[i].stageTileData[moveStageData].stageEase.isMove = false;
					stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
					stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
				}

				if (stageData[i].stageTileData[moveStageData].isFold)
				{
					if (BodyStatus[2].IsActivate == true && OpenCount == 2 && IsOpens[2] == true)
					{
						Open(playerTile, direction, i, onPlayerStageTile, moveStageData);

						stageData[i].stageTileData[moveStageData].stageEase.isMove = true;
						stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0Ui64;
						stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
						stageData[i].stageTileData[moveStageData].stageEase.addTime = 0.1f;
						stageData[i].stageTileData[moveStageData].stageEase.maxTime = 1.2f;
					}
				}
				else
				{
					if (BodyStatus[2].IsActivate == true && IsFootAction == false && IsFolds[2] == true)
					{
						Fold(playerTile, direction, i, onPlayerStageTile, moveStageData);

						stageData[i].stageTileData[moveStageData].stageEase.isMove = true;
						stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
						stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
						stageData[i].stageTileData[moveStageData].stageEase.addTime = 0.1f;
						stageData[i].stageTileData[moveStageData].stageEase.maxTime = 1.2f;
					}
				}

				isAct = true;

				break;
			}
			case BodyType::right: //右入力
			{
				if (onPlayerStageTile % stageData[i].width >= static_cast<size_t>(stageData[i].width - 1))
				{
					break;
				}

				moveStageTile = onPlayerStageTile + 1;
				moveStageData = static_cast<size_t>(stageData[i].stageTile[moveStageTile]) - 1;

				if (moveStageTile >= static_cast<size_t>(stageData[i].width * stageData[i].height) ||
					stageData[i].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
				{
					break;
				}

				if (stageData[i].stageTileData[moveStageData].stageEase.isMove)
				{
					stageData[i].stageTileData[moveStageData].stageEase.isMove = false;
					stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
					stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
				}

				if (stageData[i].stageTileData[moveStageData].isFold)
				{
					if (BodyStatus[3].IsActivate == true && OpenCount == 2 && IsOpens[3] == true)
					{
						Open(playerTile, direction, i, moveStageTile, moveStageData);

						stageData[i].stageTileData[moveStageData].stageEase.isMove = true;
						stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
						stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
						stageData[i].stageTileData[moveStageData].stageEase.addTime = 0.1f;
						stageData[i].stageTileData[moveStageData].stageEase.maxTime = 1.2f;
					}
				}
				else
				{
					if (BodyStatus[3].IsActivate == true && IsFootAction == false && IsFolds[3] == true)
					{
						Fold(playerTile, direction, i, onPlayerStageTile, moveStageData);

						stageData[i].stageTileData[moveStageData].stageEase.isMove = true;
						stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
						stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
						stageData[i].stageTileData[moveStageData].stageEase.addTime = 0.1f;
						stageData[i].stageTileData[moveStageData].stageEase.maxTime = 1.2f;
					}
				}

				isAct = true;

				break;
			}
			default:
			{
				return EF;
				break;
			}
			}

			CreateParticle(i, moveStageData);

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

void Stage::Reset()
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

bool Stage::GetPositionTile(RVector3 center, int i, int j)
{
	float left = (float)stageData[i].stageTileData[j].offsetX * blockSize;
	float up = (float)stageData[i].stageTileData[j].offsetY * blockSize;
	float right = left + blockSize * (float)stageData[i].stageTileData[j].width;
	float down = up + blockSize * (float)stageData[i].stageTileData[j].height;

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
	for (int a = 0; a < 50; a++)
	{
		//位置設定
		float xpos = NY_random::floatrand_sl(
			static_cast<float>(stageData[StageDataNum].stageTileData[StageTileDataNum].offsetX + stageData[StageDataNum].stageTileData[StageTileDataNum].width) * blockSize,
			static_cast<float>(stageData[StageDataNum].stageTileData[StageTileDataNum].offsetX * blockSize));
		float ypos = NY_random::floatrand_sl(
			static_cast<float>(stageData[StageDataNum].stageTileData[StageTileDataNum].offsetY + stageData[StageDataNum].stageTileData[StageTileDataNum].height) * blockSize,
			static_cast<float>(stageData[StageDataNum].stageTileData[StageTileDataNum].offsetY * blockSize));

		RVector3 world_startpos= RV3Colider::CalcScreen2World({ xpos,ypos }, 0.0f);
		this->Particlemanager->Prototype_Add(1, { world_startpos.x,world_startpos.y,0.0f });
	}
}

int Stage::Fold(unsigned char playerTile[4], const unsigned char& direction, const size_t& onPlayerStage, const size_t& onPlayerStageTile, const size_t& moveStageData)
{
	if (playerTile[direction] <= 0)
	{
		return EF;
	}

	static size_t reverseMapchipPos = 0;

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

int Stage::Open(unsigned char playerTile[4], const unsigned char& direction, const size_t& onPlayerStage, const size_t& moveStageTile, const size_t& moveStageData)
{
	if (playerTile[direction] > 0)
	{
		return EF;
	}

	static size_t reverseMapchipPos = 0;

	if (direction == BodyType::up || direction == BodyType::down)
	{
		EaseingInit(onPlayerStage, moveStageData, direction + 2);

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
		EaseingInit(onPlayerStage, moveStageData, direction + 2);

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

	stageData[onPlayerStage].stageTileData[moveStageData].stageNumber = static_cast<char>(moveStageTile);
	stageData[onPlayerStage].stageTileData[moveStageData].direction = direction + 1 + 2;
	stageData[onPlayerStage].stageTileData[moveStageData].isFold = false;
	playerTile[direction]++;

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
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].y = static_cast<float>(y + stageData[i].stageTileData[j].offsetY);
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
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].y = static_cast<float>(y + stageData[i].stageTileData[j].offsetY);
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

			for (y = 0; y < stageData[i].stageTileData[j].height; y++)
			{
				for (x = 0; x < stageData[i].stageTileData[j].width; x++)
				{
					static RVector3 axisPos = {};

					mapchipPos = y * stageData[i].stageTileData[j].width + x;

					switch ((stageData[i].stageTileData[j].direction - 1) % 4)
					{
					case BodyType::up:
						axisPos = { static_cast<float>(stageData[i].stageTileData[j].startPos[mapchipPos].x),
							static_cast<float>(stageData[i].stageTileData[j].startPos[0].y + (stageData[i].stageTileData[j].offsetY * blockSize)),
							static_cast<float>(stageData[i].stageTileData[j].height * blockSize) };
						break;
					case BodyType::down:
						axisPos = { static_cast<float>(stageData[i].stageTileData[j].startPos[mapchipPos].x),
							static_cast<float>(stageData[i].stageTileData[j].startPos[0].y - blockSize),
							static_cast<float>(stageData[i].stageTileData[j].height * blockSize) };
						break;
					case BodyType::left:
						axisPos = { static_cast<float>(stageData[i].stageTileData[j].startPos[0].x + (stageData[i].stageTileData[j].offsetX * blockSize)),
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
	endFrame = 60;

	//速度設定
	float xvel = NY_random::floatrand_sl(3.0f, -3.0f);
	float yvel = NY_random::floatrand_sl(3.0f, -3.0f);

	vel = RVector3(0, 1, 0);

	acc = RVector3(0, 0.1f, 0);

	scale = 5.0f;
}

void ParticleSingle::Update()
{
	//毎フレーム加算
	vel += acc;
	pos += vel;
}

ParticlePrototype* ParticleSingle::clone(RVector3 start)
{
	return new ParticleSingle();
}
