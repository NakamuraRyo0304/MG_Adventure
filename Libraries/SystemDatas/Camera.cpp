/*
 *	@File	Camera.cpp
 *	@Brief	�J�����̐ݒ�B
 *	@Date	2023-04-28
 *  @Author NakamuraRyo
 */

#include "pch.h"

// ���[�U�[���[�e�B���e�B
#include "../UserUtility.h"

#include "Camera.h"

 // �萔�̏�����
const float Camera::DEFAULT_CAMERA_DISTANCE = 20.0f;

const float Camera::DEFAULT_CAMERA_SPEED = 0.05f;

// �X�N���[���l�̏���Ɖ������`
const int Camera::MAX_SCROLL_VALUE = 1640;
const int Camera::MIN_SCROLL_VALUE = -1440;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
Camera::Camera():
	m_angle{},					// ��]�p
	m_eye{},					// �J�����̍��W
	m_eyePos{},					// �J�����̈ړ����W
	m_prevMouse{},				// �ߋ��̃}�E�X�̈ʒu
	m_scrollWheelValue{},		// �}�E�X�z�C�[���̉�]��
	m_tempScrollValue{},		// �}�E�X�z�C�[���̉�]��(�s�����̒~�ϗp)
	m_prevScrollWheelValue{},	// �}�E�X�z�C�[���̉�]��(�O��̕ۑ��p)
	m_view{},					// �r���[�s��
	m_proj{},					// �v���W�F�N�V�����s��
	is_eagleMode{false},		// �J�����̎��_�ړ��t���O�}�E�X�z�C�[�����g�p����
	is_prevEagleMode{false},	// �J�����̎��_�ړ��t���O�}�E�X�z�C�[�����g�p����(�O��̕ۑ��p)
	m_screenHeight{},			// ��ʂ̍���
	m_screenWidth{},			// ��ʂ̕�
	m_rot{}						// ��]��
{
	// �}�E�X�̉�]�ʂ����Z�b�g
	Mouse::Get().ResetScrollWheelValue();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
Camera::~Camera()
{
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void Camera::Update()
{
	// �L�[�{�[�h�̃C���X�^���X�擾
	auto kbState = Keyboard::Get().GetState();
	// �}�E�X�̃C���X�^���X�擾
	auto msState = Mouse::Get().GetState();

	// �}�E�X�̍��N���b�N���h���b�O�ŃJ�������W���X�V����
	if (msState.leftButton)
	{
		// �}�E�X�̃h���b�O�ɂ��J�����ړ�
		DraggedDistance(msState.x, msState.y);
	}

	// �}�E�X�̍��W��O��̒l�Ƃ��ĕۑ�
	m_prevMouse.x = static_cast<float>(msState.x); // X���W��ۑ�
	m_prevMouse.y = static_cast<float>(msState.y); // Y���W��ۑ�

	// �g�k����
	RollWheelToRate(msState);

	// �r���[�s��̎Z�o
	CalculateViewMatrix();

	// �㉺��]�̂ݐ���
	m_angle.x = UserUtility::Clamp(m_angle.x, ANGLEX_MIN, ANGLEX_MAX);

	// �ړ���
	SimpleMath::Vector2 moveVal = SimpleMath::Vector2::Zero;

	// �ړ��ʂ�ݒ�
	if (kbState.Right){ moveVal.y =  0.01f; }
	if (kbState.Left) { moveVal.y = -0.01f; }
	if (kbState.Up)   { moveVal.x =  0.01f; }
	if (kbState.Down) { moveVal.x = -0.01f; }

	// �p�x�ύX
	m_angle += moveVal;
}

/// <summary>
/// �}�E�X�̃h���b�O�����������v�Z
/// </summary>
/// <param name="x">�X�N���[�����WX</param>
/// <param name="y">�X�N���[�����WY</param>
/// <returns>�Ȃ�</returns>
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
		float angleX = dy * XM_PI / 180.0f;
		float angleY = dx * XM_PI / 180.0f;

		// �p�x�̍X�V
		m_angle.x += angleX;
		m_angle.y += angleY;
	}

	// �J�����̊p�x���N�����v
	m_angle.x = UserUtility::Clamp(m_angle.x, ANGLEX_MIN, ANGLEX_MAX);

}

