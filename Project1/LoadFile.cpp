#include "LoadFile.h"

#define EF (-1) //Error Function

namespace
{
static size_t i = 0;

static char string[512];
static int index;
static bool isMinus;
static bool end;
static bool isSlash;
}

LoadFile* LoadFile::Get()
{
	static LoadFile instance = {};
	return &instance;
}

int LoadFile::LoadCSV(int* mapArray, const size_t& mapSize, const char* filePath, const int& loadStopNumber)
{
	for (i = 0; i < mapSize; i++)
	{
		mapArray[i] = 0;
	}

	static FILE* fileHandle;
	static errno_t err;
	for (i = 0; i < sizeof(string) / sizeof(string[0]); i++) string[i] = '\0';
	index = 0;

	err = fopen_s(&fileHandle, filePath, "r");
	if (err != 0)
	{
		return err;
	}

	isMinus = false;

	while (fgets(string, sizeof(string) / sizeof(string[0]), fileHandle) != nullptr)
	{
		end = false;
		isSlash = false;

		for (i = 0; string[i] != '\0'; i++)
		{
			if (string[i] == '/')
			{
				// コメントアウト
				if (isSlash)
				{
					break;
				}
				else
				{
					isSlash = true;
				}
			}
			else
			{
				isSlash = false;
			}
			if (string[i] == ',' || string[i] == '\n')
			{
				if (mapArray[index] == loadStopNumber)
				{
					// ヒットしたら、読み込みを強制的に終了する。
					end = true;
					break;
				}

				// 次の数字へ
				index++;
				isMinus = false;

				if (static_cast<size_t>(index) >= mapSize)
				{
					// ヒットしたら、読み込みを強制的に終了する。
					end = true;
					break;
				}
			}
			else if (string[i] == '-')
			{
				isMinus = true;
			}
			else if (string[i] >= '0' && string[i] <= '9')
			{
				mapArray[index] *= 10;

				if (isMinus == true)
				{
					mapArray[index] -= string[i] - '0';
				}
				else
				{
					mapArray[index] += string[i] - '0';
				}
			}
		}
		if (end)
		{
			break;
		}
	}
	fclose(fileHandle);

	return 0;
}

int LoadFile::LoadCSV(int* mapArray, const size_t& mapSize, const char* filePath, const char& loadStopCharacter)
{
	for (i = 0; i < mapSize; i++)
	{
		mapArray[i] = 0;
	}

	static FILE* fileHandle;
	static errno_t err;
	for (i = 0; i < sizeof(string) / sizeof(string[0]); i++) string[i] = '\0';
	index = 0;

	err = fopen_s(&fileHandle, filePath, "r");
	if (err != 0)
	{
		return err;
	}

	isMinus = false;

	while (fgets(string, sizeof(string) / sizeof(string[0]), fileHandle) != nullptr)
	{
		end = false;
		isSlash = false;

		for (i = 0; string[i] != '\0'; i++)
		{
			if (string[i] == '/')
			{
				// コメントアウト
				if (isSlash)
				{
					break;
				}
				else
				{
					isSlash = true;
				}
			}
			else
			{
				isSlash = false;
			}
			if (string[i] == ',' || string[i] == '\n')
			{
				// 次の数字へ
				index++;
				isMinus = false;
			}
			else if (string[i] == '-')
			{
				isMinus = true;
			}
			else if (string[i] >= '0' && string[i] <= '9')
			{
				mapArray[index] *= 10;

				if (isMinus == true)
				{
					mapArray[index] -= string[i] - '0';
				}
				else
				{
					mapArray[index] += string[i] - '0';
				}
			}
			else if (string[i] == loadStopCharacter)
			{
				end = true;
				break;
			}
		}
		if (end)
		{
			break;
		}
	}
	fclose(fileHandle);

	return 0;
}

int LoadFile::LoadCSV(int* mapArray, const size_t& mapSize, FILE* fileHandle, const int& loadStopNumber)
{
	if (fileHandle == nullptr)
	{
		return EF;
	}
	for (i = 0; i < mapSize; i++)
	{
		mapArray[i] = 0;
	}

	for (i = 0; i < sizeof(string) / sizeof(string[0]); i++) string[i] = '\0';
	index = 0;

	isMinus = false;

	while (fgets(string, sizeof(string) / sizeof(string[0]), fileHandle) != nullptr)
	{
		end = false;
		isSlash = false;

		for (i = 0; string[i] != '\0'; i++)
		{
			if (string[i] == '/')
			{
				// コメントアウト
				if (isSlash)
				{
					break;
				}
				else
				{
					isSlash = true;
				}
			}
			else
			{
				isSlash = false;
			}
			if (string[i] == ',' || string[i] == '\n')
			{
				if (mapArray[index] == loadStopNumber)
				{
					// ヒットしたら、読み込みを強制的に終了する。
					end = true;
					break;
				}

				// 次の数字へ
				index++;
				isMinus = false;

				if (static_cast<size_t>(index) >= mapSize)
				{
					// ヒットしたら、読み込みを強制的に終了する。
					end = true;
					break;
				}
			}
			else if (string[i] == '-')
			{
				isMinus = true;
			}
			else if (string[i] >= '0' && string[i] <= '9')
			{
				mapArray[index] *= 10;

				if (isMinus == true)
				{
					mapArray[index] -= string[i] - '0';
				}
				else
				{
					mapArray[index] += string[i] - '0';
				}
			}
		}
		if (end)
		{
			break;
		}
	}

	return 0;
}

