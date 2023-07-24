/*
 *	@File	LoadFile.h
 *	@Brief	�t�@�C����ǂݍ��ރN���X�B
 *	@Date	2023-07-07
 *  @Author NakamuraRyo
 */

#pragma once

#ifndef LOADFILE
#define LOADFILE

class LoadFile
{
private:

	// �f�[�^
	std::unique_ptr<char[]> m_data;

	// �T�C�Y
	unsigned int m_size;

public:

	// �t�@�C�������w�肵�ă��[�h
	static LoadFile LoadBinaryFile(const wchar_t* fileName);


	// ���[�u�R���X�g���N�^
	LoadFile(LoadFile&& in)  noexcept(false);
	LoadFile();
	~LoadFile();


	// �A�N�Z�T
	char* GetData() { return m_data.get(); }
	unsigned int GetSize() { return m_size; }
};

#endif // LOADFILE