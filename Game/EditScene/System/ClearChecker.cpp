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

// �R���X�g���N�^
ClearChecker::ClearChecker()
	: m_checkMap{}				// �}�b�v�f�[�^
	, m_playerNum{0}			// �v���C���[�̐�
	, m_playerPos{}				// �v���C���[�̍��W
	, m_coinNum{0}				// �R�C���̐�
	, m_coinLength{0.0f}		// �R�C���̋���
	, m_cloudNum{0}				// �_�̐�
	, is_clearPossibility{false}// �N���A�ł���z��͈͂�
	, is_playerCheck{false}		// �v���C���[�t���O
	, is_coinCheck{false}		// �R�C���t���O
	, is_cloudCheck{false}		// �_�t���O
	, is_startCheck{false}		// �X�^�[�g�t���O
{
}


// �{���}�b�v�̐ݒ�
void ClearChecker::SetMap(const std::vector<Object>& map)
{
	// �{������}�b�v���i�[
	m_checkMap = map;

	// ���̃��Z�b�g
	m_playerNum = 0;
	m_coinNum = 0;
	m_cloudNum = 0;

	// ���W
	m_playerPos = SimpleMath::Vector3::Zero;

	// �`�F�b�N�t���O
	is_playerCheck = false;
	is_coinCheck = false;
	is_cloudCheck = false;
	is_startCheck = false;
	is_clearPossibility = false;

	// ����
	m_coinLength = 0.0f;
}

// �N���A�`�F�b�N�����s
bool ClearChecker::RunCheck()
{
	// �I�u�W�F�N�g�̐����v�Z
	for (int i = 0; i < m_checkMap.size(); i++)
	{
		if (m_checkMap[i].id == MAPSTATE::PLAYER)
		{
			m_playerNum++;
			m_playerPos = m_checkMap[i].position;
		}
		if (m_checkMap[i].id == MAPSTATE::COIN)
		{
			m_coinNum++;
		}
		if (m_checkMap[i].id == MAPSTATE::CLOUD)
		{
			m_cloudNum++;
		}
	}

	// �R�C���̐����K��ʂ��z���Ă��Ȃ����v��
	is_clearPossibility = m_coinNum < CHECK_COIN_NUM ? true : false;

	// �R�C���ƃR�C���̋������v�Z����
	for (auto& i : m_checkMap)
	{
		for (auto& j : m_checkMap)
		{
			// �R�C������Ȃ���Ώ������΂�
			if (i.id != MAPSTATE::COIN || j.id != MAPSTATE::COIN) continue;

			// �������v�Z���čő���z���������ւ���
			 float _check = SimpleMath::Vector3::Distance(i.position, j.position);

			if (_check > m_coinLength)
			{
				m_coinLength = _check;
			}
		}
	}

	// �R�C���Ԃ̋������K��I�[�o�[���v��
	is_clearPossibility = m_coinLength < CHECK_COIN_LENGTH ? true : false;

	// �v���C���[���P�̂������Ȃ����Ƃ��m�F
	is_playerCheck = m_playerNum < 2 ? true : false;

	// �R�C�����u���Ă��邱�Ƃ��m�F
	is_coinCheck = m_coinNum > 0 ? true : false;

	// ���W��r
	is_cloudCheck = CheckSameHeight();

	// ����X�^�[�g���\���`�F�b�N
	is_startCheck = CheckNormalStart();

	return is_playerCheck && is_coinCheck && is_cloudCheck && is_startCheck;
}

// �_�����邩����
bool ClearChecker::CheckSameHeight()
{
	// �_���Ȃ���Δ�r����������
	if (m_cloudNum == 0) return true;

	for (auto& i : m_checkMap)
	{
		if (i.id == MAPSTATE::CLOUD)
		{
			// �����True�A���Ȃ����False������
			if (static_cast<int>(m_playerPos.y) > static_cast<int>(i.position.y))
			{
				return true;
			}
		}
	}

	return false;
}

// �X�^�[�g�\������
bool ClearChecker::CheckNormalStart()
{
	for (auto& i : m_checkMap)
	{
		// �u���b�N������΃X�L�b�v
		if (i.id != MAPSTATE::NONE) continue;

		// �v���C���[�̉��Ƀu���b�N���Ȃ����false��Ԃ�
		if (static_cast<int>(i.position.x) == static_cast<int>(m_playerPos.x) &&
			static_cast<int>(i.position.z) == static_cast<int>(m_playerPos.z) &&
			static_cast<int>(i.position.y) <  static_cast<int>(m_playerPos.y))
		{
			return false;
		}
	}

	// �`�F�b�N����蔲�����琳��X�^�[�g�\
	return true;
}
