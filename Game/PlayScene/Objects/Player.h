/*
 *	@File	Player.h
 *	@Brief	�v���C���̏���������N���X�B
 *	@Date	2023-06-14
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYER
#define PLAYER

struct Parameter
{
	DirectX::SimpleMath::Vector3 velocity;		// �ړ���
	DirectX::SimpleMath::Quaternion rotate;		// �����Ă������
	float gravity;								// �d��
	float accelerate;							// �����x
	void reset()								// ���Z�b�g�֐�
	{
		velocity = DirectX::SimpleMath::Vector3::Zero;
		gravity = 0.0f;
		accelerate = 0.0f;
		rotate = DirectX::SimpleMath::Quaternion::Identity;
	}
};

class SystemManager;
class Player
{
private:

	// ����
	float m_timer;

	// �r�̓����Ɏg���ϐ�
	float m_footMove;

	// ���W
	DirectX::SimpleMath::Vector3 m_position;

	// �v���C���̃p�����[�^
	Parameter m_parameter;

	// ��̓���
	DirectX::SimpleMath::Quaternion m_neckQuaternion;
	DirectX::SimpleMath::Vector2 m_neckRotate;
	bool is_lookFlag;

	// �O�l�̂̉�]
	DirectX::SimpleMath::Quaternion m_thirdRotate;

	// ���f���f�[�^
	std::unique_ptr<DirectX::Model> m_head, m_body, m_leftLeg, m_rightLeg;

	// �V�X�e��
	std::weak_ptr<SystemManager> m_system;

	// ���S����
	bool is_deathFlag;

private:

	// �v���C���̃T�C�Y
	const float SIZE = 0.85f;
	// ��]���x
	const float ROT_SPEED = 0.05f;
	const float NECK_ROT_SPEED = 0.30f;
	// ���C�W��
	const float DECELERATION = 0.851f;
	// �v���C���̕��V
	const float OFFSET_Y = 0.2f;
	// ���S���C��
	const float DEATH_LINE = -10.0f;


public:
	// �R���X�g���N�^�i���A�g�́A�E���A�����j
	Player(std::unique_ptr<DirectX::Model> head,std::unique_ptr<DirectX::Model> body,
		std::unique_ptr<DirectX::Model> rightLeg, std::unique_ptr<DirectX::Model> leftLeg);
	~Player();

	// �����������i�V�X�e���}�l�[�W���|�C���^�j
	void Initialize(std::shared_ptr<SystemManager> system);

	// �X�V�����i�L�[�{�[�h�A�^�C�}�[�A���_�ύX�t���O�j
	void Update(DirectX::Keyboard::State& keyState,float timer, bool lookFlag);

	// �`�揈���i�R���e�L�X�g�A�X�e�[�g�A�r���[�s��A�ˉe�s��j
	void Render(ID3D11DeviceContext* context, DirectX::CommonStates& states,
		        DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	// �I������
	void Finalize();

private:

	// �d�͏���
	void UpdateGravity();

public:

	// �A�N�Z�T----------------------------------------------------------------------------//

	// �|�W�V�����A�N�Z�T
	DirectX::SimpleMath::Vector3& GetPosition() { return m_position; }
	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }

	// �X�|�[���֐�
	void Spawn(DirectX::SimpleMath::Vector3 spawnPosition);

	// �d�̓��Z�b�g
	void ResetGravity() { m_parameter.gravity = 0.0f; }
	// �d�̓A�N�Z�T
	const float& GetGravity() { return m_parameter.gravity; }
	void SetGravity(const float& gravity) { m_parameter.gravity = gravity; }

	// �T�C�Y�A�N�Z�T
	const float& GetSize() { return SIZE; }

	// ���S����Q�b�^�[
	const bool& GetDeathFlag() { return is_deathFlag; }

	// ��]�ʂ��擾
	const DirectX::SimpleMath::Quaternion& GetRotate() { return m_parameter.rotate; }

	// ��̉�]�ʂ��擾
	const DirectX::SimpleMath::Quaternion& GetNeckRotate() { return m_thirdRotate; }
};

#endif // PLAYER