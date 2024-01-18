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
	DirectX::SimpleMath::Vector3 position;		// ���W
	DirectX::SimpleMath::Vector3 velocity;		// �ړ���
	DirectX::SimpleMath::Quaternion rotate;		// �����Ă������
	float gravity;								// �d��
	float accelerate;							// �����x
	void reset()								// ���Z�b�g�֐�
	{
		position = DirectX::SimpleMath::Vector3::Zero;
		velocity = DirectX::SimpleMath::Vector3::Zero;
		gravity = 0.0f;
		accelerate = 0.0f;
		rotate = DirectX::SimpleMath::Quaternion::Identity;
	}
};

class Head;
class SystemManager;
class Player
{
private:

	// ��
	std::unique_ptr<Head> m_head;

	// �v���C���̃p�����[�^
	Parameter m_parameter;

	// �Z���N�g����󂯎�����R�C����
	int m_coinNum;

	// �r�̓����Ɏg���ϐ�
	float m_footMove;

	// ���f���f�[�^
	std::unique_ptr<DirectX::Model> m_body, m_leftLeg, m_rightLeg;

	// �V�X�e��
	std::shared_ptr<SystemManager> m_system;

	// ���S����
	bool is_deathFlag;

	// ���C�e�B���O
	DirectX::SimpleMath::Vector3 m_lightDirection;

private:

	// �v���C���̃T�C�Y
	const float SIZE = 0.85f;
	// �ړ����x
	const float NORMAL_SPEED = 0.01f;
	const float FOOT_SPEED = 0.1f;
	// ��]���x
	const float ROT_SPEED = 0.05f;
	// ���C�W��
	const float DECELERATION = 0.851f;
	// �v���C���̕��V
	const float OFFSET_Y = 0.2f;
	// ���S���C��
	const float DEATH_LINE = -10.0f;
	// �E�C���N�X�p��
	const float WINK_SPAN = 0.95f;
	// ���C�g�̕ϊ����x
	const float LIGHT_SPEED = 0.1f;


public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="head">�����̃��f���f�[�^</param>
	/// <param name="body">���̂̃��f���f�[�^</param>
	/// <param name="right">�E�����f���f�[�^</param>
	/// <param name="left">�������f���f�[�^</param>
	/// <returns>�Ȃ�</returns>
	Player(std::unique_ptr<DirectX::Model> head,std::unique_ptr<DirectX::Model> body,
		std::unique_ptr<DirectX::Model> rightLeg, std::unique_ptr<DirectX::Model> leftLeg);
	~Player();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="system">�V�X�e���f�[�^</param>
	/// <returns>�Ȃ�</returns>
	void Initialize(std::shared_ptr<SystemManager> system);

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="states">�R�����X�e�[�g</param>
	/// <param name="view">�r���[�s��</param>
	/// <param name="proj">�ˉe�s��</param>
	/// <param name="lightDir">���C�e�B���O</param>
	/// <returns>�Ȃ�</returns>
	void Render(DirectX::CommonStates& states,DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj,
				const DirectX::SimpleMath::Vector3& lightDir);

	/// <summary>
	/// �I������
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Finalize();

private:

	/// <summary>
	/// �d�͏���
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void UpdateGravity();

	/// <summary>
	/// ���C�e�B���O���X�V����
	/// </summary>
	/// <param name="dir">���C�g�̕���</param>
	/// <returns>���C�g�̌v�Z����</returns>
	std::function<void(IEffect* effect)> UpdateLighting(DirectX::SimpleMath::Vector3 dir);
public:

	/// <summary>
	/// ���X�|�[��������֐�
	/// </summary>
	/// <param name="spawnPosition">�v���C���[�̍��W���w�肷��</param>
	/// <returns>�Ȃ�</returns>
	void Spawn(DirectX::SimpleMath::Vector3 spawnPosition);

	// �A�N�Z�T----------------------------------------------------------------------------//

	// ���C�e�B���O�ݒ�
	void InitializeLighting(const DirectX::SimpleMath::Vector3& lightDir) { m_lightDirection = lightDir; }
	// �|�W�V�������擾
	const DirectX::SimpleMath::Vector3& GetPosition() { return m_parameter.position; }
	// �|�W�V������ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_parameter.position = position; }
	// �d�̓��Z�b�g
	void ResetGravity() { m_parameter.gravity = 0.0f; }
	// �d�͂��擾
	const float& GetGravity() { return m_parameter.gravity; }
	// �d�͂�ݒ�
	void SetGravity(const float& gravity) { m_parameter.gravity = gravity; }
	// �T�C�Y���擾
	const float& GetSize() { return SIZE; }
	// ���S������擾
	const bool& GetDeathFlag() { return is_deathFlag; }
	// ��]�ʂ��擾
	const DirectX::SimpleMath::Quaternion& GetRotate() { return m_parameter.rotate; }
	// ���v�R�C������ݒ�
	void SetAllCoins(const int& allCoins) { m_coinNum = allCoins; }
};

#endif // PLAYER