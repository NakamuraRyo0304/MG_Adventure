/*
 *	@File	MapLoad.h
 *	@Brief	�}�b�v��ǂݏ����B
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef MAPLOAD
#define MAPLOAD

#include <vector>

// �}�b�v�u���b�N�̍\����
struct Object
{
	int id = 0;									// �}�b�vID
	DirectX::SimpleMath::Vector3 position;		// ���W
	bool hit = false;							// �������Ă�����True
	int index = 0;								// �C���f�b�N�X�ԍ����i�[

	// ���Z�q�̃I�[�o�[���[�h
	bool operator==(const Object& name) const
	{
		return id == name.id && position == name.position;
	}
};

class MapLoad
{
public:
	// ���̒���
	static const int MAP_COLUMN = 15;
	// �c�̒���
	static const int MAP_RAW = 15;
	// ����̍���
	static const int MAP_HEIGHT = 15;

	// �G���A�{�b�N�X�̃X�e�[�^�X
	struct BoxState
	{
		static const int None		= 0;		// �f�[�^�Ȃ�
		static const int GrassBox	= 1;		// ���̃u���b�N
		static const int ClowdBox	= 2;		// �����u���b�N
		static const int CoinBox	= 3;		// �R�C���̃u���b�N
		static const int ResetClowd	= 4;		// �_�̈ʒu�̃��Z�b�g�u���b�N
		static const int PlayerPos	= 5;		// �v���C��
		static const int LENGTH     = 6;		// �u���b�N�X�e�[�^�X�̒���
	};

private:
	// �}�b�v�f�[�^�i�[�p�z��
	std::vector<Object> m_mapData;

	// �ۑ���t�@�C���p�X
	std::wstring m_filename;

	// �t�@�C���ۑ��p�t���O
	bool is_saveFileOpenFlag;

	// �E�B���h�E�n���h��
	HWND m_hWnd;

	// �G�f�B�^���[�h
	bool is_editMode;

public:

	MapLoad();
	~MapLoad();

	// �}�b�v�̓ǂݍ���
	void LoadMap(std::wstring filename);

	// �}�b�v�̏����o��
	void WriteMap(std::vector<Object> obj);

	// �G�f�B�b�g���[�h�ؑ�
	void SetEditMode(const bool& flag) { is_editMode = flag; }

	// �}�b�v�̃f�[�^��n��
	std::vector<Object> GetMapData() { return m_mapData; }

	// �}�b�v�̃p�X����n��
	std::wstring GetFilePath() { return m_filename; }

	// �������̉��
	void ReleaseMemory();

private:
	// �}�b�v�̕ۑ��p�X�w��
	bool SaveMapPath(std::wstring& filePath);

	// �}�b�v�̓ǂݍ��݃p�X�w��
	bool LoadMapPath();

	// �g���q�����Ă��Ȃ��ꍇ�Ɏ����Ŋg���q��ǉ�����֐�
	std::wstring AutoAddExtension(const std::wstring& filePath, const std::wstring& extension);

public:
	// �V�����}�b�v���쐬����
	void CreateNewMap();

};

#endif // MAPLOAD