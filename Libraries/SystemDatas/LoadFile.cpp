/*
 *	@File	LoadFile.cpp
 *	@Brief	�t�@�C����ǂݍ��ރN���X�B
 *	@Date	2023-07-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

// �t�@�C���ǂݍ���
#include <fstream>
#include <assert.h>
#include <memory>

#include "LoadFile.h"

 /// <summary>
 /// �t�@�C���ǂݍ���
 /// </summary>
 /// <param name="fileName">�t�@�C���p�X</param>
 /// <returns>�o�C�i���p�X</returns>
LoadFile LoadFile::LoadBinaryFile(const wchar_t* fileName)
{
	LoadFile bin;

	std::ifstream ifs;

	// �t�@�C�����J��
	ifs.open(fileName, std::ios::in | std::ios::binary);

	// �ǂݍ��ݎ��s���A�����I��
	assert(ifs);

	// �t�@�C���T�C�Y���擾
	ifs.seekg(0, std::fstream::end);
	std::streamoff eofPos = ifs.tellg();
	ifs.clear();
	ifs.seekg(0, std::fstream::beg);
	std::streamoff begPos = ifs.tellg();
	bin.m_size = (unsigned int)(eofPos - begPos);

	// �ǂݍ��ނ��߂̃��������m��
	bin.m_data.reset(new char[bin.m_size]);

	// �t�@�C���擪����o�b�t�@�փR�s�[
	ifs.read(bin.m_data.get(), bin.m_size);

	// �t�@�C���N���[�Y
	ifs.close();

	return bin;
}

/// <summary>
/// ���[�u�R���X�g���N�^
/// </summary>
/// <param name="int">�t�@�C���ǂݍ��݃N���X</param>
/// <returns>�Ȃ�</returns>
LoadFile::LoadFile(LoadFile&& in) noexcept(false) :
	m_size{0}
{
	// �f�[�^�ƃT�C�Y���󂯎��
	m_size = in.m_size;
	m_data = std::move(in.m_data);
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
LoadFile::LoadFile()
{
	m_size = 0;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
LoadFile::~LoadFile()
{
}
