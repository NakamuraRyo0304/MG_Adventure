/*
 *	@File	MementMap.cpp
 *	@Brief	�}�b�v�̉ߋ��̏���ۑ�����N���X�B
 *	@Date	2023-10-26
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "MementMap.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="state">�ύX���������I�u�W�F�N�g</param>
 /// <returns>�Ȃ�</returns>
MementMap::MementMap(const std::vector<Object>& state)
	: m_state{ state }
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
MementMap::~MementMap()
{
	m_state.clear();
}

/// <summary>
/// �X�e�[�g�Q�b�^�[
/// </summary>
/// <param name="��������"></param>
/// <returns>�O��̃X�e�[�g</returns>
const std::vector<Object>& MementMap::GetState()
{
	return m_state;
}

