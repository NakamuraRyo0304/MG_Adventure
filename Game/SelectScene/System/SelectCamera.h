/*
 *	@File	SelectCamera.h
 *	@Brief	�Z���N�g�V�[���̃J�����N���X�B
 *	@Date	2023-11-11
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SELECTCAMERA
#define SELECTCAMERA

class SelectCamera
{
private:

	// �J�����s��
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_projection;

	// �J�����|�W�V����
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_target;

	// �X�N���[���T�C�Y
	DirectX::SimpleMath::Vector2 m_screenSize;

	// �؂�ւ��\�����肷��
	bool is_canChangeFlag;

private:

	// �J�����̍ŋߋ���
	const float NEAR_PLANE = 0.1f;

	// �J�����̍ŉ�����
	const float FAR_PLANE = 245.0f;

	// �J�����A���O��
	const float ANGLE = 45.0f;

	// �J������Y���W
	const float POS_Y = 30.0f;

	// �J�����̉�]���a
	const float CAMERA_RADIUS = 4.0f;

	// �ڋߑ��x
	const float APPROACH_SPEED = 0.045f;

	// �㉺����
	const float VERTICAL_MOTION = 1.5f;

	// ���_�̓��쑬�x
	const float UP_VALUE = 60.0f;
	const float UP_SPAN = 0.25f;
	const float UP_SPEED = 0.8f;

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="screenSize">�X�N���[���T�C�Y</param>
	/// <returns>�Ȃ�</returns>
	SelectCamera(const DirectX::SimpleMath::Vector2& screenSize);
	~SelectCamera();

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Update();
	/// <summary>
	/// �J�����̃^�[�Q�b�g�̍X�V
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void MoveTarget();
	// �؂�ւ��\�������Ԃ�
	const bool& IsCanChange(){ return is_canChangeFlag;}
	// �r���[�s����擾
	const DirectX::SimpleMath::Matrix& GetView() { return m_view; }
	// �v���W�F�N�V�����s����擾
	const DirectX::SimpleMath::Matrix& GetProjection() { return m_projection; }
	// ���W���擾
	const DirectX::SimpleMath::Vector3& GetPosition() { return m_position; }
	// ���W��ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector3& pos = { 0.f,1.f,1.f }) { m_position = pos; }
	// �����_���擾
	const DirectX::SimpleMath::Vector3& GetTarget() { return m_target; }
	// �����_��ݒ�
	void SetTarget(const DirectX::SimpleMath::Vector3& target = DirectX::SimpleMath::Vector3::Zero) { m_target = target; }

private:

	/// <summary>
	/// �v���W�F�N�V�������쐬
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void CreateProjection();

};

#endif // SELECTCAMERA