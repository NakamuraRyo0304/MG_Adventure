/*
 *	@File	Logo.h
 *	@Brief	�^�C�g�����S�N���X�B
 *	@Date	2023-11-03
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef LOGO
#define LOGO

#include "ITitleObject.h"

class Logo : public ITitleObject
{
private:

	// ���f��
	std::unique_ptr<DirectX::Model> m_model;

	// �J�n�t���O
	bool is_startFlag;

	// ���S���ړI�n�ɒ����������ׂ�t���O
	bool is_endFlag;

	// ���̃V�[���ɍs���t���O
	bool is_nextFlag;

	// ���W
	DirectX::SimpleMath::Vector3 m_position;

	// ���S�̊J�n���̉��o
	float m_logoMoveScale;
	float m_logoMoveY;
	const float END_MOVE_POS = 1.0f;
	const float LOGO_CHANGE_SCALE = 1.05f;
	const float MAX_HEIGHT = 10.0f;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="path">�^�C�g�����S���f���̃p�X</param>
	/// <returns>�Ȃ�</returns>
	Logo(const wchar_t* path);
	~Logo();

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Update() override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="states">�R�����X�e�[�g</param>
	/// <param name="view">�r���[�s��</param>
	/// <param name="proj">�ˉe�s��</param>
	/// <returns>�Ȃ�</returns>
	void Render(DirectX::CommonStates& states,
		const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;

	/// <summary>
	/// �I������
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Finalize() override;
public:

	// ���S�̍��W���擾
	const DirectX::SimpleMath::Vector3& GetPosition() { return m_position; }

	// �X�^�[�g�t���O��ݒ肷��
	void SetStartFlag(const bool& flag) { is_startFlag = flag; }

	// ���S�̈ړ����I���n�_�ɒB���������肷��
	const bool& IsLogoEndFlag() { return is_endFlag; }
	// ���̃V�[���ɂ����t���O
	const bool& IsCanNextFlag() { return is_nextFlag; }
};

#endif // LOGO