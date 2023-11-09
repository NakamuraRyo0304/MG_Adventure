/*
 *	@File	EditSky.h
 *	@Brief	�G�f�B�^�V�[���̃X�J�C�h�[���B
 *	@Date	2023-11-09
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef EDITSKY
#define EDITSKY

class FactoryManager;
class EditSky
{
private:

	// ���f��
	std::unique_ptr<DirectX::Model> m_model;

	// ��]���x
	const float ROT_SPEED = 0.02f;
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="factory">�t�@�N�g���}�l�[�W��</param>
	/// <param name="path">�G�f�B�^�̃X�J�C�p�X</param>
	/// <returns>�Ȃ�</returns>
	EditSky(std::shared_ptr<FactoryManager> fm, const wchar_t* path);
	~EditSky();

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

#endif // EDITSKY