/// <summary>
/// �r���[�s��v�Z
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void Camera::CalculateViewMatrix()
{
	// �r���[�s����Z�o����
	SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(m_angle.y);
	SimpleMath::Matrix rotX = SimpleMath::Matrix::CreateRotationX(m_angle.x);

	// ��]�ʂ��v�Z
	SimpleMath::Matrix rt = rotY * rotX;
	m_rot = rt;

	// �|�W�V����
	SimpleMath::Vector3    eye(0.0f, 0.1f, 1.0f);

	// �J�����̌X���i�ڐ��̊p�x)
	SimpleMath::Vector3     up(0.0f, 1.0f, 0.0f);
	SimpleMath::Vector3 target(0.0f, 0.0f, 0.0f);

	eye = SimpleMath::Vector3::Transform(eye, rt.Invert());
	eye *= (DEFAULT_CAMERA_DISTANCE - static_cast<float>(m_scrollWheelValue / 100));
	up  = SimpleMath::Vector3::Transform(up,  rt.Invert());

	// �f�t�H���g�̏����ʒu
	m_eye = eye;
	m_target = target;
	m_view = SimpleMath::Matrix::CreateLookAt(eye, target, up);

}

/// <summary>
/// �J�����̐U������
/// </summary>
/// <param name="duration">�h��p������</param>
/// <param name="tremor">�h�ꕝ</param>
/// <param name="pos">�ΏۃI�u�W�F�̃|�W�V����</param>
/// <returns>�Ȃ�</returns>
void Camera::ShakeCamera(float duration, float tremor, SimpleMath::Vector3* pos)
{
	int counta = 0;
	counta++;

	float d = (rand() % 10) * 0.01f * tremor;

	SimpleMath::Vector3 sav = (*pos);

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

/// <summary>
/// �ˉe�s����쐬
/// </summary>
/// <param name="width">��ʉ���</param>
/// <param name="height">��ʏc��</param>
/// <param name="angle">�J������p</param>
/// <returns>�ˉe�s��</returns>
const SimpleMath::Matrix& Camera::CreateProjection(float width, float height,float angle)
{
	// ��ʃT�C�Y�ƃA���O���̕ۑ�
	m_screenWidth = static_cast<int>(width);
	m_screenHeight = static_cast<int>(height);

	// ��p
	float fieldOfView = XMConvertToRadians(angle);
	
	// ��ʏc����
	float aspectRatio = width / height;
	
	// �J���������ԋ߂����e��
	float nearPlane = NEAR_PLANE;

	// �J���������ԉ������e��
	float farPlane = FAR_PLANE;
	
	// �J�����̃����Y�̍쐬
	SimpleMath::Matrix projection = 
	SimpleMath::Matrix::CreatePerspectiveFieldOfView(
			fieldOfView, 
			aspectRatio, 
			nearPlane, 
			farPlane
	);

	m_angle.x = angle;

	// �J�������Ŏg���ϐ�
	m_proj = projection;

	// �v���W�F�N�V�����s���ԋp
	return m_proj;
}

/// <summary>
/// �J��������̋������v�Z
/// </summary>
/// <param name="objPos">�ΏۃI�u�W�F�̃|�W�V����</param>
/// <returns>�J��������Ώە��܂ł̋���</returns>
float Camera::CalculateDistanceToObject(const SimpleMath::Vector3& objPos)
{
	SimpleMath::Vector3 distanceVector = objPos - m_eye;
	float distance = distanceVector.Length();

	return distance;
}

/// <summary>
/// �J�����̊g�嗦�̕ύX
/// </summary>
/// <param name="state">�}�E�X�̃X�e�[�g</param>
/// <returns>�Ȃ�</returns>
void Camera::RollWheelToRate(Mouse::State state)
{
	// �}�E�X�z�C�[���̃X�N���[���l�̍������v�Z
	int scrollDelta = state.scrollWheelValue - m_prevScrollWheelValue;

	// �J�������[�h�̎��̏���
	if (is_eagleMode)
	{
		// �X�N���[���l���ꎞ�ϐ��ɕۑ�
		int newScrollValue = m_scrollWheelValue + scrollDelta;

		// �X�N���[���l������E�����𒴂��Ȃ��悤�ɐ���
		newScrollValue = UserUtility::Clamp(newScrollValue, MIN_SCROLL_VALUE, MAX_SCROLL_VALUE);

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

	// �O��̃t���O���X�V
	is_prevEagleMode = is_eagleMode;

	// �}�E�X�z�C�[���̑O��̒l���X�V(��A�̍�Ƃ��I����Ă���X�V����)
	m_prevScrollWheelValue = state.scrollWheelValue;
}
