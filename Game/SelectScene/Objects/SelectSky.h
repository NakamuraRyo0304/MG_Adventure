/*
 *	@File	SelectSky.h
 *	@Brief	�Z���N�g�V�[���̃X�J�C�h�[���B
 *	@Date	2023-11-06
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SELECTSKY
#define SELECTSKY

class FactoryManager;
class SelectSky
{
private:

	// ���f��
	std::unique_ptr<DirectX::Model> m_model;

	// ��]���x
	const float ROT_SPEED = 2.0f;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="factory">�t�@�N�g���}�l�[�W��</param>
	/// <param name="path">�^�C�g�����S���f���̃p�X</param>
	/// <returns>�Ȃ�</returns>
	SelectSky(std::shared_ptr<FactoryManager> fm, const wchar_t* path);
	~SelectSky();

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="states">�R�����X�e�[�g</param>
	/// <param name="view">�r���[�s��</param>
	/// <param name="proj">�ˉe�s��</param>
	/// <param name="timer">�^�C�}�[</param>
	/// <returns>�Ȃ�</returns>
	void Draw(DirectX::CommonStates& states, const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj, const float& timer);
};

#endif // SELECTSKY