/*
 *	@File	Body.cpp
 *	@Brief	�v���C���[�̐g�́B
 *	@Date	2023-01-19
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemDatas/Input.h"
#include "Libraries/UserUtility.h"
#include "Body.h"

 // �R���X�g���N�^
Body::Body(std::unique_ptr<Model> model)
	: IParts(std::move(model))	// ���N���X
	, m_param{}					// �p�����[�^
{
}

// �f�X�g���N�^
Body::~Body()
{
}

// �ʏ�̍X�V
void Body::Update()
{
	auto _key = Keyboard::Get().GetState();

	// ���E���������
	float _rotationAngle = 0.0f;
	_rotationAngle = _key.A ? ROT_SPEED : _key.D ? -ROT_SPEED : 0.0f;
	SimpleMath::Quaternion _q = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, _rotationAngle);
	m_param.rotation *= _rotationAngle != 0.0f ? _q : SimpleMath::Quaternion::Identity;

	// �O��ړ�������
	SimpleMath::Vector3 _moveDirection(0.0f, 0.0f, 0.0f);
	_moveDirection.z = _key.W ? -m_param.accelerate : _key.S ? m_param.accelerate * 0.5f : 0.0f;
	_moveDirection = SimpleMath::Vector3::Transform(_moveDirection, m_param.rotation);
	m_param.velocity += _moveDirection;

	// �ړ��ʂ̌v�Z
	m_param.position += m_param.velocity;

	// ��������
	if (m_param.velocity != SimpleMath::Vector3::Zero)
	{
		m_param.velocity *= DECELERATION;
	}

	// �d�͂̉��Z
	m_param.gravity += 0.015f;

	// �d�͂̔��f
	m_param.position.y -= m_param.gravity;

	// ��]�s��
	SimpleMath::Matrix _rotate = SimpleMath::Matrix::CreateFromQuaternion(m_param.rotation);

	// �ړ��s��
	SimpleMath::Matrix _trans =
		SimpleMath::Matrix::CreateTranslation(
			m_param.position.x, m_param.position.y + OFFSET_Y, m_param.position.z);

	// �s��̍쐬
	SimpleMath::Matrix _world = _rotate * _trans;

	// �s��̐ݒ�
	SetMatrix(_world);
}

// �`�揈��
void Body::Draw(CommonStates& states, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	GetModel()->Draw(_context, states, GetMatrix(), view, proj);
}

// �p�����[�^���Z�b�g
void Body::ResetAll()
{
	m_param.reset();

	// �����x�̐ݒ�
	m_param.accelerate = MOVE_SPEED;
}
