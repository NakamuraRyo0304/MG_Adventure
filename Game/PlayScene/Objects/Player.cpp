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
 /// <param name="head">�����̃��f���f�[�^</param>
 /// <param name="body">���̂̃��f���f�[�^</param>
 /// <param name="right">�E�����f���f�[�^</param>
 /// <param name="left">�������f���f�[�^</param>
 /// <returns>�Ȃ�</returns>
Player::Player(std::unique_ptr<Model> head, std::unique_ptr<Model> body, std::unique_ptr<Model> right, std::unique_ptr<Model> left):
	m_head{std::move(head)},
	m_body{std::move(body)},
	m_rightLeg{std::move(right)},
	m_leftLeg{std::move(left)},
	m_position{},
	m_parameter{},
	m_system{},
	is_deathFlag{},
	m_footMove{0.0f},
	m_neckQuaternion{},
	m_neckRotate{},
	is_lookFlag{},
	m_thirdRotate{}
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

	// ��]�ʂ̏�����
	m_neckRotate = SimpleMath::Vector2::Zero;
	m_neckQuaternion = SimpleMath::Quaternion::Identity;
	m_thirdRotate = SimpleMath::Quaternion::Identity;
	is_lookFlag = false;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="keyState">�L�[�{�[�h</param>
/// <param name="timer">�h���V�[����StepTimer(TotalTime)</param>
/// <param name="lookFlag">���_�t���O</param>
/// <returns>�Ȃ�</returns>
void Player::Update(Keyboard::State& keyState, float timer, bool lookFlag)
{
	m_timer = timer;

	is_lookFlag = lookFlag;

	// �d�͏���
	UpdateGravity();

	// �����]����
	if (lookFlag)
	{
		// �㉺��]
		if (keyState.Up)
		{
			m_neckRotate.x += NECK_ROT_SPEED;
		}
		if (keyState.Down)
		{
			m_neckRotate.x -= NECK_ROT_SPEED;
		}
		// �N�����v
		m_neckRotate.x = UserUtility::Clamp(m_neckRotate.x, -10.0f, 15.0f);
		m_neckQuaternion.x = XMConvertToRadians(m_neckRotate.x);
		m_neckRotate.x = UserUtility::Lerp(m_neckRotate.x, 0.0f, NECK_ROT_SPEED * 0.1f);

		// ���E��]
		if (keyState.Left)
		{
			m_neckRotate.y += NECK_ROT_SPEED;
		}
		if (keyState.Right)
		{
			m_neckRotate.y -= NECK_ROT_SPEED;
		}
		// �N�����v
		m_neckRotate.y = UserUtility::Clamp(m_neckRotate.y, -15.0f, 15.0f);
		m_neckQuaternion.y = XMConvertToRadians(m_neckRotate.y);
		m_neckRotate.y = UserUtility::Lerp(m_neckRotate.y, 0.0f, NECK_ROT_SPEED * 0.1f);
	}

	// ���E��]
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
	// �O��ړ�
	if (keyState.W)
	{
		SimpleMath::Vector3 vec(0.0f, 0.0f, m_parameter.accelerate);
		vec = SimpleMath::Vector3::Transform(vec, m_parameter.rotate);
		m_parameter.velocity -= vec;
	}
	else if (keyState.S)
	{
		SimpleMath::Vector3 vec(0.0f, 0.0f, m_parameter.accelerate / 2);
		vec = SimpleMath::Vector3::Transform(vec, m_parameter.rotate);
		m_parameter.velocity += vec;
	}

	// �r�̓���
	if (keyState.W || keyState.A || keyState.S || keyState.D)
	{
		m_footMove++;
	}
	else
	{
		m_footMove = 0.0f;
	}

	// �ړ��ʂ̌v�Z
	m_position += m_parameter.velocity;

	// ��������
	if (m_parameter.velocity != SimpleMath::Vector3::Zero)
	{
		m_parameter.velocity *= DECELERATION;
	}

	// �ŏI�I�ȉ�]��
	m_thirdRotate = m_neckQuaternion * m_parameter.rotate;
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="context">ID3D11DeviceContext�|�C���^</param>
/// <param name="states">�R�����X�e�[�g</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
/// <returns>�Ȃ�</returns>
void Player::Render(ID3D11DeviceContext* context, CommonStates& states,
	SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	// ���[���h�s��
	SimpleMath::Matrix world, headWorld, legRWorld, legLWorld;

	// ��]�s��
	SimpleMath::Matrix rotate = SimpleMath::Matrix::CreateFromQuaternion(m_parameter.rotate);

	// �v���C���[�̈ړ��s��
	SimpleMath::Matrix trans =
		SimpleMath::Matrix::CreateTranslation(
			m_position.x,
			m_position.y + OFFSET_Y,
			m_position.z
		);

	// �E�r�̓���
	SimpleMath::Matrix rightTrans =
		SimpleMath::Matrix::CreateTranslation(
			0.0f,
			0.0f,
			sinf(m_footMove) * 0.1f
		);

	// ���r�̓���
	SimpleMath::Matrix leftTrans =
		SimpleMath::Matrix::CreateTranslation(
			0.0f,
			0.0f,
			-sinf(m_footMove) * 0.1f
		);

	// �����̓���
	SimpleMath::Matrix headTrans =
		SimpleMath::Matrix::CreateTranslation(
			0.0f,
			0.0f,
			sinf(m_footMove * 0.25f) * 0.1f
		);

	// �s��v�Z
	world = rotate * trans;

	// �ړ����Ă����]������
	legRWorld = rightTrans * rotate * trans;
	legLWorld = leftTrans  * rotate * trans;

	// ���͑O��ɓ���
	if (!is_lookFlag)
	{
		headWorld = headTrans * rotate * trans;
	}
	else
	{
		headWorld = SimpleMath::Matrix::CreateFromQuaternion(m_neckQuaternion) * headTrans * rotate * trans;
	}

	// ���C�g�̐ݒ�
	SimpleMath::Vector3 lightDirection(1.0f, -1.0f, -1.0f);

	// �r���[�s�񂩂�J�����̉�]���擾
	SimpleMath::Matrix cameraRotation;

	// �r���[�s����t�ϊ�
	cameraRotation = view.Invert();

	// �ړ��ʂ��Ȃ���
	cameraRotation._41 = 0.0f;
	cameraRotation._42 = 0.0f;
	cameraRotation._43 = 0.0f;

	// ���C�g�̕������J�����̉�]�ɋt�����ɂ���
	lightDirection = SimpleMath::Vector3::TransformNormal(lightDirection, cameraRotation);
	SimpleMath::Color lightColor(0.3f, 0.3f, 0.3f, 1.0f);

	auto setLightForModel = [&](IEffect* effect)
	{
		auto lights = dynamic_cast<IEffectLights*>(effect);
		if (lights)
		{
			// ���C�g�I��
			lights->SetLightEnabled(0, true);
			lights->SetLightEnabled(1, true);
			lights->SetLightEnabled(2, true);

			// ���C�g�̕�����ݒ�
			lights->SetLightDirection(0, lightDirection);
			lights->SetLightDirection(1, lightDirection);
			lights->SetLightDirection(2, lightDirection);

			// ���C�g�̐F���Â߂̃O���[�ɐݒ�
			lights->SetLightDiffuseColor(0, lightColor);
			lights->SetLightDiffuseColor(1, lightColor);
			lights->SetLightDiffuseColor(2, lightColor);
		}
	};

	// ���f���̕`��
	// �E���̕`��
	m_rightLeg->UpdateEffects(setLightForModel);
	m_rightLeg->Draw(context, states, legRWorld, view, proj);

	// �����̕`��
	m_leftLeg->UpdateEffects(setLightForModel);
	m_leftLeg->Draw(context, states, legLWorld, view, proj);

	// �����̕`��
	m_head->UpdateEffects(setLightForModel);
	m_head->Draw(context, states, headWorld, view, proj);

	// �g�̂̕`��
	m_body->UpdateEffects(setLightForModel);
	m_body->Draw(context, states, world, view, proj);
}

/// <summary>
/// �I������
/// </summary>
/// <param name="��������"></param>
/// <returns>�Ȃ�</returns>
void Player::Finalize()
{
	m_head.reset();
	m_body.reset();
	m_rightLeg.reset();
	m_leftLeg.reset();
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
