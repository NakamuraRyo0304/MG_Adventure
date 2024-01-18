/*
 *	@File	IParts.h
 *	@Brief	�p�[�c�̊��N���X�B
 *	@Date	2023-01-18
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef IPARTS
#define IPARTS

class IParts
{
private:

	// ���f���f�[�^
	std::unique_ptr<DirectX::Model> m_model;

	// �����̍s��
	DirectX::SimpleMath::Matrix m_matrix;

	// �e�̍s��
	DirectX::SimpleMath::Matrix m_parentMatrix;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�Ȃ�</returns>
	IParts(std::unique_ptr<DirectX::Model> model);
	~IParts() = default;

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	virtual void Update() = 0;

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="states">�R�����X�e�[�g�|�C���^</param>
	/// <param name="view">�r���[�s��</param>
	/// <param name="proj">�v���W�F�N�V�����s��</param>
	/// <returns>�Ȃ�</returns>
	virtual void Draw(DirectX::CommonStates& states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) = 0;

public:

	// ���f���擾
	const std::unique_ptr<DirectX::Model>& GetModel() { return m_model; }

	// �}�g���N�X�擾
	DirectX::SimpleMath::Matrix GetMatrix() { return m_matrix; }

	// �}�g���N�X�ݒ�
	void SetMatrix(const DirectX::SimpleMath::Matrix& mat) { m_matrix = mat; }

	// �e�}�g���N�X�擾
	DirectX::SimpleMath::Matrix GetParentMatrix() { return m_parentMatrix; }

	// �e�}�g���N�X�ݒ�
	void SetParentMatrix(const DirectX::SimpleMath::Matrix& mat) { m_parentMatrix = mat; }
};

#endif // IPARTS