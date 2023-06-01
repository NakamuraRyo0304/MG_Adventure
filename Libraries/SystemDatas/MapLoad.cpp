/*
 *	@File	MapLoad.cpp
 *	@Brief	�}�b�v��ǂݍ��݁B
 *	@Date	2023-01-20
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "MapLoad.h"

//--------------------------------------------------------//
//�}�b�v�̓ǂݍ���                                        //
//--------------------------------------------------------//
void MapLoad::LoadMap()
{
	// �t�@�C���ǂݍ��ݕϐ���`
	std::ifstream ifs;

	// �t�@�C����ǂݎ���p�ŊJ��
	ifs.open(m_fileName, std::ios::in | std::ios::binary);

	// �t�@�C�����J���Ȃ������ꍇ�̓G���[���b�Z�[�W���o��
	if (!ifs)
	{
		MessageBoxA(0, "�t�@�C�����J���܂���ł���", NULL, MB_OK);
		return;			// �������^�[��
	}

	// ��s���i�������j�̃f�[�^��񐔕��i�c�����j�����z��
	std::string s[MAP_RAW];

	for (int y = 0; y < MAP_RAW; y++)
	{
		// ��s���̃f�[�^��ǂݍ���
		ifs >> s[y];

		// �J���}���󔒂ɕύX
		std::string tmp = std::regex_replace(s[y], std::regex(","), " ");

		// �󔒂ŕ�������
		std::istringstream iss(tmp);
		for (int x = 0; x < MAP_COLUMN; x++)
		{
			// null(-1)������
			int num = -1;

			// �󔒂܂ł̃f�[�^�i����̃f�[�^�j��ǂݍ���
			iss >> num;

			// �ǂݍ��񂾃f�[�^���i�[����
			m_mapData[y][x] = num;
		}
	}
	// �J�����t�@�C�������
	ifs.close();
}

//--------------------------------------------------------//
//�R���X�g���N�^                                          //
//--------------------------------------------------------//
MapLoad::MapLoad():
	m_mapData{}
{
}

//--------------------------------------------------------//
//�}�b�v�Q�b�^�[                                          //
//--------------------------------------------------------//
int MapLoad::GetMapData(int x,int y)
{
	return m_mapData[y][x];
}

//--------------------------------------------------------//
//�}�b�v���Z�b�g                                          //
//--------------------------------------------------------//
void MapLoad::SetMapData(const char* filename)
{
	// ���O��ݒ肵�ēǂݍ���
	this->m_fileName = filename;
	LoadMap();
}