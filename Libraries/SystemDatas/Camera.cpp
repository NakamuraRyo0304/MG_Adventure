/*
 *	@File	Camera.cpp
 *	@Brief	�J�����̐ݒ�B
 *	@Date	2023-04-28
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "../UserUtility.h"

#include "Camera.h"

 // �萔�̏�����
const float Camera::DEFAULT_CAMERA_DISTANCE = 20.0f;

const float Camera::DEFAULT_CAMERA_SPEED = 0.05f;

// �X�N���[���l�̏���Ɖ������`
const int Camera::MAX_SCROLL_VALUE = 1440;
const int Camera::MIN_SCROLL_VALUE = -1440;

//--------------------------------------------------------//
//�R���X�g���N�^                                          //
//--------------------------------------------------------//
Camera::Camera():
	m_angle{},					// ��]�p
	m_eye{},					// �J�����̍��W
	m_eyePos{},					// �J�����̈ړ����W
	m_prevMouse{},				// �ߋ��̃}�E�X�̈ʒu
	m_scrollWheelValue{},		// �}�E�X�z�C�[���̉�]��
	m_tempScrollValue{},		// �}�E�X�z�C�[���̉�]��(�s�����̒~�ϗp)
	m_prevScrollWheelValue{},	// �}�E�X�z�C�[���̉�]��(�O��̕ۑ��p)
	m_target{},					// �����_
	m_view{},					// �r���[�s��
	m_proj{},					// �v���W�F�N�V�����s��
	is_moveMode{false},			// �J�����𓮂����t���O
	is_eagleMode{false},		// �J�����̎��_�ړ��t���O�}�E�X�z�C�[�����g�p����
	is_prevEagleMode{false},	// �J�����̎��_�ړ��t���O�}�E�X�z�C�[�����g�p����(�O��̕ۑ��p)
	m_screenHeight{},			// ��ʂ̍���
	m_screenWidth{}				// ��ʂ̕�
{
	// �}�E�X�̉�]�ʂ����Z�b�g
	DirectX::Mouse::Get().ResetScrollWheelValue();
}

//--------------------------------------------------------//
//�f�X�g���N�^                                            //
//--------------------------------------------------------//
Camera::~Camera()
{
}

//--------------------------------------------------------//
//�X�V����                                                //
//--------------------------------------------------------//
void Camera::Update()
{
	// �}�E�X�̃C���X�^���X�擾
	auto state = DirectX::Mouse::Get().GetState();

	// �}�E�X�̍��N���b�N���h���b�O�ŃJ�������W���X�V����
	if (state.leftButton)
	{
		// �}�E�X�̃h���b�O�ɂ��J�����ړ�
		DraggedDistance(state.x, state.y);
	}

	// �}�E�X�̍��W��O��̒l�Ƃ��ĕۑ�
	m_prevMouse.x = static_cast<float>(state.x); // X���W��ۑ�
	m_prevMouse.y = static_cast<float>(state.y); // Y���W��ۑ�

	// �ȉ��A�J�����̊g�嗦�̌v�Z

	// �}�E�X�z�C�[���̃X�N���[���l�̍������v�Z
	int scrollDelta = state.scrollWheelValue - m_prevScrollWheelValue;

	// �J�������[�h�̎��̏���
	if (is_eagleMode)
	{
		// �X�N���[���l���ꎞ�ϐ��ɕۑ�
		int newScrollValue = m_scrollWheelValue + scrollDelta;

		// �X�N���[���l������E�����𒴂��Ȃ��悤�ɐ���
		newScrollValue = UserUtillity::Clamp(newScrollValue, MIN_SCROLL_VALUE, MAX_SCROLL_VALUE);

		// �X�N���[���l������E�����ɒB���Ă��Ȃ��ꍇ�ɂ̂ݔ��f����
		if (newScrollValue != MAX_SCROLL_VALUE && newScrollValue != MIN_SCROLL_VALUE)
		{
			m_scrollWheelValue = newScrollValue;
		}
		else
		{
			// ����E�����ɒB�����ꍇ�̓X�N���[���l�����̂܂܂ɂ���
			scrollDelta = 0;
		}

		// �}�E�X�z�C�[���̑O���True�̒l��ێ�
		m_tempScrollValue = m_scrollWheelValue;

		// �X�N���[�����N�����v����Ă���Ԃɉ񂳂ꂽ����߂�
		if (m_scrollWheelValue == MAX_SCROLL_VALUE && scrollDelta > 0)
		{
			m_scrollWheelValue -= scrollDelta;
		}
		else if (m_scrollWheelValue == MIN_SCROLL_VALUE && scrollDelta < 0)
		{
			m_scrollWheelValue -= scrollDelta;
		}
	}
	// ��J�������[�h�̎��̏���
	else
	{
		// �t���O��False�̏ꍇ�͑O���True�̒l����
		m_scrollWheelValue = m_tempScrollValue;
	}

	// �r���[�s��̎Z�o
	CalculateViewMatrix();

	// �O��̃t���O���X�V
	is_prevEagleMode = is_eagleMode;

	// �}�E�X�z�C�[���̑O��̒l���X�V(��A�̍�Ƃ��I����Ă���X�V����)
	m_prevScrollWheelValue = state.scrollWheelValue;

}

//--------------------------------------------------------//
//�}�E�X�̈ړ������̌v�Z                                  //
//--------------------------------------------------------//
void Camera::DraggedDistance(int x, int y)
{
	// ���_�ړ����Ȃ���Ώ������Ȃ�
	if (!is_eagleMode)	return;

	// �}�E�X�|�C���^�̑O�񂩂�̕ψ�
	// DEFAULT_CAMERA_SPEED����Z���ăh���b�O�̈ړ��ʂ𒲐�����
	float dx = static_cast<float>(x - m_prevMouse.x) * DEFAULT_CAMERA_SPEED;
	float dy = static_cast<float>(y - m_prevMouse.y) * DEFAULT_CAMERA_SPEED;

	if (dx != 0.0f || dy != 0.0f)
	{
		// �}�E�X�|�C���^�̕ψʂ����ɁA�w���x���̉�]�p�����߂�
		float angleX = dy * DirectX::XM_PI / 180.0f;
		float angleY = dx * DirectX::XM_PI / 180.0f;

		// �p�x�̍X�V
		m_angle.x += angleX;
		m_angle.y += angleY;
	}
}

//-------------------------------------------------------------------
// �r���[�s��̎Z�o�ƁA�J�������W�E�����_�̎擾
//-------------------------------------------------------------------
void Camera::CalculateViewMatrix()
{
	// �r���[�s����Z�o����
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(m_angle.y);
	DirectX::SimpleMath::Matrix rotX = DirectX::SimpleMath::Matrix::CreateRotationX(m_angle.x);

	// ��]�ʂ��v�Z
	DirectX::SimpleMath::Matrix rt = rotY * rotX;

	// �|�W�V����
	DirectX::SimpleMath::Vector3    eye(0.0f, 0.1f, 1.0f);

	// �J�����̌X���i�ڐ��̊p�x�j:0.1.0�Ő��ʒu
	DirectX::SimpleMath::Vector3     up(0.0f, 1.0f, 0.0f);
	DirectX::SimpleMath::Vector3 target(m_target);

	eye = DirectX::SimpleMath::Vector3::Transform(eye, rt.Invert());
	eye *= (DEFAULT_CAMERA_DISTANCE - static_cast<float>(m_scrollWheelValue / 100));
	up  = DirectX::SimpleMath::Vector3::Transform(up,  rt.Invert());

	// �f�t�H���g�̏����ʒu
	m_eye = eye;
	m_target = target;
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(eye, target, up);
}
//--------------------------------------------------------//
//�J������h�炷�֐�                                      //
//--------------------------------------------------------//
// ��P�����F��������(60FPS = 1sec) / ��Q�����F�h�ꕝ / ��R�����F�Ώۂ̃|�W�V����
void Camera::ShakeCamera(float duration, float tremor, DirectX::SimpleMath::Vector3* pos)
{
	int counta = 0;
	counta++;

	float d = (rand() % 10) * 0.01f * tremor;

	DirectX::SimpleMath::Vector3 sav = (*pos);

	if (counta > duration)
	{
		(*pos) = sav;
	}
	else
	{
		(*pos).x += d * (rand() % 3 - 1);
		(*pos).y += d * (rand() % 3 - 1);
		(*pos).z += d * (rand() % 3 - 1);
	}
}


//--------------------------------------------------------//
//�ˉe�s��̍쐬�Ǝ擾                                    //
//--------------------------------------------------------//
// ��P�����F��ʉ��� / ��Q�����F��ʏc�� / ��R�����F�J������p(float�l��ϊ��Ȃ��œn��)
const DirectX::SimpleMath::Matrix& Camera::GetProjection(float width, float height,float angle)
{
	// ��ʃT�C�Y�ƃA���O���̕ۑ�
	m_screenWidth = static_cast<int>(width);
	m_screenHeight = static_cast<int>(height);

	// ��p
	float fieldOfView = DirectX::XMConvertToRadians(angle);
	
	// ��ʏc����
	float aspectRatio = width / height;
	
	// �J���������ԋ߂����e��
	float nearPlane = 1.0f;

	// �J���������ԉ������e��
	float farPlane = 100.0f;
	
	// �J�����̃����Y�̍쐬
	DirectX::SimpleMath::Matrix projection = 
	DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
			fieldOfView, 
			aspectRatio, 
			nearPlane, 
			farPlane
	);

	// �J�������Ŏg���ϐ�
	m_proj = projection;

	// �v���W�F�N�V�����s���ԋp
	return m_proj;
}