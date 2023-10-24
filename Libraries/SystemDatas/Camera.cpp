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
Camera::Camera()
	: m_screenHeight{}			// ��ʂ̍���
	, m_screenWidth{}			// ��ʂ̕�
	, m_angle{}					// ��]�p
	, m_position{}				// �J�����̍��W
	, m_addPos{}				// �J�������Z�p�̍��W
	, m_prevMouse{}				// �ߋ��̃}�E�X�̈ʒu
	, m_scrollWheelValue{}		// �}�E�X�z�C�[���̉�]��
	, m_tempScrollValue{}		// �}�E�X�z�C�[���̉�]��(�s�����̒~�ϗp)
	, m_prevScrollWheelValue{}	// �}�E�X�z�C�[���̉�]��(�O��̕ۑ��p)
	, m_view{}					// �r���[�s��
	, m_projection{}			// �v���W�F�N�V�����s��
	, m_rotateMatrix{}			// ��]��
	, is_allowMode{ false }		// �J�����̎��_�ړ��t���O(�\������)
	, is_eagleMode{ false }		// �J�����̎��_�ړ��t���O(�}�E�X����)
	, is_prevEagleMode{ false }	// �J�����̎��_�ړ��t���O(�O��̕ۑ��p)
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
	auto _kbState = Keyboard::Get().GetState();
	// �}�E�X�̃C���X�^���X�擾
	auto _msState = Mouse::Get().GetState();

	// �}�E�X�̍��N���b�N���h���b�O�ŃJ�������W���X�V����
	if (_msState.leftButton)
	{
		// ���_�ړ����Ȃ���Ώ������Ȃ�
		if (!is_eagleMode)	return;

		// �}�E�X�̃h���b�O�ɂ��J�����ړ�
		DraggedDistance(_msState.x, _msState.y);
	}

	// �}�E�X�̍��W��O��̒l�Ƃ��ĕۑ�
	m_prevMouse.x = static_cast<float>(_msState.x); // X���W��ۑ�
	m_prevMouse.y = static_cast<float>(_msState.y); // Y���W��ۑ�

	// �g�k����
	RollWheelToRate(_msState);

	// �r���[�s��̎Z�o
	CalculateViewMatrix();

	// �\���X�C�b�`���I�t�Ȃ瓮�����Ȃ�
	if (!is_allowMode) return;

	// �ړ���
	SimpleMath::Vector2 _moveVal = SimpleMath::Vector2::Zero;

	// �ړ��ʂ�ݒ�
	if (_kbState.Right) { _moveVal.y =  ALLOW_SPEED; }
	if (_kbState.Left)  { _moveVal.y = -ALLOW_SPEED; }
	if (_kbState.Up)    { _moveVal.x =  ALLOW_SPEED; }
	if (_kbState.Down)  { _moveVal.x = -ALLOW_SPEED; }

	// �p�x�ύX
	m_angle += _moveVal;
}

/// <summary>
/// �}�E�X�̃h���b�O�����������v�Z
/// </summary>
/// <param name="x">�X�N���[�����WX</param>
/// <param name="y">�X�N���[�����WY</param>
/// <returns>�Ȃ�</returns>
void Camera::DraggedDistance(int x, int y)
{
	// �}�E�X�|�C���^�̑O�񂩂�̕ψ�
	// DEFAULT_CAMERA_SPEED����Z���ăh���b�O�̈ړ��ʂ𒲐�����
	float _dx = static_cast<float>(x - m_prevMouse.x) * DEFAULT_CAMERA_SPEED;
	float _dy = static_cast<float>(y - m_prevMouse.y) * DEFAULT_CAMERA_SPEED;

	if (_dx != 0.0f || _dy != 0.0f)
	{
		// �}�E�X�|�C���^�̕ψʂ����ɁA�w���x���̉�]�p�����߂�
		float _angleX = _dy * XM_PI / 180.0f;
		float _angleY = _dx * XM_PI / 180.0f;

		// �p�x�̍X�V
		m_angle.x += _angleX;
		m_angle.y += _angleY;
	}

	// �J�����̊p�x���N�����v
	m_angle.x = UserUtility::Clamp(m_angle.x, ANGLE_X_MIN, ANGLE_X_MAX);

}

/// <summary>
/// �I�u�W�F�N�g�̐U������
/// </summary>
/// <param name="duration">�h��p������</param>
/// <param name="tremor">�h�ꕝ</param>
/// <param name="pos">�ΏۃI�u�W�F�̃|�W�V����</param>
/// <returns>�Ȃ�</returns>
void Camera::ShakeObject(float duration, float tremor, SimpleMath::Vector3* pos)
{
	int _counta = 0;
	_counta++;

	float _d = (rand() % 10) * 0.01f * tremor;

	SimpleMath::Vector3 _sav = (*pos);

	if (_counta > duration)
	{
		(*pos) = _sav;
	}
	else
	{
		(*pos).x += _d * (rand() % 3 - 1);
		(*pos).y += _d * (rand() % 3 - 1);
		(*pos).z += _d * (rand() % 3 - 1);
	}
}


