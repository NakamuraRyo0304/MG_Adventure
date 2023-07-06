/*
 *	@File	Player.h
 *	@Brief	�v���C���̏���������N���X�B
 *	@Date	2023-06-14
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYER
#define PLAYER

struct PlayerParameter
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
		rotate = SimpleMath::Quaternion::Identity;
	}
};

class SystemManager;
class Player
{
private:
	// ����
	float m_timer;

	// ���W
	DirectX::SimpleMath::Vector3 m_position;

	// �v���C���̃p�����[�^
	PlayerParameter m_parameter;

	// ���f���f�[�^
	std::unique_ptr<DirectX::Model> m_model;

	// �V�X�e��
	std::weak_ptr<SystemManager> m_system;

	// ���S����
	bool is_deathFlag;

	// �v���C���̃T�C�Y
	const float SIZE = 0.95f;
	// ��]���x
	const float ROT_SPEED = 0.05f;
	// ���C�W��
	const float DECELERATION = 0.842f;
	// �v���C���̕��V
	const float OFFSET_Y = 0.2f;
	// ���S���C��
	const float DEATH_LINE = -10.0f;


public:
	// �R���X�g���N�^�i���f���f�[�^�j
	Player(std::unique_ptr<DirectX::Model> model);
	~Player();

	// ����������
	void Initialize(std::shared_ptr<SystemManager> system);

	// �X�V�����i�L�[�{�[�h�j
	void Update(DirectX::Keyboard::State& keyState,float timer);
	
	// �`�揈���i�R���e�L�X�g�A�X�e�[�g�A�r���[�s��A�ˉe�s��j
	void Render(ID3D11DeviceContext* context, DirectX::DX11::CommonStates& states, 
		        DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	
	// �I������
	void Finalize();

	// �d�͏���
	void UpdateGravity();

	// ���f���̃Z�b�g
	void SetModel(std::unique_ptr<DirectX::Model> model) { m_model = std::move(model); }

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
};

#endif // PLAYER