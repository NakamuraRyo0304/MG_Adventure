/*
 *	@File	Player.h
 *	@Brief	�v���C���̏���������N���X�B
 *	@Date	2023-06-14
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYER
#define PLAYER

class Head;
class Body;
class RightLeg;
class LeftLeg;
class FactoryManager;
class Player
{
private:

	// �t�@�N�g��
	std::unique_ptr<FactoryManager> m_factory;

	// �e(�g��)
	std::unique_ptr<Body> m_parent;

	// ��
	std::unique_ptr<Head> m_head;

	// �E��
	std::unique_ptr<RightLeg> m_legR;

	// ����
	std::unique_ptr<LeftLeg> m_legL;

	// �Z���N�g����󂯎�����R�C����
	int m_coinNum;

	// ���S����
	bool is_deathFlag;

private:

	// �v���C���̃T�C�Y
	const float SIZE = 0.85f;
	// �ړ����x
	const float NORMAL_SPEED = 0.01f;
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
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Initialize();

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
	/// <returns>�Ȃ�</returns>
	void Render(DirectX::CommonStates& states,DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	/// <summary>
	/// �I������
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Finalize();

private:

	/// <summary>
	/// null��������I�u�W�F�N�g���쐬����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void NewCreate();

public:

	/// <summary>
	/// ���X�|�[��������֐�
	/// </summary>
	/// <param name="spawnPosition">�v���C���[�̍��W���w�肷��</param>
	/// <returns>�Ȃ�</returns>
	void Spawn(DirectX::SimpleMath::Vector3 spawnPosition);

	// �A�N�Z�T----------------------------------------------------------------------------//

	// �|�W�V�������擾
	const DirectX::SimpleMath::Vector3& GetPosition();
	// �|�W�V������ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector3& position);
	// �d�̓��Z�b�g
	void ResetGravity();
	// �d�͂��擾
	const float& GetGravity();
	// �d�͂�ݒ�
	void SetGravity(const float& gravity);
	// ��]�ʂ��擾
	const DirectX::SimpleMath::Quaternion& GetRotation();
	// �T�C�Y���擾
	const float& GetSize() { return SIZE; }
	// ���S������擾
	const bool& GetDeathFlag() { return is_deathFlag; }
	// ���v�R�C������ݒ�
	void SetAllCoins(const int& allCoins) { m_coinNum = allCoins; }
};

#endif // PLAYER