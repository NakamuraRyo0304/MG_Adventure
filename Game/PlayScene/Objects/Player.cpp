/*
 *	@File	Player.cpp
 *	@Brief	�v���C���̏���������N���X�B
 *	@Date	2023-06-14
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "../../../Libraries/UserUtility.h"

#include "Player.h"

//--------------------------------------------------------//
//�R���X�g���N�^                                          //
//--------------------------------------------------------//
Player::Player(std::unique_ptr<DirectX::Model> model):
	m_model{std::move(model)},
	m_parameter{}
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
void Player::Initialize()
{
	// �p�����[�^�̃��Z�b�g
	m_parameter.reset();
}

//--------------------------------------------------------//
//�X�V����                                                //
//--------------------------------------------------------//
void Player::Update(DirectX::Keyboard::State& keyState, float timer)
{
	m_timer = timer;

	// �ړ�����
	if (keyState.W) m_parameter.position.z -= 0.05f;
	if (keyState.S) m_parameter.position.z += 0.05f;
	if (keyState.A) m_parameter.position.x -= 0.05f;
	if (keyState.D) m_parameter.position.x += 0.05f;

	m_parameter.gravity += 0.015f;

	m_parameter.position.y -= m_parameter.gravity;

	if (m_parameter.position.y < -7.0f) m_parameter.position.y = 10.0f;
}

//--------------------------------------------------------//
//�`�揈��                                                //
//--------------------------------------------------------//
void Player::Render(ID3D11DeviceContext* context, DirectX::DX11::CommonStates& states,
	DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	// ���[���h�s��
	DirectX::SimpleMath::Matrix world =
		DirectX::SimpleMath::Matrix::CreateTranslation(
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
void Player::Spawn(DirectX::SimpleMath::Vector3 spawnPosition)
{
	// �p�����[�^�̏�����
	m_parameter.reset();

	// ���W�̐ݒ�
	m_parameter.position = spawnPosition;
}
