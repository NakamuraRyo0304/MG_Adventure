/*
 *	@File	MapLoad.cpp
 *	@Brief	マップを読み書き。
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include<iostream>    // string,cout,cinに使用
#include<regex>       // 文字列の置換に使用
#include<sstream>     // ファイルの保存に使用
#include<ostream>     // ファイルの保存に使用
#include<fstream>     // ファイルの保存に使用
#include<Windows.h>   // WindowsAPIに使用
#include<shobjidl.h>  // IFileSaveDialogに使用


#include "MapLoad.h"

//--------------------------------------------------------//
//コンストラクタ                                          //
//--------------------------------------------------------//
MapLoad::MapLoad() :
	m_mapData{},
    is_saveFileOpenFlag{false},
    m_hWnd{nullptr}
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
//ファイルを書きだす関数                                  //
//--------------------------------------------------------//
void MapLoad::WriteMap()
{
	// ファイルパスを指定
	SaveMapPath(m_filename);

	//　ファイルを開く
	std::ofstream ofs(m_filename);

	//　１行書きだしたら次の列へ
	for (int y = 0; y < MAP_RAW; y++)
	{
		for (int x = 0; x < MAP_COLUMN; x++)
		{
			ofs << m_mapData[y][x] << ",";
		}
		ofs << std::endl;
	}

	//　ファイルを閉じる
	ofs.close();
}

//--------------------------------------------------------//
//マップデータの格納                                      //
//--------------------------------------------------------//
// 第１引数：マップの値 第２、３引数：配列番号
void MapLoad::SetMapData(int state, int x, int y)
{
	m_mapData[y][x] = state;
}

//--------------------------------------------------------//
//ファイルパスをセーブする関数                            //
//--------------------------------------------------------//
// private
bool MapLoad::SaveMapPath(std::wstring& filePath)
{
	// 例外エラー用変数
	HRESULT hr;

	// ファイル保存ダイアログ用のインターフェースを作成
	IFileSaveDialog* pFileDialog = nullptr;
	hr = CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));

	// インスタンスの作成に失敗した場合はfalseを返して終了
	if (FAILED(hr))
	{
		return false; 
	}

	// 拡張子フィルタを設定する
	COMDLG_FILTERSPEC fileTypes[] = 
	{
		{ L"CSV(カンマ区切り)"	, L"*.csv" },			// カンマ区切りファイル
		{ L"テキスト ファイル"	, L"*.txt" }			// テキストファイル
	};

	// 引数：フィルタの数、フィルタの配列
	hr = pFileDialog->SetFileTypes(2, fileTypes);

	// ウィンドウハンドルを指定してダイアログを表示
	hr = pFileDialog->Show(m_hWnd); // m_hWndはウィンドウハンドル

	// ウィンドウダイアログが開けた場合はパスの取得処理へ
	if (SUCCEEDED(hr))
	{
		// 選択されたファイルパスを取得
		IShellItem* pShell;
		hr = pFileDialog->GetResult(&pShell);

		// パスが取得出来たら以下処理を実行
		if (SUCCEEDED(hr))
		{
			wchar_t* tmpFilePath;
			hr = pShell->GetDisplayName(SIGDN_FILESYSPATH, &tmpFilePath);

			if (SUCCEEDED(hr))
			{
				// 選択されたファイルパスをfilePathに格納
				filePath = tmpFilePath;

				// メモリの解放
				CoTaskMemFree(tmpFilePath);

				// pItemの解放
				pShell->Release(); 

				// pFileDialogの解放
				pFileDialog->Release(); 

				// 選択されたフィルタのインデックスを取得
				unsigned int filterIndex;
				hr = pFileDialog->GetFileTypeIndex(&filterIndex);

				// 一番最後に現在のファイルパスを格納
				std::wstring extension = L".csv";					// デフォルトはCSV

				// 条件によって変更する
				if (filterIndex == 1) extension = L".csv";			// CSVカンマ区切り
				if (filterIndex == 2) extension = L".txt";			// txtファイル

				filePath = AutoAddExtension(filePath, extension);

				m_filename = filePath;

				// 正常終了
				return true;
			}
			pShell->Release();
		}
	}

	// pFileDialogの解放
	pFileDialog->Release();

	// ダイアログが閉じられた場合やエラーが発生した場合はデータを削除
	filePath.clear();

	// 一番最後に現在のファイルパスを格納
	m_filename = filePath;

	// 異常終了
	return false;
}

//--------------------------------------------------------//
//拡張子がないときのみ処理する                            //
//--------------------------------------------------------//
std::wstring MapLoad::AutoAddExtension(const std::wstring& filePath, const std::wstring& extension)
{
	// 元ファイル名を格納する
	std::wstring result = filePath;
	
	// 文字列"."を見つける
	std::wstring::size_type extPos = filePath.rfind(L".");
	
	// ドットが見つかった位置が対象文字列の最後尾or何もついていない場合追加する
	if (extPos == std::wstring::npos || extPos < filePath.rfind(L"\\")) 
	{
		result += extension;
	}
	return result;
}
