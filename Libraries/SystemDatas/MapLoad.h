/*
 *	@File	MapLoad.h
 *	@Brief	マップを読み書き。
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef MAPLOAD
#define MAPLOAD

#include <vector>

// マップブロックの構造体
struct Object
{
	int id = 0;									// マップID
	DirectX::SimpleMath::Vector3 position;		// 座標
	bool hit = false;							// 当たっていたらTrue
	int index = 0;								// インデックス番号を格納

	Object()
		: id(0), position(DirectX::SimpleMath::Vector3(0, 0, 0)), hit(false), index(0)
	{
	}

	// 演算子のオーバーロード
	bool operator==(const Object& name) const
	{
		return id == name.id && position == name.position;
	}
};

class MapLoad
{
public:
	// 横の長さ
	static const int MAP_COLUMN = 15;
	// 縦の長さ
	static const int MAP_RAW = 15;
	// 足場の高さ
	static const int MAP_HEIGHT = 10;

	// エリアボックスのステータス
	struct BOXSTATE
	{
		static const int NONE		= 0;	// データなし
		static const int GRASS		= 1;	// 草のブロック
		static const int CLOUD		= 2;	// 雲のブロック
		static const int COIN		= 3;	// コインのブロック
		static const int GRAVITY	= 4;	// 重力ブロック
		static const int PLAYER		= 5;	// プレイヤー
		static const int LENGTH		= 6;	// ブロックステータスの長さ
	};

private:
	// マップデータ格納用配列
	std::vector<Object> m_mapData;

	// 保存先ファイルパス
	std::wstring m_filename;

	// ファイル保存用フラグ
	bool is_fileFlag;

	// ウィンドウハンドル
	HWND m_hWnd;

public:

	MapLoad();
	~MapLoad();

	// マップの読み込み
	bool LoadMap(std::wstring filename);

	// マップの書き出し
	void WriteMap(std::vector<Object> obj);

	// マップのデータを渡す
	std::vector<Object> GetMapData() { return m_mapData; }

	// マップのパス名を渡す
	std::wstring GetFilePath() { return m_filename; }

	// メモリの解放
	void ReleaseMemory();

private:
	// マップの保存パス指定
	bool SaveMapPath(std::wstring& filePath);

	// マップの読み込みパス指定
	bool LoadMapPath();

	// 拡張子がついていない場合に自動で拡張子を追加する関数
	std::wstring AutoAddExtension(const std::wstring& filePath, const std::wstring& extension);

public:
	// 新しくマップを作成する
	void CreateNewMap();

};

#endif // MAPLOAD