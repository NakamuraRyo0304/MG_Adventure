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
	: m_checkMap{}			// �}�b�v�f�[�^
	, m_playerNum{0}		// �v���C���[�̐�
	, is_checkFlag{false}	// �`�F�b�N�t���O
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
	bool checkPlayer = false;
	bool checkCoin = false;

	// �K�{�v�f���܂܂�Ă��邩
	for (auto& idx : m_checkMap)
	{
		for (auto& jud : m_checkMap)
		{
			if (idx.id == MAPSTATE::PLAYER)
			{
				if (checkPlayer)
				{
					m_playerNum++;
				}
				else
				{
					// �v���C���[��������True
					checkPlayer = true;
				}
			}
			if (idx.id == MAPSTATE::COIN)
			{
				// �R�C���������True
				checkCoin = true;
			}
		}
	}

	return checkPlayer && checkCoin;
}
