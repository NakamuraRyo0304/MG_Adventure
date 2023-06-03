/*
 *	@File	MapLoad.h
 *	@Brief	マップを読み込み。
 *	@Date	2023-01-20
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef MAPLOAD
#define MAPLOAD

#include<iostream>
#include<sstream>
#include<regex>
#include<fstream>
#include<Windows.h>

class MapLoad
{
public:
	// 横の長さ
	const static int MAP_COLUMN = 15;
	// 縦の長さ
	const static int MAP_RAW = 15;

	// 読み込むファイルの名前を指定
	const wchar_t* m_fileName;

private:
	// マップデータ格納用配列
	int m_mapData[MAP_RAW][MAP_COLUMN];

private:
	// マップの読み込み(セットしたらMapLoad内で処理される関数)
	void LoadMap();

public:

	MapLoad();
	~MapLoad() = default;
	// マップのデータを渡す
	int GetMapData(int x, int y);

	// ファイル名セッター
	void SetMapData(const wchar_t* filename);
};

#endif // MAPLOAD