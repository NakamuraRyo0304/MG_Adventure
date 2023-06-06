/*
 *	@File	MapLoad.h
 *	@Brief	�}�b�v��ǂݍ��݁B
 *	@Date	2023-01-20
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef MAPLOAD
#define MAPLOAD

class MapLoad
{
public:
	// ���̒���
	const static int MAP_COLUMN = 15;
	// �c�̒���
	const static int MAP_RAW = 15;

	// �G���A�{�b�N�X�̃X�e�[�^�X
	struct BoxState
	{
		const static UINT GrassBox = 101;		// ���̃u���b�N
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

public:

	MapLoad();
	~MapLoad() = default;

	// �}�b�v�̓ǂݍ���
	void LoadMap(const wchar_t* filename);

	// �}�b�v�̏����o��
	void WriteMap();

	void SetMapData(int state, int x, int y);

	// �}�b�v�̃f�[�^��n��
	int GetMapData(int x, int y) { return m_mapData[y][x]; }

private:
	// �}�b�v�̃p�X�w��
	bool SaveMapPath(std::wstring& filePath);

	// �g���q�����Ă��Ȃ��ꍇ�Ɏ����Ŋg���q��ǉ�����֐�
	std::wstring AutoAddExtension(const std::wstring& filePath, const std::wstring& extension);

};

#endif // MAPLOAD