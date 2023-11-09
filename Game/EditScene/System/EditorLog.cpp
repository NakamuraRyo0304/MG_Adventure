/*
 *	@File	EditorLog.cpp
 *	@Brief	�}�b�v�̃��O�̕ۑ���B
 *	@Date	2023-10-26
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "MementoMap.h"
#include "EditorLog.h"

// �R���X�g���N�^
EditorLog::EditorLog()
	: m_currentIndex{0}
	, m_histories{}
{
	m_histories.clear();
	m_currentIndex = 0;
}

// �f�X�g���N�^
EditorLog::~EditorLog()
{
	m_histories.clear();
}

// ������~�ς���
void EditorLog::AddHistory(const MementoMap& history)
{
	// �ύX�_���܂Ƃ߂�����ۑ�
	m_histories.push_back(history);
	m_currentIndex = m_histories.size() - 1;
}

// Undo�@�\
const MementoMap& EditorLog::GetUndo()
{
	if (m_currentIndex > 0)
	{
		m_currentIndex--;
	}
	return m_histories[m_currentIndex];
}

// Redo�@�\
const MementoMap& EditorLog::GetRedo()
{
	if (m_currentIndex < m_histories.size() - 1)
	{
		m_currentIndex++;
	}
	return m_histories[m_currentIndex];
}

