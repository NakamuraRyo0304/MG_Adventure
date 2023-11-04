/*
 *	@File	Stage.h
 *	@Brief	�^�C�g���̃X�e�[�W�B
 *	@Date	2023-11-03
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef STAGE
#define STAGE

#include "ITitleObject.h"

class Stage : public ITitleObject
{
private:

	std::unique_ptr<DirectX::Model> m_model;

	// �I��ύX���̉���
	float m_accelerate;
	bool is_accelerateFlag;
	const float MAX_ACCELERATE_TIME = 6.0f;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="factory">�t�@�N�g���}�l�[�W��</param>
	/// <param name="path">�^�C�g�����S���f���̃p�X</param>
	/// <returns>�Ȃ�</returns>
	Stage(std::shared_ptr<FactoryManager> factory, const wchar_t* path);
	~Stage();

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Update() override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="states">�R�����X�e�[�g</param>
	/// <param name="view">�r���[�s��</param>
	/// <param name="proj">�ˉe�s��</param>
	/// <returns>�Ȃ�</returns>
	void Render(DirectX::CommonStates& states,
		const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;

	/// <summary>
	/// �I������
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Finalize() override;
public:

	// ����������
	void AddAccelerate() { is_accelerateFlag = true; }
};

#endif // STAGE