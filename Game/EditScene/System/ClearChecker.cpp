/*
 *	@File	ClearChecker.cpp
 *	@Brief	�N���A�`�F�b�N������N���X�B
 *	@Date	2023-10-07
 *  @Author NakamuraRyo
 */

#include "pch.h"

// ���[�U���[�e�B���e�B
#include "../../../Libraries/UserUtility.h"

#include "ClearChecker.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
ClearChecker::ClearChecker()
	: m_checkMap{}				// �}�b�v�f�[�^
	, m_playerNum{0}			// �v���C���[�̐�
	, m_coinNum{0}				// �R�C���̐�
	, m_coinLength{0.0f}		// �R�C���̋���
	, is_coinPossibility{false}	// �N���A�ł���z��͈͂�
	, is_playerCheck{false}		// �v���C���[�t���O
	, is_coinCheck{false}		// �R�C���t���O
{
}

/// <summary>
 /// �N���A�`�F�b�N�����s
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>������True�F���s��False</returns>
bool ClearChecker::RunCheck()
{
	m_playerNum = 0;
	m_coinNum = 0;

	is_playerCheck = false;
	is_coinCheck = false;
	is_coinPossibility = true;

	// �v���C���[�̐����v�Z
	for (int i = 0; i < m_checkMap.size(); i++)
	{
		if (m_checkMap[i].id == MAPSTATE::PLAYER)
		{
			m_playerNum++;
		}
		if (m_checkMap[i].id == MAPSTATE::COIN)
		{
			m_coinNum++;
		}
	}

	// �R�C���̐����K��ʂ��z���Ă��Ȃ����v��
	is_coinPossibility = m_coinNum < CHECK_COIN_NUM ? true : false;

	// �R�C���ƃR�C���̋������v�Z����
	m_coinLength = 0.0f;

	for (auto& i : m_checkMap)
	{
		for (auto& j : m_checkMap)
		{
			if (i.id != MAPSTATE::COIN || j.id != MAPSTATE::COIN) continue;

			float check = 0.0f;

			// �������v�Z���čő���z���������ւ���
			check = SimpleMath::Vector3::Distance(i.position, j.position);

			if (check > m_coinLength)
			{
				m_coinLength = check;
			}
		}
	}

	// �R�C���Ԃ̋������K��I�[�o�[���v��
	is_coinPossibility = m_coinLength < CHECK_COIN_LENGTH ? true : false;

	// �v���C���[���P�̂������Ȃ����Ƃ��m�F
	is_playerCheck = m_playerNum < 2 ? true : false;

	// �R�C�����u���Ă��邱�Ƃ��m�F
	is_coinCheck = m_coinNum > 0 ? true : false;

	return is_playerCheck && is_coinCheck;
}

/// <summary>
/// �N���A�ł���\�����������True��Ԃ�
/// </summary>
/// <param name="��������"></param>
/// <returns>Summary�Ɠ���</returns>
const bool& ClearChecker::GetCoinCheck()
{
	return is_coinPossibility;
}
