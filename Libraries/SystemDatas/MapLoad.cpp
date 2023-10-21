/*
 *	@File	MapLoad.cpp
 *	@Brief	マップを読み書き。
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include<regex>       // 文字列の置換に使用
#include<sstream>     // ファイルの保存に使用
#include<fstream>     // ファイルの保存に使用
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
MapLoad::MapLoad()
	: m_mapData{}			// マップデータ
	, m_hWnd{ nullptr }		// ウィンドウハンドル
	, is_fileFlag{ false }	// ファイルを開くフラグ
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
/// <returns>成功したか</returns>
bool MapLoad::LoadMap(std::wstring filename)
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
	std::ifstream _ifs(m_filename);

	if (!_ifs) return false;

	std::string _line;

	// データがなくなるまで格納
	for (; std::getline(_ifs, _line); )
	{
		// カンマを空白に変更
		std::string _tmpDatas = std::regex_replace(_line, std::regex(","), " ");

		// 空白で分割する
		std::istringstream _iss(_tmpDatas);

		// ヘッダーの場合スキップ
		if (_line.find("マップＩＤ") != std::string::npos ||
			_line.find("0:なし") != std::string::npos)
		{
			continue;
		}

		Object _obj;

		// マップステータスID
		if (!(_iss >> _obj.id))
		{
			_ifs.close();
			m_mapData.clear();
		}

		// 座標情報
		if (_obj.id == BOXSTATE::NONE)
		{
			_obj.position = SimpleMath::Vector3::Zero;
		}
		else if (!(_iss >> _obj.position.x) || !(_iss >> _obj.position.y) || !(_iss >> _obj.position.z))
		{
			_ifs.close();
			m_mapData.clear();
		}

		// 読み込んだデータを格納する
		m_mapData.push_back(_obj);
	}

	// 開いたファイルを閉じる
	_ifs.close();

	return true;
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
	std::ofstream _ofs(m_filename);

	// ファイルがなければ処理しない
	if (!_ofs) return;

	// ヘッダー
	_ofs << "マップＩＤ" << ",Ｘ" << ",Ｙ" << ",Ｚ" << ",\n";
	_ofs << "0:なし,1:草,2:雲,3:コイン,4:重力,5:プレイヤー,\n";

	for (auto& _obj : obj)
	{
		std::ostringstream _oss;

		// ID,X,Y,X,改行
		if (_obj.id == BOXSTATE::NONE)
		{
			_oss << BOXSTATE::NONE << ",\n";
		}
		else
		{
			_oss << _obj.id << "," << _obj.position.x << "," << _obj.position.y << "," << _obj.position.z << ",\n";
		}
		// １ブロックの情報を出力
		_ofs << _oss.str();
	}

	_ofs.close();
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
	HRESULT _hr;

	// ファイル保存ダイアログ用のインターフェースを作成
	IFileSaveDialog* _dialog = nullptr;
	_hr = CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&_dialog));

	// インスタンスの作成に失敗した場合はfalseを返して終了
	if (FAILED(_hr))
	{
		return false;
	}

	// 拡張子フィルタを設定する
	COMDLG_FILTERSPEC _fileTypes[] =
	{
		{ L"CSV(カンマ区切り)"	, L"*.csv" },			// カンマ区切りファイル
		{ L"テキスト ファイル"	, L"*.txt" }			// テキストファイル
	};

	// フィルタの数、フィルタの配列を設定する
	_hr = _dialog->SetFileTypes(2, _fileTypes);

	// ウィンドウハンドルを指定してダイアログを表示
	_hr = _dialog->Show(m_hWnd);

	// ウィンドウダイアログが開けた場合はパスの取得
	if (SUCCEEDED(_hr))
	{
		// 選択されたファイルパスを取得
		IShellItem* _shell;
		_hr = _dialog->GetResult(&_shell);

		// パスが取得出来たら以下処理を実行
		if (SUCCEEDED(_hr))
		{
			wchar_t* _tmpPath;
			_hr = _shell->GetDisplayName(SIGDN_FILESYSPATH, &_tmpPath);

			if (SUCCEEDED(_hr))
			{
				// 選択されたファイルパスをfilePathに格納
				filePath = _tmpPath;

				// 選択されたフィルタのインデックスを取得
				unsigned int _filterIdx;
				_hr = _dialog->GetFileTypeIndex(&_filterIdx);

				// 一番最後に現在のファイルパスを格納
				std::wstring _extension = L".csv";					// デフォルトはCSV

				// 条件によって変更する
				if (_filterIdx == 1) _extension = L".csv";			// CSVカンマ区切り
				if (_filterIdx == 2) _extension = L".txt";			// txtファイル

				filePath = AutoAddExtension(filePath, _extension);

				m_filename = filePath;

				// 正常終了
				return true;
			}
			_shell->Release();
		}
	}

	// pFileDialogの解放
	_dialog->Release();

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
	HRESULT _hr;

	// ファイル保存ダイアログ用のインターフェースを作成
	IFileOpenDialog* _dialog = nullptr;
	_hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&_dialog));

	// インスタンスの作成に失敗した場合はfalseを返して終了
	if (FAILED(_hr))
	{
		return false;
	}

	// 拡張子フィルタを設定する
	COMDLG_FILTERSPEC _fileTypes[] =
	{
		{ L"CSV(カンマ区切り)"	, L"*.csv" },			// カンマ区切りファイル
		{ L"テキスト ファイル"	, L"*.txt" }			// テキストファイル
	};

	// 引数：フィルタの数、フィルタの配列
	_hr = _dialog->SetFileTypes(2, _fileTypes);

	// ウィンドウハンドルを指定してダイアログを表示
	_hr = _dialog->Show(m_hWnd); // m_hWndはウィンドウハンドル

	// ウィンドウダイアログが開けた場合はパスの取得処理へ
	if (SUCCEEDED(_hr))
	{
		// 選択されたファイルパスを取得
		IShellItem* _shell;
		_hr = _dialog->GetResult(&_shell);

		// パスが取得出来たら以下処理を実行
		if (SUCCEEDED(_hr))
		{
			wchar_t* _tmpPath;
			_hr = _shell->GetDisplayName(SIGDN_FILESYSPATH, &_tmpPath);

			if (SUCCEEDED(_hr))
			{
				// 選択されたファイルパスをfilePathに格納
				m_filename = _tmpPath;

				// メモリの解放
				CoTaskMemFree(_tmpPath);

				// pItemの解放
				_shell->Release();

				// pFileDialogの解放
				_dialog->Release();

				// 選択されたフィルタのインデックスを取得
				unsigned int filterIdx;
				_hr = _dialog->GetFileTypeIndex(&filterIdx);

				// 一番最後に現在のファイルパスを格納
				std::wstring _extension = L".csv";					// デフォルトはCSV

				// 条件によって変更する
				if (filterIdx == 1) _extension = L".csv";			// CSVカンマ区切り
				if (filterIdx == 2) _extension = L".txt";			// txtファイル

				m_filename = AutoAddExtension(m_filename, _extension);

				// 正常終了
				return true;
			}
			_shell->Release();
		}
		else
		{
			CreateNewMap();
		}
	}

	// pFileDialogの解放
	_dialog->Release();

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
	std::wstring _output = filePath;

	// 文字列"."を見つける
	std::wstring::size_type _extPos = filePath.rfind(L".");

	// ドットが見つかった位置が対象文字列の最後尾or何もついていない場合追加する
	if (_extPos == std::wstring::npos || _extPos < filePath.rfind(L"\\"))
	{
		_output += extension;
	}
	return _output;
}

/// <summary>
/// マップ新規作成(更地)
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void MapLoad::CreateNewMap()
{
	std::vector<Object> _obj;

	// 最大マスのサイズにする
	const int _initialNumber = MAP_COLUMN;

	for (int x = 0; x < _initialNumber; ++x)
	{
		for (int y = 0; y < MAP_HEIGHT; ++y)
		{
			for (int z = 0; z < _initialNumber; ++z)
			{
				Object _tmpObj;

				if (y == 0)
				{
					_tmpObj.id = BOXSTATE::GRASS;
				}
				else
				{
					_tmpObj.id = BOXSTATE::NONE;
				}
				_tmpObj.position.x = static_cast<float>(x);
				_tmpObj.position.y = static_cast<float>(y);
				_tmpObj.position.z = static_cast<float>(z);

				_obj.push_back(_tmpObj);
			}
		}
	}

	// 作ったデータを格納
	m_mapData = _obj;

	_obj.clear();
}
