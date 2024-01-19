/*
 *	@File	Body.h
 *	@Brief	�v���C���[�̐g�́B
 *	@Date	2023-01-19
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef BODY
#define BODY

 // ���N���X
#include "../IParts.h"

class Body : public IParts
{
private:

	struct Parameter
	{
		DirectX::SimpleMath::Vector3 position;		// ���W
		DirectX::SimpleMath::Vector3 velocity;		// �ړ���
		DirectX::SimpleMath::Quaternion rotation;		// �����Ă������
		float gravity;								// �d��
		float accelerate;							// �����x
		void reset()								// ���Z�b�g�֐�
		{
			position = DirectX::SimpleMath::Vector3::Zero;
			velocity = DirectX::SimpleMath::Vector3::Zero;
			gravity = 0.0f;
			accelerate = 0.0f;
			rotation = DirectX::SimpleMath::Quaternion::Identity;
		}
	} m_param;

	// �ړ����x
	const float MOVE_SPEED = 0.01f;

	// ��]���x
	const float ROT_SPEED = 0.05f;

	// ���C�W��
	const float DECELERATION = 0.851f;

	// �v���C���̃I�t�Z�b�g
	const float OFFSET_Y = 0.2f;
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�Ȃ�</returns>
	Body(std::unique_ptr<DirectX::Model> model);
	~Body();

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

public:

	// �p�����[�^�̃��Z�b�g
	void ResetAll();

	// ���W�̎擾
	const DirectX::SimpleMath::Vector3& GetPosition() { return m_param.position; }
	// ���W�̐ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_param.position = pos; }
	// �d�͂̎擾
	const float& GetGravity() { return m_param.gravity; }
	// �d�͂�0�ɂ���
	void ZeroGravity() { m_param.gravity = 0.0f; }
	// �d�͂̐ݒ�
	void SetGravity(const float& gravity) { m_param.gravity = gravity; }
	// ��]�̎擾
	const DirectX::SimpleMath::Quaternion& GetRotation() { return m_param.rotation; }
};

#endif // BODY