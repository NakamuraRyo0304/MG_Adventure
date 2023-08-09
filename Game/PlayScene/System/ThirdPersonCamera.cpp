/*
 *	@File	ThurdPersonCamera.cpp
 *	@Brief	�O�l�̎��_�̃J�����B
 *	@Date	2023-08-05
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "ThirdPersonCamera.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
ThirdPersonCamera::ThirdPersonCamera()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
ThirdPersonCamera::~ThirdPersonCamera()
{
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="pos">��_</param>
/// <param name="rotate">��]��</param>
/// <param name="distance">��_����̋���</param>
/// <returns>�Ȃ�</returns>
void ThirdPersonCamera::UpdateFollow(const SimpleMath::Vector3& pos, const SimpleMath::Quaternion& rotate,
	const SimpleMath::Vector3& distance)
{
	// �J�����̖ڐ��̕������v���C���[�̉�]�ɍ��킹�Čv�Z
	SimpleMath::Vector3 forward =
		SimpleMath::Vector3::Transform(SimpleMath::Vector3::UnitZ, rotate);

	SimpleMath::Vector3 newPos = { pos.x,pos.y - 0.5f,pos.z };

	// �J�����̈ʒu�Ɩڐ����v�Z
	SimpleMath::Vector3 eye =
		newPos + SimpleMath::Vector3::Transform(distance, rotate);
	SimpleMath::Vector3 target = eye - forward;

	// �r���[�s��
	m_followView = SimpleMath::Matrix::CreateLookAt(eye, target, SimpleMath::Vector3::Up);
}
