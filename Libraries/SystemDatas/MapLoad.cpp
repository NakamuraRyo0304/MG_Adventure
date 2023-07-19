/*
 *	@File	MapLoad.cpp
 *	@Brief	�}�b�v��ǂݏ����B
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include<iostream>    // �W���@�\�Ɏg�p
#include<regex>       // ������̒u���Ɏg�p
#include<sstream>     // �t�@�C���̕ۑ��Ɏg�p
#include<fstream>     // |
#include<shobjidl.h>  // �_�C�A���O����Ɏg�p

// <shobjidl.h>�̎Q�l����
//-------------------------------------------------------------------------------------// 
//   Microsoft�A�v���J��
//
//   https://learn.microsoft.com/ja-jp/windows/win32/api/shobjidl/
// �@���y�[�W�������N�ȗ�
//-------------------------------------------------------------------------------------// 

#include "MapLoad.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
MapLoad::MapLoad() :
	m_mapData{},
    m_hWnd{nullptr},
    is_saveFileOpenFlag{false},
	is_editMode{false}
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
MapLoad::~MapLoad()
{
	m_mapData.clear();
}


/// <summary>
/// �}�b�v�̓ǂݍ���
/// </summary>
/// <param name="filename">�t�@�C���p�X���w��(L"...csv")�g���q�́u.csv�v</param>
/// <returns>�Ȃ�</returns>
void MapLoad::LoadMap(std::wstring filename)
{
	m_filename = filename;

	// �t�@�C�������󔒂Ȃ�_�C�A���O����t�@�C�������擾����
	if (m_filename == L"")
	{
		m_mapData.clear();
		
		if (LoadMapPath() == false)
		{
			CreateNewMap();
		}
	}

	// �t�@�C�����J��
	std::ifstream ifs(m_filename);

	std::string line;

	// �f�[�^���Ȃ��Ȃ�܂Ŋi�[
	for (; std::getline(ifs, line); )
	{
		// �J���}���󔒂ɕύX
		std::string tmp = std::regex_replace(line, std::regex(","), " ");

		// �󔒂ŕ�������
		std::istringstream iss(tmp);

		Object obj;

		// �}�b�v�X�e�[�^�XID
		if (!(iss >> obj.id))
		{
			ifs.close();
			m_mapData.clear();
			return;
		}

		// ���W���
		if (!(iss >> obj.position.x) || !(iss >> obj.position.y) || !(iss >> obj.position.z))
		{
			ifs.close();
			m_mapData.clear();
			return;
		}

		// �ǂݍ��񂾃f�[�^���i�[����
		m_mapData.push_back(obj);
	}

	// �J�����t�@�C�������
	ifs.close();
}

/// <summary>
/// �t�@�C�������������֐�
/// </summary>
/// <param name="obj">�����o���I�u�W�F�z��</param>
/// <returns>�Ȃ�</returns>
void MapLoad::WriteMap(std::vector<Object> obj)
{
	// �t�@�C���p�X���w��
	SaveMapPath(m_filename);

	// �t�@�C���o�͕ϐ����`
	std::ofstream ofs(m_filename);

	// �t�@�C�����Ȃ���Ώ������Ȃ�
	if (!ofs)return;

	// �t�@�C�����o�͂���
	for (const auto& o : obj)
	{
		std::ostringstream oss;
		oss << o.id << "," << o.position.x << "," << o.position.y << "," << o.position.z << ",\n";
		ofs << oss.str();
	}

	ofs.close();
}

/// <summary>
/// ���������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void MapLoad::ReleaseMemory()
{
	std::vector<Object>().swap(m_mapData);
}

/// <summary>
/// �p�X���擾���A�ۑ�����֐�
/// </summary>
/// <param name="filePath">�t�@�C���p�X</param>
/// <returns>����I����True</returns>
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

	// �t�B���^�̐��A�t�B���^�̔z���ݒ肷��
	hr = pFileDialog->SetFileTypes(2, fileTypes);

	// �E�B���h�E�n���h�����w�肵�ă_�C�A���O��\��
	hr = pFileDialog->Show(m_hWnd);

	// �E�B���h�E�_�C�A���O���J�����ꍇ�̓p�X�̎擾
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

/// <summary>
/// �t�@�C����ǂݍ���
/// </summary>
/// <param name="��������"></param>
/// <returns>����I����True</returns>
bool MapLoad::LoadMapPath()
{
	// ��O�G���[�p�ϐ�
	HRESULT hr;

	// �t�@�C���ۑ��_�C�A���O�p�̃C���^�[�t�F�[�X���쐬
	IFileOpenDialog* pFileDialog = nullptr;
	hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));

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
				m_filename = tmpFilePath;

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

				m_filename = AutoAddExtension(m_filename, extension);

				// ����I��
				return true;
			}
			pShell->Release();
		}
		else
		{
			CreateNewMap();
		}
	}

	// pFileDialog�̉��
	pFileDialog->Release();

	// �_�C�A���O������ꂽ�ꍇ��G���[�����������ꍇ�̓f�[�^���폜
	m_filename.clear();

	// ��ԍŌ�Ɍ��݂̃t�@�C���p�X���i�[
	m_filename = m_filename;

	// �ُ�I��
	return false;
}

/// <summary>
/// �g���q�����t���֐�
/// </summary>
/// <param name="filePath">�t�@�C���p�X</param>
/// <param name="extension">�ǉ�����������(�g���q)</param>
/// <returns>�g���q��t�^����������</returns>
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

/// <summary>
/// �}�b�v�V�K�쐬(�X�n)
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void MapLoad::CreateNewMap()
{
	std::vector<Object> obj;

	// �ő�}�X�̃T�C�Y�ɂ���
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

	// ������f�[�^���i�[
	m_mapData = obj;

	obj.clear();
}
