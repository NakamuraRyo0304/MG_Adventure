/*
 *	@File	Camera.cpp
 *	@Brief	�J�����̐ݒ�B
 *	@Date	2023-04-28
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Camera.h"

 // �萔�̏�����
const float Camera::DEFAULT_CAMERA_DISTANCE = 5.0f;

const float Camera::DEFAULT_CAMERA_SPEED = 0.05f;

//--------------------------------------------------------//
//�R���X�g���N�^                                          //
//--------------------------------------------------------//
Camera::Camera():
	m_angle{},					// ��]�p
	m_eye{},					// �J�����̍��W
	m_eyePos{},					// �J�����̈ړ����W
	m_prevMouse{},				// �ߋ��̃}�E�X�̈ʒu
	m_viewAngle{},				// ��ʂ̉�p
	m_scrollWheelValue{},		// �}�E�X�z�C�[���̉�]��
	m_target{},					// �����_
	m_view{},					// �r���[�s��
	m_proj{},					// �v���W�F�N�V�����s��
	is_moveMode{false},			// �J�����𓮂����t���O
	is_eagleMode{false},		// �J�����̎��_�ړ��t���O
	m_screenHeight{},			// ��ʂ̍���
	m_screenWidth{}				// ��ʂ̕�
{
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
		DraggedDistance(state.x, state.y);
	}

	// �}�E�X�̍��W��O��̒l�Ƃ��ĕۑ�
	m_prevMouse.x = state.x;
	m_prevMouse.y = state.y;

	// �}�E�X�z�C�[���̃X�N���[���l���擾
	m_scrollWheelValue = state.scrollWheelValue;
	if (m_scrollWheelValue > 0)
	{
		m_scrollWheelValue = 0;
		DirectX::Mouse::Get().ResetScrollWheelValue();
	}

	// �r���[�s��̎Z�o
	CalculateViewMatrix();
}

//--------------------------------------------------------//
//�}�E�X�̈ړ������̌v�Z                                  //
//--------------------------------------------------------//
void Camera::DraggedDistance(int x, int y)
{
	// ���_�ړ����Ȃ���Ώ������Ȃ�
	if (!is_eagleMode)return;

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

	// �s��̏�����
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


	// �ړ����[�h����Ȃ���Ώ������Ȃ�----------------------------------------------------
	if (!is_moveMode)return;

	// �L�[�{�[�h�̎擾
	auto keyState = DirectX::Keyboard::Get().GetState();

	// �ړ����[�h�N�����Ȃ�J�����ړ�
	MoveCamera(eye, up, keyState);
}

//--------------------------------------------------------//
//�J�����𓮂���                                          //
//--------------------------------------------------------//
// ��P�����F�J�����̃����Y�̈ʒu / ��Q�����F�J�����̌X�� / ��R�����F�L�[�{�[�h
void Camera::MoveCamera(DirectX::SimpleMath::Vector3 eye,DirectX::SimpleMath::Vector3 up,DirectX::Keyboard::State keyState)
{
	// �J�����̕����x�N�g�����琳�ʕ����x�N�g�����擾����
	DirectX::SimpleMath::Vector3 front = DirectX::SimpleMath::Vector3(-eye.x, 0.0f, -eye.z);
	DirectX::SimpleMath::Vector3 updo = DirectX::SimpleMath::Vector3(0.0f, -eye.y, 0.0f);
	
	// �����x�N�g���𐳋K��
	front.Normalize();
	updo.Normalize();

	// ���K���x�N�g�������������Ĉړ��x�N�g���ɕϊ�
	DirectX::SimpleMath::Vector3 moveW = front * DEFAULT_CAMERA_SPEED;
	DirectX::SimpleMath::Vector3 moveH = updo * DEFAULT_CAMERA_SPEED;

	// �O��ړ�
	if (keyState.W)
	{
		m_target += moveW;
		m_eyePos += moveW;
	}
	if (keyState.S)
	{
		m_target -= moveW;
		m_eyePos -= moveW;
	}

	// ���E�ړ�
	if (keyState.A)
	{
		DirectX::SimpleMath::Vector3 mX = DirectX::SimpleMath::Vector3::Zero;
		mX.x = moveW.z;
		mX.z = -moveW.x;
		m_target += mX;
		m_eyePos += mX;
	}
	if (keyState.D)
	{
		DirectX::SimpleMath::Vector3 mX = DirectX::SimpleMath::Vector3::Zero;
		mX.x = moveW.z;
		mX.z = -moveW.x;
		m_target -= mX;
		m_eyePos -= mX;
	}

	// �㉺�ړ�
	if (keyState.Up)
	{
		DirectX::SimpleMath::Vector3 mY = DirectX::SimpleMath::Vector3::Zero;
		mY.y += moveH.y;
		m_target -= mY;
		m_eyePos -= mY;
	}
	if (keyState.Down)
	{
		DirectX::SimpleMath::Vector3 mY = DirectX::SimpleMath::Vector3::Zero;
		mY.y += moveH.y;
		m_target += mY;
		m_eyePos += mY;
	}

	m_eye = eye + m_eyePos;

	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, up);
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
	m_screenWidth = width;
	m_screenHeight = height;
	m_viewAngle = angle;

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