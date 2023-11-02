/*
 *	@File	ClearChecker.h
 *	@Brief	�N���A�`�F�b�N������N���X�B
 *	@Date	2023-10-07
 *  @Author NakamuraRyo
 */

#ifndef CLEARCHECKER
#define CLEARCHECKER

 // �}�b�v�f�[�^�ǂݍ���
#include "../../../Libraries/SystemDatas/MapLoad.h"

// �G�C���A�X��錾
using MAPSTATE = MapLoad::BOXSTATE;

class ClearChecker
{
private:

	// �`�F�b�N�z��
	std::vector<Object> m_checkMap;

	// �`�F�b�N�t���O
	bool is_playerCheck;
	bool is_coinCheck;
	bool is_cloudCheck;
	bool is_startCheck;

	// �v���C���[�̐����`�F�b�N����
	int m_playerNum;
	DirectX::SimpleMath::Vector3 m_playerPos;

	// �R�C���̐�
	int m_coinNum;
	// �R�C���̋����𑪂�
	float m_coinLength;
	bool is_clearPossibility;

	// �_�̐�
	int m_cloudNum;

private:

	// �͈�
	const float JIDGE = 0.9f * 1.5f;

	// �R�C���̃`�F�b�N�{�[�_�[
	const int CHECK_COIN_NUM = 20;
	const float CHECK_COIN_LENGTH = 12.5f;

public:

	ClearChecker();
	~ClearChecker() = default;

	/// <summary>
	/// �}�b�v�̃Z�b�g�Ƃ��̑��ݒ�
	/// </summary>
	/// <param name="map">�`�F�b�N�Ώۂ̃}�b�v</param>
	/// <returns>�Ȃ�</returns>
	void SetMap(const std::vector<Object>& map);


	/// <summary>
	 /// �N���A�`�F�b�N�����s
	 /// </summary>
	 /// <param name="��������"></param>
	 /// <returns>������True�F���s��False</returns>
	bool RunCheck();

	// �v���C���[�̐����擾
	const int& GetPlayerNum() { return m_playerNum; }

	// �R�C���̐��̐����擾
	const int& GetCoinNum() { return m_coinNum; }

	// �_�̈ʒu������ʒu������
	const bool& GetCloudFlag() { return is_cloudCheck; }

	// �v���C���[������ɃX�^�[�g�ł��邩����
	const bool& GetCanStart() { return is_startCheck; }

	// �N���A�ł���\�����������True��Ԃ�
	const bool& GetClearPossibility() { return is_clearPossibility; }

private:

	// �_�ƃv���C���[�̍��W�̔�r
	bool CheckSameHeight();

	// ����X�^�[�g�\���`�F�b�N
	bool CheckNormalStart();
};

#endif // CLEARCHECKER