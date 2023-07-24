/*
 *	@File	LoadFile.cpp
 *	@Brief	ファイルを読み込むクラス。
 *	@Date	2023-07-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

// ファイル読み込み
#include <fstream>
#include <assert.h>
#include <memory>

#include "LoadFile.h"

 /// <summary>
 /// ファイル読み込み
 /// </summary>
 /// <param name="fileName">ファイルパス</param>
 /// <returns>バイナリパス</returns>
LoadFile LoadFile::LoadBinaryFile(const wchar_t* fileName)
{
	LoadFile bin;

	std::ifstream ifs;

	// ファイルを開く
	ifs.open(fileName, std::ios::in | std::ios::binary);

	// 読み込み失敗時、強制終了
	assert(ifs);

	// ファイルサイズを取得
	ifs.seekg(0, std::fstream::end);
	std::streamoff eofPos = ifs.tellg();
	ifs.clear();
	ifs.seekg(0, std::fstream::beg);
	std::streamoff begPos = ifs.tellg();
	bin.m_size = (unsigned int)(eofPos - begPos);

	// 読み込むためのメモリを確保
	bin.m_data.reset(new char[bin.m_size]);

	// ファイル先頭からバッファへコピー
	ifs.read(bin.m_data.get(), bin.m_size);

	// ファイルクローズ
	ifs.close();

	return bin;
}

/// <summary>
/// ムーブコンストラクタ
/// </summary>
/// <param name="int">ファイル読み込みクラス</param>
/// <returns>なし</returns>
LoadFile::LoadFile(LoadFile&& in) noexcept(false) :
	m_size{0}
{
	// データとサイズを受け取る
	m_size = in.m_size;
	m_data = std::move(in.m_data);
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
LoadFile::LoadFile()
{
	m_size = 0;
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
LoadFile::~LoadFile()
{
}
