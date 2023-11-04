/*
 *	@File	TitleSky.h
 *	@Brief	�^�C�g���̋�̕`��B
 *	@Date	2023-11-03
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef TITLESKY
#define TITLESKY

#include "ITitleObject.h"

class TitleSky : public ITitleObject
{
private:

	std::unique_ptr<DirectX::Model> m_model;

	// �X�^�[�g�t���O
	bool is_startFlag;

	// Y���W
	float m_posY;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="factory">�t�@�N�g���}�l�[�W��</param>
	/// <param name="path">�^�C�g�����S���f���̃p�X</param>
	/// <returns>�Ȃ�</returns>
	TitleSky(std::shared_ptr<FactoryManager> factory, const wchar_t* path);
	~TitleSky();

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

	// �X�^�[�g�t���O��ݒ�
	void SetStartFlag(const bool& flag) { is_startFlag = flag; }

	// Y���W��ݒ�
	void SetPositionY(const float& y) { m_posY = y; }
};

#endif // TITLESKY