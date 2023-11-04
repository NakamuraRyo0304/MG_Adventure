/*
 *	@File	Player.cpp
 *	@Brief	�v���C���̏���������N���X�B
 *	@Date	2023-06-14
 *  @Author NakamuraRyo
 */

#include "pch.h"

// �g�p���郉�C�u����
#include "Libraries/SystemManager/SystemManager.h"
#include "Libraries/UserUtility.h"
#include "Libraries/SystemDatas/Input.h"
#include "Libraries/Factories/ModelFactory.h"

#include "Player.h"

// �R���X�g���N�^
Player::Player(std::unique_ptr<Model> head, std::unique_ptr<Model> body,
			   std::unique_ptr<Model> right, std::unique_ptr<Model> left, std::unique_ptr<Model> wink)
	: m_system{}					// �V�X�e��
	, m_parameter{}					// �p�����[�^�[
	, m_coinNum{}					// ���v�l���R�C����
	, m_head{ std::move(head) }		// ���̃��f��
	, m_body{ std::move(body) }		// �g�̂̃��f��
	, m_rightLeg{ std::move(right) }// �E���̃��f��
	, m_leftLeg{ std::move(left) }	// �����̃��f��
	, m_wink{ std::move(wink)}		// �E�C���N����
	, m_headMove{0.0f}				// ���̓���
	, m_footMove{0.0f}				// ���̓���
	, m_neckQuaternion{}			// ��̉�]�s��
	, m_neckRotate{}				// ��̉�]��
	, m_thirdRotate{}				// �O�l�̎��_�̉�]��
	, m_lightDirection{}			// ���C�e�B���O�̌���
	, is_lookFlag{}					// ���_�t���O
	, is_deathFlag{}				// ���S�t���O
{
}

// �f�X�g���N�^
Player::~Player()
{
	Finalize();
	m_system.reset();
}

// ����������
void Player::Initialize(std::shared_ptr<SystemManager> system)
{
	// �V�X�e���̎擾
	m_system = system;

	// �p�����[�^�̃��Z�b�g
	m_parameter.reset();

	// �����x�̐ݒ�
	m_parameter.accelerate = NORMAL_SPEED;

	// ���S����̏�����
	is_deathFlag = false;

	// ��]�ʂ̏�����
	m_neckRotate = SimpleMath::Vector2::Zero;
	m_neckQuaternion = SimpleMath::Quaternion::Identity;
	m_thirdRotate = SimpleMath::Quaternion::Identity;
	is_lookFlag = false;

	// ���C�e�B���O���Z�b�g
	m_lightDirection = SimpleMath::Vector3(1.0f, -1.0f, -1.0f);
}

// �X�V����
void Player::Update(bool lookFlag)
{
	auto _key = Keyboard::Get().GetState();

	is_lookFlag = lookFlag;

	// ���_�ɂ���đ��x��ύX����
	m_parameter.accelerate = is_lookFlag ? THIRD_SPEED : NORMAL_SPEED;

	// �d�͏���
	UpdateGravity();

	// �����]����
	if (lookFlag)
	{
		// �㉺��]
		if (_key.Up)
		{
			m_neckRotate.x += NECK_ROT_SPEED;
		}
		if (_key.Down)
		{
			m_neckRotate.x -= NECK_ROT_SPEED;
		}
		// �N�����v
		m_neckRotate.x = UserUtility::Clamp(m_neckRotate.x, -10.0f, 15.0f);
		m_neckQuaternion.x = XMConvertToRadians(m_neckRotate.x);
		m_neckRotate.x = UserUtility::Lerp(m_neckRotate.x, 0.0f, NECK_ROT_SPEED * 0.1f);

		// ���E��]
		if (_key.Left)
		{
			m_neckRotate.y += NECK_ROT_SPEED;
		}
		if (_key.Right)
		{
			m_neckRotate.y -= NECK_ROT_SPEED;
		}
		// �N�����v
		m_neckRotate.y = UserUtility::Clamp(m_neckRotate.y, -15.0f, 15.0f);
		m_neckQuaternion.y = XMConvertToRadians(m_neckRotate.y);
		m_neckRotate.y = UserUtility::Lerp(m_neckRotate.y, 0.0f, NECK_ROT_SPEED * 0.1f);
	}

	// ���E���������
	float _rotationAngle = 0.0f;
	_rotationAngle = _key.A ? ROT_SPEED : _key.D ? -ROT_SPEED : 0.0f;
	SimpleMath::Quaternion _q = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, _rotationAngle);
	m_parameter.rotate *= _rotationAngle != 0.0f ? _q : SimpleMath::Quaternion::Identity;

	// �O��ړ�������
	SimpleMath::Vector3 _moveDirection(0.0f, 0.0f, 0.0f);
	_moveDirection.z = _key.W ? -m_parameter.accelerate : _key.S ? m_parameter.accelerate / 2 : 0.0f;
	_moveDirection = SimpleMath::Vector3::Transform(_moveDirection, m_parameter.rotate);
	m_parameter.velocity += _moveDirection;

	// �r�̓���
	if (_key.W || _key.A || _key.S || _key.D)
	{
		m_footMove++;
	}
	else
	{
		m_footMove = 0.0f;
	}

	// ����������
	m_headMove = m_footMove;

	// �ړ��ʂ̌v�Z
	m_parameter.position += m_parameter.velocity;

	// ��������
	if (m_parameter.velocity != SimpleMath::Vector3::Zero)
	{
		m_parameter.velocity *= DECELERATION;
	}

	// �ŏI�I�ȉ�]��
	m_thirdRotate = m_neckQuaternion * m_parameter.rotate;
}

