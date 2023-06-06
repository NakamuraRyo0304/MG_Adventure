/*
 *	@File	MapLoad.h
 *	@Brief	マップを読み込み。
 *	@Date	2023-01-20
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef MAPLOAD
#define MAPLOAD

class MapLoad
{
public:
	// 横の長さ
	const static int MAP_COLUMN = 15;
	// 縦の長さ
	const static int MAP_RAW = 15;

	// エリアボックスのステータス
	struct BoxState
	{
		const static UINT GrassBox = 101;		// 草のブロック
	};

private:
	// マップデータ格納用配列
	int m_mapData[MAP_RAW][MAP_COLUMN];

	// 保存先ファイルパス
	std::wstring m_filename;

	// ファイル保存用フラグ
	bool is_saveFileOpenFlag;

	// ウィンドウハンドル
	HWND m_hWnd;

public:

	MapLoad();
	~MapLoad() = default;

	// マップの読み込み
	void LoadMap(const wchar_t* filename);

	// マップの書き出し
	void WriteMap();

	void SetMapData(int state, int x, int y);

	// マップのデータを渡す
	int GetMapData(int x, int y) { return m_mapData[y][x]; }

private:
	// マップのパス指定
	bool SaveMapPath(std::wstring& filePath);

	// 拡張子がついていない場合に自動で拡張子を追加する関数
	std::wstring AutoAddExtension(const std::wstring& filePath, const std::wstring& extension);

};

#endif // MAPLOAD