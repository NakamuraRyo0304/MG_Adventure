/*
 *	@File	Player.h
 *	@Brief	�v���C���̏���������N���X�B
 *	@Date	2023-06-14
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYER
#define PLAYER

#include "../../../Libraries/SystemDatas/Collider.h"

struct PlayerParameter
{
	DirectX::SimpleMath::Vector3 position;		// ���W
	float velocity;								// �ړ���
	float gravity;								// �d��
	float accelerate;							// �����x
	void reset()								// ���Z�b�g�֐�
	{
		position = DirectX::SimpleMath::Vector3::Zero;
		velocity = 0.0f;
		gravity = 0.0f;
		accelerate = 0.0f;
	}
};

class Player
{
private:
	// �v���C���̃p�����[�^
	PlayerParameter m_parameter;

	// ���f���f�[�^
	std::unique_ptr<DirectX::Model> m_model;

	// �v���C���̃T�C�Y
	const float SIZE = 0.5f;
public:
	// �R���X�g���N�^�i���f���f�[�^�j
	Player(std::unique_ptr<DirectX::Model> model);
	~Player();

	// ����������
	void Initialize();

	// �X�V�����i�L�[�{�[�h�j
	void Update(DirectX::Keyboard::State& keyState);
	
	// �`�揈���i�R���e�L�X�g�A�X�e�[�g�A�r���[�s��A�ˉe�s��j
	void Render(ID3D11DeviceContext* context, DirectX::DX11::CommonStates& states, 
		        DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	
	// �I������
	void Finalize();

	// ���f���̃Z�b�g
	void SetModel(std::unique_ptr<DirectX::Model> model) { m_model = std::move(model); }

	// �|�W�V�����A�N�Z�T
	DirectX::SimpleMath::Vector3& GetPosition() { return m_parameter.position; }
	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_parameter.position = position; }

	// �X�|�[���֐�
	void Spawn(DirectX::SimpleMath::Vector3 spawnPosition);

	// �d�̓��Z�b�g
	void ResetGravity() { m_parameter.gravity = 0.0f; }
	// �d�̓A�N�Z�T
	const float& GetGravity() { return m_parameter.gravity; }
	void SetGravity(const float& gravity) { m_parameter.gravity = gravity; }

	// �T�C�Y�A�N�Z�T
	const float& GetSize() { return SIZE; }
};

#endif // PLAYER