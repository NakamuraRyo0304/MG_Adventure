/*
 *	@File	MapLoad.cpp
 *	@Brief	マップを読み書き。
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include<iostream>    // 標準機能に使用
#include<regex>       // 文字列の置換に使用
#include<sstream>     // ファイルの保存に使用
#include<fstream>     // |
#include<shobjidl.h>  // ダイアログ操作に使用

// <shobjidl.h>の参考文献
//-------------------------------------------------------------------------------------// 
//   Microsoftアプリ開発
//
//   https://learn.microsoft.com/ja-jp/windows/win32/api/shobjidl/
// 　※ページ内リンク省略
//-------------------------------------------------------------------------------------// 

#include "MapLoad.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
MapLoad::MapLoad() :
	m_mapData{},
    m_hWnd{nullptr},
    is_saveFileOpenFlag{false},
	is_editMode{false}
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
MapLoad::~MapLoad()
{
	m_mapData.clear();
}


/// <summary>
/// マップの読み込み
/// </summary>
/// <param name="filename">ファイルパスを指定(L"...csv")拡張子は「.csv」</param>
/// <returns>なし</returns>
void MapLoad::LoadMap(std::wstring filename)
{
	m_filename = filename;

	// ファイル名が空白ならダイアログからファイル名を取得する
	if (m_filename == L"")
	{
		m_mapData.clear();
		
		if (LoadMapPath() == false)
		{
			CreateNewMap();
		}
	}

	// ファイルを開く
	std::ifstream ifs(m_filename);

	std::string line;

	// データがなくなるまで格納
	for (; std::getline(ifs, line); )
	{
		// カンマを空白に変更
		std::string tmp = std::regex_replace(line, std::regex(","), " ");

		// 空白で分割する
		std::istringstream iss(tmp);

		Object obj;

		// マップステータスID
		if (!(iss >> obj.id))
		{
			ifs.close();
			m_mapData.clear();
			return;
		}

		// 座標情報
		if (!(iss >> obj.position.x) || !(iss >> obj.position.y) || !(iss >> obj.position.z))
		{
			ifs.close();
			m_mapData.clear();
			return;
		}

		// 読み込んだデータを格納する
		m_mapData.push_back(obj);
	}

	// 開いたファイルを閉じる
	ifs.close();
}

/// <summary>
/// ファイルを書きだす関数
/// </summary>
/// <param name="obj">書き出すオブジェ配列</param>
/// <returns>なし</returns>
void MapLoad::WriteMap(std::vector<Object> obj)
{
	// ファイルパスを指定
	SaveMapPath(m_filename);

	// ファイル出力変数を定義
	std::ofstream ofs(m_filename);

	// ファイルがなければ処理しない
	if (!ofs)return;

	// ファイルを出力する
	for (const auto& o : obj)
	{
		std::ostringstream oss;
		oss << o.id << "," << o.position.x << "," << o.position.y << "," << o.position.z << ",\n";
		ofs << oss.str();
	}

	ofs.close();
}

/// <summary>
/// メモリ解放
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void MapLoad::ReleaseMemory()
{
	std::vector<Object>().swap(m_mapData);
}

/// <summary>
/// パスを取得し、保存する関数
/// </summary>
/// <param name="filePath">ファイルパス</param>
/// <returns>正常終了でTrue</returns>
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

	// フィルタの数、フィルタの配列を設定する
	hr = pFileDialog->SetFileTypes(2, fileTypes);

	// ウィンドウハンドルを指定してダイアログを表示
	hr = pFileDialog->Show(m_hWnd);

	// ウィンドウダイアログが開けた場合はパスの取得
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

/// <summary>
/// ファイルを読み込む
/// </summary>
/// <param name="引数無し"></param>
/// <returns>正常終了でTrue</returns>
bool MapLoad::LoadMapPath()
{
	// 例外エラー用変数
	HRESULT hr;

	// ファイル保存ダイアログ用のインターフェースを作成
	IFileOpenDialog* pFileDialog = nullptr;
	hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));

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
				m_filename = tmpFilePath;

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

				m_filename = AutoAddExtension(m_filename, extension);

				// 正常終了
				return true;
			}
			pShell->Release();
		}
		else
		{
			CreateNewMap();
		}
	}

	// pFileDialogの解放
	pFileDialog->Release();

	// ダイアログが閉じられた場合やエラーが発生した場合はデータを削除
	m_filename.clear();

	// 一番最後に現在のファイルパスを格納
	m_filename = m_filename;

	// 異常終了
	return false;
}

/// <summary>
/// 拡張子自動付加関数
/// </summary>
/// <param name="filePath">ファイルパス</param>
/// <param name="extension">追加したい文字(拡張子)</param>
/// <returns>拡張子を付与した文字列</returns>
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

/// <summary>
/// マップ新規作成(更地)
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void MapLoad::CreateNewMap()
{
	std::vector<Object> obj;

	// 最大マスのサイズにする
	const int initialNumber = MAP_COLUMN;

	for (int x = 0; x < initialNumber; ++x)
	{
		for (int y = 0; y < initialNumber; ++y)
		{
			for (int z = 0; z < initialNumber; ++z)
			{
				Object newObj;

				if (y == 0)
				{
					newObj.id = BoxState::GrassBox;
				}
				else
				{
					newObj.id = BoxState::None;
				}
				newObj.position.x = static_cast<float>(x);
				newObj.position.y = static_cast<float>(y);
				newObj.position.z = static_cast<float>(z);
				
				obj.push_back(newObj);
			}
		}
	}

	// 作ったデータを格納
	m_mapData = obj;

	obj.clear();
}
