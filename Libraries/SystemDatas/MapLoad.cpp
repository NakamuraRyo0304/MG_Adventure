/*
 *	@File	MapLoad.cpp
 *	@Brief	マップを読み込み。
 *	@Date	2023-01-20
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "MapLoad.h"

//--------------------------------------------------------//
//コンストラクタ                                          //
//--------------------------------------------------------//
MapLoad::MapLoad() :
	m_mapData{}
{
}


 //--------------------------------------------------------//
 //マップの読み込み                                        //
 //--------------------------------------------------------//
void MapLoad::LoadMap(const wchar_t* filename)
{
	// ファイル読み込み変数定義
	std::ifstream ifs;

	// ファイルを読み取り専用で開く
	ifs.open(filename, std::ios::in | std::ios::binary);

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
//ファイルの書き出し                                      //
//--------------------------------------------------------//
// ファイルの名前
void MapLoad::WriteMap(const wchar_t* filename)
{
	//OPENFILENAME ofn;							//　FILENAME構造体の定義
	//char path[MAX_PATH] = "";					//　ファイルパスを初期化
	//ZeroMemory(&ofn, sizeof(ofn));				//　構造体のパスを空にする
	//ofn.lStructSize = sizeof(OPENFILENAME);		//　構造体のサイズを指定
	//ofn.lpstrFilter = "csv(*.csv)\0*.csv\0"		//　拡張子のフィルターをかける \nは文の終わりを表す
	//	"json(*.json)\0*.json\0"
	//	"すべてのファイル(*.*)\0*.*\0\0";
	//ofn.lpstrFile = path;						//　ファイルパスを格納するポインタ
	//ofn.nMaxFile = MAX_PATH;					//　lpstrFileのバッファサイズ
	//ofn.lpstrDefExt = ".csv";					//　拡張子が指定されていなければ".csv"をつける
	//int result = 0;

	//GetSaveFileName(&ofn);

	////　ファイルを開く
	//ofstream ofs(path);


	//for (int y = 0; y < MAP_RAW; y++)
	//{
	//	// 一行分のデータを読み込む
	//	std::ofs >> s[y];

	//	// カンマを空白に変更
	//	std::string tmp = std::regex_replace(s[y], std::regex(","), " ");

	//	// 空白で分割する
	//	std::istringstream iss(tmp);
	//	for (int x = 0; x < MAP_COLUMN; x++)
	//	{
	//		ofs << m_MapBlock[y][x] << ",";
	//	}
	//	ofs << endl;
	//}

	////　ファイルを閉じる
	//ofs.close();
}

//--------------------------------------------------------//
//マップゲッター                                          //
//--------------------------------------------------------//
int MapLoad::GetMapData(int x, int y)
{
	return m_mapData[y][x];
}