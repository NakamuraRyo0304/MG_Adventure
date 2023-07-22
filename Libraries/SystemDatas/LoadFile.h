/*
 *	@File	LoadFile.h
 *	@Brief	ファイルを読み込むクラス。
 *	@Date	2023-07-07
 *  @Author NakamuraRyo
 */

#pragma once

#ifndef LOADFILE
#define LOADFILE

class LoadFile
{
private:

	// データ
	std::unique_ptr<char[]> m_data;

	// サイズ
	unsigned int m_size;

public:

	// ファイル名を指定してロード
	static LoadFile LoadBinaryFile(const wchar_t* fileName);


	// ムーブコンストラクタ
	LoadFile(LoadFile&& in)  noexcept(false);
	LoadFile();
	~LoadFile();


	// アクセサ
	char* GetData() { return m_data.get(); }
	unsigned int GetSize() { return m_size; }
};

#endif // LOADFILE