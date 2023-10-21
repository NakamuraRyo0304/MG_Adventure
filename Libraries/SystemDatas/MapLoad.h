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

	Object()
		: id(0), position(DirectX::SimpleMath::Vector3(0, 0, 0)), hit(false), index(0)
	{
	}

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
	static const int MAP_HEIGHT = 10;

	// �G���A�{�b�N�X�̃X�e�[�^�X
	struct BOXSTATE
	{
		static const int NONE		= 0;	// �f�[�^�Ȃ�
		static const int GRASS		= 1;	// ���̃u���b�N
		static const int CLOUD		= 2;	// �_�̃u���b�N
		static const int COIN		= 3;	// �R�C���̃u���b�N
		static const int GRAVITY	= 4;	// �d�̓u���b�N
		static const int PLAYER		= 5;	// �v���C���[
		static const int LENGTH		= 6;	// �u���b�N�X�e�[�^�X�̒���
	};

private:
	// �}�b�v�f�[�^�i�[�p�z��
	std::vector<Object> m_mapData;

	// �ۑ���t�@�C���p�X
	std::wstring m_filename;

	// �t�@�C���ۑ��p�t���O
	bool is_fileFlag;

	// �E�B���h�E�n���h��
	HWND m_hWnd;

public:

	MapLoad();
	~MapLoad();

	// �}�b�v�̓ǂݍ���
	bool LoadMap(std::wstring filename);

	// �}�b�v�̏����o��
	void WriteMap(std::vector<Object> obj);

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