/// <summary>
/// �ˉe�s����쐬
/// </summary>
/// <param name="width">��ʉ���</param>
/// <param name="height">��ʏc��</param>
/// <param name="angle">�J������p</param>
/// <returns>�ˉe�s��</returns>
const SimpleMath::Matrix& Camera::CreateProjection(float width, float height, float angle)
{
	// ��ʃT�C�Y�ƃA���O���̕ۑ�
	m_screenWidth  = static_cast<int>(width);
	m_screenHeight = static_cast<int>(height);

	// ��p
	float _fieldOfView = XMConvertToRadians(angle);

	// ��ʏc����
	float _aspectRatio = width / height;

	// �J���������ԋ߂����e��
	float _nearPlane = NEAR_PLANE;

	// �J���������ԉ������e��
	float _farPlane = FAR_PLANE;

	// �J�����̃����Y�̍쐬
	SimpleMath::Matrix _projection =
		SimpleMath::Matrix::CreatePerspectiveFieldOfView(
			_fieldOfView,
			_aspectRatio,
			_nearPlane,
			_farPlane
		);

	// �J������p
	m_angle.x = angle;

	// �v���W�F�N�V�����s���ԋp
	return m_projection = _projection;
}

/// <summary>
/// �r���[�s��v�Z
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void Camera::CalculateViewMatrix()
{
	// �r���[�s����Z�o����
	SimpleMath::Matrix _rotY = SimpleMath::Matrix::CreateRotationY(m_angle.y);
	SimpleMath::Matrix _rotX = SimpleMath::Matrix::CreateRotationX(m_angle.x);

	// ��]�ʂ��v�Z
	SimpleMath::Matrix _rt = _rotY * _rotX;
	m_rotateMatrix = _rt;

	// �|�W�V����
	SimpleMath::Vector3    _eye(0.0f, 0.1f, 1.0f);

	// �J�����̌X���i�ڐ��̊p�x)
	SimpleMath::Vector3     _up(0.0f, 1.0f, 0.0f);
	SimpleMath::Vector3 _target(0.0f, 0.0f, 0.0f);

	_eye = SimpleMath::Vector3::Transform(_eye, _rt.Invert());
	_eye *= (DEFAULT_CAMERA_DISTANCE - static_cast<float>(m_scrollWheelValue / 100));
	_up  = SimpleMath::Vector3::Transform(_up,  _rt.Invert());

	// �f�t�H���g�̏����ʒu
	m_position = _eye;
	m_target = _target;
	m_view = SimpleMath::Matrix::CreateLookAt(_eye + m_addPos, _target + m_addPos, _up);

	// �㉺��]�̂ݐ���
	m_angle.x = UserUtility::Clamp(m_angle.x, ANGLE_X_MIN, ANGLE_X_MAX);
}

/// <summary>
/// �J�����̊g�嗦�̕ύX
/// </summary>
/// <param name="state">�}�E�X�̃X�e�[�g</param>
/// <returns>�Ȃ�</returns>
void Camera::RollWheelToRate(Mouse::State state)
{
	// �}�E�X�z�C�[���̃X�N���[���l�̍������v�Z
	int _scrollDelta = state.scrollWheelValue - m_prevScrollWheelValue;

	// �J�������[�h�̎��̏���
	if (is_eagleMode)
	{
		// �X�N���[���l���ꎞ�ϐ��ɕۑ�
		int _newScrollValue = m_scrollWheelValue + _scrollDelta;

		// �X�N���[���l������E�����𒴂��Ȃ��悤�ɐ���
		_newScrollValue = UserUtility::Clamp(_newScrollValue, MIN_SCROLL_VALUE, MAX_SCROLL_VALUE);

		// �X�N���[���l������E�����ɒB���Ă��Ȃ��ꍇ�ɂ̂ݔ��f����
		if (_newScrollValue != MAX_SCROLL_VALUE && _newScrollValue != MIN_SCROLL_VALUE)
		{
			m_scrollWheelValue = _newScrollValue;
		}
		else
		{
			// ����E�����ɒB�����ꍇ�̓X�N���[���l�����̂܂܂ɂ���
			_scrollDelta = 0;
		}

		// �}�E�X�z�C�[���̑O���True�̒l��ێ�
		m_tempScrollValue = m_scrollWheelValue;

		// �X�N���[�����N�����v����Ă���Ԃɉ񂳂ꂽ����߂�
		if (m_scrollWheelValue == MAX_SCROLL_VALUE && _scrollDelta > 0)
		{
			m_scrollWheelValue -= _scrollDelta;
		}
		else if (m_scrollWheelValue == MIN_SCROLL_VALUE && _scrollDelta < 0)
		{
			m_scrollWheelValue -= _scrollDelta;
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
