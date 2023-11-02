/*
 *	@File	MementoMap.h
 *	@Brief	�}�b�v�̉ߋ��̏���ۑ�����N���X�B
 *	@Date	2023-10-26
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef MEMENTMAP
#define MEMENTMAP

 // �\���̎擾
#include "Libraries/SystemDatas/MapLoad.h"

class MementoMap
{
private:
	// �ۑ�
	std::vector<Object> m_state;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="state">�ύX���������I�u�W�F�N�g</param>
	/// <returns>�Ȃ�</returns>
	MementoMap(const std::vector<Object>& state);
	~MementoMap();

	/// <summary>
	/// �X�e�[�g���擾
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�O��̃X�e�[�g</returns>
	const std::vector<Object>& GetState();
};

#endif // MEMENTMAP