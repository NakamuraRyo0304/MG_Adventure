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

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="model">�v���C�V�[���ō쐬�������f���f�[�^</param>
 /// <returns>�Ȃ�</returns>
Player::Player(std::unique_ptr<Model> model):
	m_model{std::move(model)},
	m_position{},
	m_parameter{},
	m_system{},
	is_deathFlag{}
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
Player::~Player()
{
	Finalize();
	m_system.reset();
}

/// <summary>
/// ����������
/// </summary>
/// <param name="system">�V�X�e���f�[�^</param>
/// <returns>�Ȃ�</returns>
void Player::Initialize(std::shared_ptr<SystemManager> system)
{
	// �V�X�e���̎擾
	m_system = system;

	// �p�����[�^�̃��Z�b�g
	m_parameter.reset();

	// �����x�̐ݒ�
	m_parameter.accelerate = 0.01f;

	// ���S����̏�����
	is_deathFlag = false;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="keyState">�L�[�{�[�h</param>
/// <param name="timer">�h���V�[����StepTimer(TotalTime)</param>
/// <returns>�Ȃ�</returns>
void Player::Update(Keyboard::State& keyState, float timer)
{
	m_timer = timer;

	// �d�͏���
	UpdateGravity();

	// �ړ�����
	if (keyState.A)
	{
		m_parameter.rotate *= SimpleMath::Quaternion::CreateFromAxisAngle(
			SimpleMath::Vector3::UnitY, ROT_SPEED);
	}
	if (keyState.D)
	{
		m_parameter.rotate *= SimpleMath::Quaternion::CreateFromAxisAngle(
			SimpleMath::Vector3::UnitY, -ROT_SPEED);
	}
	if (keyState.S)
	{
		SimpleMath::Vector3 vec(0.0f, 0.0f, m_parameter.accelerate);
		vec = SimpleMath::Vector3::Transform(vec, m_parameter.rotate);
		m_parameter.velocity += vec;
	}

	if (keyState.W)
	{
		SimpleMath::Vector3 vec(0.0f, 0.0f, m_parameter.accelerate);
		vec = SimpleMath::Vector3::Transform(vec, m_parameter.rotate);
		m_parameter.velocity -= vec;
	}
	// �ړ��ʂ̌v�Z
	m_position += m_parameter.velocity;

	// ��������
	if (m_parameter.velocity != SimpleMath::Vector3::Zero)
	{
		m_parameter.velocity *= DECELERATION;
	}
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="context">ID3D11DeviceContext�|�C���^</param>
/// <param name="states">�R�����X�e�[�g</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
/// <returns>�Ȃ�</returns>
void Player::Render(ID3D11DeviceContext* context, DX11::CommonStates& states,
	SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	// ���[���h�s��
	SimpleMath::Matrix world;

	// ��]�s��
	SimpleMath::Matrix rotate = SimpleMath::Matrix::CreateFromQuaternion(m_parameter.rotate);

	// �ړ��s��
	SimpleMath::Matrix trans =
		SimpleMath::Matrix::CreateTranslation(
			m_position.x,
			m_position.y + sinf(m_timer) / 10.0f + OFFSET_Y,
			m_position.z
		);

	// �s��v�Z
	world = rotate * trans;

	// ���f���̕`��
	m_model->Draw(context, states, world, view, proj, false);
}

/// <summary>
/// �I������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void Player::Finalize()
{
	m_model.reset();
	m_parameter.reset();
}

/// <summary>
/// �d�͏���
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void Player::UpdateGravity()
{
	// �d�͂̉��Z
	m_parameter.gravity += 0.015f;

	// �d�͂̔��f
	m_position.y -= m_parameter.gravity;

	// ���S����
	if (m_position.y < DEATH_LINE)
	{
		is_deathFlag = true;
	}
}

/// <summary>
/// ���X�|�[��������֐�
/// </summary>
/// <param name="spawnPosition">�v���C���[�̍��W���w�肷��</param>
/// <returns>�Ȃ�</returns>
void Player::Spawn(SimpleMath::Vector3 spawnPosition)
{
	// �p�����[�^�̏�����
	m_parameter.reset();

	// ���W�̐ݒ�
	m_position = spawnPosition;
}
