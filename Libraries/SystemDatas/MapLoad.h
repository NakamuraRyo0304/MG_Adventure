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
#include<ostream>
#include<fstream>
#include<Windows.h>

class MapLoad
{
public:
	// 横の長さ
	const static int MAP_COLUMN = 15;
	// 縦の長さ
	const static int MAP_RAW = 15;

private:
	// マップデータ格納用配列
	int m_mapData[MAP_RAW][MAP_COLUMN];
public:

	MapLoad();
	~MapLoad() = default;

	// マップの読み込み
	void LoadMap(const wchar_t* filename);

	void WriteMap(const wchar_t* filename);

	// マップのデータを渡す
	int GetMapData(int x, int y);
};

#endif // MAPLOAD