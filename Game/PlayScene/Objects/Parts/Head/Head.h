/*
 *	@File	Head.h
 *	@Brief	�v���C���[�̓��B
 *	@Date	2023-01-18
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef HEAD
#define HEAD

// ���N���X
#include "../IParts.h"

class Head : public IParts
{
private:

	// �O��ړ���
	float m_moveFB;

	// ��̉�]
	DirectX::SimpleMath::Vector2 m_rotate;
	DirectX::SimpleMath::Quaternion m_normalRotate;

	// ���̑O�㑬�x
	const float FB_SPEED = 0.25f;

	// ��̉�]���x
	const float NECK_ROT_SPEED = 0.30f;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�Ȃ�</returns>
	Head(std::unique_ptr<DirectX::Model> model);
	~Head();

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

#endif // HEAD