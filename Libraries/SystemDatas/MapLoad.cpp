/*
 *	@File	MapLoad.cpp
 *	@Brief	マップを読み込み。
 *	@Date	2023-01-20
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "MapLoad.h"

//--------------------------------------------------------//
//マップの読み込み                                        //
//--------------------------------------------------------//
void MapLoad::LoadMap()
{
	// ファイル読み込み変数定義
	std::ifstream ifs;

	// ファイルを読み取り専用で開く
	ifs.open(m_fileName, std::ios::in | std::ios::binary);

	// ファイルが開けなかった場合はエラーメッセージを出す
	if (!ifs)
	{
		MessageBoxA(0, "ファイルを開けませんでした", NULL, MB_OK);
		return;			// 早期リターン
	}

	// 一行分（横方向）のデータを列数分（縦方向）入れる配列
	std::string s[MAP_RAW];

	for (int y = 0; y < MAP_RAW; y++)
	{
		// 一行分のデータを読み込む
		ifs >> s[y];

		// カンマを空白に変更
		std::string tmp = std::regex_replace(s[y], std::regex(","), " ");

		// 空白で分割する
		std::istringstream iss(tmp);
		for (int x = 0; x < MAP_COLUMN; x++)
		{
			// null(-1)を入れる
			int num = -1;

			// 空白までのデータ（一つ分のデータ）を読み込む
			iss >> num;

			// 読み込んだデータを格納する
			m_mapData[y][x] = num;
		}
	}
	// 開いたファイルを閉じる
	ifs.close();
}

//--------------------------------------------------------//
//コンストラクタ                                          //
//--------------------------------------------------------//
MapLoad::MapLoad():
	m_mapData{}
{
}

//--------------------------------------------------------//
//マップゲッター                                          //
//--------------------------------------------------------//
int MapLoad::GetMapData(int x,int y)
{
	return m_mapData[y][x];
}

//--------------------------------------------------------//
//マップをセット                                          //
//--------------------------------------------------------//
void MapLoad::SetMapData(const char* filename)
{
	// 名前を設定して読み込む
	this->m_fileName = filename;
	LoadMap();
}