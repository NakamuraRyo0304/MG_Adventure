/*
 *	@File	LeftLeg.h
 *	@Brief	�v���C���[�̍����B
 *	@Date	2023-01-19
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef LEFTLEG
#define LEFTLEG

 // ���N���X
#include "../IParts.h"

class LeftLeg : public IParts
{
private:


public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�Ȃ�</returns>
	LeftLeg(std::unique_ptr<DirectX::Model> model);
	~LeftLeg();

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Update() override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="states">�R�����X�e�[�g�|�C���^</param>
	/// <param name="view">�r���[�s��</param>
	/// <param name="proj">�v���W�F�N�V�����s��</param>
	/// <returns>�Ȃ�</returns>
	void Draw(DirectX::CommonStates& states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) override;

};

#endif // LEFTLEG