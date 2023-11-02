/*
 *	@File	MementoMap.cpp
 *	@Brief	�}�b�v�̉ߋ��̏���ۑ�����N���X�B
 *	@Date	2023-10-26
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "MementoMap.h"

// �R���X�g���N�^
MementoMap::MementoMap(const std::vector<Object>& state)
	: m_state{ state }
{
}

// �f�X�g���N�^
MementoMap::~MementoMap()
{
	m_state.clear();
}

// �X�e�[�g���擾
const std::vector<Object>& MementoMap::GetState()
{
	return m_state;
}

