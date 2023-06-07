/*
 *	@File	MapLoad.cpp
 *	@Brief	�}�b�v��ǂݏ����B
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include<iostream>    // string,cout,cin�Ɏg�p
#include<regex>       // ������̒u���Ɏg�p
#include<sstream>     // �t�@�C���̕ۑ��Ɏg�p
#include<ostream>     // �t�@�C���̕ۑ��Ɏg�p
#include<fstream>     // �t�@�C���̕ۑ��Ɏg�p
#include<Windows.h>   // WindowsAPI�Ɏg�p
#include<shobjidl.h>  // IFileSaveDialog�Ɏg�p


#include "MapLoad.h"

//--------------------------------------------------------//
//�R���X�g���N�^                                          //
//--------------------------------------------------------//
MapLoad::MapLoad() :
	m_mapData{},
    is_saveFileOpenFlag{false},
    m_hWnd{nullptr}
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
//�t�@�C�������������֐�                                  //
//--------------------------------------------------------//
void MapLoad::WriteMap()
{
	// �t�@�C���p�X���w��
	SaveMapPath(m_filename);

	//�@�t�@�C�����J��
	std::ofstream ofs(m_filename);

	//�@�P�s�����������玟�̗��
	for (int y = 0; y < MAP_RAW; y++)
	{
		for (int x = 0; x < MAP_COLUMN; x++)
		{
			ofs << m_mapData[y][x] << ",";
		}
		ofs << std::endl;
	}

	//�@�t�@�C�������
	ofs.close();
}

//--------------------------------------------------------//
//�}�b�v�f�[�^�̊i�[                                      //
//--------------------------------------------------------//
// ��P�����F�}�b�v�̒l ��Q�A�R�����F�z��ԍ�
void MapLoad::SetMapData(int state, int x, int y)
{
	m_mapData[y][x] = state;
}

//--------------------------------------------------------//
//�t�@�C���p�X���Z�[�u����֐�                            //
//--------------------------------------------------------//
// private
bool MapLoad::SaveMapPath(std::wstring& filePath)
{
	// ��O�G���[�p�ϐ�
	HRESULT hr;

	// �t�@�C���ۑ��_�C�A���O�p�̃C���^�[�t�F�[�X���쐬
	IFileSaveDialog* pFileDialog = nullptr;
	hr = CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));

	// �C���X�^���X�̍쐬�Ɏ��s�����ꍇ��false��Ԃ��ďI��
	if (FAILED(hr))
	{
		return false; 
	}

	// �g���q�t�B���^��ݒ肷��
	COMDLG_FILTERSPEC fileTypes[] = 
	{
		{ L"CSV(�J���}��؂�)"	, L"*.csv" },			// �J���}��؂�t�@�C��
		{ L"�e�L�X�g �t�@�C��"	, L"*.txt" }			// �e�L�X�g�t�@�C��
	};

	// �����F�t�B���^�̐��A�t�B���^�̔z��
	hr = pFileDialog->SetFileTypes(2, fileTypes);

	// �E�B���h�E�n���h�����w�肵�ă_�C�A���O��\��
	hr = pFileDialog->Show(m_hWnd); // m_hWnd�̓E�B���h�E�n���h��

	// �E�B���h�E�_�C�A���O���J�����ꍇ�̓p�X�̎擾������
	if (SUCCEEDED(hr))
	{
		// �I�����ꂽ�t�@�C���p�X���擾
		IShellItem* pShell;
		hr = pFileDialog->GetResult(&pShell);

		// �p�X���擾�o������ȉ����������s
		if (SUCCEEDED(hr))
		{
			wchar_t* tmpFilePath;
			hr = pShell->GetDisplayName(SIGDN_FILESYSPATH, &tmpFilePath);

			if (SUCCEEDED(hr))
			{
				// �I�����ꂽ�t�@�C���p�X��filePath�Ɋi�[
				filePath = tmpFilePath;

				// �������̉��
				CoTaskMemFree(tmpFilePath);

				// pItem�̉��
				pShell->Release(); 

				// pFileDialog�̉��
				pFileDialog->Release(); 

				// �I�����ꂽ�t�B���^�̃C���f�b�N�X���擾
				unsigned int filterIndex;
				hr = pFileDialog->GetFileTypeIndex(&filterIndex);

				// ��ԍŌ�Ɍ��݂̃t�@�C���p�X���i�[
				std::wstring extension = L".csv";					// �f�t�H���g��CSV

				// �����ɂ���ĕύX����
				if (filterIndex == 1) extension = L".csv";			// CSV�J���}��؂�
				if (filterIndex == 2) extension = L".txt";			// txt�t�@�C��

				filePath = AutoAddExtension(filePath, extension);

				m_filename = filePath;

				// ����I��
				return true;
			}
			pShell->Release();
		}
	}

	// pFileDialog�̉��
	pFileDialog->Release();

	// �_�C�A���O������ꂽ�ꍇ��G���[�����������ꍇ�̓f�[�^���폜
	filePath.clear();

	// ��ԍŌ�Ɍ��݂̃t�@�C���p�X���i�[
	m_filename = filePath;

	// �ُ�I��
	return false;
}

//--------------------------------------------------------//
//�g���q���Ȃ��Ƃ��̂ݏ�������                            //
//--------------------------------------------------------//
std::wstring MapLoad::AutoAddExtension(const std::wstring& filePath, const std::wstring& extension)
{
	// ���t�@�C�������i�[����
	std::wstring result = filePath;
	
	// ������"."��������
	std::wstring::size_type extPos = filePath.rfind(L".");
	
	// �h�b�g�����������ʒu���Ώە�����̍Ō��or�������Ă��Ȃ��ꍇ�ǉ�����
	if (extPos == std::wstring::npos || extPos < filePath.rfind(L"\\")) 
	{
		result += extension;
	}
	return result;
}
