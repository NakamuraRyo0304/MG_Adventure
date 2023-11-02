/*
 *	@File	TitleUI.h
 *	@Brief	Start��Exit�̕\���B
 *	@Date	2023-08-19
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef TITLEUI
#define TITLEUI

class SystemManager;
class TitleUI
{
private:
	// �V�X�e��
	std::shared_ptr<SystemManager> m_system;

	// �E�B���h�E�T�C�Y
	DirectX::SimpleMath::Vector2 m_windowSize;

	// �Z���N�g�t���O
	bool is_selectFlag;

	// �T�C�Y
	const float SELECT_FONT_RATE = 1.05f;
	const float DEFAULT_FONT_RATE = 1.0f;
	DirectX::SimpleMath::Vector2 m_sFontRate;
	DirectX::SimpleMath::Vector2 m_eFontRate;
	DirectX::SimpleMath::Vector2 m_sLineRate;
	DirectX::SimpleMath::Vector2 m_eLineRate;

	// �F
	DirectX::SimpleMath::Color m_startColor;
	DirectX::SimpleMath::Color m_exitColor;

private:
	// �t���X�N���[���T�C�Y
	DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

public:

	TitleUI();
	~TitleUI();

	/// <summary>
	/// �쐬�֐�
	/// </summary>
	/// <param name="system">�V�X�e���}�l�[�W��</param>
	/// <param name="windowSize">�E�B���h�E�T�C�Y</param>
	/// <returns>�Ȃ�</returns>
	void Create(std::shared_ptr<SystemManager> system, const DirectX::SimpleMath::Vector2& windowSize);

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="selectFlag">�J�nor�I���t���O</param>
	/// <returns>�Ȃ�</returns>
	void Update(const bool& selectFlag);

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="fadeValue">�t�F�[�h�̒l</param>
	/// <param name="endAnim"�A�j���[�V�����̏�������</param>
	/// <returns>�Ȃ�</returns>
	void Render(const float& fadeValue, const bool& endAnim);

	/// <summary>
	/// �I������
	/// </summary>
	/// <param name="��������"></param>
	/// <returns>�Ȃ�</returns>
	void Finalize();

};

#endif // TITLEUI