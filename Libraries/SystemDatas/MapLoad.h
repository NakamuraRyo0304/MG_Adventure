/*
 *	@File	MapLoad.h
 *	@Brief	マップを読み書き。
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef MAPLOAD
#define MAPLOAD

class MapLoad
{
public:
	// 横の長さ
	static const int MAP_COLUMN = 15;
	// 縦の長さ
	static const int MAP_RAW = 15;
	// 足場の高さ
	static const int MAP_HEIGHT = 15;

	// エリアボックスのステータス
	struct BoxState
	{
		static const int GrassBox = 100;		// 草のブロック
		static const int ClowdBox = 200;		// コインのブロック
		static const int CoinBox  = 300;		// 雲のブロック
		static const int None = 0;				// データなし
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

	// エディタモード
	bool is_editMode;

public:

	MapLoad();
	~MapLoad() = default;

	// マップの読み込み
	void LoadMap(std::wstring filename);

	// マップの書き出し
	void WriteMap();

	// マップデータの保存
	void SetMapData(int state, int x, int y);

	// エディットモード切替
	void SetEditMode(const bool& flag) { is_editMode = flag; }

	// マップのデータを渡す
	int GetMapData(int x, int y) { return m_mapData[y][x]; }

	// マップのパス名を渡す
	std::wstring GetFilePath() { return m_filename; }

private:
	// マップの保存パス指定
	bool SaveMapPath(std::wstring& filePath);

	// マップの読み込みパス指定
	bool LoadMapPath();

	// 拡張子がついていない場合に自動で拡張子を追加する関数
	std::wstring AutoAddExtension(const std::wstring& filePath, const std::wstring& extension);

};

#endif // MAPLOAD