/*
 *	@File	MementMap.h
 *	@Brief	�}�b�v�̉ߋ��̏���ۑ�����N���X�B
 *	@Date	2023-10-26
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef MEMENTMAP
#define MEMENTMAP

 // �\���̎擾
#include "Libraries/SystemDatas/MapLoad.h"

class MementMap
{
private:
	// �ۑ�
	std::vector<Object> m_state;

public:

	MementMap(const std::vector<Object>& state);
	~MementMap();

	// �X�e�[�g�Q�b�^�[
	const std::vector<Object>& GetState();
};

#endif // MEMENTMAP