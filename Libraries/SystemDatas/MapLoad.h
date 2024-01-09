/*
 *	@File	MapLoad.h
 *	@Brief	マップを読み書き。
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef MAPLOAD
#define MAPLOAD

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
	static const int MAP_HEIGHT = 8;

	// エリアボックスのステータス
	struct BOXSTATE
	{
		static const int NONE		= 0;	// データなし
		static const int GRASS		= 1;	// 草のブロック
		static const int CLOUD		= 2;	// 雲のブロック
		static const int COIN		= 3;	// コインのブロック
		static const int GRAVITY	= 4;	// 重力ブロック
		static const int BIRD		= 5;	// プレイヤー
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

	/// <summary>
	/// マップの読み込み
	/// </summary>
	/// <param name="filename">ファイルパスを指定(L"...csv")拡張子は「.csv」</param>
	/// <returns>成功したらTrueを返す</returns>
	bool LoadMap(std::wstring filename);

	/// <summary>
	/// ファイルを書きだす関数
	/// </summary>
	/// <param name="obj">書き出すオブジェ配列</param>
	/// <returns>なし</returns>
	void WriteMap(std::vector<Object> obj);

	/// <summary>
	/// マップを取得
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>マップのデータ</returns>
	std::vector<Object> GetMapData() { return m_mapData; }

	/// <summary>
	/// マップのパスを取得
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>マップのCSVパス</returns>
	std::wstring GetFilePath() { return m_filename; }

	/// <summary>
	/// メモリ解放
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void ReleaseMemory();

private:
	/// <summary>
	/// パスを取得し、保存する関数
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns>正常終了でTrue</returns>
	bool SaveMapPath(std::wstring& filePath);

	/// <summary>
	/// ファイルを読み込む
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>正常終了でTrue</returns>
	bool LoadMapPath();

	/// <summary>
	/// 拡張子自動付加関数
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <param name="extension">追加したい文字(拡張子)</param>
	/// <returns>拡張子を付与した文字列</returns>
	std::wstring AutoAddExtension(const std::wstring& filePath, const std::wstring& extension);

public:

	/// <summary>
	/// マップ新規作成関数
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void CreateNewMap();

};

#endif // MAPLOAD