/*
 *	@File	MapLoad.h
 *	@Brief	�}�b�v��ǂݏ����B
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef MAPLOAD
#define MAPLOAD

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
		static const int GrassBox = 100;		// ���̃u���b�N
		static const int ClowdBox = 200;		// �R�C���̃u���b�N
		static const int CoinBox  = 300;		// �_�̃u���b�N
		static const int None = 0;				// �f�[�^�Ȃ�
	};

private:
	// �}�b�v�f�[�^�i�[�p�z��
	int m_mapData[MAP_RAW][MAP_COLUMN];

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
	~MapLoad() = default;

	// �}�b�v�̓ǂݍ���
	void LoadMap(std::wstring filename);

	// �}�b�v�̏����o��
	void WriteMap();

	// �}�b�v�f�[�^�̕ۑ�
	void SetMapData(int state, int x, int y);

	// �G�f�B�b�g���[�h�ؑ�
	void SetEditMode(const bool& flag) { is_editMode = flag; }

	// �}�b�v�̃f�[�^��n��
	int GetMapData(int x, int y) { return m_mapData[y][x]; }

	// �}�b�v�̃p�X����n��
	std::wstring GetFilePath() { return m_filename; }

private:
	// �}�b�v�̕ۑ��p�X�w��
	bool SaveMapPath(std::wstring& filePath);

	// �}�b�v�̓ǂݍ��݃p�X�w��
	bool LoadMapPath();

	// �g���q�����Ă��Ȃ��ꍇ�Ɏ����Ŋg���q��ǉ�����֐�
	std::wstring AutoAddExtension(const std::wstring& filePath, const std::wstring& extension);

};

#endif // MAPLOAD