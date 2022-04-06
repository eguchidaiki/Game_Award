#pragma once
#include <stdio.h>

class LoadFile final
{
public: //シングルトン化
	static LoadFile* Get();
private:
	LoadFile() {}
	LoadFile(const LoadFile&) = delete;
	~LoadFile() {}
	LoadFile operator=(const LoadFile&) = delete;

public: //静的メンバ関数
	/// <summary>
	/// CSVファイルの読み込み
	/// </summary>
	/// <param name="mapArray"> 読み込んだCSVファイルの保存先 </param>
	/// <param name="mapSize"> マップチップ一枚あたりの大きさ </param>
	/// <param name="filePath"> CSVファイルのパス </param>
	/// <param name="loadStopNumber"> ここに指定された値がヒットすると読み込みを終了する </param>
	/// <returns> 0で成功、0以外で失敗 </returns>
	static int LoadCSV(int* mapArray, const size_t& mapSize, const char* filePath, const int& loadStopNumber = -1);
	/// <summary>
	/// CSVファイルの読み込み
	/// </summary>
	/// <param name="mapArray"> 読み込んだCSVファイルの保存先 </param>
	/// <param name="mapSize"> マップチップ一枚あたりの大きさ </param>
	/// <param name="filePath"> CSVファイルのパス </param>
	/// <param name="loadStopCharacter"> ここに指定された値がヒットすると読み込みを終了する </param>
	/// <returns> 0で成功、0以外で失敗 </returns>
	static int LoadCSV(int* mapArray, const size_t& mapSize, const char* filePath, const char& loadStopCharacter);
	/// <summary>
	/// CSVファイルの読み込み
	/// </summary>
	/// <param name="mapArray"> 読み込んだCSVファイルの保存先 </param>
	/// <param name="mapSize"> マップチップ一枚あたりの大きさ </param>
	/// <param name="fileHandle"> CSVファイルのハンドル </param>
	/// <param name="loadStopNumber"> ここに指定された値がヒットすると読み込みを終了する </param>
	/// <returns> 0で成功、-1で失敗 </returns>
	static int LoadCSV(int* mapArray, const size_t& mapSize, FILE* fileHandle, const int& loadStopNumber = -1);
	/// <summary>
	/// CSVファイルの読み込み
	/// </summary>
	/// <param name="mapArray"> 読み込んだCSVファイルの保存先 </param>
	/// <param name="mapSize"> マップチップ一枚あたりの大きさ </param>
	/// <param name="fileHandle"> CSVファイルのハンドル </param>
	/// <param name="loadStopCharacter"> ここに指定された値がヒットすると読み込みを終了する </param>
	/// <returns> 0で成功、-1で失敗 </returns>
	static int LoadCSV(int* mapArray, const size_t& mapSize, FILE* fileHandle, const char& loadStopCharacter);
	/// <summary>
	/// CSVファイルの読み込み
	/// </summary>
	/// <param name="mapArray"> 読み込んだCSVファイルの保存先 </param>
	/// <param name="mapSize"> マップチップ一枚あたりの大きさ </param>
	/// <param name="fileHandle"> CSVファイルのハンドル </param>
	/// <param name="loadStopNumber"> ここに指定された値がヒットすると読み込みを終了する </param>
	/// <returns> 0で成功、-1で失敗 </returns>
	static int LoadCSV(char* mapArray, const size_t& mapSize, FILE* fileHandle, const int& loadStopNumber);
	/// <summary>
	/// CSVファイルの読み込み
	/// </summary>
	/// <param name="mapArray"> 読み込んだCSVファイルの保存先 </param>
	/// <param name="mapSize"> マップチップ一枚あたりの大きさ </param>
	/// <param name="fileHandle"> CSVファイルのハンドル </param>
	/// <param name="loadStopCharacter"> ここに指定された値がヒットすると読み込みを終了する </param>
	/// <returns> 0で成功、-1で失敗 </returns>
	static int LoadCSV(char* mapArray, const size_t& mapSize, FILE* fileHandle, const char& loadStopCharacter);
};
