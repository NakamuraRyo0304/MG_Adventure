/*
 *	@File	Player.cpp
 *	@Brief	�v���C���̏���������N���X�B
 *	@Date	2023-06-14
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/UserUtility.h"
#include "Libraries/SystemDatas/Input.h"
#include "Libraries/Factories/ModelFactory.h"
#include "Parts/Head/Head.h"
#include "Parts/RightLeg/RightLeg.h"
#include "Parts/LeftLeg/LeftLeg.h"
#include "Player.h"

// �R���X�g���N�^
Player::Player(std::unique_ptr<Model> head, std::unique_ptr<Model> body,
			   std::unique_ptr<Model> right, std::unique_ptr<Model> left)
	: m_parameter{}					// �p�����[�^�[
	, m_coinNum{}					// ���v�l���R�C����
	, m_body{ std::move(body) }		// �g�̂̃��f��
	, m_lightDirection{}			// ���C�e�B���O�̌���
	, is_deathFlag{}				// ���S�t���O
{
	m_head = std::make_unique<Head>(std::move(head));
	m_legR = std::make_unique<RightLeg>(std::move(right));
	m_legL = std::make_unique<LeftLeg>(std::move(left));
}

// �f�X�g���N�^
Player::~Player()
{
	Finalize();
}

// ����������
void Player::Initialize()
{
	// �p�����[�^�̃��Z�b�g
	m_parameter.reset();

	// �����x�̐ݒ�
	m_parameter.accelerate = NORMAL_SPEED;

	// ���S����̏�����
	is_deathFlag = false;

	// ���C�e�B���O���Z�b�g
	m_lightDirection = SimpleMath::Vector3(1.0f, -1.0f, -1.0f);
}

// �X�V����
void Player::Update()
{
	auto _key = Keyboard::Get().GetState();

	// ���_�ɂ���đ��x��ύX����
	m_parameter.accelerate = NORMAL_SPEED;

	// �d�͏���
	UpdateGravity();

	// ���E���������
	float _rotationAngle = 0.0f;
	_rotationAngle = _key.A ? ROT_SPEED : _key.D ? -ROT_SPEED : 0.0f;
	SimpleMath::Quaternion _q = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, _rotationAngle);
	m_parameter.rotate *= _rotationAngle != 0.0f ? _q : SimpleMath::Quaternion::Identity;

	// �O��ړ�������
	SimpleMath::Vector3 _moveDirection(0.0f, 0.0f, 0.0f);
	_moveDirection.z = _key.W ? -m_parameter.accelerate : _key.S ? m_parameter.accelerate * 0.5f: 0.0f;
	_moveDirection = SimpleMath::Vector3::Transform(_moveDirection, m_parameter.rotate);
	m_parameter.velocity += _moveDirection;

	// ���̍X�V����
	m_head->Update();

	// ���̍X�V����
	m_legR->Update();
	m_legL->Update();

	// �ړ��ʂ̌v�Z
	m_parameter.position += m_parameter.velocity;

	// ��������
	if (m_parameter.velocity != SimpleMath::Vector3::Zero)
	{
		m_parameter.velocity *= DECELERATION;
	}
}

// �`�揈��
void Player::Render(CommonStates& states, SimpleMath::Matrix view, SimpleMath::Matrix proj,
	const SimpleMath::Vector3& lightDir)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	// ��]�s��
	SimpleMath::Matrix _rotate = SimpleMath::Matrix::CreateFromQuaternion(m_parameter.rotate);

	// �ړ��s��
	SimpleMath::Matrix _trans =
		SimpleMath::Matrix::CreateTranslation(
			m_parameter.position.x, m_parameter.position.y + OFFSET_Y, m_parameter.position.z);
	// ���ʍs��v�Z
	SimpleMath::Matrix _world = _rotate * _trans;

	// �r���[�s�񂩂�J�����̉�]���擾
	SimpleMath::Matrix _cameraRot;

	// �r���[�s����t�ϊ�
	_cameraRot = view.Invert();

	// �ړ��ʂ��Ȃ���
	_cameraRot._41 = 0.0f;
	_cameraRot._42 = 0.0f;
	_cameraRot._43 = 0.0f;

	// ���C�g�̕������J�����̉�]�ɋt�����ɂ���
	m_lightDirection = SimpleMath::Vector3::Lerp(m_lightDirection,
		SimpleMath::Vector3::TransformNormal(lightDir, _cameraRot), LIGHT_SPEED);

	// �E���̕`��
	m_legR->SetParentMatrix(_world);
	m_legR->Draw(states, view, proj);

	// �����̕`��
	m_legL->SetParentMatrix(_world);
	m_legL->Draw(states, view, proj);

	// �����̕`��
	m_head->SetParentMatrix(_world);
	m_head->Draw(states, view, proj);

	// �g�̂̕`��
	m_body->UpdateEffects(UpdateLighting(m_lightDirection));
	m_body->Draw(_context, states, _world, view, proj);
}

// �I������
void Player::Finalize()
{
	m_head.reset();
	m_legR.reset();
	m_legL.reset();

	m_body.reset();
	m_parameter.reset();
}

// �d�͏���
void Player::UpdateGravity()
{
	// �d�͂̉��Z
	m_parameter.gravity += 0.015f;

	// �d�͂̔��f
	m_parameter.position.y -= m_parameter.gravity;

	// ���S����
	if (m_parameter.position.y < DEATH_LINE)
	{
		is_deathFlag = true;
	}
}

// ���C�e�B���O�̍X�V
std::function<void(IEffect* effect)> Player::UpdateLighting(SimpleMath::Vector3 dir)
{
	auto _lightingEffects = [&](IEffect* effect)
		{
			SimpleMath::Color lightColor(0.3f, 0.3f, 0.3f, 1.0f);
			// �x�[�V�b�N�G�t�F�N�g
			auto _basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (_basicEffect)
			{
				// �A���r�G���g���C�g�J���[��ݒ�
				_basicEffect->SetAmbientLightColor(SimpleMath::Color(0.2f, 0.2f, 0.2f));
			}

			// ���C�g
			auto _lights = dynamic_cast<IEffectLights*>(effect);
			if (_lights)
			{
				for (int i = 0; i < 3; ++i)
				{
					_lights->SetLightEnabled(i, true);
					_lights->SetLightDirection(i, dir);
					_lights->SetLightDiffuseColor(i, lightColor);
				}
			}

			// �t�H�O
			auto _fog = dynamic_cast<IEffectFog*>(effect);
			if (_fog)
			{
				// �����g���V�F�[�_�[�ɐ؂�ւ���
				_fog->SetFogEnabled(true);

				// �t�H�O�̐F�����߂�
				_fog->SetFogColor(Colors::LightGray);

				// �X�^�[�g
				_fog->SetFogStart(-50.0f);

				// �G���h
				_fog->SetFogEnd(150.0f);
			}
		};

	return _lightingEffects;
}

// ���X�|�[���֐�
void Player::Spawn(SimpleMath::Vector3 spawnPosition)
{
	// �p�����[�^�̏�����
	m_parameter.reset();

	// ���W�̐ݒ�
	m_parameter.position = spawnPosition;
}
