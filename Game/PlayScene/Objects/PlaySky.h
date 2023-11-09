/*
 *	@File	PlaySky.h
 *	@Brief	�v���C�V�[���̃X�J�C�h�[���B
 *	@Date	2023-11-09
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYSKY
#define PLAYSKY

class FactoryManager;
class PlaySky
{
private:

	// ���f��
	std::unique_ptr<DirectX::Model> m_model;

	// �X�J�C�h�[���̉�]���x
	const float SKY_ROT_SPEED = 0.02f;

	// ��̐F�J��
	struct Color
	{
		float red = 0.0f;
		float green = 0.0f;
		float blue = 0.0f;
		Color(float R = 1.0f, float G =  1.0f, float B = 1.0f)
		{
			this->red = R;
			this->green = G;
			this->blue = B;
		}
	};
	Color m_skyColor;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="factory">�t�@�N�g���}�l�[�W��</param>
	/// <param name="path">�v���C�̃X�J�C�p�X</param>
	/// <returns>�Ȃ�</returns>
	PlaySky(std::shared_ptr<FactoryManager> fm, const wchar_t* path);
	~PlaySky();

	/// <summary>
	/// ��̍X�V
	/// </summary>
	/// <param name="gameTimer">�Q�[��������</param>
	/// <returns>�Ȃ�</returns>
	void Update(const float& gameTimer);

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

#endif // PLAYSKY