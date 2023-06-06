/*
 *	@File	MapLoad.cpp
 *	@Brief	�}�b�v��ǂݍ��݁B
 *	@Date	2023-01-20
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "MapLoad.h"

//--------------------------------------------------------//
//�R���X�g���N�^                                          //
//--------------------------------------------------------//
MapLoad::MapLoad() :
	m_mapData{}
{
}


 //--------------------------------------------------------//
 //�}�b�v�̓ǂݍ���                                        //
 //--------------------------------------------------------//
void MapLoad::LoadMap(const wchar_t* filename)
{
	// �t�@�C���ǂݍ��ݕϐ���`
	std::ifstream ifs;

	// �t�@�C����ǂݎ���p�ŊJ��
	ifs.open(filename, std::ios::in | std::ios::binary);

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
//�t�@�C���̏����o��                                      //
//--------------------------------------------------------//
// �t�@�C���̖��O
void MapLoad::WriteMap(const wchar_t* filename)
{
	//OPENFILENAME ofn;							//�@FILENAME�\���̂̒�`
	//char path[MAX_PATH] = "";					//�@�t�@�C���p�X��������
	//ZeroMemory(&ofn, sizeof(ofn));				//�@�\���̂̃p�X����ɂ���
	//ofn.lStructSize = sizeof(OPENFILENAME);		//�@�\���̂̃T�C�Y���w��
	//ofn.lpstrFilter = "csv(*.csv)\0*.csv\0"		//�@�g���q�̃t�B���^�[�������� \n�͕��̏I����\��
	//	"json(*.json)\0*.json\0"
	//	"���ׂẴt�@�C��(*.*)\0*.*\0\0";
	//ofn.lpstrFile = path;						//�@�t�@�C���p�X���i�[����|�C���^
	//ofn.nMaxFile = MAX_PATH;					//�@lpstrFile�̃o�b�t�@�T�C�Y
	//ofn.lpstrDefExt = ".csv";					//�@�g���q���w�肳��Ă��Ȃ����".csv"������
	//int result = 0;

	//GetSaveFileName(&ofn);

	////�@�t�@�C�����J��
	//ofstream ofs(path);


	//for (int y = 0; y < MAP_RAW; y++)
	//{
	//	// ��s���̃f�[�^��ǂݍ���
	//	std::ofs >> s[y];

	//	// �J���}���󔒂ɕύX
	//	std::string tmp = std::regex_replace(s[y], std::regex(","), " ");

	//	// �󔒂ŕ�������
	//	std::istringstream iss(tmp);
	//	for (int x = 0; x < MAP_COLUMN; x++)
	//	{
	//		ofs << m_MapBlock[y][x] << ",";
	//	}
	//	ofs << endl;
	//}

	////�@�t�@�C�������
	//ofs.close();
}

//--------------------------------------------------------//
//�}�b�v�Q�b�^�[                                          //
//--------------------------------------------------------//
int MapLoad::GetMapData(int x, int y)
{
	return m_mapData[y][x];
}