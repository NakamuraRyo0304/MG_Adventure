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

// ���f���t�@�N�g���[
#include "../../../Libraries/Factories/ModelFactory.h"

#include "Player.h"

 /// <summary>
 /// �R���X�g���N�^
 /// </summary>
 /// <param name="head">�����̃��f���f�[�^</param>
 /// <param name="body">���̂̃��f���f�[�^</param>
 /// <param name="right">�E�����f���f�[�^</param>
 /// <param name="left">�������f���f�[�^</param>
 /// <returns>�Ȃ�</returns>
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

/// <summary>
/// �X�V����
/// </summary>
/// <param name="keyState">�L�[�{�[�h</param>
/// <param name="timer">�h���V�[����StepTimer(TotalTime)</param>
/// <param name="lookFlag">���_�t���O</param>
/// <returns>�Ȃ�</returns>
void Player::Update(Keyboard::State& keyState, float timer, bool lookFlag)
{
	_timer = timer;

	is_lookFlag = lookFlag;

	// ���_�ɂ���đ��x��ύX����
	m_parameter.accelerate = is_lookFlag ? THIRD_SPEED : NORMAL_SPEED;

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
		SimpleMath::Vector3 _newVec(0.0f, 0.0f, m_parameter.accelerate);
		_newVec = SimpleMath::Vector3::Transform(_newVec, m_parameter.rotate);
		m_parameter.velocity -= _newVec;
	}
	else if (keyState.S)
	{
		SimpleMath::Vector3 _newVec(0.0f, 0.0f, m_parameter.accelerate / 2);
		_newVec = SimpleMath::Vector3::Transform(_newVec, m_parameter.rotate);
		m_parameter.velocity += _newVec;
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

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="context">ID3D11DeviceContext�|�C���^</param>
/// <param name="states">�R�����X�e�[�g</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
/// <param name="lightDir">���C�e�B���O</param>
/// <returns>�Ȃ�</returns>
void Player::Render(ID3D11DeviceContext* context, CommonStates& states,
	SimpleMath::Matrix view, SimpleMath::Matrix proj,const SimpleMath::Vector3& lightDir)
{
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
	m_rightLeg->Draw(context, states, _legRMat, view, proj);

	// �����̕`��
	m_leftLeg->UpdateEffects(_lightingEffects);
	m_leftLeg->Draw(context, states, _legLMat, view, proj);

	// �����̕`��
	if (sinf(_timer) <= WINK_SPAN)
	{
		m_head->UpdateEffects(_lightingEffects);
		m_head->Draw(context, states, _headMat, view, proj);
	}
	else
	{
		m_wink->UpdateEffects(_lightingEffects);
		m_wink->Draw(context, states, _headMat, view, proj);
	}
	// �g�̂̕`��
	m_body->UpdateEffects(_lightingEffects);
	m_body->Draw(context, states, _commonMat, view, proj);
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
