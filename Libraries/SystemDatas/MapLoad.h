/*
 *	@File	MapLoad.h
 *	@Brief	�}�b�v��ǂݏ����B
 *	@Date	2023-06-07
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef MAPLOAD
#define MAPLOAD

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
	static const int MAP_HEIGHT = 8;

	// �G���A�{�b�N�X�̃X�e�[�^�X
	struct BOXSTATE
	{
		static const int NONE		= 0;	// �f�[�^�Ȃ�
		static const int GRASS		= 1;	// ���̃u���b�N
		static const int CLOUD		= 2;	// �_�̃u���b�N
		static const int COIN		= 3;	// �R�C���̃u���b�N
		static const int GRAVITY	= 4;	// �d�̓u���b�N
		static const int BIRD		= 5;	// �v���C���[
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

	/// <summary>
	/// �}�b�v�̓ǂݍ���
	/// </summary>
	/// <param name="filename">�t�@�C���p�X���w��(L"...csv")�g���q�́u.csv�v</param>
	/// <returns>����������True��Ԃ�</returns>
	bool LoadMap(std::wstring filename);

	/// <summary>
	/// �t�@�C�������������֐�
	/// </summary>
	/// <param name="obj">�����o���I�u�W�F�z��</param>
	/// <returns>�Ȃ�</returns>
	void WriteMap(std::vector<Object> obj);

	/// <summary>
	/// �}�b�v���擾
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�}�b�v�̃f�[�^</returns>
	std::vector<Object> GetMapData() { return m_mapData; }

	/// <summary>
	/// �}�b�v�̃p�X���擾
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�}�b�v��CSV�p�X</returns>
	std::wstring GetFilePath() { return m_filename; }

	/// <summary>
	/// ���������
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void ReleaseMemory();

private:
	/// <summary>
	/// �p�X���擾���A�ۑ�����֐�
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	/// <returns>����I����True</returns>
	bool SaveMapPath(std::wstring& filePath);

	/// <summary>
	/// �t�@�C����ǂݍ���
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>����I����True</returns>
	bool LoadMapPath();

	/// <summary>
	/// �g���q�����t���֐�
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	/// <param name="extension">�ǉ�����������(�g���q)</param>
	/// <returns>�g���q��t�^����������</returns>
	std::wstring AutoAddExtension(const std::wstring& filePath, const std::wstring& extension);

public:

	/// <summary>
	/// �}�b�v�V�K�쐬�֐�
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void CreateNewMap();

};

#endif // MAPLOAD