int LoadFile::LoadCSV(int* mapArray, const size_t& mapSize, FILE* fileHandle, const char& loadStopCharacter)
{
	if (fileHandle == nullptr)
	{
		return EF;
	}
	for (i = 0; i < mapSize; i++)
	{
		mapArray[i] = 0;
	}

	for (i = 0; i < sizeof(string) / sizeof(string[0]); i++) string[i] = '\0';
	index = 0;

	isMinus = false;

	while (fgets(string, sizeof(string) / sizeof(string[0]), fileHandle) != nullptr)
	{
		end = false;
		isSlash = false;

		for (i = 0; string[i] != '\0'; i++)
		{
			if (string[i] == '/')
			{
				// コメントアウト
				if (isSlash)
				{
					break;
				}
				else
				{
					isSlash = true;
				}
			}
			else
			{
				isSlash = false;
			}
			if (string[i] == ',' || string[i] == '\n')
			{
				// 次の数字へ
				index++;
				isMinus = false;
			}
			else if (string[i] == '-')
			{
				isMinus = true;
			}
			else if (string[i] >= '0' && string[i] <= '9')
			{
				mapArray[index] *= 10;

				if (isMinus == true)
				{
					mapArray[index] -= string[i] - '0';
				}
				else
				{
					mapArray[index] += string[i] - '0';
				}
			}
			else if (string[i] == loadStopCharacter)
			{
				end = true;
				break;
			}
		}
		if (end)
		{
			break;
		}
	}

	return 0;
}

int LoadFile::LoadCSV(char* mapArray, const size_t& mapSize, FILE* fileHandle, const int& loadStopNumber)
{
	if (fileHandle == nullptr)
	{
		return EF;
	}
	for (i = 0; i < mapSize; i++)
	{
		mapArray[i] = 0;
	}

	for (i = 0; i < sizeof(string) / sizeof(string[0]); i++) string[i] = '\0';
	index = 0;

	isMinus = false;

	while (fgets(string, sizeof(string) / sizeof(string[0]), fileHandle) != nullptr)
	{
		end = false;
		isSlash = false;

		for (i = 0; string[i] != '\0'; i++)
		{
			if (string[i] == '/')
			{
				// コメントアウト
				if (isSlash)
				{
					break;
				}
				else
				{
					isSlash = true;
				}
			}
			else
			{
				isSlash = false;
			}
			if (string[i] == ',' || string[i] == '\n')
			{
				if (mapArray[index] == loadStopNumber)
				{
					// ヒットしたら、読み込みを強制的に終了する。
					end = true;
					break;
				}

				// 次の数字へ
				index++;
				isMinus = false;

				if (static_cast<size_t>(index) >= mapSize)
				{
					// ヒットしたら、読み込みを強制的に終了する。
					end = true;
					break;
				}
			}
			else if (string[i] == '-')
			{
				isMinus = true;
			}
			else if (string[i] >= '0' && string[i] <= '9')
			{
				mapArray[index] *= 10;

				if (isMinus == true)
				{
					mapArray[index] -= string[i] - '0';
				}
				else
				{
					mapArray[index] += string[i] - '0';
				}
			}
		}
		if (end)
		{
			break;
		}
	}

	return 0;
}

int LoadFile::LoadCSV(char* mapArray, const size_t& mapSize, FILE* fileHandle, const char& loadStopCharacter)
{
	if (fileHandle == nullptr)
	{
		return EF;
	}
	for (i = 0; i < mapSize; i++)
	{
		mapArray[i] = 0;
	}

	for (i = 0; i < sizeof(string) / sizeof(string[0]); i++) string[i] = '\0';
	index = 0;

	isMinus = false;

	while (fgets(string, sizeof(string) / sizeof(string[0]), fileHandle) != nullptr)
	{
		end = false;
		isSlash = false;

		for (i = 0; string[i] != '\0'; i++)
		{
			if (string[i] == '/')
			{
				// コメントアウト
				if (isSlash)
				{
					break;
				}
				else
				{
					isSlash = true;
				}
			}
			else
			{
				isSlash = false;
			}
			if (string[i] == ',' || string[i] == '\n')
			{
				// 次の数字へ
				index++;
				isMinus = false;
			}
			else if (string[i] == '-')
			{
				isMinus = true;
			}
			else if (string[i] >= '0' && string[i] <= '9')
			{
				mapArray[index] *= 10;

				if (isMinus == true)
				{
					mapArray[index] -= string[i] - '0';
				}
				else
				{
					mapArray[index] += string[i] - '0';
				}
			}
			else if (string[i] == loadStopCharacter)
			{
				end = true;
				break;
			}
		}
		if (end)
		{
			break;
		}
	}

	return 0;
}
