/*
 *	@File	EditorLog.h
 *	@Brief	�}�b�v�̃��O�̕ۑ���B
 *	@Date	2023-10-26
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef EDITORLOG
#define EDITORLOG

 // �\���̎擾
#include "Libraries/SystemDatas/MapLoad.h"

class MementoMap;
class EditorLog
{
private:

	// �ۑ������z��
	std::vector<MementoMap> m_histories;

	// ���݂̃C���f�b�N�X
	size_t m_currentIndex;

public:

	EditorLog();
	~EditorLog();

	/// <summary>
	/// ��Ԃ�~�ς��Ă����֐�
	/// </summary>
	/// <param name="history">�ύX�_</param>
	/// <returns>�Ȃ�</returns>
	void AddHistory(const MementoMap& history);

	/// <summary>
	/// �ЂƂO�̏�Ԃɖ߂�
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�ЂƂO�̏��</returns>
	const MementoMap& GetUndo();

	/// <summary>
	/// Undo��������
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�O���Undo�̑O�̏��</returns>
	const MementoMap& GetRedo();
};

#endif // EDITORLOG