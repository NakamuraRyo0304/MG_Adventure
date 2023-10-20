/*
 *	@File	PlayCamera.cpp
 *	@Brief	�ŏ��̃J�������o�B
 *	@Date	2023-10-03
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "PlayCamera.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="��������"></param>
 /// <returns>�Ȃ�</returns>
PlayCamera::PlayCamera(const SimpleMath::Vector2& screenSize)
	: m_view{}						// �r���[�s��
	, m_projection{}				// �v���W�F�N�V�����s��
	, m_position{}					// �J�������W
	, m_target{}					// �����_
	, m_screenSize{ screenSize }	// �X�N���[���T�C�Y
{
	CreateProjection();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
PlayCamera::~PlayCamera()
{
}

/// <summary>
/// �r���[���쐬
/// </summary>
/// <param name="��������"></param>
/// <returns>�r���[�s��</returns>
const SimpleMath::Matrix& PlayCamera::CreateView()
{
	m_view = SimpleMath::Matrix::CreateLookAt(
		m_position,
		m_target,
		SimpleMath::Vector3::UnitY
	);

	return m_view;
}

/// <summary>
/// �v���W�F�N�V�������쐬
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void PlayCamera::CreateProjection()
{
	// ��p
	float _fieldOfView = XMConvertToRadians(ANGLE);

	// ��ʏc����
	float _aspectRatio = m_screenSize.x / m_screenSize.y;

	// �J�����̃����Y�̍쐬
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		_fieldOfView,		// �J�����̉�p
		_aspectRatio,		// �A�X�y�N�g��
		NEAR_PLANE,			// �ŋߋ���
		FAR_PLANE			// �ŉ�����
	);
}
