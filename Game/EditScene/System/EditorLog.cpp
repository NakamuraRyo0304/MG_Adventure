/*
 *	@File	EditorLog.cpp
 *	@Brief	�}�b�v�̃��O�̕ۑ���B
 *	@Date	2023-10-26
 *  @Author NakamuraRyo
 */

#include "pch.h"

 // �������g
#include "MementMap.h"

#include "EditorLog.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
EditorLog::EditorLog()
	: m_currentIndex{0}
	, m_histories{}
{
	m_histories.clear();
	m_currentIndex = 0;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
EditorLog::~EditorLog()
{
	m_histories.clear();
}

/// <summary>
/// ��Ԃ�~�ς��Ă����֐�
/// </summary>
/// <param name="history">�ύX�_</param>
/// <returns>�Ȃ�</returns>
void EditorLog::AddHistory(const MementMap& history)
{
	// �ύX�_���܂Ƃ߂�����ۑ�
	m_histories.push_back(history);
	m_currentIndex = m_histories.size() - 1;
}

/// <summary>
/// �ЂƂO�̏�Ԃɖ߂�
/// </summary>
/// <param name="��������"></param>
/// <returns>�ЂƂO�̏��</returns>
const MementMap& EditorLog::GetUndo()
{
	if (m_currentIndex > 0)
	{
		m_currentIndex--;
	}
	return m_histories[m_currentIndex];
}

/// <summary>
/// Undo��������
/// </summary>
/// <param name="��������"></param>
/// <returns>�O���Undo�̑O�̏��</returns>
const MementMap& EditorLog::GetRedo()
{
	if (m_currentIndex < m_histories.size() - 1)
	{
		m_currentIndex++;
	}
	return m_histories[m_currentIndex];
}

