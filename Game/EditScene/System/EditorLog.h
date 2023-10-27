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

	// ��Ԃ�~�ς��Ă����֐�
	void AddHistory(const MementoMap& history);

	const MementoMap& GetUndo();
	const MementoMap& GetRedo();
};

#endif // EDITORLOG