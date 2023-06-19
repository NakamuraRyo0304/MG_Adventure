/*
 *	@File	Player.cpp
 *	@Brief	�v���C���̏���������N���X�B
 *	@Date	2023-06-14
 *  @Author NakamuraRyo
 */

#include "pch.h"

// �V�X�e��
#include "../../../Libraries/SystemManager/SystemManager.h"

// �C�����C���֐��Q
#include "../../../Libraries/UserUtility.h"

#include "Player.h"

//--------------------------------------------------------//
//�R���X�g���N�^                                          //
//--------------------------------------------------------//
Player::Player(std::unique_ptr<Model> model):
	m_model{std::move(model)},
	m_parameter{},
	m_system{nullptr}
{
}

//--------------------------------------------------------//
//�f�X�g���N�^                                            //
//--------------------------------------------------------//
Player::~Player()
{
	Finalize();
}

//--------------------------------------------------------//
//����������                                              //
//--------------------------------------------------------//
void Player::Initialize(std::shared_ptr<SystemManager> system)
{
	// �V�X�e���̎擾
	m_system = system;

	// �p�����[�^�̃��Z�b�g
	m_parameter.reset();

	// �����x�̐ݒ�
	m_parameter.accelerate = 0.05f;
}

//--------------------------------------------------------//
//�X�V����                                                //
//--------------------------------------------------------//
void Player::Update(Keyboard::State& keyState, float timer)
{
	m_timer = timer;

	// �ړ�����
	if (keyState.W)
	{
		m_parameter.velocity.z -= m_parameter.accelerate;
	}
	if (keyState.S)
	{
		m_parameter.velocity.z += m_parameter.accelerate;
	}
	if (keyState.A)
	{
		m_parameter.velocity.x -= m_parameter.accelerate;
	}
	if (keyState.D)
	{
		m_parameter.velocity.x += m_parameter.accelerate;
	}

	// �ړ��ʂ̌v�Z
	m_parameter.position += m_parameter.velocity;

	// ��������
	if (m_parameter.velocity != SimpleMath::Vector3::Zero)
	{
		m_parameter.velocity *= DECELERATION;
	}
	m_parameter.gravity += 0.015f;

	m_parameter.position.y -= m_parameter.gravity;

	// �f�o�b�O�p
	if (m_parameter.position.y < -7.0f) m_parameter.position.y = 10.0f;
}

//--------------------------------------------------------//
//�`�揈��                                                //
//--------------------------------------------------------//
void Player::Render(ID3D11DeviceContext* context, DX11::CommonStates& states,
	SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	// ���[���h�s��
	SimpleMath::Matrix world =
		SimpleMath::Matrix::CreateTranslation(
			m_parameter.position.x,
			m_parameter.position.y + sinf(m_timer) / 10.0f + OFFSET_Y,
			m_parameter.position.z
		);

	// ���f���̕`��
	m_model->Draw(context, states, world, view, proj, false);
}

//--------------------------------------------------------//
//�I������                                                //
//--------------------------------------------------------//
void Player::Finalize()
{
	m_model.reset();
	m_parameter.reset();
}

//--------------------------------------------------------//
//�����ʒu�̐ݒ�ƃ��Z�b�g                                //
//--------------------------------------------------------//
void Player::Spawn(SimpleMath::Vector3 spawnPosition)
{
	// �p�����[�^�̏�����
	m_parameter.reset();

	// ���W�̐ݒ�
	m_parameter.position = spawnPosition;
}
