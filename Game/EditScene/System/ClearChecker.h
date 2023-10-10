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
	bool is_checkFlag;

	// �v���C���[�̐����`�F�b�N����
	int m_playerNum;

	// �͈�
	const float JIDGE = 0.9f * 1.5f;
public:

	ClearChecker();
	~ClearChecker() = default;

	// �}�b�v�Z�b�^�[
	void SetMap(const std::vector<Object>& map) { m_checkMap = map; }

	// �N���A�`�F�b�N�����s
	bool RunCheck();

	// �v���C���[�̐�
	const int& GetPlayerNum() { return m_playerNum; }
};

#endif // CLEARCHECKER