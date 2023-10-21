/*
 *	@File	MapLoad.cpp
 *	@Brief	�}�b�v��ǂݏ����B
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include<regex>       // ������̒u���Ɏg�p
#include<sstream>     // �t�@�C���̕ۑ��Ɏg�p
#include<fstream>     // �t�@�C���̕ۑ��Ɏg�p
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
MapLoad::MapLoad()
	: m_mapData{}			// �}�b�v�f�[�^
	, m_hWnd{ nullptr }		// �E�B���h�E�n���h��
	, is_fileFlag{ false }	// �t�@�C�����J���t���O
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
/// <returns>����������</returns>
bool MapLoad::LoadMap(std::wstring filename)
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
	std::ifstream _ifs(m_filename);

	if (!_ifs) return false;

	std::string _line;

	// �f�[�^���Ȃ��Ȃ�܂Ŋi�[
	for (; std::getline(_ifs, _line); )
	{
		// �J���}���󔒂ɕύX
		std::string _tmpDatas = std::regex_replace(_line, std::regex(","), " ");

		// �󔒂ŕ�������
		std::istringstream _iss(_tmpDatas);

		// �w�b�_�[�̏ꍇ�X�L�b�v
		if (_line.find("�}�b�v�h�c") != std::string::npos ||
			_line.find("0:�Ȃ�") != std::string::npos)
		{
			continue;
		}

		Object _obj;

		// �}�b�v�X�e�[�^�XID
		if (!(_iss >> _obj.id))
		{
			_ifs.close();
			m_mapData.clear();
		}

		// ���W���
		if (_obj.id == BOXSTATE::NONE)
		{
			_obj.position = SimpleMath::Vector3::Zero;
		}
		else if (!(_iss >> _obj.position.x) || !(_iss >> _obj.position.y) || !(_iss >> _obj.position.z))
		{
			_ifs.close();
			m_mapData.clear();
		}

		// �ǂݍ��񂾃f�[�^���i�[����
		m_mapData.push_back(_obj);
	}

	// �J�����t�@�C�������
	_ifs.close();

	return true;
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
	std::ofstream _ofs(m_filename);

	// �t�@�C�����Ȃ���Ώ������Ȃ�
	if (!_ofs) return;

	// �w�b�_�[
	_ofs << "�}�b�v�h�c" << ",�w" << ",�x" << ",�y" << ",\n";
	_ofs << "0:�Ȃ�,1:��,2:�_,3:�R�C��,4:�d��,5:�v���C���[,\n";

	for (auto& _obj : obj)
	{
		std::ostringstream _oss;

		// ID,X,Y,X,���s
		if (_obj.id == BOXSTATE::NONE)
		{
			_oss << BOXSTATE::NONE << ",\n";
		}
		else
		{
			_oss << _obj.id << "," << _obj.position.x << "," << _obj.position.y << "," << _obj.position.z << ",\n";
		}
		// �P�u���b�N�̏����o��
		_ofs << _oss.str();
	}

	_ofs.close();
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
	HRESULT _hr;

	// �t�@�C���ۑ��_�C�A���O�p�̃C���^�[�t�F�[�X���쐬
	IFileSaveDialog* _dialog = nullptr;
	_hr = CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&_dialog));

	// �C���X�^���X�̍쐬�Ɏ��s�����ꍇ��false��Ԃ��ďI��
	if (FAILED(_hr))
	{
		return false;
	}

	// �g���q�t�B���^��ݒ肷��
	COMDLG_FILTERSPEC _fileTypes[] =
	{
		{ L"CSV(�J���}��؂�)"	, L"*.csv" },			// �J���}��؂�t�@�C��
		{ L"�e�L�X�g �t�@�C��"	, L"*.txt" }			// �e�L�X�g�t�@�C��
	};

	// �t�B���^�̐��A�t�B���^�̔z���ݒ肷��
	_hr = _dialog->SetFileTypes(2, _fileTypes);

	// �E�B���h�E�n���h�����w�肵�ă_�C�A���O��\��
	_hr = _dialog->Show(m_hWnd);

	// �E�B���h�E�_�C�A���O���J�����ꍇ�̓p�X�̎擾
	if (SUCCEEDED(_hr))
	{
		// �I�����ꂽ�t�@�C���p�X���擾
		IShellItem* _shell;
		_hr = _dialog->GetResult(&_shell);

		// �p�X���擾�o������ȉ����������s
		if (SUCCEEDED(_hr))
		{
			wchar_t* _tmpPath;
			_hr = _shell->GetDisplayName(SIGDN_FILESYSPATH, &_tmpPath);

			if (SUCCEEDED(_hr))
			{
				// �I�����ꂽ�t�@�C���p�X��filePath�Ɋi�[
				filePath = _tmpPath;

				// �I�����ꂽ�t�B���^�̃C���f�b�N�X���擾
				unsigned int _filterIdx;
				_hr = _dialog->GetFileTypeIndex(&_filterIdx);

				// ��ԍŌ�Ɍ��݂̃t�@�C���p�X���i�[
				std::wstring _extension = L".csv";					// �f�t�H���g��CSV

				// �����ɂ���ĕύX����
				if (_filterIdx == 1) _extension = L".csv";			// CSV�J���}��؂�
				if (_filterIdx == 2) _extension = L".txt";			// txt�t�@�C��

				filePath = AutoAddExtension(filePath, _extension);

				m_filename = filePath;

				// ����I��
				return true;
			}
			_shell->Release();
		}
	}

	// pFileDialog�̉��
	_dialog->Release();

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
	HRESULT _hr;

	// �t�@�C���ۑ��_�C�A���O�p�̃C���^�[�t�F�[�X���쐬
	IFileOpenDialog* _dialog = nullptr;
	_hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&_dialog));

	// �C���X�^���X�̍쐬�Ɏ��s�����ꍇ��false��Ԃ��ďI��
	if (FAILED(_hr))
	{
		return false;
	}

	// �g���q�t�B���^��ݒ肷��
	COMDLG_FILTERSPEC _fileTypes[] =
	{
		{ L"CSV(�J���}��؂�)"	, L"*.csv" },			// �J���}��؂�t�@�C��
		{ L"�e�L�X�g �t�@�C��"	, L"*.txt" }			// �e�L�X�g�t�@�C��
	};

	// �����F�t�B���^�̐��A�t�B���^�̔z��
	_hr = _dialog->SetFileTypes(2, _fileTypes);

	// �E�B���h�E�n���h�����w�肵�ă_�C�A���O��\��
	_hr = _dialog->Show(m_hWnd); // m_hWnd�̓E�B���h�E�n���h��

	// �E�B���h�E�_�C�A���O���J�����ꍇ�̓p�X�̎擾������
	if (SUCCEEDED(_hr))
	{
		// �I�����ꂽ�t�@�C���p�X���擾
		IShellItem* _shell;
		_hr = _dialog->GetResult(&_shell);

		// �p�X���擾�o������ȉ����������s
		if (SUCCEEDED(_hr))
		{
			wchar_t* _tmpPath;
			_hr = _shell->GetDisplayName(SIGDN_FILESYSPATH, &_tmpPath);

			if (SUCCEEDED(_hr))
			{
				// �I�����ꂽ�t�@�C���p�X��filePath�Ɋi�[
				m_filename = _tmpPath;

				// �������̉��
				CoTaskMemFree(_tmpPath);

				// pItem�̉��
				_shell->Release();

				// pFileDialog�̉��
				_dialog->Release();

				// �I�����ꂽ�t�B���^�̃C���f�b�N�X���擾
				unsigned int filterIdx;
				_hr = _dialog->GetFileTypeIndex(&filterIdx);

				// ��ԍŌ�Ɍ��݂̃t�@�C���p�X���i�[
				std::wstring _extension = L".csv";					// �f�t�H���g��CSV

				// �����ɂ���ĕύX����
				if (filterIdx == 1) _extension = L".csv";			// CSV�J���}��؂�
				if (filterIdx == 2) _extension = L".txt";			// txt�t�@�C��

				m_filename = AutoAddExtension(m_filename, _extension);

				// ����I��
				return true;
			}
			_shell->Release();
		}
		else
		{
			CreateNewMap();
		}
	}

	// pFileDialog�̉��
	_dialog->Release();

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
	std::wstring _output = filePath;

	// ������"."��������
	std::wstring::size_type _extPos = filePath.rfind(L".");

	// �h�b�g�����������ʒu���Ώە�����̍Ō��or�������Ă��Ȃ��ꍇ�ǉ�����
	if (_extPos == std::wstring::npos || _extPos < filePath.rfind(L"\\"))
	{
		_output += extension;
	}
	return _output;
}

/// <summary>
/// �}�b�v�V�K�쐬(�X�n)
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void MapLoad::CreateNewMap()
{
	std::vector<Object> _obj;

	// �ő�}�X�̃T�C�Y�ɂ���
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

	// ������f�[�^���i�[
	m_mapData = _obj;

	_obj.clear();
}
