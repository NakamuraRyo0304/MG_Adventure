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
	: IGameCamera(screenSize)		// ���ʕ�����������
	, is_canChangeFlag{ false }		// ��ʐ؂�ւ��\���𔻒�
{
	SetPosition(SimpleMath::Vector3(0.0f, POS_Y, 0.0f));
	SetTarget(SimpleMath::Vector3(LOOK_VALUE, 0.0f, 0.0f));
	SetInitialPosition(GetPosition());
}

// �f�X�g���N�^
SelectCamera::~SelectCamera()
{
}

// �X�V����
void SelectCamera::Update()
{
	// �؂�ւ��\�Ȃ�True�ɂ���
	is_canChangeFlag = (GetTarget().x >= LOOK_VALUE * LOOK_SPAN);

	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// �t�H���g�܂ňړ�����
	SetTarget(SimpleMath::Vector3(
		UserUtility::EaseInOutLerp(GetTarget().x, 0.0f, APPROACH_SPEED),
		GetTarget().y,
		GetTarget().z)
	);

	// ��]�ʂ��v�Z
	float _rotValueX = CAMERA_RADIUS * sinf(_timer * 0.5f);
	float _rotValueZ = CAMERA_RADIUS * cosf(_timer * 0.5f);
	// �㉺�ړ��ʂ��v�Z(01�N�����v�~�����̑傫��)
	float _transValueY = (sinf(_timer) + 1.0f) * VERTICAL_MOTION;

	// �r���[�s��
	SetPosition(SimpleMath::Vector3(_rotValueX, _transValueY + GetInitialPosition().y, _rotValueZ));
	SetView(SimpleMath::Matrix::CreateLookAt(GetPosition(), GetTarget(), GetUp()));
}

// �^�[�Q�b�g�̍X�V
void SelectCamera::MoveTarget()
{
	SetTarget(SimpleMath::Vector3(
		UserUtility::Lerp(GetTarget().x, LOOK_VALUE, LOOK_SPEED),
		GetTarget().y, GetTarget().z));
}
