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
	: _timer{0.0f}					// �^�C�}�[
	, m_system{}					// �V�X�e��
	, m_parameter{}					// �p�����[�^�[
	, m_coinNum{}					// ���v�l���R�C����
	, m_head{ std::move(head) }		// ���̃��f��
	, m_body{ std::move(body) }		// �g�̂̃��f��
	, m_rightLeg{ std::move(right) }// �E���̃��f��
	, m_leftLeg{ std::move(left) }	// �����̃��f��
	, m_wink{ std::move(wink)}		// �E�C���N����
	, m_position{}					// �|�W�V����
	, m_headMove{0.0f}				// ���̓���
	, m_footMove{0.0f}				// ���̓���
	, m_neckQuaternion{}			// ��̉�]�s��
	, m_neckRotate{}				// ��̉�]��
	, m_thirdRotate{}				// �O�l�̎��_�̉�]��
	, m_lighting{}					// ���C�e�B���O
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
	m_lighting = SimpleMath::Vector3::Zero;
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

	// ���E��]
	if (_key.A)
	{
		m_parameter.rotate *= SimpleMath::Quaternion::CreateFromAxisAngle(
			SimpleMath::Vector3::UnitY, ROT_SPEED);
	}
	if (_key.D)
	{
		m_parameter.rotate *= SimpleMath::Quaternion::CreateFromAxisAngle(
			SimpleMath::Vector3::UnitY, -ROT_SPEED);
	}
	// �O��ړ�
	if (_key.W)
	{
		SimpleMath::Vector3 _newVec(0.0f, 0.0f, m_parameter.accelerate);
		_newVec = SimpleMath::Vector3::Transform(_newVec, m_parameter.rotate);
		m_parameter.velocity -= _newVec;
	}
	else if (_key.S)
	{
		SimpleMath::Vector3 _newVec(0.0f, 0.0f, m_parameter.accelerate / 2);
		_newVec = SimpleMath::Vector3::Transform(_newVec, m_parameter.rotate);
		m_parameter.velocity += _newVec;
	}

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
	m_position += m_parameter.velocity;

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

	// ���[���h�s��
	SimpleMath::Matrix _commonMat, _headMat, _legRMat, _legLMat;

	// ��]�s��
	SimpleMath::Matrix _rotate = SimpleMath::Matrix::CreateFromQuaternion(m_parameter.rotate);

	// �v���C���[�̈ړ��s��
	SimpleMath::Matrix _trans =
		SimpleMath::Matrix::CreateTranslation(
			m_position.x,
			m_position.y + OFFSET_Y,
			m_position.z
		);

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

	// �s��v�Z
	_commonMat = _rotate * _trans;

	// �ړ����Ă����]������
	_legRMat = _rightTrans * _rotate * _trans;
	_legLMat = _leftTrans  * _rotate * _trans;

	// ���͑O��ɓ���
	if (!is_lookFlag)
	{
		_headMat = _headTrans * _rotate * _trans;
	}
	else
	{
		_headMat = SimpleMath::Matrix::CreateFromQuaternion(m_neckQuaternion) * _headTrans * _rotate * _trans;
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
	m_lighting = SimpleMath::Vector3::TransformNormal(lightDir, _cameraRot);
	SimpleMath::Color _lightColor(0.3f, 0.3f, 0.3f, 1.0f);

	auto _lightingEffects = [&](IEffect* effect)
	{
		auto _lights = dynamic_cast<IEffectLights*>(effect);
		if (_lights)
		{
			// ���C�g�I��
			_lights->SetLightEnabled(0, true);
			_lights->SetLightEnabled(1, true);
			_lights->SetLightEnabled(2, true);

			// ���C�g�̕�����ݒ�
			_lights->SetLightDirection(0, m_lighting);
			_lights->SetLightDirection(1, m_lighting);
			_lights->SetLightDirection(2, m_lighting);

			// ���C�g�̐F���Â߂̃O���[�ɐݒ�
			_lights->SetLightDiffuseColor(0, _lightColor);
			_lights->SetLightDiffuseColor(1, _lightColor);
			_lights->SetLightDiffuseColor(2, _lightColor);
		}
		auto _basicEffect = dynamic_cast<BasicEffect*>(effect);
		if (_basicEffect)
		{
			// �A���r�G���g���C�g�J���[�̐ݒ�
			_basicEffect->SetAmbientLightColor(SimpleMath::Color(0.2f, 0.2f, 0.2f));
		}
	};


	// �E���̕`��
	m_rightLeg->UpdateEffects(_lightingEffects);
	m_rightLeg->Draw(_context, states, _legRMat, view, proj);

	// �����̕`��
	m_leftLeg->UpdateEffects(_lightingEffects);
	m_leftLeg->Draw(_context, states, _legLMat, view, proj);

	// �����̕`��
	if (sinf(_timer) <= WINK_SPAN)
	{
		m_head->UpdateEffects(_lightingEffects);
		m_head->Draw(_context, states, _headMat, view, proj);
	}
	else
	{
		m_wink->UpdateEffects(_lightingEffects);
		m_wink->Draw(_context, states, _headMat, view, proj);
	}
	// �g�̂̕`��
	m_body->UpdateEffects(_lightingEffects);
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
	m_position.y -= m_parameter.gravity;

	// ���S����
	if (m_position.y < DEATH_LINE)
	{
		is_deathFlag = true;
	}
}

// ���X�|�[���֐�
void Player::Spawn(SimpleMath::Vector3 spawnPosition)
{
	// �p�����[�^�̏�����
	m_parameter.reset();

	// ���W�̐ݒ�
	m_position = spawnPosition;
}