// �`�揈��
void Player::Render(CommonStates& states, SimpleMath::Matrix view, SimpleMath::Matrix proj,
	const SimpleMath::Vector3& lightDir)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// ���[���h�s��
	SimpleMath::Matrix _headMat, _legRMat, _legLMat;

	// ��]�s��
	SimpleMath::Matrix _rotate = SimpleMath::Matrix::CreateFromQuaternion(m_parameter.rotate);

	// �ړ��s��
	SimpleMath::Matrix _trans =
		SimpleMath::Matrix::CreateTranslation(
			m_parameter.position.x, m_parameter.position.y + OFFSET_Y, m_parameter.position.z);
	// ���ʍs��v�Z
	SimpleMath::Matrix _commonMat = _rotate * _trans;

	// �E�r�̓���
	SimpleMath::Matrix _rightTrans =
		SimpleMath::Matrix::CreateTranslation(
			0.0f,
			0.0f,
			sinf(m_footMove) * FOOT_SPEED
		);

	// ���r�̓���
	SimpleMath::Matrix _leftTrans =
		SimpleMath::Matrix::CreateTranslation(
			0.0f,
			0.0f,
			-sinf(m_footMove) * FOOT_SPEED
		);

	// �����̓���
	SimpleMath::Matrix _headTrans =
		SimpleMath::Matrix::CreateTranslation(
			0.0f,
			0.0f,
			sinf(m_headMove * HEAD_SPEED) * 0.05f
		);

	// �ړ����Ă����]������
	_legRMat = _rightTrans * _commonMat;
	_legLMat = _leftTrans  * _commonMat;

	// ���͑O��ɓ���
	if (!is_lookFlag)
	{
		_headMat = _headTrans * _commonMat;
	}
	else
	{
		_headMat = SimpleMath::Matrix::CreateFromQuaternion(m_neckQuaternion) * _headTrans * _commonMat;
	}

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
	m_rightLeg->UpdateEffects(UpdateLighting(m_lightDirection));
	m_rightLeg->Draw(_context, states, _legRMat, view, proj);

	// �����̕`��
	m_leftLeg->UpdateEffects(UpdateLighting(m_lightDirection));
	m_leftLeg->Draw(_context, states, _legLMat, view, proj);

	// �����̕`��
	if (sinf(_timer) <= WINK_SPAN)
	{
		m_head->UpdateEffects(UpdateLighting(m_lightDirection));
		m_head->Draw(_context, states, _headMat, view, proj);
	}
	else
	{
		m_wink->UpdateEffects(UpdateLighting(m_lightDirection));
		m_wink->Draw(_context, states, _headMat, view, proj);
	}
	// �g�̂̕`��
	m_body->UpdateEffects(UpdateLighting(m_lightDirection));
	m_body->Draw(_context, states, _commonMat, view, proj);
}

// �I������
void Player::Finalize()
{
	m_head.reset();
	m_body.reset();
	m_rightLeg.reset();
	m_leftLeg.reset();
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
