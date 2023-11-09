/*
 *	@File	PlayCamera.cpp
 *	@Brief	�ŏ��̃J�������o�B
 *	@Date	2023-10-03
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "PlayCamera.h"

// �R���X�g���N�^
PlayCamera::PlayCamera(const SimpleMath::Vector2& screenSize)
	: m_view{}						// �r���[�s��
	, m_projection{}				// �v���W�F�N�V�����s��
	, m_position{}					// �J�������W
	, m_target{}					// �����_
	, m_screenSize{ screenSize }	// �X�N���[���T�C�Y
{
	CreateProjection();
}

// �f�X�g���N�^
PlayCamera::~PlayCamera()
{
}

// �r���[�s����쐬
const SimpleMath::Matrix& PlayCamera::CreateView()
{
	m_view = SimpleMath::Matrix::CreateLookAt(
		m_position,
		m_target,
		SimpleMath::Vector3::UnitY
	);

	return m_view;
}

// �ˉe�s����쐬
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
