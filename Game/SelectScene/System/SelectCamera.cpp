/*
 *	@File	SelectCamera.cpp
 *	@Brief	�Z���N�g�V�[���̃J�����N���X�B
 *	@Date	2023-11-11
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/UserUtility.h"
#include "SelectCamera.h"

// �R���X�g���N�^
SelectCamera::SelectCamera(const SimpleMath::Vector2& screenSize)
	: m_view{}						// �r���[�s��
	, m_projection{}				// �v���W�F�N�V�����s��
	, m_position{}					// �J�������W
	, m_target{}					// �����_
	, m_screenSize{ screenSize }	// �X�N���[���T�C�Y
	, is_canChangeFlag{ false }		// ��ʐ؂�ւ��\���𔻒�
{
	CreateProjection();

	m_position.y = POS_Y;
	m_target.x = UP_VALUE;
}

// �f�X�g���N�^
SelectCamera::~SelectCamera()
{
}

// �X�V����
void SelectCamera::Update()
{
	// �؂�ւ��\�Ȃ�True�ɂ���
	is_canChangeFlag = (m_target.x >= UP_VALUE * UP_SPAN);

	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// �t�H���g�܂ňړ�����
	m_target.x = UserUtility::Lerp(m_target.x, 0.0f, APPROACH_SPEED);

	// ��]�ʂ��v�Z
	float _rotValueX = CAMERA_RADIUS * sinf(_timer * 0.5f);
	float _rotValueZ = CAMERA_RADIUS * cosf(_timer * 0.5f);
	// �㉺�ړ��ʂ��v�Z(01�N�����v�~�����̑傫��)
	float _transValueY = m_position.y + (sinf(_timer) + 1.0f) * VERTICAL_MOTION;

	// �r���[�s��
	SimpleMath::Vector3 _eye(_rotValueX, _transValueY, _rotValueZ);
	m_view = SimpleMath::Matrix::CreateLookAt(_eye, m_target, SimpleMath::Vector3::Up);
}

// �^�[�Q�b�g�̍X�V
void SelectCamera::MoveTarget()
{
	m_target.x = UserUtility::Lerp(m_target.x, UP_VALUE, UP_SPEED);
}

// �ˉe�s����쐬
void SelectCamera::CreateProjection()
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
